#include "myDecay.cxx"

void npv_year() {

    // Create two subcanvases, one for
    // data and one for MC
    TCanvas* c1 = new TCanvas();
    c1->Divide(1,2);

    gStyle->SetOptStat(0);

    //=============
    //=== FILES ===
    //=============
    // MC files
    TFile * f11_s = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    TFile * f12_s = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    TFile * f15_s = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    TFile * f16_s = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    /*
    // Data files
    TFile * f11_d = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile * f12_d = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile * f15_d = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile * f16_d = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile * f17_d = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2017/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    */
    //=============
    //=== TREES ===
    //=============
    // MC trees
    TTree * t11_s = (TTree*)f11_s->Get("DecayTree");
    TTree * t12_s = (TTree*)f12_s->Get("DecayTree");
    TTree * t15_s = (TTree*)f15_s->Get("DecayTree");
    TTree * t16_s = (TTree*)f16_s->Get("DecayTree");
    /*
    // Data trees
    TTree * t11_d = (TTree*)f11_d->Get("DecayTree");
    TTree * t12_d = (TTree*)f12_d->Get("DecayTree");
    TTree * t15_d = (TTree*)f15_d->Get("DecayTree");
    TTree * t16_d = (TTree*)f16_d->Get("DecayTree");
    TTree * t17_d = (TTree*)f17_d->Get("DecayTree");
    */
    myDecay d11 = myDecay("B2DPi_D2KPiPi0", 2011);
    myDecay d12 = myDecay("B2DPi_D2KPiPi0", 2012);
    myDecay d15 = myDecay("B2DPi_D2KPiPi0", 2015);
    myDecay d16 = myDecay("B2DPi_D2KPiPi0", 2016);
    myDecay d17 = myDecay("B2DPi_D2KPiPi0", 2017);
    
    TTree* t11_d = (TTree*)d11.t;
    TTree* t12_d = (TTree*)d12.t;
    TTree* t15_d = (TTree*)d15.t;
    TTree* t16_d = (TTree*)d16.t;
    TTree* t17_d = (TTree*)d17.t;

    //=============
    //=== HISTS ===
    //=============
    // MC hists
    t11_s->Draw("nPV>>h1_s(10, 0, 10)");
    t12_s->Draw("nPV>>h2_s(10, 0, 10)");
    t15_s->Draw("nPV>>h3_s(10, 0, 10)");
    t16_s->Draw("nPV>>h4_s(10, 0, 10)");
    // Data hists
    t11_d->Draw("nPV>>h1_d(10, 0, 10)");
    t12_d->Draw("nPV>>h2_d(10, 0, 10)");
    t15_d->Draw("nPV>>h3_d(10, 0, 10)");
    t16_d->Draw("nPV>>h4_d(10, 0, 10)");
    t17_d->Draw("nPV>>h5_d(10, 0, 10)");

    //==================
    //=== SAVE HISTS ===
    //==================
    // MC
    TH1D* h1_s = (TH1D*)gDirectory->Get("h1_s");
    TH1D* h2_s = (TH1D*)gDirectory->Get("h2_s");
    TH1D* h3_s = (TH1D*)gDirectory->Get("h3_s");
    TH1D* h4_s = (TH1D*)gDirectory->Get("h4_s");
    // Data
    TH1D* h1_d = (TH1D*)gDirectory->Get("h1_d");
    TH1D* h2_d = (TH1D*)gDirectory->Get("h2_d");
    TH1D* h3_d = (TH1D*)gDirectory->Get("h3_d");
    TH1D* h4_d = (TH1D*)gDirectory->Get("h4_d");
    TH1D* h5_d = (TH1D*)gDirectory->Get("h5_d");

    //===================
    //=== PLOT LIMITS ===
    //===================
    // MC limits
    double maximum_s = 0;
    if (h1_s->GetMaximum() > maximum_s) maximum_s = h1_s->GetMaximum();
    if (h2_s->GetMaximum() > maximum_s) maximum_s = h2_s->GetMaximum();
    if (h3_s->GetMaximum() > maximum_s) maximum_s = h3_s->GetMaximum();
    if (h4_s->GetMaximum() > maximum_s) maximum_s = h4_s->GetMaximum();
    // Data limits
    double maximum_d = 0;
    if (h1_d->GetMaximum() > maximum_d) maximum_d = h1_d->GetMaximum();
    if (h2_d->GetMaximum() > maximum_d) maximum_d = h2_d->GetMaximum();
    if (h3_d->GetMaximum() > maximum_d) maximum_d = h3_d->GetMaximum();
    if (h4_d->GetMaximum() > maximum_d) maximum_d = h4_d->GetMaximum();
    if (h5_d->GetMaximum() > maximum_d) maximum_d = h5_d->GetMaximum();

    //==================
    //=== MAKE PLOTS ===
    //==================
    // MC
    c1->cd(1);
    h1_s->GetYaxis()->SetRangeUser(0, maximum_s*1.1);
    h1_s->GetXaxis()->SetTitle("Number of Primary Vertices");
    h1_s->GetYaxis()->SetTitle("Events");
    h1_s->SetTitle("Number of Primary Vertices [MC]");
    h1_s->Draw();
    h1_s->SetFillColor(kRed);
    h1_s->SetFillStyle(3007);
    h2_s->Draw("same");
    h2_s->SetFillColor(kBlue);
    h2_s->SetFillStyle(3004);    
    h3_s->Draw("same");
    h3_s->SetFillStyle(3006);
    h3_s->SetFillColor(kGreen);
    h4_s->Draw("same");
    h4_s->SetFillStyle(3005);
    h4_s->SetFillColor(kGreen+2);

    TLegend* leg_s = new TLegend(0.75, 0.7, 0.9, 0.9);
    leg_s->SetHeader("Year", "C");
    leg_s->AddEntry(h1_s, "2011", "f");
    leg_s->AddEntry(h2_s, "2012", "f");
    leg_s->AddEntry(h3_s, "2015", "f");
    leg_s->AddEntry(h4_s, "2016", "f");
    leg_s->Draw();

    gPad->Update();

    // Data
    c1->cd(2);
    h1_d->GetYaxis()->SetRangeUser(0, maximum_d*1.1);
    h1_d->GetXaxis()->SetTitle("Number of Primary Vertices");
    h1_d->GetYaxis()->SetTitle("Events");
    h1_d->SetTitle("Number of Primary Vertices [Data]");
    h1_d->Draw();
    h1_d->SetFillColor(kRed);
    h1_d->SetFillStyle(3007);
    h2_d->Draw("same");
    h2_d->SetFillColor(kBlue);
    h2_d->SetFillStyle(3004);
    h3_d->Draw("same");
    h3_d->SetFillStyle(3006);
    h3_d->SetFillColor(kGreen);
    h4_d->Draw("same");
    h4_d->SetFillStyle(3005);
    h4_d->SetFillColor(kGreen+2);
    h5_d->Draw("same");
    h5_d->SetFillStyle(3005);
    h5_d->SetFillColor(kRed+2);

    TLegend* leg_d = new TLegend(0.75, 0.7, 0.9, 0.9);
    leg_d->SetHeader("Year", "C");
    leg_d->AddEntry(h1_d, "2011", "f");
    leg_d->AddEntry(h2_d, "2012", "f");
    leg_d->AddEntry(h3_d, "2015", "f");
    leg_d->AddEntry(h4_d, "2016", "f");
    leg_d->AddEntry(h5_d, "2017", "f");
    leg_d->Draw();

    gPad->Update();
    
    c1->SaveAs("npv_plots.root", "root");
    c1->SaveAs("npv_plots.pdf", "pdf");
}
