#include "myDecay.cxx"

void MCDalitzYears() {

myDecay a = myDecay("B2DPi_D2KPiPi0", 2015);
TString tm_cuts = a.GetTMCuts();

// Parameters
std::string bach = "Pi";

TString Bach_cut;
if (bach == "Pi") Bach_cut = "Bach_PIDK < 12";
else Bach_cut = "Bach_PIDK > 12";

//TString other_cuts = "(Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && fabs(D0_M - 1864) < 50 && K0_PIDK > 2 && P0_PIDK < -2 && "+Bach_cut+" && Bu_M<5900 && Bu_M>5080 && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && D0_FD_ZSIG > 2";
TString other_cuts = tm_cuts+" && FullBDT_2011>0.1";


// Open file
TFile* f;
TFile* g;
// Truth matched
/*
f1 = TFile::Open(Form("~/TruthMatchedTrees/2011/MagCombined/CombinedCharge/TM_B2D%s_D2KPiPi0_MC.root", bach.c_str()));
f2 = TFile::Open(Form("~/TruthMatchedTrees/2012/MagCombined/CombinedCharge/TM_B2D%s_D2KPiPi0_MC.root", bach.c_str()));
f3 = TFile::Open(Form("~/TruthMatchedTrees/2015/MagCombined/CombinedCharge/TM_B2D%s_D2KPiPi0_MC.root", bach.c_str()));
f4 = TFile::Open(Form("~/TruthMatchedTrees/2016/MagCombined/CombinedCharge/TM_B2D%s_D2KPiPi0_MC.root", bach.c_str()));
*/


f1 = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC_actual.root"));
f2 = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC_actual.root"));
f3 = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC_actual.root"));
f4 = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC_actual.root"));

TTree* tm11 = (TTree*)f1->Get("DecayTree");
TTree* tm12 = (TTree*)f2->Get("DecayTree");
TTree* tm15 = (TTree*)f3->Get("DecayTree");
TTree* tm16 = (TTree*)f4->Get("DecayTree");

// Draw Dalitz plot

tm11->Draw("M_KP:M_KPi0>>h1(30, 300000, 4000000, 30, 300000, 4100000)", other_cuts , "colz");
tm12->Draw("M_KP:M_KPi0>>h2(30, 300000, 4000000, 30, 300000, 4100000)", other_cuts , "colz");
tm15->Draw("M_KP:M_KPi0>>h3(30, 300000, 4000000, 30, 300000, 4100000)", other_cuts , "colz");
tm16->Draw("M_KP:M_KPi0>>h4(30, 300000, 4000000, 30, 300000, 4100000)", other_cuts , "colz");

// Get Histogram

TCanvas* c = new TCanvas("c", "c", 800, 700);
c->Divide(2,2);

TH2D* h11 = (TH2D*)gDirectory->Get("h1");
TH2D* h12 = (TH2D*)gDirectory->Get("h2");
TH2D* h15 = (TH2D*)gDirectory->Get("h3");
TH2D* h16 = (TH2D*)gDirectory->Get("h4");

TPaveText * year = new TPaveText(0.1, 0.6, 0.3, 0.8);
//year->AddText(Form("Bach = %s", bach.c_str()));

year->AddText(Form("2011"));

year->AddText("BDT > 0.1");

gStyle->SetOptStat(0);
c->SetLeftMargin(0.14);
c->SetRightMargin(0.12);
gStyle->SetOptTitle(0);
TPaveLabel *title = new TPaveLabel(.1,.5,.5,.99,"2012 Simulation: B^{#pm}#rightarrow[K^{#pm}#pi^{#mp}#pi^{0}]_{D^{0}}#pi^{#pm}","brndc");
//title->Draw();

for (i=1; i<5; i++) {
    TH2D* h = (TH2D*)gROOT->FindObject(Form("h%i", i));
    h->GetXaxis()->SetTitle("m^{2}(K^{#pm}#pi^{#mp})");
    h->GetYaxis()->SetTitleOffset(1.4);
    h->GetYaxis()->SetTitle("m^{2}(K^{#pm}#pi^{0})");
    h->GetYaxis()->SetTitleSize(0.04);
    h->GetXaxis()->SetTitleSize(0.04);
    h->SetTitle("");
    c->cd(i);
    h->Draw("colz");
}



/*
h->SetTitle("Dalitz Plot");
h->GetXaxis()->SetTitle("m^{2}(K^{#pm}#pi^{#mp})");
h->GetYaxis()->SetTitleOffset(1.4);
h->GetYaxis()->SetTitle("m^{2}(K^{#pm}#pi^{0})");
h->GetYaxis()->SetTitleSize(0.04);
h->GetXaxis()->SetTitleSize(0.04);

c->cd(1);
h11->Draw("colz");
c->cd(2);
h12->Draw("colz");
c->cd(3);
h15->Draw("colz");
c->cd(4);
h16->Draw("colz");
*/

gPad->Update();

}
