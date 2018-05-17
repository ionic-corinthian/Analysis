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

void apply_neut_others_bdt(const char* TupleName, int year, const char* mag, const char* charge) {

    //==============================//
    //=== Setup tree to apply to ===//
    //==============================//
    //int year = 2011;
    //const char* charge = "Combined";
    //const char* mag = "Combined";
    //const char* TupleName = "B2DK_D2KPiPi0_MC"; // append _MC for simulation
    
    //=== Get tree ===//
    TFile* g = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s.root", year, mag, charge, TupleName));
    TTree* dataTree = (TTree*)g->Get("DecayTree");
    
    //=== Create output file ===//
    TFile* f = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/neut_othersBDT_%s.root", year, mag, charge, TupleName), "RECREATE");
    TTree* tree   = dataTree->CloneTree(0);
    
    TMVA::Tools::Instance();
    TMVA::Reader *reader = new TMVA::Reader( "V:Color:!Silent" );
    
    //===============================================================================//
    //=== Create a map to make AddVariable and SetBranchAddress processes clearer ===//
    //===============================================================================//
    std::map < std::string , Double_t > uservar;
    uservar["D0_PT"];
    uservar["D0_P"];
    uservar["Bach_PT"];
    uservar["Bach_P"];
    uservar["K0_PT"];
    uservar["K0_P"];
    uservar["P0_PT"];
    uservar["P0_P"];
    uservar["Pi0_PT"];
    uservar["Pi0_PX"];
    uservar["Pi0_PY"];
    uservar["Gamma1_CL"];
    uservar["Gamma2_CL"];
    uservar["Bu_VTXCHI2DOF"];
    uservar["D0_VTXCHI2DOF"];
    uservar["Bu_MIPCHI2_PV"];
    uservar["D0_MIPCHI2_PV"];
    uservar["Bach_MIPCHI2_PV"];
    uservar["K0_MIPCHI2_PV"];
    uservar["P0_MIPCHI2_PV"];
    uservar["Bu_FDCHI2_OWNPV"];
    uservar["D0_FDCHI2_OWNPV"];
    uservar["Bu_DIRA_BPV"];
    uservar["D0_DIRA_BPV"];
    uservar["Bu_MAXDOCA"];
    uservar["D0_AMAXDOCA"];
    uservar["Bu_LT_BPV"];
    uservar["D0_LT_BPV"];
    uservar["Bu_ptasy_1.50"];
    
    //=============================================================================//
    //=== Add the variables needed for the BDT calculation to the TMVA::Reader  ===//
    //=== which will use them in conjunction with the weights file to calculate ===//
    //=== a value for the BDT variable                                          ===//
    //=============================================================================//
    Float_t var[14];
       
    //=== Particle momenta ===//
    reader->AddVariable("D0_PT", &var[0]);
    reader->AddVariable("D0_P", &var[1]);
    reader->AddVariable("Bach_PT", &var[2]);
    reader->AddVariable("Bach_P", &var[3]);
    reader->AddVariable("K0_PT", &var[4]);
    /*
    //reader->AddVariable("K0_P", &var[5]);
    //reader->AddVariable("P0_PT", &var[6]);
    //reader->AddVariable("P0_P", &var[7]);
    reader->AddVariable("Pi0_PT", &var[0]);
    reader->AddVariable("Pi0_PX", &var[1]);
    reader->AddVariable("Pi0_PY", &var[2]);
    //reader->AddVariable("Pi0_P", &var[6]);
    //=== Neutral particle variables ===//
    reader->AddVariable("Gamma1_CL", &var[3]);
    reader->AddVariable("Gamma2_CL", &var[4]);
    */
    //=== Vertex Chi^2 ===//
    reader->AddVariable("Bu_VTXCHI2DOF", &var[5]);
    //reader->AddVariable("D0_VTXCHI2DOF", &var[13]);
    //=== Impact parameters ===//
    //reader->AddVariable("Bu_MIPCHI2_PV", &var[14]);
    reader->AddVariable("log(D0_MIPCHI2_PV) := log(D0_MIPCHI2_PV)", &var[6]);
    reader->AddVariable("log(Bach_MIPCHI2_PV) := log(Bach_MIPCHI2_PV)", &var[7]);
    reader->AddVariable("log(K0_MIPCHI2_PV) := log(K0_MIPCHI2_PV)", &var[8]);
    //reader->AddVariable("log(P0_MIPCHI2_PV) := log(P0_MIPCHI2_PV)", &var[18]);
    //=== Flight distance ===//
    reader->AddVariable("log(Bu_FDCHI2_OWNPV) := log(Bu_FDCHI2_OWNPV)", &var[9]);
    reader->AddVariable("log(D0_FDCHI2_OWNPV) := log(D0_FDCHI2_OWNPV)", &var[10]);
    //=== Director angle (DIRA) ===//
    reader->AddVariable("log(1-Bu_DIRA_BPV) := log(1-Bu_DIRA_BPV)", &var[11]);
    reader->AddVariable("log(1-D0_DIRA_BPV) := log(1-D0_DIRA_BPV)", &var[12]);
    //=== Distance of closest approach (DOCA) ===//
    //reader->AddVariable("Bu_MAXDOCA", &var[23]);
    //reader->AddVariable("D0_AMAXDOCA", &var[24]);
    //=== Lifetimes (LT) ===//
    //reader->AddVariable("Bu_LT_BPV", &var[25]);
    //reader->AddVariable("D0_LT_BPV", &var[17]);
    //=== P_T asymmetry (ptasy) ===//
    reader->AddVariable("Bu_ptasy_1.50", &var[13]);  
    
    
    //===============================================================//
    //=== Add the ML variables one wants calculated to the reader ===//
    //===============================================================//
   // reader->BookMVA("neutral", "weights/neutAllMC_B2DPi_D2KPiPi0_2012_MagCombined_CombinedCharge_BDT.weights.xml");
    reader->BookMVA("others", "weights/Neutral_others_BDT_BDT.weights.xml");
   // reader->BookMVA("full", "weights/NormalBDT_BDT.weights.xml");
    
    
    //========================================================================//
    //=== Set branch addresses for the input tree for which the BDT values ===//
    //=== will be calculated!                                              ===//
    //========================================================================//
    //=== Particle momenta ===//
    dataTree->SetBranchAddress("D0_PT", &uservar["D0_PT"]);
    dataTree->SetBranchAddress("D0_P", &uservar["D0_P"]);
    dataTree->SetBranchAddress("Bach_PT", &uservar["Bach_PT"]);
    dataTree->SetBranchAddress("Bach_P", &uservar["Bach_P"]);
    dataTree->SetBranchAddress("K0_PT", &uservar["K0_PT"]);
    dataTree->SetBranchAddress("K0_P", &uservar["K0_P"]);
    dataTree->SetBranchAddress("P0_PT", &uservar["P0_PT"]);
    dataTree->SetBranchAddress("P0_P", &uservar["P0_P"]);
    dataTree->SetBranchAddress("Pi0_PX", &uservar["Pi0_PX"]);
    dataTree->SetBranchAddress("Pi0_PY", &uservar["Pi0_PY"]);
    
    dataTree->SetBranchAddress("Pi0_PT", &uservar["Pi0_PT"]);
    dataTree->SetBranchAddress("Pi0_P", &uservar["Pi0_P"]);
    //=== Neutral particle variables ===//
    dataTree->SetBranchAddress("Gamma1_CL", &uservar["Gamma1_CL"]);
    dataTree->SetBranchAddress("Gamma2_CL", &uservar["Gamma2_CL"]);
    //=== Vertex Chi^2 ===//
    dataTree->SetBranchAddress("Bu_VTXCHI2DOF", &uservar["Bu_VTXCHI2DOF"]);
    dataTree->SetBranchAddress("D0_VTXCHI2DOF", &uservar["D0_VTXCHI2DOF"]);
    //=== Impact parameters ===//
    dataTree->SetBranchAddress("Bu_MIPCHI2_PV", &uservar["Bu_MIPCHI2_PV"]);
    dataTree->SetBranchAddress("D0_MIPCHI2_PV", &uservar["D0_MIPCHI2_PV"]);
    dataTree->SetBranchAddress("Bach_MIPCHI2_PV", &uservar["Bach_MIPCHI2_PV"]);
    dataTree->SetBranchAddress("K0_MIPCHI2_PV", &uservar["K0_MIPCHI2_PV"]);
    dataTree->SetBranchAddress("P0_MIPCHI2_PV", &uservar["P0_MIPCHI2_PV"]);
    //=== Flight distance ===//
    dataTree->SetBranchAddress("Bu_FDCHI2_OWNPV", &uservar["Bu_FDCHI2_OWNPV"]);
    dataTree->SetBranchAddress("D0_FDCHI2_OWNPV", &uservar["D0_FDCHI2_OWNPV"]);
    //=== Director angle (DIRA) ===//
    dataTree->SetBranchAddress("Bu_DIRA_BPV", &uservar["Bu_DIRA_BPV"]);
    dataTree->SetBranchAddress("D0_DIRA_BPV", &uservar["D0_DIRA_BPV"]);
    //=== Distance of closest approach (DOCA) ===//
    dataTree->SetBranchAddress("Bu_MAXDOCA", &uservar["Bu_MAXDOCA"]);
    dataTree->SetBranchAddress("D0_AMAXDOCA", &uservar["D0_AMAXDOCA"]);
    //=== Lifetimes (LT) ===//
    dataTree->SetBranchAddress("Bu_LT_BPV", &uservar["Bu_LT_BPV"]);
    dataTree->SetBranchAddress("D0_LT_BPV", &uservar["D0_LT_BPV"]);
    //=== P_T asymmetry (ptasy) ===//
    dataTree->SetBranchAddress("Bu_ptasy_1.50", &uservar["Bu_ptasy_1.50"]);  

    Float_t neut_othersBDT;
    //tree->Branch("neutBDT", &neutBDT);
    tree->Branch("neut_othersBDT", &neut_othersBDT);
    //tree->Branch("fullBDT", &fullBDT);
    
    //========================================================================================//
    //=== Histogram defined for holding which variables cause a failure: see comment below ===//
    //========================================================================================//
    
    /*TH1D * h1 = new TH1D("Entries failing","h1", 8, 15,22);
    h1->GetXaxis()->SetTitle("User Var #");
    h1->GetYaxis()->SetTitle("Count");
    */
    
    for (Int_t i=0; i<dataTree->GetEntries(); i++) {
    
        if (i%100000 == 0) std::cout << "--- Processing Event: " << i << std::endl;
        
        dataTree->GetEntry(i);
        
        //=====================================================//
        //=== Check if you're about to log a negative value ===//
        //=== and skip event if so!                         ===//
        //=====================================================//====================//
        //=== Fill histogram with a number of the variable which causes a failure ===//
        //===========================================================================//
        //=== NOTE: This should now be superfluous as I have applied extra        ===//
        //=== "bkgcuts" in the bdt_train.cxx script which makes sure none of the  ===//
        //=== following entries are added in the first place!                     ===//
        //===========================================================================//       
     /*
        if ( uservar[15] < 0 ) {std::cout << "15 Failed" << std::endl; h1->Fill(15); continue;}
        if ( uservar[16] < 0 ) {std::cout << "16 Failed" << std::endl; h1->Fill(16); continue;}
        if ( uservar[17] < 0 ) {std::cout << "17 Failed" << std::endl; h1->Fill(17); continue;}
        if ( uservar[18] < 0 ) {std::cout << "18 Failed" << std::endl; h1->Fill(18); continue;}
        if ( uservar[19] < 0 ) {std::cout << "19 Failed" << std::endl; h1->Fill(19); continue;}
        if ( uservar[20] < 0 ) {std::cout << "20 Failed" << std::endl; h1->Fill(20); continue;}
        if ( uservar[21] > 1 ) {std::cout << "21 Failed" << std::endl; h1->Fill(21); continue;}        
        if ( uservar[22] > 1 ) {std::cout << "22 Failed" << std::endl; h1->Fill(22); continue;}
        
        */
        //============================================================================//
        //=== In the event loop, we assign the TMVA::Reader's variables by casting ===//
        //=== our input tree branches which are used in the BDT value calculation  ===//
        //=== as a float                                                           ===//
        //============================================================================//
        
        var[0]  = (Float_t)  uservar["D0_PT"];
        var[1]  = (Float_t)  uservar["D0_P"];
        var[2]  = (Float_t)  uservar["Bach_PT"];
        var[3]  = (Float_t)  uservar["Bach_P"];
        var[4]  = (Float_t)  uservar["K0_PT"];
        //var[5]  = (Float_t)  uservar["K0_P"];
        //var[6]  = (Float_t)  uservar["P0_PT"];
        //var[7]  = (Float_t)  uservar["P0_P"];
        /*
        var[0]  = (Float_t)  uservar["Pi0_PT"];
        var[1]  = (Float_t)  uservar["Pi0_PX"];
        var[2]  = (Float_t)  uservar["Pi0_PY"];
        var[3] = (Float_t)  uservar["Gamma1_CL"];
        var[4] = (Float_t)  uservar["Gamma2_CL"];
        */
        var[5] = (Float_t)  uservar["Bu_VTXCHI2DOF"];
        //var[13] = (Float_t)  uservar["D0_VTXCHI2DOF"];
        //var[14] = (Float_t)  uservar["Bu_MIPCHI2_PV"];
        var[6] = (Float_t)  TMath::Log(uservar["D0_MIPCHI2_PV"]);
        var[7] = (Float_t)  TMath::Log(uservar["Bach_MIPCHI2_PV"]);
        var[8] = (Float_t)  TMath::Log(uservar["K0_MIPCHI2_PV"]);
        //var[18] = (Float_t)  TMath::Log(uservar["P0_MIPCHI2_PV"]);
        var[9] = (Float_t)  TMath::Log(uservar["Bu_FDCHI2_OWNPV"]);
        var[10] = (Float_t)  TMath::Log(uservar["D0_FDCHI2_OWNPV"]);
        var[11] = (Float_t)  TMath::Log(1-uservar["Bu_DIRA_BPV"]);
        var[12] = (Float_t)  TMath::Log(1-uservar["D0_DIRA_BPV"]);
        //var[23] = (Float_t)  uservar["Bu_MAXDOCA"];
        //var[24] = (Float_t)  uservar["D0_AMAXDOCA"];
        //var[25] = (Float_t)  uservar["Bu_LT_BPV"];
        //var[17] = (Float_t)  uservar["D0_LT_BPV"];
        var[13] = (Float_t)  uservar["Bu_ptasy_1.50"];
        
       
        //=== Calculate the BDT value using the TMVA::Reader ===//
     //   neutBDT = reader->EvaluateMVA("neutral");
        neut_othersBDT = reader->EvaluateMVA("others");
      //  fullBDT = reader->EvaluateMVA("full");
        //=== Fill the copied tree, which will now become the main analysis tree ===//
        //=== containing the BDT variable for cutting!                           ===//
        tree->Fill();
    }
    
    //=== Save the tree and error plots in opened TFile ===//
    //h1->Draw();
    //h1->Write();
    tree->Write();
    f->Close();
}
