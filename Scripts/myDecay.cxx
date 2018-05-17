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
#include <ctime>
#include <time.h>
#include <sys/time.h>

double get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time,NULL)){
        //  Handle error
        return 0;
    }
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

using namespace RooFit ;
#include "myDecay.h"
//=== ROOT needs this ===//
ClassImp(myDecay);

//===============================//
//=== C O N S T R U C T O R S ===//
//===============================//
myDecay::myDecay () {}

myDecay::myDecay(TString x, Int_t y, TString m="unset", TString c="unset") {
    TupleName = x;
    t = new TChain(Form("%s/DecayTree", TupleName.Data()));
    t->SetMaxTreeSize(900000000000LL);
    mc_t = new TChain(Form("%s_MC/DecayTree", TupleName.Data()));
    lumi_t = new TChain("GetIntegratedLuminosity/LumiTuple");
    SetYear(y);
    if (m == "unset") SetMag("Combined");
    else SetMag(m);
    if (c == "unset") SetCharge("Combined");
    else SetCharge(c);
    std::cout << "--- myDecay: Constructor called with parameters:\n"
        << "            TupleName --- " << x << '\n'
        << "            Year      --- " << y << '\n'
        << "            Mag       --- " << GetMag() << '\n'
        << "            Charge    --- " << GetCharge() << std::endl; 
    findTupleCode();
    makeChain((*this).t, (*this).mc_t, (*this).lumi_t);
    SetTMCuts();
}

void myDecay::findTupleCode () {
    if (TupleName == "B2DK_D2KKPi0") {
        TupleCode = 111;
    }
    else if (TupleName == "B2DK_D2KPiPi0") {
        TupleCode = 112;
    }
    else if (TupleName == "B2DK_D2PiKPi0") {
        TupleCode = 121;
    }
    else if (TupleName == "B2DK_D2PiPiPi0") {
        TupleCode = 122;
    }
    else if (TupleName == "B2DPi_D2KKPi0") {
        TupleCode = 211;
    }
    else if (TupleName == "B2DPi_D2KPiPi0") {
        TupleCode = 212;
    }
    else if (TupleName == "B2DPi_D2PiKPi0") {
        TupleCode = 221;
    }
    else if (TupleName == "B2DPi_D2PiPiPi0") {
        TupleCode = 222;
    }
    else std::cout << "--- ERROR: '" << TupleName << "' is not an acceptable TupleName! Please check it is typed correctly and it is a decay which is considered in this analysis!\n";

    std::cout << "--- myDecay: TupleCode set to " << TupleCode << "\n";

    SetSelectionCuts();
    makeLatex();
}

//=== myDecay setter functions ===//
void myDecay::SetMag(TString x) {
    if (x == "Up") {
        eMag = Mag::Up;
        mag = x;
    }
    else if (x == "Down") {
        eMag = Mag::Down;
        mag = x;
    }
    else if (x == "Combined") {
        eMag = Mag::Combined;
        mag = x;
    }
    else std::cout << "--- ERROR: '" << x << "' is not a valid magnet polarity! Please enter 'Up', 'Down', or 'Combined'!" << std::endl;
}

void myDecay::SetYear(Int_t x) {
    if (x == 1) {
        year = 1;
        eYear = Year::r1;
    }
    else if (x == 2) {
        year = 2;
        eYear = Year::r2;
    }
    else if (x == 2011) {
        year = 2011;
        year_s = "2011";
        eYear = Year::y2011;
    }
    else if (x == 2012) {
        year = 2012;
        year_s = "2012";
        eYear = Year::y2012;
    }
    else if (x == 2015) {
        year = 2015;
        year_s = "2015";
        eYear = Year::y2015;
    }
    else if (x == 2016) {
        year = 2016;
        year_s = "2016";
        eYear = Year::y2016;
    }
    else if (x == 2017) {
        year = 2017;
        year_s = "2017";
        eYear = Year::y2017;
    }
    else std::cout << "--- ERROR: Please enter a valid year or run number!\n";
}

void myDecay::SetCharge(TString x) {
    if (x == "Plus") {
        eCharge = Charge::Plus;
        charge = x;
    }
    else if (x == "Minus") {
        eCharge = Charge::Minus;
        charge = x;
    }
    else if (x == "Minus") {
        eCharge = Charge::Both;
        charge = x;
    }
    else if (x == "Combined") {
        eCharge = Charge::Combined;
        charge = x;
    }
    else std::cout << "--- ERROR: '" << x << "' is not a valid charge! Please enter 'Plus', 'Minus' or 'Both'!" << std::endl;
}

void myDecay::SetCuts(TString x) {
    std::cout << "--- myDecay: Setting cuts as " << x << "\n";
    cuts = x;
}

//=== myDecay getter functions ===//
TString myDecay::GetMag() const {
    return mag;
}
Int_t myDecay::GetYear() const {
    return year;
}
TString myDecay::GetCharge() const {
    return charge;
}
TString myDecay::GetTupleName() const {
    return TupleName;
}
Int_t myDecay::GetTupleCode() const {
    return TupleCode;
}
TChain * myDecay::GetChain() const {
    return t;
}

TTree * myDecay::GetReducedTree() const {
    return reduced_tree; 
}

TString myDecay::GetTMCuts() const {
    return TMCuts;
}

Double_t myDecay::GetLumi() const {
    return SumLumi;
}

Double_t myDecay::GetLumiErr() const {
    return SumLumiErr;
}

