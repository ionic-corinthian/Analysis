std::tuple<double, double, double, double> cut_effs_runs(int run, TString cut, bool verbose=false) {

  // A simple script to print the number of 
  // entries that a cut removes after truth
  // matching has been done
  //int run = 1;
  //TString cut = "Bu_DPVCFIT_M>5080 && Bu_DPVCFIT_M<5900";
  TString tm_cut = "Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0";

  TChain* t = new TChain("DecayTree");
  if (run==1 || run==3) {
    t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
  }
  if (run==2 || run==3) { 
    t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
  }

  double n_before = t->GetEntries(tm_cut);
  double n_after = t->GetEntries(tm_cut+" && "+cut);
  double diff = n_before - n_after; 
  double eff = double(n_after)/n_before;

  if (verbose) {
    std::cout << "CUT           : TM + " << cut.Data() << '\n'
      << "Entries before: " << n_before << '\n'
      << "Entries after : " << n_after << '\n'
      << "DIFFERENCE    : " << diff << '\n'
      << "EFFICIENCY    : " << eff << std::endl;
  }
  return std::make_tuple(n_before, n_after, diff, eff);
}
