#include "cut_effs.cpp"


template <typename T> void printElement(T t, const int& width, char fill_char) {
  std::cout << left << setw(width) << setfill(fill_char) << t;
}

void table_cut_effs() {

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

  // Find longest name in std::map and use that
  // to define the width of the table
  double maximum=0;
  for (cut_iter=cut_array.begin(); cut_iter!=cut_array.end(); cut_iter++) {
    if ( cut_iter->second.Length() > maximum ) maximum = cut_iter->second.Length();
  }
  //  maximum += 2;

  // Get coloured text
  TString red = "\033[1;31m";
  TString yellow= "\033[1;33m";
  TString end = "\033[0m";

  // Vectors of run numbers and corresponding
  // strings
  std::vector <int> runs = {2011, 2012, 1, 2015, 2016, 2, 3};
  std::vector <TString> run_name = {"2011", "2012", "Run 1", "2015", "2016", "Run 2", "All"};
  std::vector <int>::const_iterator run_it;
  std::vector <TString>::const_iterator name_it;

  // Spaces
  int modifier = 2;
  int cut_space = maximum +modifier;
  int natm_space = 15+modifier;
  int nac_space = 16+modifier;
  int diff_space = 10+modifier;
  int eff_space = 10+modifier;

  for (int i=0; i<runs.size(); i++) {
    std::cout << "\n====== "<< run_name[i] << " "; printElement('=', cut_space+natm_space+nac_space+diff_space+eff_space-8-run_name[i].Length(), '=');
    std::cout << '\n' << red;
    printElement("Cut", cut_space, ' '); printElement("Number after TM", natm_space, ' '); printElement("Number after cut", nac_space, ' '); printElement("Difference", diff_space, ' '); printElement("Efficiency", eff_space, ' ');
    std::cout << end << '\n';
    printElement('=', cut_space+natm_space+nac_space+diff_space+eff_space, '=');
    std::cout << '\n';
    for (cut_iter = cut_array.begin(); cut_iter != cut_array.end(); cut_iter++) {
      output = cut_effs(runs[i], cut_iter->first);
      std::cout << red;
      printElement(cut_iter->second, cut_space, ' ');
      std::cout << end;
      std::cout << yellow;
      printElement(std::get<0>(output), natm_space, ' ');
      printElement(std::get<1>(output), nac_space, ' ');
      printElement(std::get<2>(output), diff_space, ' ');
      printElement(std::get<3>(output), eff_space, ' ');
      std::cout << end;
      std::cout << std::endl;
    }
  }
}
