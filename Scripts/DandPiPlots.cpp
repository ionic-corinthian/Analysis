#include "myDecay.cxx"

void DandPiPlots() {

//==========================================================//

// In this function I will plot the MC mass distributions
// for the D0 and Pi0.

// I will compare each channel, and plot different years
// of MC on top of each other.

// Once these plots are made I will fit them with a gaussian
// to get yield information.

// Perhaps I can then apply cuts sequentially to each to get
// efficiencies.


//==========================================================//

///*
// ================ NOTE ================
// Following code block is histogram generating code
// which only needs to be run if the histogram (i.e
// variable being considered, bin width, range) is
// changed. Once it has been run once and an output file
// saved, the histograms can simply be read in and 
// modified without needing to re-generate them.


// Initialise the different decays in order to get their
// MC trees (mc_t)

myDecay DK_2011 = myDecay("B2DK_D2KPiPi0", 2011);
myDecay DK_2012 = myDecay("B2DK_D2KPiPi0", 2012);
myDecay DK_2015 = myDecay("B2DK_D2KPiPi0", 2015);
myDecay DK_2016 = myDecay("B2DK_D2KPiPi0", 2016);

myDecay DPi_2011 = myDecay("B2DPi_D2KPiPi0", 2011);
myDecay DPi_2012 = myDecay("B2DPi_D2KPiPi0", 2012);
myDecay DPi_2015 = myDecay("B2DPi_D2KPiPi0", 2015);
myDecay DPi_2016 = myDecay("B2DPi_D2KPiPi0", 2016);




// Open a TFile to save the histograms
TFile* f1 = new TFile("~/B2Dh/B_Mass_B2Dh_D2KPiPi0_MC.root", "recreate");

// Plot the D mass for DK. Pipe to a histogram so that the
// different years can be accessed and saved
const char* var = "Bu_DPVCFIT_M";
int bins = 80; //58 //35
int lower = 5100; //1580 //100
int upper = 5500; //2160 //170
TString cut = "K0_MC_MOTHER_ID == 421 && P0_MC_MOTHER_ID == 421 && Pi0_MC_MOTHER_ID == 421 && abs(K0_MC_GD_MOTHER_ID) == 521 && abs(P0_MC_GD_MOTHER_ID) == 521 && abs(Pi0_MC_GD_MOTHER_ID) == 521 && K0_MC_MOTHER_KEY == P0_MC_MOTHER_KEY && P0_MC_MOTHER_KEY == Pi0_MC_MOTHER_KEY && K0_MC_GD_MOTHER_KEY == P0_MC_GD_MOTHER_KEY && P0_MC_GD_MOTHER_KEY == Pi0_MC_GD_MOTHER_KEY && abs(Bach_MC_MOTHER_ID) == 521 && abs(D0_MC_MOTHER_ID) == 521 && Bach_MC_MOTHER_KEY == D0_MC_MOTHER_KEY && D0_MC_MOTHER_KEY == K0_MC_GD_MOTHER_KEY && Pi0_TRUEID==111 && abs(P0_TRUEID) == 211 && abs(K0_TRUEID) == 321 && abs(Bu_TRUEID)==521 && D0_TRUEID==421";

//TCut cut = "";

DK_2011.mc_t->Draw(Form("%s>>h1(%i, %i, %i)", var, bins, lower, upper), cut);  // Bins are 10 MeV wide
DK_2012.mc_t->Draw(Form("%s>>h2(%i, %i, %i)", var, bins, lower, upper), cut);
DK_2015.mc_t->Draw(Form("%s>>h3(%i, %i, %i)", var, bins, lower, upper), cut);
DK_2016.mc_t->Draw(Form("%s>>h4(%i, %i, %i)", var, bins, lower, upper), cut);

DPi_2011.mc_t->Draw(Form("%s>>h5(%i, %i, %i)", var, bins, lower, upper), cut);  // Bins are 10 MeV wide
DPi_2012.mc_t->Draw(Form("%s>>h6(%i, %i, %i)", var, bins, lower, upper), cut);
DPi_2015.mc_t->Draw(Form("%s>>h7(%i, %i, %i)", var, bins, lower, upper), cut);
DPi_2016.mc_t->Draw(Form("%s>>h8(%i, %i, %i)", var, bins, lower, upper), cut);

// Now get the individual histograms so I can change their
// line colour etc
TH1D* hK_2011 = (TH1D*)gDirectory->Get("h1");
TH1D* hK_2012 = (TH1D*)gDirectory->Get("h2");
TH1D* hK_2015 = (TH1D*)gDirectory->Get("h3");
TH1D* hK_2016 = (TH1D*)gDirectory->Get("h4");
TH1D* hPi_2011 = (TH1D*)gDirectory->Get("h5");
TH1D* hPi_2012 = (TH1D*)gDirectory->Get("h6");
TH1D* hPi_2015 = (TH1D*)gDirectory->Get("h7");
TH1D* hPi_2016 = (TH1D*)gDirectory->Get("h8");

hK_2011->Write();
hK_2012->Write();
hK_2015->Write();
hK_2016->Write();
hPi_2011->Write();
hPi_2012->Write();
hPi_2015->Write();
hPi_2016->Write();

/*
std::cout << "--- # events after truth matching!\n"
          << "       B2DK\n"
          << "         2011\t" << DK_2011.mc_t->GetEntries(cut) << '\n'
          << "         2012\t" << hK_2012->GetEntries() << '\n'
          << "         2015\t" << hK_2015->GetEntries() << '\n'
          << "         2016\t" << hK_2016->GetEntries() << '\n'
          << "       B2DPi\n"
          << "         2011\t" << hPi_2011->GetEntries() << '\n'
          << "         2012\t" << hPi_2012->GetEntries() << '\n'
          << "         2015\t" << hPi_2015->GetEntries() << '\n'
          << "         2016\t" << hPi_2016->GetEntries() << '\n'
          << "------------------------------------" << std::endl;
*/
//*/

/*
TFile* hist_files = TFile::Open("Bu_mass_B2Dh_D2KPiPi0_MC_TM.root");
TH1D* hK_2011 = (TH1D*)hist_files->Get("h1");
TH1D* hK_2012 = (TH1D*)hist_files->Get("h2");
TH1D* hK_2015 = (TH1D*)hist_files->Get("h3");
TH1D* hK_2016 = (TH1D*)hist_files->Get("h4");
TH1D* hPi_2011 = (TH1D*)hist_files->Get("h5");
TH1D* hPi_2012 = (TH1D*)hist_files->Get("h6");
TH1D* hPi_2015 = (TH1D*)hist_files->Get("h7");
TH1D* hPi_2016 = (TH1D*)hist_files->Get("h8");
*/

hK_2011->SetLineColor(kRed);
hK_2011->SetLineWidth(3);
hK_2012->SetLineColor(kBlue);
hK_2012->SetLineWidth(3);
hK_2015->SetLineColor(kBlack);
hK_2015->SetLineWidth(3);
hK_2016->SetLineColor(kGreen);
hK_2016->SetLineWidth(3);

hPi_2011->SetLineColor(kRed);
hPi_2011->SetLineWidth(3);
hPi_2011->SetLineStyle(2);
hPi_2012->SetLineColor(kBlue);
hPi_2012->SetLineWidth(3);
hPi_2012->SetLineStyle(2);
hPi_2015->SetLineColor(kBlack);
hPi_2015->SetLineWidth(3);
hPi_2015->SetLineStyle(2);
hPi_2016->SetLineColor(kGreen);
hPi_2016->SetLineWidth(3);
hPi_2016->SetLineStyle(2);

// Find the maximum values to set the axis range
double max11 = hK_2011->GetMaximum();
double max12 = hK_2012->GetMaximum();
double max15 = hK_2015->GetMaximum();
double max16 = hK_2016->GetMaximum();
double pmax11 = hPi_2011->GetMaximum();
double pmax12 = hPi_2012->GetMaximum();
double pmax15 = hPi_2015->GetMaximum();
double pmax16 = hPi_2016->GetMaximum();

double maxK[] = {max11, max12, max15, max16, pmax11, pmax12, pmax15, pmax16}; 
double limit = *std::max_element(std::begin(maxK), std::end(maxK));

gStyle->SetOptStat(0);

// Draw the histograms
TCanvas* c1 = new TCanvas("c1", "c1", 700, 700);
c1->SetLeftMargin(0.12);
hK_2011->Draw();
hK_2011->GetYaxis()->SetRangeUser(0, limit+50);
hK_2012->Draw("same");
hK_2015->Draw("same");
hK_2016->Draw("same");
hPi_2011->Draw("same");
hPi_2012->Draw("same");
hPi_2015->Draw("same");
hPi_2016->Draw("same");

hK_2011->SetTitle("#pi^{0} Mass");
hK_2011->GetXaxis()->SetTitle("m(#pi^{0})[MeV/c^{2}]");
hK_2011->GetYaxis()->SetTitle("Events");
hK_2011->GetYaxis()->SetTitleOffset(1.4);
hK_2011->GetYaxis()->SetTitleSize(0.04);
hK_2011->GetXaxis()->SetTitleSize(0.04);

// Put TPaves onto the canvas to
// indicate the channel and that the
// data used is MC
TPaveText* channel = new TPaveText(0.15, 0.7, 0.38, 0.8, "NDCbr");
TPaveText* MC      = new TPaveText(0.15, 0.6, 0.3, 0.65, "NDCbr");
TPaveText* cuts    = new TPaveText(0.15, 0.5, 0.3, 0.55, "NDCbr");
channel->AddText("B^{#pm}#rightarrow[K^{#pm}#pi^{#mp}#pi^{0}]_{D}h^{#pm}"); 
MC->AddText("Simulation");
cuts->AddText("Fully truth matched");
channel->Draw();
MC->Draw();
cuts->Draw();

// Make a legend
auto leg = new TLegend(0.7,0.6,1.1,0.9);

TLine* dummy = new TLine();
dummy->SetLineStyle(2);
leg->AddEntry(dummy, "h = #pi", "L");
TLine* dummy2 = new TLine();
leg->AddEntry(dummy2, "h = K", "L");

leg->AddEntry(hK_2011, "2011", "L");
leg->AddEntry(hK_2012, "2012", "L");
leg->AddEntry(hK_2015, "2015", "L");
leg->AddEntry(hK_2016, "2016", "L");

leg->SetTextSize(0.03);
leg->SetLineWidth(0);
leg->SetFillStyle(0);
leg->SetTextSize(0.04);
leg->Draw();
gPad->Update();








}