void myDecay::SetSelectionCuts () {

    //====================
    //=== MASS WINDOWS ===
    //====================
    // D0
    D0_M_Window = "fabs(D0_M - 1865) < 50";
    // B
    Bu_M_Window = "Bu_DPVCFIT_M > 5080 && Bu_DPVCFIT_M < 5900";
    // Pi0
    Pi0_M_Window = "fabs(Pi0_M - 135) < 20";

    //===============
    //=== TRIGGER ===
    //===============
    Trigger_Cuts = "(Bu_L0Global_TIS || Bu_L0HadronDecision_TOS)";        
    if (year==2011 || year==2012 || year==1) {
        Trigger_Cuts += " && Bu_Hlt1TrackAllL0Decision_TOS"; 
        Trigger_Cuts += " && (Bu_Hlt2Topo2BodyBBDTDecision_TOS || Bu_Hlt2Topo3BodyBBDTDecision_TOS || Bu_Hlt2Topo4BodyBBDTDecision_TOS)"; 
    }
    if (year==2015 || year==2016 || year==2017 || year==2) {
        std::cout << "ADD Run2 HLT trigger cut strings!!!!!" << std::endl;
    }

    //===============
    //=== MOMENTA ===
    //===============
    // Bachelor
    Bach_Mom_Cuts = "Bach_P > 5000 && Bach_P < 100000 && Bach_PT > 500 && Bach_PT < 10000";

    //============
    //=== MISC ===
    //============
    // NB add BDT cut in actual script, not here!!!
    Misc_Cuts = "D0_FD_ZSIG > 2";
    Misc_Cuts += " && fabs(D0_SWAP_M - 1865) > 30";


    Int_t TC = GetTupleCode();
    if (TC == 112 || TC == 121 || TC == 212 || TC == 221) {
        PIDK_Cuts = "K0_PIDK > 2 && P0_PIDK < -2";
        Daughter_Mom_Cuts = "K0_PT > 250 && P0_PT > 250";
    }
    else if (TC == 122 || TC == 222) {
        PIDK_Cuts = "P0_PIDK < -2 && P1_PIDK < -2";
        Daughter_Mom_Cuts = "P0_PT > 250 && P1_PT > 250";
    }
    else if (TC == 111 || TC == 211) {
        PIDK_Cuts = "K0_PIDK > 2 && K1_PIDK > 2";
        Daughter_Mom_Cuts = "K0_PT > 250 && K1_PT > 250";
    }
    if (TC == 111 || TC == 112 || TC == 121 || TC == 122) Bach_PIDK_Cuts = "Bach_PIDK > 4";
    else if (TC == 211 || TC == 212 || TC == 221 || TC == 222) Bach_PIDK_Cuts = "Bach_PIDK < 4";

    // Pi0 Mass window removed
    AllCuts = Trigger_Cuts+ And +D0_M_Window+ And +PIDK_Cuts+ And +Bach_PIDK_Cuts+ And +Bu_M_Window+ And +Bach_Mom_Cuts+ And +Misc_Cuts;
    //AllCuts = Trigger_Cuts+ And +D0_M_Window+ And +PIDK_Cuts+ And +Bach_PIDK_Cuts+ And +Bu_M_Window+ And +Pi0_M_Window+ And +Bach_Mom_Cuts+ And +Misc_Cuts;
    //AllCuts = Trigger_Cuts+ And +D0_M_Window+ And +PIDK_Cuts+ And +Bach_PIDK_Cuts+ And +Bu_M_Window+ And +Pi0_M_Window+ And +Pi0_Mom_Cuts+ And +Bach_Mom_Cuts+ And +Daughter_Mom_Cuts+ And +Misc_Cuts;

    std::cout << "--- myDecay: Set PIDK cuts for decay " << TupleName << '\n';

}

TString myDecay::GetSelectionCuts() const {

    std::cout << "--- myDecay: Displaying all cuts\n"
        << "          " << AllCuts << std::endl;

    return AllCuts;

}

