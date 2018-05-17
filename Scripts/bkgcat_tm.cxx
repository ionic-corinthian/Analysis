void bkgcat_tm(int year, TString Tuple, TString mag="Combined", TString charge="Combined") {
    // A simple script to perform BKGCAT
    // based truth matching on MC files

    TFile* old_file = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s_MC.root", year, mag.Data(), charge.Data(), Tuple.Data()));    
    TTree* old_tree = (TTree*)old_file->Get("DecayTree");
   
    TFile* new_file = new TFile(Form("/home/colmmurphy/TruthMatchedTrees/%i/Mag%s/%sCharge/Full_%s_MC.root", year, mag.Data(), charge.Data(), Tuple.Data()), "recreate");
    TTree* new_tree = old_tree->CopyTree("Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0");
    new_tree->Write();
    std::cout << "Performed BKGCAT truth match on " << year << " " << Tuple.Data() << " (with Mag = " << mag.Data() << " and Charge = " << charge.Data() << ')' << std::endl;
    std::cout << "Events before truth match: " << old_tree->GetEntries() << '\n';
    std::cout << "Events after  truth match: " << new_tree->GetEntries() << std::endl;
    old_file->Close();
    new_file->Close();
}
