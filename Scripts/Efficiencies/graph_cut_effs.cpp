#include "cut_effs.cpp"

void graph_cut_effs() {

  // A simple script to create a TGraph
  // for efficiency of a given cut versus
  // year
  //
  // Interfaces with the cut_effs.cpp script

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

  std::vector <int> years = {2011, 2012, 1, 2015, 2016, 2, 3};
  std::vector <double> x;
  std::vector <double> y;
  std::vector <TGraph*> gr;
  TGraph* temp;
  TText* t;
  TCanvas* c1;
  int n=0;
  double minimum=0;
  double maximum=0;
 
  for (cut_iter = cut_array.begin(); cut_iter != cut_array.end(); cut_iter++) {
    for (int i=0; i<years.size(); i++) {
      x.push_back(n+1);
      y.push_back(std::get<3>(cut_effs(years[i], cut_iter->first)));
      n++;
    }
    c1 = new TCanvas();
    temp =  new TGraph(n, &x[0], &y[0]);
    temp->SetTitle(cut_iter->second+" efficiencies;;Efficiency");
    temp->SetName(cut_iter->second);
    temp->GetYaxis()->SetTitleOffset(1.4);
    minimum = *(std::min_element(std::begin(y), std::end(y)));
    maximum = *(std::max_element(std::begin(y), std::end(y)));
    double range = maximum;
    range = 0.005*range;
    double upper = (1+range)*maximum;
    double lower = (1-range)*minimum;
    if (upper > 1) upper=1; 
    temp->GetYaxis()->SetRangeUser(lower, upper);
    temp->GetYaxis()->SetNdivisions(10);
    temp->GetXaxis()->SetNdivisions(years.size());
    temp->GetXaxis()->SetAlphanumeric();
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(1), "2011");
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(2), "2012");
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(3), "Run 1");
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(4), "2015");
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(5), "2016");
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(6), "Run 2");
    temp->GetXaxis()->SetBinLabel(temp->GetXaxis()->FindBin(7), "Run 1 + 2");
    temp->GetXaxis()->CenterLabels();
    temp->GetXaxis()->SetLabelSize(0.05);
    temp->GetXaxis()->LabelsOption("h");
    temp->SetFillColor(kBlue+1);
    gr.push_back(temp);
    // Reset variables for next loop
    minimum=0;
    maximum=0;
    n=0;
    x.clear();
    y.clear();
  }
  for (std::vector<TGraph*>::const_iterator gr_iter=gr.begin(); gr_iter!=gr.end(); gr_iter++) {
    c1 = new TCanvas();
    (*gr_iter)->Draw("AB");
    c1->SaveAs(Form("/home/colmmurphy/Plots/Kpipi0/Efficiency_Graph_%s.pdf", (*gr_iter)->GetName()), "pdf");
  }
}