void myDecay::makeChain(TChain * chain, TChain* mc_chain, TChain* lumi_chain) {

    //============================================================//
    //=== Function : makeChain                                 ===//
    //============================================================//
    //=== This function simply creates TChains to be used in   ===//
    //=== the filling of histograms by adding together merged  ===//
    //=== ROOT files from the MagUp and MagDown directories in ===//
    //=== my gangadir                                          ===//
    //============================================================//

    TString down_dir;
    TString up_dir;
    Int_t   down_size;
    Int_t   up_size;

    TString mc_down_dir;
    TString mc_up_dir;
    Int_t   mc_down_size;
    Int_t   mc_up_size;

    //=== Set up directory locations and sizes depending on year ===//
    switch (eYear) {

        case (Year::r1) :
            std::cout << "--- myDecay: Adding all Run 1 years to TChain\n";
        case (Year::y2011) :
            std::cout << "--- myDecay: Adding 2011 to TChain\n";
            year_s    = "2011";
            //=== Data ===//
            down_dir  = "352";
            up_dir    = "351";     
            down_size = 473;
            up_size   = 405;
            t = chainLoop(down_dir, up_dir, down_size, up_size, chain);
            lumi_t = chainLoop(down_dir, up_dir, down_size, up_size, lumi_chain);

            //=== MC ===//  (Must be split by TupleName as these MC jobs are run separately)
            if (TupleName == "B2DK_D2KPiPi0") {
                mc_down_dir  = "332";
                mc_up_dir    = "334";
                mc_down_size = 15;
                mc_up_size   = 14;
            }
            if (TupleName == "B2DPi_D2KPiPi0") {
                mc_down_dir  = "329";
                mc_up_dir    = "331";
                mc_down_size = 13;
                mc_up_size   = 16;
            }
            mc_t = chainLoop(mc_down_dir, mc_up_dir, mc_down_size, mc_up_size, mc_chain, true);
            if (year == 2011) break;
            // Reassinging the chain to be added to as the previous
            // result allows to recursively add years to the total
            // run chain.
            chain = t;
            mc_chain = mc_t;
            lumi_chain = lumi_t;

        case (Year::y2012) :
            std::cout << "--- myDecay: Adding 2012 to TChain\n";
            year_s    = "2012";
            down_dir  = "354";
            up_dir    = "353";     
            down_size = 597;
            up_size   = 689;
            t = chainLoop(down_dir, up_dir, down_size, up_size, chain);
            lumi_t = chainLoop(down_dir, up_dir, down_size, up_size, lumi_chain);

            //=== MC ===//
            if (TupleName == "B2DK_D2KPiPi0") {
                mc_down_dir  = "327";
                mc_up_dir    = "313";
                mc_down_size = 30;
                mc_up_size   = 29;
            }
            if (TupleName == "B2DK_D2PiPiPi0") {
                mc_down_dir  = "245";
                mc_up_dir    = "249";
                mc_down_size = 12;
                mc_up_size   = 14;
            }
            if (TupleName == "B2DK_D2KKPi0") {
                mc_down_dir  = "244";
                mc_up_dir    = "271";
                mc_down_size = 12;
                mc_up_size   = 13;
            }
            if (TupleName == "B2DPi_D2KPiPi0") {
                mc_down_dir  = "312";
                mc_up_dir    = "314";
                mc_down_size = 31;
                mc_up_size   = 31;
            }
            if (TupleName == "B2DPi_D2PiPiPi0") {
                mc_down_dir  = "247";
                mc_up_dir    = "251";
                mc_down_size = 13;
                mc_up_size   = 12;
            }
            if (TupleName == "B2DPi_D2KKPi0") {
                mc_down_dir  = "246";
                mc_up_dir    = "250";
                mc_down_size = 11;
                mc_up_size   = 12;
            }
            mc_t = chainLoop(mc_down_dir, mc_up_dir, mc_down_size, mc_up_size, mc_chain, true);
            break;

        case (Year::r2) :
            std::cout << "--- myDecay: Adding all Run 2 years to TChain\n";
        case (Year::y2015) :
            std::cout << "--- myDecay: Adding 2015 to TChain\n";
            year_s    = "2015";
            down_dir  = "356";
            up_dir    = "355";     
            down_size = 880;
            up_size   = 509;
            t = chainLoop(down_dir, up_dir, down_size, up_size, chain);
            lumi_t = chainLoop(down_dir, up_dir, down_size, up_size, lumi_chain);

            //=== MC ===//
            if (TupleName == "B2DK_D2KPiPi0") {
                mc_down_dir  = "307";
                mc_up_dir    = "308";
                mc_down_size = 11;  
                mc_up_size   = 11;
            }
            if (TupleName == "B2DPi_D2KPiPi0") {
                mc_down_dir  = "295";
                mc_up_dir    = "296";
                mc_down_size = 10;
                mc_up_size   = 10;
            }
            mc_t = chainLoop(mc_down_dir, mc_up_dir, mc_down_size, mc_up_size, mc_chain, true);

            if (year == 2015) break;
            chain = t;
            mc_chain = mc_t;
            lumi_chain = lumi_t;

        case (Year::y2016) :
            std::cout << "--- myDecay: Adding 2016 to TChain\n";
            year_s    = "2016";
            down_dir  = "358";
            up_dir    = "357";
            down_size = 4406;
            up_size   = 3959;
            t = chainLoop(down_dir, up_dir, down_size, up_size, chain);
            lumi_t = chainLoop(down_dir, up_dir, down_size, up_size, lumi_chain);

            //=== MC ===//
            if (TupleName == "B2DK_D2KPiPi0") {
                mc_down_dir  = "303";
                mc_up_dir    = "304";
                mc_down_size = 39;
                mc_up_size   = 37;
            }
            if (TupleName == "B2DPi_D2KPiPi0") {
                mc_down_dir  = "299";
                mc_up_dir    = "300";
                mc_down_size = 37;
                mc_up_size   = 37;
            }
            mc_t = chainLoop(mc_down_dir, mc_up_dir, mc_down_size, mc_up_size, mc_chain, true);
            if (year == 2016) break;
            chain = t;
            mc_chain = mc_t;
            lumi_chain = lumi_t;

        case (Year::y2017) :
            std::cout << "--- myDecay: Adding 2017 to TChain\n";
            year_s    = "2017";
            down_dir  = "183";
            up_dir    = "184";
            down_size = 713;
            up_size   = 223;
            t = chainLoop(down_dir, up_dir, down_size, up_size, chain);
            lumi_t = chainLoop(down_dir, up_dir, down_size, up_size, lumi_chain);
            break;

    }
}

TChain* myDecay::chainLoop(TString down_dir, TString up_dir, Int_t down_size, Int_t up_size, TChain* &chain, bool sim) {

    TString tag;
    TString path;
    TFile* null_test;

    if (sim) tag = "MC_Bu2" + TupleName(2,20);
    else tag = "Data_Bu2DH_AllModes";

    // Open a file to record to location of null pointers,
    // which are files which had problems downloading.
    std::ofstream myfile;
    myfile.open("/home/colmmurphy/null_subjobs.txt", ios::app);
    
    switch(eMag) {
        case Mag::Combined :
            std::cout << "--- myDecay: Will add both MagUp and MagDown directories to TChain!\n";
        case Mag::Down :
            std::cout << "--- myDecay: Adding MagDown directory to TChain\n";
            for (int i=0 ; i<down_size ; i++) {
                std::string s = to_string(i);
                path = Form("/data/lhcb/users/colmmurphy/gangadir/workspace/colmmurphy/LocalXML/"+down_dir+"/%s/output/%s_%s_MagDown.root", s.c_str(), tag.Data(), year_s.Data());
                null_test = TFile::Open(path);
                if (null_test != NULL && !(null_test->TestBit(TFile::kRecovered))) {
                   chain->Add(path);
                   null_test->Close();
                }
                   
                else myfile << down_dir << '/' << s << std::endl;
            }
            // Here switch/case is being used as a jump table
            // which will add both up and down directories if
            // MagCombined is specified. If it just requires Down
            // then the statement is broken and Up will not be
            // added
            if (mag == "Down") break;
        case Mag::Up :
            std::cout << "--- myDecay: Adding MagUp directory to TChain\n";
            for (int i=0 ; i<up_size ; i++) {
                std::string s = to_string(i);
                path = Form("/data/lhcb/users/colmmurphy/gangadir/workspace/colmmurphy/LocalXML/"+up_dir+"/%s/output/%s_%s_MagUp.root", s.c_str(), tag.Data(), year_s.Data());
                null_test = TFile::Open(path);
                if (null_test != NULL && !(null_test->TestBit(TFile::kRecovered))) {
                    chain->Add(path);
                    null_test->Close();
                }
                else myfile << up_dir << '/' << s << std::endl;
            }
    }

    myfile.close();

    return chain;
}

