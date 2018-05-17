#include "cut_effs.cpp"

using info = std::tuple < std::vector<TString>, std::vector<TString>, std::vector<double>,  std::vector<double>, std::vector<double>, std::vector<TString> >;

void filler (info &a, TString a_name, TString a_cut, double start, double end, double step, TString leg_loc) {
  // Shortcut function for filling entries into the 'matrix'
  std::get<0>(a).push_back(a_name);
  std::get<1>(a).push_back(a_cut);
  std::get<2>(a).push_back(start);
  std::get<3>(a).push_back(end);
  std::get<4>(a).push_back(step);
  std::get<5>(a).push_back(leg_loc);
}

void vary_cut_eff(info c) {

  // A simple script to create graphs 
  // showing the variation of efficiency
  // with differing values of a given
  // cut.
  //
  // Interfaces with cut_effs.cpp

  int ncuts = std::get<0>(c).size();

  std::vector <double> x; 
  std::vector <double> y_2011; 
  std::vector <double> y_2012; 
  std::vector <double> y_2015; 
  std::vector <double> y_2016;
  std::vector <double> maxima;
  std::vector <double> minima;
  std::vector <std::vector<double>> y; 
  std::vector <TGraph*> gr_2011; 
  std::vector <TGraph*> gr_2012; 
  std::vector <TGraph*> gr_2015; 
  std::vector <TGraph*> gr_2016; 
  std::vector <std::vector<TGraph*>> gr; 
  std::vector <TGraph*>::const_iterator gr_it; 
  std::vector <TPaveText*> t1;
  TPaveText* temp_t;
  TGraph* temp;
  TString cut;
  TCanvas* c1;
  int nyears;

  //info c;

  // Fill with cut names
  //filler (c, "D0 Flight Significance", "D0_FD_ZSIG>%f", 0, 5, 0.1);
  //filler (c, "D0 Swapped Mass Window", "fabs(D0_SWAP_M-1865)>%f", 10, 100, 2);
  //filler (c, "D0 Mass Window", "fabs(D0_M-1865)<%f", 10, 100, 2);
  //filler (c, "K0 PIDK", "K0_PIDK>%f", 0, 5, 0.1, "TR");
  //filler (c, "P0 PIDK", "P0_PIDK<%f", -5, 0, 0.1, "TL");
  //filler (c, "Bach PIDK", "Bach_PIDK<%f", 0, 15, 0.5);
  //filler (c, "BDT", "FullBDT_2011>%f", -1, 0.4, 0.05);
  //filler (c, "Pi0 Mass Window", "fabs(Pi0_M-135)<%f", 1, 30, 1);

  gr = {gr_2011, gr_2012, gr_2015, gr_2016};

  for (int j=0; j<ncuts; j++) {
    for (double i=std::get<2>(c)[j]; i<std::get<3>(c)[j]+std::get<4>(c)[j]; i+=std::get<4>(c)[j]) {
      cut = Form(std::get<1>(c)[j], i);
      x.push_back(i);
      y_2011.push_back(std::get<3>(cut_effs(2011, cut)));
      y_2012.push_back(std::get<3>(cut_effs(2012, cut)));
      y_2015.push_back(std::get<3>(cut_effs(2015, cut)));
      y_2016.push_back(std::get<3>(cut_effs(2016, cut)));
    }

    // Fill matrix y with vectors of each year's graph's y values
    y = {y_2011, y_2012, y_2015, y_2016};    
    nyears = y.size();
    // Combine all years vectors together so can find min/max
    std::vector<double> all;
    all.reserve( y[0].size()+ y[1].size() + y[2].size() + y[3].size());
    all.insert(all.end(), y[0].begin(), y[0].end());
    all.insert(all.end(), y[1].begin(), y[1].end());
    all.insert(all.end(), y[2].begin(), y[2].end());
    all.insert(all.end(), y[3].begin(), y[3].end());
    // Record maxima and minima for this cut, for plots
    maxima.push_back(*(std::max_element(std::begin(all), std::end(all))));
    minima.push_back(*(std::min_element(std::begin(all), std::end(all))));

    for (int k=0; k<nyears; k++) {
      temp = new TGraph(x.size(), &x[0], &y[k][0]);
      temp->SetTitle(std::get<0>(c)[j]+" efficiency; Cut value; Efficiency");
      temp->SetName(Form("%s_%i", std::get<0>(c)[j].Data(),k));
      temp->GetYaxis()->SetTitleOffset(1.2);
      temp->GetXaxis()->SetLabelSize(0.04);
      temp->SetLineWidth(3);
      temp->SetLineColor(kBlue+1);
      temp->SetMarkerSize(0.5);
      temp->SetMarkerStyle(21);
      temp->SetMarkerColor(kRed+1);
      if (k==0) {
        temp_t = new TPaveText(0.33,0.101,0.63,0.30,"NDCNB");
        temp_t->AddText(std::get<1>(c)[j]);
        temp_t->SetFillStyle(0);
        temp_t->SetLineColor(0);
        temp_t->SetTextSize(0.05);
        temp_t->SetTextFont(82);
        t1.push_back(temp_t);
      }
      gr[k].push_back(temp);
    }
    // All graphs for this cut have now been created
    // Clear vectors to save space
    x.clear();
    y.clear();
    y_2011.clear();
    y_2012.clear();
    y_2015.clear();
    y_2016.clear();
  }


  gr = {gr_2011, gr_2012, gr_2015, gr_2016};
  int k=0;
  double x_low, x_high, y_low, y_high;
  TString legend_position;
  std::vector <TString> year = {"2011", "2012", "2015", "2016"};
  TLegend* leg;
  for (int j=0; j<ncuts; j++) {
    c1 = new TCanvas();
    legend_position = std::get<5>(c)[j];
    
    // Set legend position
    // X
    if (legend_position(1)=='R') {
      x_low = 0.7;
      x_high = 0.89;
    }
    if (legend_position(1)=='M') {
      x_low = 0.4;
      x_high = 0.59;
    }
    if (legend_position(1)=='L') {
      x_low = 0.2;
      x_high = 0.39;
    }
    // Y
    if (legend_position(0)=='T') {
      y_low = 0.6;
      y_high = 0.89;
    }
    if (legend_position(0)=='M') {
      y_low = 0.4;
      y_high = 0.69;
    }
    if (legend_position(0)=='B') {
      y_low = 0.2;
      y_high = 0.49;
    }
    leg = new TLegend(x_low, y_low, x_high, y_high, "Year");
    leg->SetLineColor(kWhite);
    for (int i=0; i<nyears; i++) {
      gr[i][j]->GetYaxis()->SetRangeUser(minima[j]*0.98, maxima[j]*1.02);
      // Set different colours for different years
      if (i==1) gr[i][j]->SetLineColor(kRed+1);
      if (i==2) gr[i][j]->SetLineColor(kGreen+1);
      if (i==3) gr[i][j]->SetLineColor(kGray+1);
      if (i==0) gr[i][j]->Draw("ACP");
      else gr[i][j]->Draw("SAMECP");
      leg->AddEntry(gr[i][j], year[i], "L");
    }
    //t1[k]->Draw();
    leg->Draw();
    c1->SaveAs(Form("Efficiency_vs_Cut_%s.pdf", (std::get<0>(c)[j]).Data()), "pdf");
    k++;
  }
  // Clean up
  std::get<0>(c).clear();
  std::get<1>(c).clear();
  std::get<2>(c).clear();
  std::get<3>(c).clear();
  std::get<4>(c).clear();
  std::get<5>(c).clear();
  for (int l=0; l<gr.size(); l++) {
    gr[l].clear();
  }
}
