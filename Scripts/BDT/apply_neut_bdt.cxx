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

void apply_neut_bdt(const char* TupleName, int year, const char* mag, const char* charge) {

    //=== Get tree ===//
    TFile* g = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s_MC.root", year, mag, charge, TupleName), "UPDATE");
    TTree* dataTree = (TTree*)g->Get("DecayTree");
    
    //=== Create output file ===//
  //  TFile* f = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/AllBDT_%s_MC.root", year, mag, charge, TupleName), "RECREATE");
    TTree* tree   = dataTree->CloneTree(0);
    
    TMVA::Tools::Instance();
    TMVA::Reader *neut = new TMVA::Reader( "V:Color:!Silent" );
    TMVA::Reader *others = new TMVA::Reader( "V:Color:!Silent" );
    
    //===============================================================================//
    //=== Create a map to make AddVariable and SetBranchAddress processes clearer ===//
    //===============================================================================//
    std::map < std::string , Double_t > others_uservar;
    std::map < std::string , Double_t > neut_uservar;
    
    //=== OTHERS BDT ===//
    others_uservar["D0_PT"];
    others_uservar["D0_P"];
    others_uservar["Bach_PT"];
    others_uservar["Bach_P"];
    others_uservar["K0_PT"];
    others_uservar["Bu_VTXCHI2DOF"];
    others_uservar["D0_MIPCHI2_PV"];
    others_uservar["Bach_MIPCHI2_PV"];
    others_uservar["K0_MIPCHI2_PV"];
    others_uservar["Bu_FDCHI2_OWNPV"];
    others_uservar["D0_FDCHI2_OWNPV"];
    others_uservar["Bu_DIRA_BPV"];
    others_uservar["D0_DIRA_BPV"];
    others_uservar["Bu_ptasy_1.50"];
    
    //=== NEUTRAL BDT ===//
    neut_uservar["Pi0_PT"];
    neut_uservar["Gamma1_PT"];
    neut_uservar["Gamma2_PT"];
    neut_uservar["Gamma1_CL"];
    neut_uservar["Gamma2_CL"];
    neut_uservar["Gamma1_PP_IsNotE"];
    neut_uservar["Gamma2_PP_IsNotE"];

    
    //=============================================================================//
    //=== Add the variables needed for the BDT calculation to the TMVA::Reader  ===//
    //=== which will use them in conjunction with the weights file to calculate ===//
    //=== a value for the BDT variable                                          ===//
    //=============================================================================//
    Float_t neut_var[7];
    Float_t others_var[14];
    
    //===============================================//
    //=== NEUTRAL BDT ===============================//   
    //=== Particle momenta ===//
    neut->AddVariable("Pi0_PT", &neut_var[0]);
    neut->AddVariable("Gamma1_PT", &neut_var[1]);
    neut->AddVariable("Gamma2_PT", &neut_var[2]);
    //=== Neutral particle variables ===//
    neut->AddVariable("Gamma1_CL", &neut_var[3]);
    neut->AddVariable("Gamma2_CL", &neut_var[4]);
    neut->AddVariable("Gamma1_PP_IsNotE", &neut_var[5]);
    neut->AddVariable("Gamma2_PP_IsNotE", &neut_var[6]);
    //===============================================//
    //-----------------------------------------------//
    //===============================================//
    //=== OTHER BDT =================================//
    //=== Particle momenta ===//
    others->AddVariable("D0_PT", &others_var[0]);
    others->AddVariable("D0_P", &others_var[1]);
    others->AddVariable("Bach_PT", &others_var[2]);
    others->AddVariable("Bach_P", &others_var[3]);
    others->AddVariable("K0_PT", &others_var[4]);
    //=== Vertex Chi^2 ===//
    others->AddVariable("Bu_VTXCHI2DOF", &others_var[5]);
    //=== Impact parameters ===//
    others->AddVariable("log(D0_MIPCHI2_PV) := log(D0_MIPCHI2_PV)", &others_var[6]);
    others->AddVariable("log(Bach_MIPCHI2_PV) := log(Bach_MIPCHI2_PV)", &others_var[7]);
    others->AddVariable("log(K0_MIPCHI2_PV) := log(K0_MIPCHI2_PV)", &others_var[8]);
    //=== Flight distance ===//
    others->AddVariable("log(Bu_FDCHI2_OWNPV) := log(Bu_FDCHI2_OWNPV)", &others_var[9]);
    others->AddVariable("log(D0_FDCHI2_OWNPV) := log(D0_FDCHI2_OWNPV)", &others_var[10]);
    //=== Director angle (DIRA) ===//
    others->AddVariable("log(1-Bu_DIRA_BPV) := log(1-Bu_DIRA_BPV)", &others_var[11]);
    others->AddVariable("log(1-D0_DIRA_BPV) := log(1-D0_DIRA_BPV)", &others_var[12]);
    //=== P_T asymmetry (ptasy) ===//
    others->AddVariable("Bu_ptasy_1.50", &others_var[13]);  
    //===============================================//
    
    //===============================================================//
    //=== Add the ML variables one wants calculated to the reader ===//
    //===============================================================//
    neut->BookMVA("neutral", "weights/neutAllMC_B2DPi_D2KPiPi0_2012_MagCombined_CombinedCharge_BDT.weights.xml");
    others->BookMVA("others", "weights/Neutral_others_BDT_BDT.weights.xml");
   // reader->BookMVA("full", "weights/NormalBDT_BDT.weights.xml");
    
    
    //========================================================================//
    //=== Set branch addresses for the input tree for which the BDT values ===//
    //=== will be calculated!                                              ===//
    //========================================================================//
    //====================================================//
    //=== NEUTRAL BDT ====================================//
    //=== Particle momenta ===//
    dataTree->SetBranchAddress("Pi0_PT", &neut_uservar["Pi0_PT"]);
    dataTree->SetBranchAddress("Gamma1_PT", &neut_uservar["Gamma1_PT"]);
    dataTree->SetBranchAddress("Gamma2_PT", &neut_uservar["Gamma2_PT"]);
    //=== Neutral particle variables ===//
    dataTree->SetBranchAddress("Gamma1_CL", &neut_uservar["Gamma1_CL"]);
    dataTree->SetBranchAddress("Gamma2_CL", &neut_uservar["Gamma2_CL"]);
    dataTree->SetBranchAddress("Gamma1_PP_IsNotE", &neut_uservar["Gamma1_PP_IsNotE"]);
    dataTree->SetBranchAddress("Gamma2_PP_IsNotE", &neut_uservar["Gamma2_PP_IsNotE"]);
    //====================================================//
    //=== OTHER BDT ======================================//
    //=== Particle momenta ===//
    dataTree->SetBranchAddress("D0_PT", &others_uservar["D0_PT"]);
    dataTree->SetBranchAddress("D0_P", &others_uservar["D0_P"]);
    dataTree->SetBranchAddress("Bach_PT", &others_uservar["Bach_PT"]);
    dataTree->SetBranchAddress("Bach_P", &others_uservar["Bach_P"]);
    dataTree->SetBranchAddress("K0_PT", &others_uservar["K0_PT"]);
    //=== Vertex Chi^2 ===//
    dataTree->SetBranchAddress("Bu_VTXCHI2DOF", &others_uservar["Bu_VTXCHI2DOF"]);
    //=== Impact parameters ===//
    dataTree->SetBranchAddress("D0_MIPCHI2_PV", &others_uservar["D0_MIPCHI2_PV"]);
    dataTree->SetBranchAddress("Bach_MIPCHI2_PV", &others_uservar["Bach_MIPCHI2_PV"]);
    dataTree->SetBranchAddress("K0_MIPCHI2_PV", &others_uservar["K0_MIPCHI2_PV"]);
    //=== Flight distance ===//
    dataTree->SetBranchAddress("Bu_FDCHI2_OWNPV", &others_uservar["Bu_FDCHI2_OWNPV"]);
    dataTree->SetBranchAddress("D0_FDCHI2_OWNPV", &others_uservar["D0_FDCHI2_OWNPV"]);
    //=== Director angle (DIRA) ===//
    dataTree->SetBranchAddress("Bu_DIRA_BPV", &others_uservar["Bu_DIRA_BPV"]);
    dataTree->SetBranchAddress("D0_DIRA_BPV", &others_uservar["D0_DIRA_BPV"]);
    //=== P_T asymmetry (ptasy) ===//
    dataTree->SetBranchAddress("Bu_ptasy_1.50", &others_uservar["Bu_ptasy_1.50"]); 
    //===================================================//
    
    Float_t neutBDT;
    Float_t neut_othersBDT;
    tree->Branch("neutBDT", &neutBDT);
    tree->Branch("neut_othersBDT", &neut_othersBDT);
    
    for (Int_t i=0; i<dataTree->GetEntries(); i++) {
    
        if (i%100000 == 0) std::cout << "--- Processing Event: " << i << std::endl;
        
        dataTree->GetEntry(i);
        
        //=== NEUTRAL ===//
        neut_var[0]  = (Float_t)  neut_uservar["Pi0_PT"];
        neut_var[1]  = (Float_t)  neut_uservar["Gamma1_PT"];
        neut_var[2]  = (Float_t)  neut_uservar["Gamma2_PT"];
        neut_var[3]  = (Float_t)  neut_uservar["Gamma1_CL"];
        neut_var[4]  = (Float_t)  neut_uservar["Gamma2_CL"];
        neut_var[5]  = (Float_t)  neut_uservar["Gamma1_PP_IsNotE"];
        neut_var[6]  = (Float_t)  neut_uservar["Gamma2_PP_IsNotE"];
        
        //=== OTHER ===//
        others_var[0]  = (Float_t)  others_uservar["D0_PT"];
        others_var[1]  = (Float_t)  others_uservar["D0_P"];
        others_var[2]  = (Float_t)  others_uservar["Bach_PT"];
        others_var[3]  = (Float_t)  others_uservar["Bach_P"];
        others_var[4]  = (Float_t)  others_uservar["K0_PT"];
        others_var[5]  = (Float_t)  others_uservar["Bu_VTXCHI2DOF"];
        others_var[6]  = (Float_t)  TMath::Log(others_uservar["D0_MIPCHI2_PV"]);
        others_var[7]  = (Float_t)  TMath::Log(others_uservar["Bach_MIPCHI2_PV"]);
        others_var[8]  = (Float_t)  TMath::Log(others_uservar["K0_MIPCHI2_PV"]);
        others_var[9]  = (Float_t)  TMath::Log(others_uservar["Bu_FDCHI2_OWNPV"]);
        others_var[10] = (Float_t)  TMath::Log(others_uservar["D0_FDCHI2_OWNPV"]);
        others_var[11] = (Float_t)  TMath::Log(1-others_uservar["Bu_DIRA_BPV"]);
        others_var[12] = (Float_t)  TMath::Log(1-others_uservar["D0_DIRA_BPV"]);
        others_var[13] = (Float_t)  others_uservar["Bu_ptasy_1.50"];
       
        //=== Calculate the BDT value using the TMVA::Reader ===//
        neutBDT = neut->EvaluateMVA("neutral");
        neut_othersBDT = others->EvaluateMVA("others");
        //=== Fill the copied tree, which will now become the main analysis tree ===//
        //=== containing the BDT variable for cutting!                           ===//
        tree->Fill();
    }
    
    //=== Save the tree and error plots in opened TFile ===//
    tree->Write();
  //  f->Close();
  g->Close();
}
