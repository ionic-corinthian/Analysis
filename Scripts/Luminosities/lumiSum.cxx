#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include <iostream>
#include "TString.h"
#include <TH1.h>
#include <TAttFill.h>
#include <TCanvas.h>

#include "myDecay.h"

void lumiSum() {

   
    

    Double_t IntegratedLuminosity = 0;
    Double_t IntegratedLuminosityErr = 0;
    
   TChain * ch = new TChain("GetIntegratedLuminosity/LumiTuple");


    for (int i=0 ; i<713 ; i++) {
        std::string s = to_string(i);
        ch->Add(Form("/data/lhcb/users/colmmurphy/gangadir/workspace/colmmurphy/LocalXML/183/%s/output/Data_Bu2DH_AllModes_2017_MagDown.root", s.c_str()), -1);
    }

    // i < 213
    // i < 1316
    for (int i=0 ; i<223 ; i++) {
        std::string s = to_string(i);
        ch->Add(Form("/data/lhcb/users/colmmurphy/gangadir/workspace/colmmurphy/LocalXML/184/%s/output/Data_Bu2DH_AllModes_2017_MagUp.root", s.c_str()), -1);
    }
    
   
   ch->SetBranchAddress("IntegratedLuminosity", &IntegratedLuminosity);
   ch->SetBranchAddress("IntegratedLuminosityErr", &IntegratedLuminosityErr);
   
   Double_t lumi = 0;
   Double_t lumi_err = 0;
   
   for ( Int_t i = 0; i < ch->GetEntries(); i++) {
   
        ch->GetEntry(i);
        lumi = lumi + IntegratedLuminosity;
        lumi_err = lumi_err + IntegratedLuminosityErr;
   
   }
   
   std::cout << "The TChain integrated luminosity is: " << lumi << " in inv. picobarns" << std::endl;
   std::cout << "The error is: " << lumi_err << std::endl;
   
  /* TFile * f1 = new TFile(upDir);
   TFile * f2 = new TFile(downDir);
   
   TTree * t1 = (TTree*)f1->Get("GetIntegratedLuminosity/LumiTuple");
   TTree * t2 = (TTree*)f2->Get("GetIntegratedLuminosity/LumiTuple");
   
   t1->SetBranchAddress("IntegratedLuminosity", &IntegratedLuminosity);
   t1->SetBranchAddress("IntegratedLuminosityErr", &IntegratedLuminosityErr);
   t2->SetBranchAddress("IntegratedLuminosity", &IntegratedLuminosity);
   t2->SetBranchAddress("IntegratedLuminosityErr", &IntegratedLuminosityErr);
   
    lumi = 0;
    lumi_err = 0;
   
   for ( Int_t i = 0; i < t1->GetEntries(); i++) {
   
        t1->GetEntry(i);
        lumi = lumi + IntegratedLuminosity;
        lumi_err = lumi_err + IntegratedLuminosityErr;
   
   }
   
      std::cout << "The Mag Up integrated luminosity is: " << lumi << " in inv. picobarns" << std::endl;
   std::cout << "The error is: " << lumi_err << std::endl;
   
   
    lumi = 0;
    lumi_err = 0;
   
    for ( Int_t i = 0; i < t2->GetEntries(); i++) {
   
        t2->GetEntry(i);
        lumi = lumi + IntegratedLuminosity;
        lumi_err = lumi_err + IntegratedLuminosityErr;
   
   }
   
      std::cout << "The Mag Down integrated luminosity is: " << lumi << " in inv. picobarns" << std::endl;
   std::cout << "The error is: " << lumi_err << std::endl;
   */
   
}
