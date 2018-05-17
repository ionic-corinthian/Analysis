#include "cut_effs.cpp"


template <typename T> void printElement(T t, const int& width) {
  std::cout << left << setw(width) << setfill(' ') << t;
}

void table_cut_effs_runs() {

  // A simple script to print a table of 
  // cuts and their efficiencies on truth
  // matched data
  //
  // This script interfaces with cut_effs.cpp
  // which takes the run number and cut
  // as arguments

  using vals = std::tuple<double, double, double, double>;
  vals output;

  // Make a map of cuts and the associated cut name 
  std::map <TString, TString> cut_array = {
    { "Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0", "B mass window" },
    { "fabs(D0_M-1865) < 50", "D0 mass window" },
    { "fabs(Pi0_M-135) < 20", "Pi0 mass window" },
    { "K0_PIDK > 2", "K0 PIDK"},
    { "P0_PIDK < -2", "P0 PIDK"},
    { "Bach_PIDK < 4", "Bach (Pi) PIDK"},
    { "Bach_P > 5000 && Bach_P < 100000 && Bach_PT > 500 && Bach_PT < 10000", "Bach momentum cuts"},
    { "(Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1)", "Trigger cuts"},
    { "D0_FD_ZSIG > 2", "D0 FDZSIG"},
    { "fabs(D0_SWAP_M-1865)>30", "Swapped D0 window"},
    { "FullBDT_2011 > 0.0", "BDT cut (0.0)"}
  };

  std::map <TString, TString>::const_iterator cut_iter;

  // Get coloured text
  TString red = "\033[1;31m";
  TString yellow= "\033[1;33m";
  TString end = "\033[0m";

  for (int run=1; run<4; run++) {
    std::cout << "====== RUN " << run << " ======\n";
    std::cout << red;
    printElement("Cut", 20); printElement("Number after TM", 20); printElement("Number after cut", 20); printElement("Difference", 20); printElement("Efficiency", 20);
    std::cout << end << '\n';
    for (cut_iter = cut_array.begin(); cut_iter != cut_array.end(); cut_iter++) {
      output = cut_effs(run, cut_iter->first);
      std::cout << red;
      printElement(cut_iter->second, 20);
      std::cout << end;
      std::cout << yellow;
      printElement(std::get<0>(output), 20);
      printElement(std::get<1>(output), 20);
      printElement(std::get<2>(output), 20);
      printElement(std::get<3>(output), 20);
      std::cout << end;
      std::cout << std::endl;
    }
    temp = new TGraph(n,&x[0],&y[0]);
    temp->Draw("AC*");
    //gr.push_back(TGraph(n,x,y));

  }
}
