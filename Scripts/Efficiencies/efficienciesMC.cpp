#include "myDecay.cxx"

double efficienciesMC (myDecay decay) {

    int year = decay.GetYear();
    TString mag = decay.GetMag();
    TString charge = decay.GetCharge();
    TString name = decay.GetTupleName();
    int code = decay.GetTupleCode();

    TFile* full   = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s_MC.root", year, mag.Data(), charge.Data(), name.Data()));
    TTree* full_t = (TTree*)full->Get("DecayTree");
    //=========================================
    // Now try using BKGCAT instead of the "full chain" truth matching, to see if this changes the overall 
    // stripping efficiency numbers
    
    full_t->SetBranchStatus("*", 0);
    full_t->SetBranchStatus("Bu_BKGCAT", 1);
    full_t->SetBranchStatus("Pi0_BKGCAT", 1);
    full_t->SetBranchStatus("D0_BKGCAT", 1);
    TTree* tm_t = full_t->CopyTree("Bu_BKGCAT==0 && Pi0_BKGCAT==0 && D0_BKGCAT==0");
    
    //=========================================
    // Uncomment for the "full chain" truth matching method
    TFile* tm_cut = TFile::Open(Form("~/TruthMatchedTrees/%i/Mag%s/%sCharge/TM_%s_MC.root", year, mag.Data(), charge.Data(), name.Data()));
    TTree* tm_t2   = (TTree*)tm_cut->Get("DecayTree");
    //=========================================
    int gen = 0;
    
    if (year==2011 && code==112) gen = 334298;
    if (year==2011 && code==212) gen = 340576;
    if (year==2012 && code==112) gen = 658135;
    if (year==2012 && code==212) gen = 664299;
    if (year==2015 && code==112) gen = 212515;
    if (year==2015 && code==212) gen = 209181;
    if (year==2016 && code==112) gen = 810340;
    if (year==2016 && code==212) gen = 805570;

    // Calculate and return stripping effiency
    double e_strip = double(tm_t->GetEntries())/gen;
    double e_strip2 = double(tm_t2->GetEntries())/gen;
    double e_ratio  = double(e_strip2)/e_strip;

    //==================================================================================================//
    //=== Print mode, year, number after davinci, number after truth match, and stripping efficiency ===//
    //==================================================================================================//
    std::cout << "\n" << "\033[1;31m(Mode)\033[0m\t" << decay.GetTupleName() << '\t' << "\033[1;31m(Year)\033[1;0m\t" << decay.GetYear() << '\t'  << "\033[1;31m(Gen)\033[0m\t" << gen << '\t'  << "\033[1;31m(BKGCAT)\033[0m\t" << tm_t->GetEntries() << " " << decay.mc_t->GetEntries("Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0") << "\033[1;33m\t" << double(tm_t->GetEntries())/gen << "\t\033[1;31m(IDs)\033[0m\t" << tm_t2->GetEntries() << " " << decay.mc_t->GetEntries(decay.GetTMCuts()) << "\033[1;33m\t" << double(tm_t2->GetEntries())/gen << "\033[0m\t" << "\033[1;31m(Ratio)\033[0m\t" << e_ratio <<std::endl;   

    //=============================================================//
    //=== Print mode, year, and ratio of BKGCAT/TM efficiencies ===//
    //=============================================================//
    //std::cout << "\n" << "\033[1;31m(Mode)\033[0m\t" << decay.GetTupleName() << '\t' << "\033[1;31m(Year)\033[1;0m\t" << decay.GetYear() << '\t'  << "\033[1;33m" << (double(tm_t->GetEntries()))/(double(tm_t2->GetEntries())) << std::endl;

    

    return e_strip;

}