void myDecay::makeLatex() {

    // B^{-}#rightarrow[#pi^{-}K^{+}#pi^{0}]_{D}#pi^{-}

    //=== This function creates the correct latex string for a given decay/charge combination ===//
    //=== The following will take different values depending on charge ===//
    TString B_Meson;
    TString K_same;
    TString K_diff;
    TString Pi_same;
    TString Pi_diff;
    TString Pi_0 = "#pi^{0}";

    switch(eCharge) {
        case Charge::Combined :
            B_Meson = "B^{#pm}"; // i.e. B^+/-
            K_same = "K^{#pm}";
            K_diff = "K^{#mp}";
            Pi_same = "#pi^{#pm}";
            Pi_diff = "#pi^{#mp}";
            break;

        case Charge::Both :
        case Charge::Plus :
            B_Meson = "B^{+}"; // i.e. B^+/-
            K_same = "K^{+}";
            K_diff = "K^{-}";
            Pi_same = "#pi^{+}";
            Pi_diff = "#pi^{-}";
            if (charge == "Plus") break;

        case Charge::Minus :
            B_Meson = "B^{-}"; // i.e. B^+/-
            K_same = "K^{-}";
            K_diff = "K^{+}";
            Pi_same = "#pi^{-}";
            Pi_diff = "#pi^{+}";
            break;

    }

    TString A = B_Meson + "#rightarrow[";
    TString B;
    TString C;
    TString D = "#pi^{0}]_{D}";
    TString Bach;

    if (TupleCode == 111 || TupleCode == 112 || TupleCode == 121 || TupleCode == 122) {
        Bach = K_same;
    }
    if (TupleCode == 211 || TupleCode == 212 || TupleCode == 221 || TupleCode == 222) {
        Bach = Pi_same;
    }
    if (TupleCode == 111 || TupleCode == 112 || TupleCode == 211 || TupleCode == 212) {
        B = K_same;
    }
    if (TupleCode == 121 || TupleCode == 122 || TupleCode == 221 || TupleCode == 222) {
        B = Pi_same;
    }
    if (TupleCode == 111 || TupleCode == 121 || TupleCode == 211 || TupleCode == 221) {
        C = K_diff;
    }
    if (TupleCode == 112 || TupleCode == 122 || TupleCode == 212 || TupleCode == 222) {
        C = Pi_diff;
    }

    TString Full_Latex = A+B+C+D+Bach;
    std::cout << "--- myDecay: LaTeX string set to " << Full_Latex << "\n";
    if (charge == "Plus" || charge == "Both") latex_p = Full_Latex;
    else if (charge == "Minus" || charge == "Both") latex_m = Full_Latex;
    else if (charge == "Combined") latex_comb = Full_Latex;


}

