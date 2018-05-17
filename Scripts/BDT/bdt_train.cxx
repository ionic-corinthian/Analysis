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
#include "TMVA/DataLoader.h"
#include "TMVA/Tools.h"
#include "TMVA/TMVAGui.h"

#include "myDecay.cxx"

void bdt_train(const char* TupleName, int year, const char* mag, const char* charge, bool all_mc=true, bool all_data=true) {
    
    //============================================//
    //=== Macro: bdt_train()                   ===//
    //===--------------------------------------===//
    //=== This macro carries out BDT training  ===//
    //=== for the year specified in the arg.   ===//
    //=== This is where you can change which   ===//
    //=== variables are used for BDT training. ===//
    //===--------------------------------------===//
    //=== Year: the year of data to train on.  ===//
    //===       N.B. the data locations and #  ===//
    //===       of subjobs must be correct!    ===//
    //============================================//
    
    
    //=====================//
    //=== Prepare files ===//
    //=====================//
    
    //=== Setup TMVA ===//
    TMVA::Tools::Instance();
    //=== Open a new file to save the BDT output ===//
    TFile* outputFile = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/BDT_Output_%s.root", year, mag, charge, TupleName), "RECREATE");
   
    //=== Create instructive name for factory, detailing which years
    //=== were used for training (both MC and data). This name will
    //=== be used in the weights file, so needs to be unique otherwise
    //=== subsequent weights files will be overwritten! 
    TString fac_name;
    if (all_mc && !all_data) fac_name = Form("AllMC_%s_%i_Mag%s_%sCharge", TupleName, year, mag, charge);
    else if (!all_mc && all_data) fac_name = Form("%s_AllDataYears_Mag%s_%sCharge", TupleName, mag, charge);
    else if (all_mc && all_data ) fac_name = Form("jicAllMC_%s_AllDataYears_Mag%s_%sCharge", TupleName, mag, charge);
    else fac_name = Form("%s_%i_Mag%s_%sCharge", TupleName, year, mag, charge);
    
    //=== Get instance of TMVA factory to use for training ===//
    TMVA::Factory *factory = new TMVA::Factory(fac_name, outputFile, "V:!Silent:Color:Transformations=I:DrawProgressBar:AnalysisType=Classification");
    TMVA::DataLoader *dataloader = new TMVA::DataLoader("dataset");
    
    //=== Weights ===//
    double  signalWeight = 1.0;
    double  backgroundWeight = 1.0;

    //=== Read in signal tree (MC which has been precut on truth matching info) ===//
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
    // Turn off all branches apart from those needed by the training
    dataTree->SetBranchStatus("*", 0);
    dataTree->SetBranchStatus("D0_PT",1);
    dataTree->SetBranchStatus("D0_P",1);
    dataTree->SetBranchStatus("Bach_PT",1);
    dataTree->SetBranchStatus("Bach_P",1);
    dataTree->SetBranchStatus("Pi0_PT",1);
    dataTree->SetBranchStatus("Pi0_P",1);
    dataTree->SetBranchStatus("Gamma1_CL",1);
    dataTree->SetBranchStatus("Gamma2_CL",1);
    dataTree->SetBranchStatus("Bu_VTXCHI2DOF",1);
    dataTree->SetBranchStatus("D0_MIPCHI2_PV",1);
    dataTree->SetBranchStatus("Bach_MIPCHI2_PV",1);
    dataTree->SetBranchStatus("Bu_FDCHI2_OWNPV",1);
    dataTree->SetBranchStatus("D0_FDCHI2_OWNPV",1);
    dataTree->SetBranchStatus("Bu_DIRA_BPV",1);
    dataTree->SetBranchStatus("D0_DIRA_BPV",1);
    dataTree->SetBranchStatus("Bu_ptasy_1.50",1);

    //=== Add the signal and background (i.e. real data) to the BDT trainer ===//
    dataloader->AddSignalTree(sigTree, signalWeight);
    dataloader->AddBackgroundTree(dataTree, backgroundWeight);
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
  //  factory->AddVariable("D0_FD_ZSIG", 'F');
    dataloader->AddVariable("D0_PT",    'F');
    dataloader->AddVariable("D0_P",     'F');
    dataloader->AddVariable("Bach_PT",  'F');
    dataloader->AddVariable("Bach_P",   'F');
  // dataloader->AddVariable("K0_PT",    'F');
  //  factory->AddVariable("K0_P",     'F');
  //  factory->AddVariable("P0_PT",    'F');
 //   factory->AddVariable("P0_P",     'F');
    dataloader->AddVariable("Pi0_PT",   'F');
    dataloader->AddVariable("Pi0_P",    'F');
    
    //=== Neutral particle variables ===//
    dataloader->AddVariable("Gamma1_CL", 'F');
    dataloader->AddVariable("Gamma2_CL", 'F');
    
    //=== Vertex Chi^2 ===//
    dataloader->AddVariable("Bu_VTXCHI2DOF", 'F');
 //   factory->AddVariable("D0_VTXCHI2DOF", 'F');
    
    //=== Impact parameters ===//
//    factory->AddVariable("Bu_MIPCHI2_PV",                                   'F');
    dataloader->AddVariable("log_D0_MIPCHI2_PV     := log(D0_MIPCHI2_PV)",     'F');
    dataloader->AddVariable("log_Bach_MIPCHI2_PV   := log(Bach_MIPCHI2_PV)",   'F');
  //  dataloader->AddVariable("log_K0_MIPCHI2_PV     := log(K0_MIPCHI2_PV)",     'F');
  //  factory->AddVariable("log_K1_MIPCHI2_PV     := log(K1_MIPCHI2_PV)",     'F');
    
    //=== Flight distance ===//
    dataloader->AddVariable("log_Bu_FDCHI2_OWNPV   := log(Bu_FDCHI2_OWNPV)", 'F');
    dataloader->AddVariable("log_D0_FDCHI2_OWNPV   := log(D0_FDCHI2_OWNPV)", 'F');
    
    //=== Director angle (DIRA) ===//
    dataloader->AddVariable("log_1_Bu_DIRA_BPV     := log(1-Bu_DIRA_BPV)", 'F');
    dataloader->AddVariable("log_1_D0_DIRA_BPV     := log(1-D0_DIRA_BPV)", 'F');
    
    //=== Distance of closest approach (DOCA) ===//
 //   factory->AddVariable("Bu_MAXDOCA",   'F');
 //   factory->AddVariable("D0_AMAXDOCA",  'F');
    
    //=== Lifetimes (LT) ===//
 //   factory->AddVariable("Bu_LT_BPV", 'F');
   // factory->AddVariable("D0_LT_BPV", 'F');

    //=== P_T asymmetry (ptasy) ===//
    dataloader->AddVariable("Bu_ptasy_1_50 := Bu_ptasy_1.50", 'F');  
    std::cout << "--- ADDED VARIABLES SUCCESSFULLY!" << std::endl;  

    //====================================//
    //=== Run the BDT training/testing ===//
    //====================================//
    
    dataloader->PrepareTrainingAndTestTree(sigcut,bkgcut,"random"); 
    
    //=== Add different MVA methods to test here! ===// 
    factory->BookMethod(dataloader, TMVA::Types::kBDT,"BDT","NTrees=600:MaxDepth=4");
 //   factory->BookMethod(TMVA::Types::kBDT,"Less_Depth","NTrees=400:MaxDepth=4");
  //  factory->BookMethod(TMVA::Types::kBDT,"More_Trees","NTrees=800:MaxDepth=4");
  //  factory->BookMethod(TMVA::Types::kBDT,"More_Depth_and_Trees","NTrees=800:MaxDepth=8");
  //  factory->BookMethod(TMVA::Types::kFisher, "BoostedFisher","Boost_Num=20:Boost_Transform=log:\Boost_Type=AdaBoost:Boost_AdaBoostBeta=0.2");
  //  factory->BookMethod(TMVA::Types::kMLP, "MLP","H:!V:NeuronType=tanh:VarTransform=N:NCycles=600:HiddenLayers=N+5:\TestRate=5" );
  //  factory->BookMethod( TMVA::Types::kTMlpANN, "TMlpANN","!H:!V:NCycles=200:HiddenLayers=N+1,N:LearningMethod=BFGS:ValidationFraction=0.3"  );
   // factory->BookMethod( TMVA::Types::kBDT, "BDTG","!H:!V:NTrees=1000:BoostType=Grad:Shrinkage=0.30:UseBaggedGrad:\GradBaggingFraction=0.6:SeparationType=GiniIndex:nCuts=20:\PruneMethod=CostComplexity:PruneStrength=50:NNodesMax=5" );
  //  factory->BookMethod( TMVA::Types::kRuleFit, "RuleFit","H:!V:RuleFitModule=RFTMVA:Model=ModRuleLinear:MinImp=0.001:\RuleMinDist=0.001:NTrees=20:fEventsMin=0.01:fEventsMax=0.5:\GDTau=-1.0:GDTauPrec=0.01:GDStep=0.01:GDNSteps=10000:GDErrScale=1.02" );
    std::cout << "--- ABOUT TO TRAIN METHODS!" << std::endl;
    
    //=== Train, test, then evaluate all methods ===//
    factory->TrainAllMethods();
    factory->TestAllMethods();
    factory->EvaluateAllMethods();
    outputFile->Close();

}
