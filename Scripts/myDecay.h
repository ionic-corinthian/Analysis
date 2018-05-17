#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TChain.h"
#include <iostream>
#include "TString.h"
#include <TH1.h>
#include <TAttFill.h>
#include <TCanvas.h>

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "TAxis.h"
#include <vector>
#include <cstring>
#include <fstream>
#include <string>
#include <stdio.h>
#include <map>
#include <iterator>

class myDecay {
    
    enum class Charge {
    Plus,
    Minus,
    Both,
    Combined
    };
    
    enum class Mag {
    Up,
    Down,
    Combined
    };
    
    enum class Year {
    y2011,
    y2012,
    y2015,
    y2016,
    y2017,
    r1,
    r2
    };
    
    //=== Needed for swapped mass calc ===//
    Double_t K_Mass = 493.677;
    Double_t P_Mass = 139.570;
    Double_t Pi0_Mass = 134.977;
    TVector3 PK;
    TVector3 PP;
    TVector3 PiP;
    TLorentzVector P4_K;
    TLorentzVector P4_P;
    TLorentzVector P4_Pi;
    TLorentzVector P;

    //=== Name of decay set via constructor ===//
    TString      TupleName;
    Int_t        TupleCode;
    
    //=== Variables which distinguish data ===//
    Int_t       year;
    TString     year_s;
    Year        eYear;
    TString     mag;
    Mag         eMag;
    TString     charge;
    Charge      eCharge;
    
    
    //=== String to hold cuts for reducting trees etc ===//
    TString     cuts;
    
    //=== Lumis ===//
    // Branch names
    Double_t IntegratedLuminosity = 0;
    Double_t IntegratedLuminosityErr = 0;
    // Sum names
    Double_t SumLumi = 0;
    Double_t SumLumiErr = 0;
    
  public:
  
    //=== All possible branch names go here ===//
    Bool_t      Bu_L0Global_TIS = 0;
    Bool_t      Bu_L0HadronDecision_TOS = 0;
    Double_t    BDT = 0;
    Double_t    D0_M = 0;
    Double_t    Pi0_M = 0;
    Double_t    Bach_PIDK = 0;
    Double_t    K0_PIDK = 0;
    Double_t    K1_PIDK = 0;
    Double_t    P0_PIDK = 0;
    Double_t    P1_PIDK = 0;
    Double_t    Bu_M = 0;
    Float_t     Bu_DPVCFIT_M = 0;
    Double_t    D0_FD_ZSIG = 0;
    Double_t    M_KP = 0;
    Double_t    M_KPi0 = 0;
    Double_t    M_PPi0 = 0;
    Int_t       Bu_ID = 0;
    Double_t    Bu_ENDVERTEX_Z = 0;
    Double_t    Bu_ENDVERTEX_ZERR = 0;
    Double_t    D0_ENDVERTEX_Z = 0;
    Double_t    D0_ENDVERTEX_ZERR = 0;
    ULong64_t   eventNumber = 0;
    UInt_t      runNumber = 0;
    Double_t    K0_PE = 0;
    Double_t    K0_PX = 0;
    Double_t    K0_PY = 0;
    Double_t    K0_PZ = 0;
    Double_t    P0_PE = 0;
    Double_t    P0_PX = 0;
    Double_t    P0_PY = 0;
    Double_t    P0_PZ = 0;
    Double_t    Pi0_PE = 0;
    Double_t    Pi0_PX = 0;
    Double_t    Pi0_PY = 0;
    Double_t    Pi0_PZ = 0;
    Double_t    D0_SWAP_M = 0;
    

    //=== LaTeX identifiers ===//
    TString     latex_p;
    TString     latex_m;
    TString     latex_comb;

    //=== It is convenient to have an identifier string for use in filenames etc ===//
    TString     identifier;
    TString     ReducedFlag = "Full";
    TString     TMLabel="BKGCAT Truth Match";
  
    //=== TChain containing data according to mag/year choices ===//
    TChain *    t;
    TChain *    mc_t;
    TTree *     reduced_tree;
    TChain *    lumi_t;
    
    //=== Histograms generated/used by analysis ===//
    TH1D *      h_p;
    TH1D *      h_m;
    TH1D *      h_comb;
    
    //============//
    //=== Cuts ===//
    //============//
    TString Trigger_Cuts = "(Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1)";
    TString D0_M_Window = "fabs(D0_M - 1864) < 50";
    TString PIDK_Cuts;      // These must be defined within the loop
    TString Bach_PIDK_Cuts; // as they depend on the TupleName
    TString Bu_M_Window = "Bu_DPVCFIT_M<5900 && Bu_DPVCFIT_M>5080";
    TString Pi0_M_Window = "Pi0_M>115 && Pi0_M < 155";
    TString Pi0_Mom_Cuts = "Pi0_PT > 500 && Pi0_P > 1000";
    TString Bach_Mom_Cuts = "Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000";
    TString Daughter_Mom_Cuts; // Also depends on the specific decay
    TString D0_FD_ZSIG_Cuts = "D0_FD_ZSIG > 2";
    TString Misc_Cuts;
    TString AllCuts;
    TString And = " && ";
    
    TString TMCuts;
    
    //==============================================//
    //=== Public member function signatures here ===//
    //==============================================//
    
    //=== Constructors ===//
    myDecay     ();
    myDecay     (TString, Int_t, TString, TString);
    
    //=== Setters ===//
    void        SetMag  (TString);
    void        SetYear (Int_t);
    void        SetCharge (TString);
    void        SetCuts (TString);
    void        SetSelectionCuts ();
    void        SetTMCuts(TString="BKGCAT");
    
    //=== Getters ===//
    TString     GetMag()        const;
    Int_t       GetYear()       const;
    TString     GetCharge()     const;
    TString     GetTupleName()  const;
    Int_t       GetTupleCode()  const;
    TChain *    GetChain()      const;
    TTree *     GetReducedTree() const;
    TString     GetSelectionCuts()    const;
    TString     GetTMCuts()     const;
    Double_t    GetLumi()       const;
    Double_t    GetLumiErr()    const;
   
    //=== Functions for analysis ===//
    void findTupleCode ();
    void makeChain (TChain*, TChain*, TChain*);
    TChain * chainLoop (TString, TString, Int_t, Int_t, TChain*&, bool = false);
    void moveChain(myDecay&);
    void reduceTrees ();
    void addBranches ();
    void addBranchesMC (bool);
    void sameCand();
    void fitHist(TString);
    void autoFit ();
    void makeLatex();
    void CutsForBDT();
    void findLumi();
    
    //=== Root needs this apparently ===//
    ClassDef(myDecay, 1);
};