void myDecay::SetTMCuts(TString cut_type) {

    // This function sets the cuts required for truth matching,
    // which is used when branches are added to TM simulation


    if (!(cut_type=="BKGCAT")) {

        TString Common = "Bach_MC_MOTHER_KEY == D0_MC_MOTHER_KEY && abs(Bach_MC_MOTHER_ID) == 521 && abs(D0_MC_MOTHER_ID) == 521 && abs(Pi0_MC_GD_MOTHER_ID) == 521 && abs(Pi0_MC_MOTHER_ID) == 421 && Gamma1_TRUEID==22 && Gamma2_TRUEID==22 && Pi0_TRUEID==111 && Gamma1_MC_MOTHER_ID==111 && Gamma2_MC_MOTHER_ID==111 && Gamma1_MC_MOTHER_KEY == Gamma2_MC_MOTHER_KEY && abs(Bu_TRUEID)==521 && abs(D0_TRUEID)==421 && Gamma1_MC_GD_MOTHER_KEY == Gamma2_MC_GD_MOTHER_KEY && abs(Gamma1_MC_GD_MOTHER_ID)==421 && abs(Gamma2_MC_GD_MOTHER_ID)==421";

        TString Depends;
        TString Bach_cut;
        if (TupleCode == 112 || TupleCode == 212 || TupleCode == 121 || TupleCode == 221) Depends = "abs(K0_MC_MOTHER_ID) == 421 && abs(P0_MC_MOTHER_ID) == 421 && abs(K0_MC_GD_MOTHER_ID) == 521 && abs(P0_MC_GD_MOTHER_ID) == 521  && K0_MC_MOTHER_KEY == P0_MC_MOTHER_KEY && P0_MC_MOTHER_KEY == Pi0_MC_MOTHER_KEY && K0_MC_GD_MOTHER_KEY == P0_MC_GD_MOTHER_KEY && P0_MC_GD_MOTHER_KEY == Pi0_MC_GD_MOTHER_KEY && D0_MC_MOTHER_KEY == K0_MC_GD_MOTHER_KEY && Gamma1_MC_GD_MOTHER_KEY == P0_MC_MOTHER_KEY && abs(K0_TRUEID)==321 && abs(P0_TRUEID)==211";

        if (TupleCode == 111 || TupleCode == 211) Depends = "abs(P0_MC_MOTHER_ID) == 421 && abs(P1_MC_MOTHER_ID) == 421 && abs(P0_MC_GD_MOTHER_ID) == 521 && abs(P1_MC_GD_MOTHER_ID) == 521  && P0_MC_MOTHER_KEY == P1_MC_MOTHER_KEY && P0_MC_MOTHER_KEY == Pi0_MC_MOTHER_KEY && P0_MC_GD_MOTHER_KEY == P1_MC_GD_MOTHER_KEY && P0_MC_GD_MOTHER_KEY == Pi0_MC_GD_MOTHER_KEY && D0_MC_MOTHER_KEY == P0_MC_GD_MOTHER_KEY && Gamma1_MC_GD_MOTHER_KEY == P0_MC_MOTHER_KEY && abs(P0_TRUEID)==211 && abs(P1_TRUEID)==211";

        if (TupleCode == 122 || TupleCode == 222) Depends = "abs(K0_MC_MOTHER_ID) == 421 && abs(K1_MC_MOTHER_ID) == 421 && abs(K0_MC_GD_MOTHER_ID) == 521 && abs(K1_MC_GD_MOTHER_ID) == 521  && K0_MC_MOTHER_KEY == K1_MC_MOTHER_KEY && K0_MC_MOTHER_KEY == Pi0_MC_MOTHER_KEY && K0_MC_GD_MOTHER_KEY == K1_MC_GD_MOTHER_KEY && K0_MC_GD_MOTHER_KEY == Pi0_MC_GD_MOTHER_KEY && D0_MC_MOTHER_KEY == K0_MC_GD_MOTHER_KEY && Gamma1_MC_GD_MOTHER_KEY == K0_MC_MOTHER_KEY && abs(K0_TRUEID)==321 && abs(K1_TRUEID)==321";

        // Add Bach ID cuts
        if (TupleCode == 111 || TupleCode == 112 || TupleCode == 121 || TupleCode == 122) Bach_cut = "abs(Bach_TRUEID)==321";
        if (TupleCode == 211 || TupleCode == 212 || TupleCode == 221 || TupleCode == 222) Bach_cut = "abs(Bach_TRUEID)==211";

        TMCuts = Common + " && " + Bach_cut + " && "+ Depends;
        std::cout << "--- myDecay: Using full chain ID matching for truth match" << std::endl;
        TMLabel = "ID/Key Truth Match";
    }
    else {
        TMCuts = "Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0";
        std::cout << "--- myDecay: Using BKGCAT requirements for truth match" << std::endl;
    }
}

