void cut_profiles(TString a_cut) {
  //TString a_cut = "Bach_PIDK";
  gStyle->SetOptStat(0);

  // A simple script to plot the distributions
  // of a given variable for each year

  // String for truth matching
  TString tm_cut = "Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0";

  // Open files
  TFile* f11 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
  TFile* f12 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
  TFile* f15 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
  TFile* f16 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");

  // Get trees
  TTree* t11 = (TTree*)f11->Get("DecayTree");
  TTree* t12 = (TTree*)f12->Get("DecayTree");
  TTree* t15 = (TTree*)f15->Get("DecayTree");
  TTree* t16 = (TTree*)f16->Get("DecayTree");

  // Set start point, end point, and number of bins
  // based off of make_vary_eff_plots.cpp numbers
  // (Hardcoded for now -- change this??)
  double start, end, step;
  if (a_cut == "D0_FD_ZSIG") {
    start = 0;
    end = 5;
    step = 0.1;
  }
  if (a_cut == "D0_SWAP_M") {
    start = 10;
    end = 100;
    step = 2;
  }
  if (a_cut == "D0_M") {
    start = 10;
    end = 100;
    step = 2;
  }
  if (a_cut == "Bach_PIDK") {
    start = 0;
    end = 15;
    step = 0.5;
  }
  if (a_cut == "FullBDT") {
    start = -1;
    end = 0.4;
    step = 0.05;
  }
  if (a_cut == "Pi0_M") {
    start = 7;
    end = 25;
    step = 1;
  }
  if (a_cut == "K0_PIDK") {
    start = 0;
    end = 5;
    step = 0.1;
  }
  if (a_cut == "P0_PIDK") {
    start = -5;
    end = 0.5;
    step = 0.1;
  }
  double nbins = double(fabs(start-end))/step;
  //nbins=(nbins/2);
  nbins=10;
  std::cout << "start: " << start << std::endl;
  std::cout << "end: " << end << std::endl;
  std::cout << "step: " << step << std::endl;
  std::cout << "nbins: " << nbins << std::endl;

  // Generate histograms
  TH1D* h11 = new TH1D("h11", Form("%s Profile;%s;Events", a_cut.Data(), a_cut.Data()), nbins, start, end);
  t11->Draw("K0_PIDK>>h11", tm_cut);
  TH1D* h12 = new TH1D("h12", "h12", nbins, start, end);
  t12->Draw("K0_PIDK>>h12", tm_cut);
  TH1D* h15 = new TH1D("h15", "h15", nbins, start, end);
  t15->Draw("K0_PIDK>>h15", tm_cut);
  TH1D* h16 = new TH1D("h16", "h16", nbins, start, end);
  t16->Draw("K0_PIDK>>h16", tm_cut);

  // Create canvas to draw multiplot
  // Calculate maximum
  TCanvas* c1 = new TCanvas();
  std::vector <double> h_vec = {h11->GetMaximum(), h12->GetMaximum(), h15->GetMaximum(), h16->GetMaximum()};
  double maximum = *(std::max_element(std::begin(h_vec), std::end(h_vec)));
  
  // Draw
  h11->GetYaxis()->SetRangeUser(0, maximum*1.1);
  h11->Draw();
  h11->SetLineColor(kBlue+1);
  h12->Draw("SAME"); 
  h12->SetLineColor(kRed+1);
  h15->Draw("SAME"); 
  h15->SetLineColor(kGreen+1);
  h16->Draw("SAME"); 
  h16->SetLineColor(kGray+1);

  h11->SetLineWidth(2);
  h12->SetLineWidth(2);
  h15->SetLineWidth(2);
  h16->SetLineWidth(2);

  TLegend* leg = new TLegend(0.1, 0.7, 0.3, 0.89, "Year");
  leg->SetLineColor(kWhite);
  leg->AddEntry(h11, "2011", "L");
  leg->AddEntry(h12, "2012", "L");
  leg->AddEntry(h15, "2015", "L");
  leg->AddEntry(h16, "2016", "L");
  //leg->Draw();
  c1->SaveAs(Form("%s_profile.pdf", a_cut.Data()), "pdf");
}
