#include "cut_effs.cpp"

using info = std::tuple < std::vector<TString>, std::vector<TString>, std::vector<double>,  std::vector<double>, std::vector<double> >;

void filler (info &a, TString a_name, TString a_cut, double start, double end, double step) {
  // Shortcut function for filling entries into the 'matrix'
  std::get<0>(a).push_back(a_name);
  std::get<1>(a).push_back(a_cut);
  std::get<2>(a).push_back(start);
  std::get<3>(a).push_back(end);
  std::get<4>(a).push_back(step);
}

void vary_cut_eff() {

  // A simple script to create graphs 
  // showing the variation of efficiency
  // with differing values of a given
  // cut.
  //
  // Interfaces with cut_effs.cpp

  std::vector <double> x; 
  std::vector <double> y; 
  std::vector <TGraph*> gr; 
  std::vector <TGraph*>::const_iterator gr_it; 
  std::vector <TPaveText*> t1;
  TPaveText* temp_t;
  TGraph* temp;
  TString cut;
  TCanvas* c1;

  // Storage of information needed for 
  // looping over all cuts
  info c;

  // Fill with cut names
/*  filler (c, "D0 Flight Significance", "D0_FD_ZSIG>%f", 0, 5, 0.1);
  filler (c, "Pi0 Mass Window", "fabs(Pi0_M-135)<%f", 1, 50, 1);
  filler (c, "D0 Mass Window", "fabs(D0_M-1865)<%f", 10, 100, 2);
  filler (c, "K0 PIDK", "K0_PIDK>%f", 0, 5, 0.1);
  filler (c, "P0 PIDK", "P0_PIDK<%f", -5, 0, 0.1);
  filler (c, "Bach PIDK", "Bach_PIDK<%f", 0, 15, 0.5);
  filler (c, "BDT", "FullBDT_2011>%f", -1, 0.4, 0.05);
  filler (c, "D0 Swapped Mass Window", "fabs(D0_SWAP_M-1865)>%f", 10, 100, 2);
*/

  filler (c, "Full BDT", "FullBDT_2011>%f", -1, 0.4, 0.05);
  filler (c, "Neutral BDT", "neutBDT>%f", -1, 0.4, 0.05);
  filler (c, "Remainder BDT", "neut_othersBDT>%f", -1, 0.4, 0.05);

  for (int j=0; j<std::get<0>(c).size(); j++) {
    for (double i=std::get<2>(c)[j]; i<std::get<3>(c)[j]; i+=std::get<4>(c)[j]) {
      cut = Form(std::get<1>(c)[j], i);
      x.push_back(i);
      y.push_back(std::get<3>(cut_effs(3, cut)));
    }

    temp = new TGraph(x.size(), &x[0], &y[0]);
    temp->SetTitle(std::get<0>(c)[j]+" efficiency; Cut value; Efficiency");
    temp->SetName(std::get<0>(c)[j]);
    temp->GetYaxis()->SetTitleOffset(1.4);
    temp->GetXaxis()->SetLabelSize(0.04);
    temp->SetLineWidth(3);
    temp->SetLineColor(kBlue+1);
    temp->SetMarkerSize(1.5);
    temp->SetMarkerStyle(3);
    temp->SetMarkerColor(kRed+1);
    temp_t = new TPaveText(0.33,0.101,0.63,0.30,"NDCNB");
    temp_t->AddText(std::get<1>(c)[j]);
    temp_t->SetFillStyle(0);
    temp_t->SetLineColor(0);
    temp_t->SetTextSize(0.05);
    temp_t->SetTextFont(82);
    t1.push_back(temp_t);
    gr.push_back(temp);
    x.clear();
    y.clear();
  }

  int k=0;
  for (gr_it=gr.begin(); gr_it!=gr.end(); gr_it++) {
    c1 = new TCanvas();
    (*gr_it)->Draw("ACP");
    t1[k]->Draw();
    c1->SaveAs(Form("Efficiency_vs_Cut_%s.pdf", (*gr_it)->GetName()), "pdf");
    k++;
  }

}