void myDecay::CutsForBDT() {

    std::cout << "--- myDecay: Performing cuts on TChain to create background sample for BDT training!" << std::endl;
    // Attempt to open consolidated TTree file
    TFile* g = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()));
    if ( g->IsZombie() ) {
        // File doesn't exist yet
        // --- code to create consolidated ttree ---
        // Consolidated TTree file has now been created, so can recursively call
        // this function again
        //(*this).CutsForBDT();
    }
    TTree* oldTree = (TTree*)g->Get("DecayTree");

    // Open file to save cut TTree
    TFile* f = new TFile(Form("/data/lhcb/users/colmmurphy/BDT_Output/%i/Mag%s/%sCharge/bkgcut_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()), "RECREATE");
    std::cout << "          Please be patient..." << std::endl;

    // Ensure sample is in upper sideband
    TString All = "Bu_DPVCFIT_M>5900.";
    // Ensure sample passes the trigger requirements
    All        += " && (Bu_L0HadronDecision_TOS || Bu_L0Global_TIS)";
    // Run 1 specific trigger names
    if (year==2011 || year==2012) {
        //All += " && Bu_Hlt1TrackAllL0Decision_TOS";
        //All += " && (Bu_Hlt2Topo2BodyBBDTDecision_TOS || Bu_Hlt2Topo3BodyBBDTDecision_TOS || Bu_Hlt2Topo4BodyBBDTDecision_TOS";
    } 
    All        += " && D0_MIPCHI2_PV>0. && Bach_MIPCHI2_PV>0. && Bu_FDCHI2_OWNPV>0. && D0_FDCHI2_OWNPV>0. && Bu_DIRA_BPV<1. && D0_DIRA_BPV<1.";

    TString Depends;

    // This section is no longer needed as the BDT will 
    // probably not include charged D daughter variables 
    //=== KPiPi0 ===//
    /*
    if (TupleCode == 112 || TupleCode == 121 || TupleCode == 212 || TupleCode == 221) {
        Depends = "K0_MIPCHI2_PV>0. && P0_MIPCHI2_PV>0.";
    }

    //=== PiPiPi0 ===//
    if (TupleCode == 122 || TupleCode == 222) { 
        Depends = "P0_MIPCHI2_PV>0. && P1_MIPCHI2_PV>0.";
    }

    //=== KKPi0 ===//
    if (TupleCode == 111 || TupleCode == 211) {
        Depends = "K0_MIPCHI2_PV>0. && K1_MIPCHI2_PV>0.";
    }
    */
    //TString Complete = All + " && " + Depends; 
    
    TString Complete = All; 
    std::cout << "Cuts: " << Complete.Data() << std::endl;

    TTree* tree = oldTree->CopyTree(Complete);

    std::cout << "          Finished applying cuts! Writing file on datadisk in 'BDT_Output' directory." << std::endl;
    std::cout << "          Events in this background sample: " << tree->GetEntries() << std::endl;
    tree->Write();
    f->Close();

}


void myDecay::addBranches() {

    //=========================//
    //=== Add new branches! ===//
    //=========================//
    // UNCOMMENT this line to add new branches to the reduced TTree!
    //TFile * g = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Reduced_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()), "update");
    //TTree * newTree = (TTree*)g->Get("DecayTree");

    // UNCOMMENT this line to add new branches the the FULL TTree!
    //TFile * g = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()), "recreate");

    // TEMPORARILY CHANGED FOR 4/5/18 MEETING, ON PARTIALLY
    // FINISHED 2011 FILE
    TFile* g = new TFile("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_new.root", "update");
    TTree* newTree = (TTree*)g->Get("DecayTree");
    // TTree * newTree = t->CloneTree(-1);
    std::cout << "--- myDecay: (addBranches)\tCloned tree!" << std::endl;

    //===========================//
    //=== Assign branch names ===//
    //===========================//
    TBranch * b_d0_fd_zsig = newTree->Branch("D0_FD_ZSIG", &D0_FD_ZSIG, "D0_FD_ZSIG/D"); 
    TBranch * m_12 = newTree->Branch("M_KP", &M_KP, "M_KP/D");
    TBranch * m_13 = newTree->Branch("M_KPi0", &M_KPi0, "M_KPi0/D");
    TBranch * m_23 = newTree->Branch("M_PPi0", &M_PPi0, "M_PPi0/D");
    TBranch * swp_m = newTree->Branch("D0_SWAP_M", &D0_SWAP_M, "D0_SWAP_M/D");

    //===================================================================//
    //=== Set branch addresses needed for calculation of new branches ===//
    //===================================================================//   
    newTree->SetBranchAddress("D0_ENDVERTEX_Z", &D0_ENDVERTEX_Z);
    newTree->SetBranchAddress("D0_ENDVERTEX_ZERR", &D0_ENDVERTEX_ZERR);
    newTree->SetBranchAddress("Bu_ENDVERTEX_Z", &Bu_ENDVERTEX_Z);
    newTree->SetBranchAddress("Bu_ENDVERTEX_ZERR", &Bu_ENDVERTEX_ZERR);

    newTree->SetBranchAddress("P0_PE", &P0_PE);
    newTree->SetBranchAddress("P0_PX", &P0_PX);
    newTree->SetBranchAddress("P0_PY", &P0_PY);
    newTree->SetBranchAddress("P0_PZ", &P0_PZ);

    newTree->SetBranchAddress("K0_PE", &K0_PE);
    newTree->SetBranchAddress("K0_PX", &K0_PX);
    newTree->SetBranchAddress("K0_PY", &K0_PY);
    newTree->SetBranchAddress("K0_PZ", &K0_PZ);

    newTree->SetBranchAddress("Pi0_PE", &Pi0_PE);
    newTree->SetBranchAddress("Pi0_PX", &Pi0_PX);
    newTree->SetBranchAddress("Pi0_PY", &Pi0_PY);
    newTree->SetBranchAddress("Pi0_PZ", &Pi0_PZ);

    newTree->SetBranchAddress("D0_M", &D0_M);

    //===============================//
    //=== Calculate new Branches! ===//
    //===============================//
    float progress = 0.;
    for (Int_t i = 0; i < newTree->GetEntries(); i++) {

        progress = ((float(i)/(newTree->GetEntries()))*100);
        if (i%100000 == 0) std::cout << "Entry number: " << i << "\t\t(" << std::setprecision(3) << progress << " percent done)\n";


        newTree->GetEntry(i);

        D0_FD_ZSIG = (D0_ENDVERTEX_Z - Bu_ENDVERTEX_Z)/TMath::Sqrt(D0_ENDVERTEX_ZERR*D0_ENDVERTEX_ZERR + Bu_ENDVERTEX_ZERR*Bu_ENDVERTEX_ZERR);  
        M_KP = ((K0_PE + P0_PE)*(K0_PE + P0_PE)) - ( (K0_PX + P0_PX)*(K0_PX + P0_PX) + (K0_PY + P0_PY)*(K0_PY + P0_PY) + (K0_PZ + P0_PZ)*(K0_PZ + P0_PZ) );
        M_KPi0 = ((K0_PE + Pi0_PE)*(K0_PE + Pi0_PE)) - ( (K0_PX + Pi0_PX)*(K0_PX + Pi0_PX) + (K0_PY + Pi0_PY)*(K0_PY + Pi0_PY) + (K0_PZ + Pi0_PZ)*(K0_PZ + Pi0_PZ) );
        M_PPi0 = ((P0_PE + Pi0_PE)*(P0_PE + Pi0_PE)) - ( (P0_PX + Pi0_PX)*(P0_PX + Pi0_PX) + (P0_PY + Pi0_PY)*(P0_PY + Pi0_PY) + (P0_PZ + Pi0_PZ)*(P0_PZ + Pi0_PZ) );

        // Swapped mass hyp:
        PK = TVector3(K0_PX, K0_PY, K0_PZ);
        PP = TVector3(P0_PX, P0_PY, P0_PZ);
        PiP = TVector3(Pi0_PX, Pi0_PY, Pi0_PZ);

        K0_PE = TMath::Sqrt(P_Mass*P_Mass + PK*PK);
        P0_PE = TMath::Sqrt(K_Mass*K_Mass + PP*PP);
        Pi0_PE = TMath::Sqrt(Pi0_Mass*Pi0_Mass + PiP*PiP);

        P4_K = TLorentzVector(PK, K0_PE);
        P4_P = TLorentzVector(PP, P0_PE);
        P4_Pi = TLorentzVector(PiP, Pi0_PE);

        P = P4_K + P4_P + P4_Pi;
        D0_SWAP_M = P.M();

        if (i%100000 == 0) {
            std::cout << "D0_FD_ZSIG: " << D0_FD_ZSIG << std::endl;
            std::cout << "M_KP = " << M_KP << std::endl;
            std::cout << "M_KPi0 = " << M_KPi0 << std::endl;
            std::cout << "M_PPi0 = " << M_PPi0 << std::endl;
            std::cout << "D0_SWAP_M = " << D0_SWAP_M << std::endl;
        }

        b_d0_fd_zsig->Fill();
        m_12->Fill();
        m_13->Fill();
        m_23->Fill();
        swp_m->Fill();
    }

    newTree->Write();
    g->Close();
}

void myDecay::findLumi() {

    lumi_t->SetBranchAddress("IntegratedLuminosity", &IntegratedLuminosity);
    lumi_t->SetBranchAddress("IntegratedLuminosityErr", &IntegratedLuminosityErr);

    Double_t lumi = 0;
    Double_t lumi_err = 0;

    for ( Int_t i = 0; i < lumi_t->GetEntries(); i++) {

        lumi_t->GetEntry(i);
        lumi = lumi + IntegratedLuminosity;
        lumi_err = lumi_err + IntegratedLuminosityErr;

    }

    SumLumi = lumi;
    SumLumiErr = lumi_err;


}

void myDecay::addBranchesMC(bool full) {

    //=========================//
    //=== Add new branches! ===//
    //=========================//
    // UNCOMMENT this line to add new branches to the reduced TTree!
    //TFile * g = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Reduced_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()), "update");
    //TTree * newTree = (TTree*)g->Get("DecayTree");

    // Bool switch "full": run on the full MC (true) or just the truth matched MC (false)

    if (full) std::cout << "--- myDecay: (addBranchesMC)\tCreating full MC tree! (no truth matching)" << std::endl;
    if (!full) std::cout << "--- myDecay: (addBranchesMC)\tCreating (ID/Key) truth matched MC tree!" << std::endl;

    TFile* g;
    TTree* newTree;
    if (full) {
        g = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s_MC.root", year, mag.Data(), charge.Data(), TupleName.Data()), "recreate");
        newTree = mc_t->CloneTree(-1);
    }
    else {
        g = new TFile(Form("~/TruthMatchedTrees/%i/Mag%s/%sCharge/TM_%s_MC.root", year, mag.Data(), charge.Data(), TupleName.Data()), "recreate");
        newTree = mc_t->CopyTree(TMCuts);
    }
    std::cout << "--- myDecay: (addBranchesMC)\tCloned simulation tree!" << std::endl;

    //===========================//
    //=== Assign branch names ===//
    //===========================//
    TBranch * b_d0_fd_zsig = newTree->Branch("D0_FD_ZSIG", &D0_FD_ZSIG, "D0_FD_ZSIG/D"); 
    TBranch * m_12 = newTree->Branch("M_KP", &M_KP, "M_KP/D");
    TBranch * m_13 = newTree->Branch("M_KPi0", &M_KPi0, "M_KPi0/D");
    TBranch * m_23 = newTree->Branch("M_PPi0", &M_PPi0, "M_PPi0/D");  
    TBranch * swp_m = newTree->Branch("D0_SWAP_M", &D0_SWAP_M, "D0_SWAP_M/D");

    //===================================================================//
    //=== Set branch addresses needed for calculation of new branches ===//
    //===================================================================//   
    newTree->SetBranchAddress("D0_ENDVERTEX_Z", &D0_ENDVERTEX_Z);
    newTree->SetBranchAddress("D0_ENDVERTEX_ZERR", &D0_ENDVERTEX_ZERR);
    newTree->SetBranchAddress("Bu_ENDVERTEX_Z", &Bu_ENDVERTEX_Z);
    newTree->SetBranchAddress("Bu_ENDVERTEX_ZERR", &Bu_ENDVERTEX_ZERR);

    newTree->SetBranchAddress("P0_PE", &P0_PE);
    newTree->SetBranchAddress("P0_PX", &P0_PX);
    newTree->SetBranchAddress("P0_PY", &P0_PY);
    newTree->SetBranchAddress("P0_PZ", &P0_PZ);

    newTree->SetBranchAddress("K0_PE", &K0_PE);
    newTree->SetBranchAddress("K0_PX", &K0_PX);
    newTree->SetBranchAddress("K0_PY", &K0_PY);
    newTree->SetBranchAddress("K0_PZ", &K0_PZ);

    newTree->SetBranchAddress("Pi0_PE", &Pi0_PE);
    newTree->SetBranchAddress("Pi0_PX", &Pi0_PX);
    newTree->SetBranchAddress("Pi0_PY", &Pi0_PY);
    newTree->SetBranchAddress("Pi0_PZ", &Pi0_PZ);

    newTree->SetBranchAddress("D0_M", &D0_M);


    //===============================//
    //=== Calculate new Branches! ===//
    //===============================//
    float progress = 0.;
    for (Int_t i = 0; i < newTree->GetEntries(); i++) {


        if (i%5000 == 0) {
            progress = ((float(i)/(newTree->GetEntries()))*100);
            std::cout << "---------------------" << std::endl;
            std::cout << "Entry number: " << i << "\t\t(" << std::setprecision(3) << progress << " percent done)\n";
        }

        newTree->GetEntry(i);

        D0_FD_ZSIG = (D0_ENDVERTEX_Z - Bu_ENDVERTEX_Z)/TMath::Sqrt(D0_ENDVERTEX_ZERR*D0_ENDVERTEX_ZERR + Bu_ENDVERTEX_ZERR*Bu_ENDVERTEX_ZERR);  
        M_KP = ((K0_PE + P0_PE)*(K0_PE + P0_PE)) - ( (K0_PX + P0_PX)*(K0_PX + P0_PX) + (K0_PY + P0_PY)*(K0_PY + P0_PY) + (K0_PZ + P0_PZ)*(K0_PZ + P0_PZ) );
        M_KPi0 = ((K0_PE + Pi0_PE)*(K0_PE + Pi0_PE)) - ( (K0_PX + Pi0_PX)*(K0_PX + Pi0_PX) + (K0_PY + Pi0_PY)*(K0_PY + Pi0_PY) + (K0_PZ + Pi0_PZ)*(K0_PZ + Pi0_PZ) );
        M_PPi0 = ((P0_PE + Pi0_PE)*(P0_PE + Pi0_PE)) - ( (P0_PX + Pi0_PX)*(P0_PX + Pi0_PX) + (P0_PY + Pi0_PY)*(P0_PY + Pi0_PY) + (P0_PZ + Pi0_PZ)*(P0_PZ + Pi0_PZ) );

        // Swapped mass hyp:
        PK = TVector3(K0_PX, K0_PY, K0_PZ);
        PP = TVector3(P0_PX, P0_PY, P0_PZ);
        PiP = TVector3(Pi0_PX, Pi0_PY, Pi0_PZ);

        K0_PE = TMath::Sqrt(P_Mass*P_Mass + PK*PK);
        P0_PE = TMath::Sqrt(K_Mass*K_Mass + PP*PP);
        Pi0_PE = TMath::Sqrt(Pi0_Mass*Pi0_Mass + PiP*PiP);

        P4_K = TLorentzVector(PK, K0_PE);
        P4_P = TLorentzVector(PP, P0_PE);
        P4_Pi = TLorentzVector(PiP, Pi0_PE);

        P = P4_K + P4_P + P4_Pi;
        D0_SWAP_M = P.M();

        if (i%5000 == 0) {
            std::cout << "D0_FD_ZSIG: " << D0_FD_ZSIG << std::endl;
            std::cout << "M_KP = " << M_KP << std::endl;
            std::cout << "M_KPi0 = " << M_KPi0 << std::endl;
            std::cout << "M_PPi0 = " << M_PPi0 << std::endl;
            std::cout << "---------------------" << std::endl;
        }

        b_d0_fd_zsig->Fill();
        m_12->Fill();
        m_13->Fill();
        m_23->Fill();
        swp_m->Fill();
    }

    if (full) std::cout << "--- myDecay: # entries after DaVinci is " << newTree->GetEntries() << std::endl;
    if (!full) std::cout << "--- myDecay: # entries after truth matching is " << newTree->GetEntries() << std::endl;
    newTree->Write();
    g->Close();
}

void myDecay::sameCand() {

    //===================================================================//
    //=== Create an embedded map:                                     ===//
    //===-------------------------------------------------------------===//
    //=== candMap   [Event Number] [Run Number] [Location(s) in Tree] ===//
    //===================================================================//


    //=== Open reduced tree file to get TTree from which we want to remove multiple candidates ===//
    TFile * f = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Reduced_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()));
    TTree * tr = (TTree*)f->Get("DecayTree"); 


    tr->SetBranchAddress("runNumber", &runNumber);
    std::cout << "a\n";
    tr->SetBranchAddress("eventNumber", &eventNumber);

    std::map <ULong64_t, std::map <ULong64_t, std::vector<Int_t>>>   candMap;

    //=== Fill the map ===//
    for ( Int_t i=0 ; i<tr->GetEntries() ; i++) {
        tr->GetEntry(i);
        candMap[runNumber][eventNumber].push_back(i); 
    }
    std::cout << "b\n";


    TFile * f1 = new TFile(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Reduced_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()), "RECREATE");
    TTree * newTree = tr->CloneTree(0);


    //=== Fill + Save a histogram with multiplicity information, i.e. size of 3rd map element ===//  
    TH1D * h1 = new TH1D("h_Multiplicity", "Event Number Multiplicity", 7, 0, 7);

    //=== Want to copy the input chain, and output a chain where duplicate Bs have been rand- ===//
    //=== onmly selected.

    typedef std::map <ULong64_t, std::map <ULong64_t, std::vector<Int_t>>>::const_iterator MapIterator;
    std::cout << "=== Map values ===" << std::endl;
    int count =0;
    for (MapIterator it = candMap.begin() ; it != candMap.end(); it++) {


        // std::cout << "Run Number: " << it->first << std::endl << "------------------";

        typedef std::map <ULong64_t, std::vector<Int_t>>::const_iterator InnerIterator;

        for (InnerIterator it2 = it->second.begin() ; it2 != it->second.end(); it2++) {
            //   std::cout << std::endl << "Event Number: " << it2->first;
            int vec_len = it2->second.size();
            h1->Fill(vec_len);       

            //=== Copy Tree if size of vector is 1 (i.e. one B cand. only!) ===//
            if (vec_len == 1) {

                tr->GetEntry(it2->second[0]);
                newTree->Fill();

            }

            //=== If there is more than one candidate, pick one at random ===//
            else if (vec_len > 1) {

                //=== Generate a random element index ===//
                int random = 0;
                random = rand() % vec_len;

                tr->GetEntry(it2->second[random]);
                newTree->Fill();


            }

            //=== Iterate over vector contents! ===//
            typedef std::vector <Int_t>::const_iterator VectorIterator;

            std:: cout << " Locations: ";
            for (VectorIterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {

                std::cout << " " << *it3;


            }
        }
        std::cout << std::endl;
    }

    h1->Write();
    // Overwrite previous tree with new tree which has multiple candidates removed
    newTree->Write();
    f1->Close();
    f->Close();

}





