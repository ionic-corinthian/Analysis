#include <cstdlib>
#include <iostream>
#include <map>
#include <string>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TObjString.h"
#include "TSystem.h"
#include "TROOT.h"
    
#include "TMVA/Factory.h"
//   #include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "myDecay.cxx"

void neutral_bdt_train(const char* TupleName, int year, const char* mag, const char* charge, bool all_mc=false, bool all_data=false) {
    
    //=====================//
    //=== Prepare files ===//
    //=====================//
    
    //=== Setup TMVA ===//
    TMVA::Tools::Instance();
    //=== Open a new file to save the BDT output ===//
    TFile* outputFile = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/neut_BDT_Output_%s.root", year, mag, charge, TupleName), "RECREATE");
    //=== Get instance of TMVA factory to use for training ===//
    
    TString fac_name;
    if (all_mc && !all_data) fac_name = Form("AllMC_%s_%i_Mag%s_%sCharge", TupleName, year, mag, charge);
    else if (!all_mc && all_data) fac_name = Form("%s_AllDataYears_Mag%s_%sCharge", TupleName, mag, charge);
    else if (all_mc && all_data ) fac_name = Form("AllMC_%s_AllDataYears_Mag%s_%sCharge", TupleName, mag, charge);
    else fac_name = Form("%s_%i_Mag%s_%sCharge", TupleName, year, mag, charge);
    
    TMVA::Factory *factory = new TMVA::Factory((TString)("neut"+fac_name), outputFile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification");
    
    //=== Weights ===//
    double  signalWeight = 1.0;
    double  backgroundWeight = 1.0;

    //=== Read in signal tree (MC which has been precut on truth matching info) ===//
    // if year is set to 999, then read in all MC!
    TChain* sigTree = new TChain("DecayTree");
    if (all_mc) {
        sigTree->Add(Form("~/TruthMatchedTrees/2011/Mag%s/%sCharge/TM_%s_MC.root", mag, charge, TupleName));
        sigTree->Add(Form("~/TruthMatchedTrees/2012/Mag%s/%sCharge/TM_%s_MC.root", mag, charge, TupleName));
        sigTree->Add(Form("~/TruthMatchedTrees/2015/Mag%s/%sCharge/TM_%s_MC.root", mag, charge, TupleName));
        sigTree->Add(Form("~/TruthMatchedTrees/2016/Mag%s/%sCharge/TM_%s_MC.root", mag, charge, TupleName));
    }
    else sigTree->Add(Form("~/TruthMatchedTrees/%i/Mag%s/%sCharge/TM_%s_MC.root", year, mag, charge, TupleName));

    //=== TChain data TTrees together (for mag up and mag down) ===//
    TChain* dataTree = new TChain("DecayTree");
    if (all_data) {
        dataTree->Add(Form("/data/lhcb/users/colmmurphy/BDT_Output/2011/Mag%s/%sCharge/bkgcut_%s.root", mag, charge, TupleName));
        dataTree->Add(Form("/data/lhcb/users/colmmurphy/BDT_Output/2012/Mag%s/%sCharge/bkgcut_%s.root", mag, charge, TupleName));
        dataTree->Add(Form("/data/lhcb/users/colmmurphy/BDT_Output/2015/Mag%s/%sCharge/bkgcut_%s.root", mag, charge, TupleName));
        dataTree->Add(Form("/data/lhcb/users/colmmurphy/BDT_Output/2016/Mag%s/%sCharge/bkgcut_%s.root", mag, charge, TupleName));
        dataTree->Add(Form("/data/lhcb/users/colmmurphy/BDT_Output/2017/Mag%s/%sCharge/bkgcut_%s.root", mag, charge, TupleName));
    }
    else dataTree->Add(Form("/data/lhcb/users/colmmurphy/BDT_Output/%i/Mag%s/%sCharge/bkgcut_%s.root", year, mag, charge, TupleName));
    
    //=== Add the signal and background (i.e. real data) to the BDT trainer ===//
    factory->AddSignalTree(sigTree, signalWeight);
    factory->AddBackgroundTree(dataTree, backgroundWeight);
    std::cout << "--- ADDED TREES SUCCESSFULLY!" << std::endl;
    
    //=== Signal has been previously truth matched. Background cut to be in upper B mass sideband ===//
    //=== Additional cuts are to ensure negative values are not logged!                           ===//
    TCut sigcut = "";
    TCut bkgcut = "";
    //Bu_M>5900. && D0_MIPCHI2_PV<0. && Bach_MIPCHI2_PV<0. && K0_MIPCHI2_PV<0. && P0_MIPCHI2_PV<0. && Bu_FDCHI2_OWNPV<0. && D0_FDCHI2_OWNPV<0. && Bu_DIRA_BPV>1. && D0_DIRA_BPV>1.

    //=========================================//
    //=== Add NTuple variables for training ===//
    //=========================================//
    
    //=== Particle momenta ===//
    factory->AddVariable("Pi0_PT",   'F');
    factory->AddVariable("Gamma1_PT",   'F');
    factory->AddVariable("Gamma2_PT",   'F');
   // factory->AddVariable("Pi0_P",    'F');   
  //  factory->AddVariable("Pi0_PE", 'F');
   // factory->AddVariable("Pi0_PX", 'F');
   // factory->AddVariable("Pi0_PY", 'F');
   // factory->AddVariable("Pi0_PZ", 'F');
    //=== Neutral particle variables ===//
    factory->AddVariable("Gamma1_CL", 'F');
    factory->AddVariable("Gamma2_CL", 'F');
  //  factory->AddVariable("Pi0_ptasy_1.50", 'F');
   // factory->AddVariable("Gamma1_ptasy_1.50", 'F');
  //  factory->AddVariable("Gamma2_ptasy_1.50", 'F');
    factory->AddVariable("Gamma1_PP_IsNotE", 'F');
    factory->AddVariable("Gamma2_PP_IsNotE", 'F');
    
   
    std::cout << "--- ADDED VARIABLES SUCCESSFULLY!" << std::endl;  

    //====================================//
    //=== Run the BDT training/testing ===//
    //====================================//
    
    factory->PrepareTrainingAndTestTree(sigcut,bkgcut,"random"); 
    
    //=== Add different MVA methods to test here! ===// 
    factory->BookMethod(TMVA::Types::kBDT,"BDT","NTrees=600:MaxDepth=4");
  // factory->BookMethod(TMVA::Types::kBDT,"More_Depth","NTrees=400:MaxDepth=8");
  // factory->BookMethod(TMVA::Types::kBDT,"More_Trees","NTrees=800:MaxDepth=4");
  // factory->BookMethod(TMVA::Types::kBDT,"More_Depth_and_Trees","NTrees=800:MaxDepth=8");
  // factory->BookMethod( TMVA::Types::kBDT, "BDTG","!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:\GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:\PruneMethod=CostComplexity:PruneStrength=50:NNodesMax=5" );

    std::cout << "--- ABOUT TO TRAIN METHODS!" << std::endl;
    
    //=== Train, test, then evaluate all methods ===//
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    outputFile->Close();

}
