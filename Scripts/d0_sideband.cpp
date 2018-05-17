
void d0_sideband() {

    

    // Open all files
    TFile* f11 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile* f12 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile* f15 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");
    TFile* f16 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");

    // Get all trees
    TTree* t11 = (TTree*)f11->Get("DecayTree");
    TTree* t12 = (TTree*)f12->Get("DecayTree");
    TTree* t15 = (TTree*)f15->Get("DecayTree");
    TTree* t16 = (TTree*)f16->Get("DecayTree");

    // Create histograms
    TFile* f = TFile::Open("/home/colmmurphy/central_sideband_hists.root", "recreate");
    TH1D* h11 = new TH1D("h11", "h11", 100, 4000, 8000);
    TH1D* h12 = new TH1D("h12", "h12", 100, 4000, 8000);
    TH1D* h15 = new TH1D("h15", "h15", 100, 4000, 8000);
    TH1D* h16 = new TH1D("h16", "h16", 100, 4000, 8000);

    // Draw the B mass in the lower sideband
    TString cuts = "(Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && fabs(Pi0_M - 135) < 20 && K0_PIDK > 2 && P0_PIDK < -2 && Bach_PIDK < 4 && Bach_P > 5000 && Bach_P < 100000 && Bach_PT > 500 && Bach_PT < 10000 && fabs(D0_SWAP_M - 1865) > 30";
    t11->Draw("Bu_DPVCFIT_M>>h11", cuts+" && D0_M>1815 && D0_M<1915");
    t12->Draw("Bu_DPVCFIT_M>>h12", cuts+" && D0_M>1815 && D0_M<1915");
    t15->Draw("Bu_DPVCFIT_M>>h15", cuts+" && D0_M>1815 && D0_M<1915");
    t16->Draw("Bu_DPVCFIT_M>>h16", cuts+" && D0_M>1815 && D0_M<1915");
    
    h11->Write();
    h12->Write();
    h15->Write();
    h16->Write();

    TCanvas* c1 = new TCanvas();
    TCanvas* c2 = new TCanvas();
    TCanvas* c3 = new TCanvas();
    TCanvas* c4 = new TCanvas();
    
    c1->cd();
    h11->Draw();
    c1->SaveAs("/home/colmmurphy/Plots/B_mass_D0_central_sideband_2011.pdf", "pdf");
    c2->cd();
    h12->Draw();
    c2->SaveAs("/home/colmmurphy/Plots/B_mass_D0_central_sideband_2012.pdf", "pdf");
    c3->cd();
    h15->Draw();
    c3->SaveAs("/home/colmmurphy/Plots/B_mass_D0_central_sideband_2015.pdf", "pdf");
    c4->cd();
    h16->Draw();
    c4->SaveAs("/home/colmmurphy/Plots/B_mass_D0_central_sideband_2016.pdf", "pdf");

    


    /*

    TFile* g = TFile::Open("/home/colmmurphy/upper_sideband_hists.root");
    TH1D* h11 = (TH1D*)g->Get("h11");
    TH1D* h12 = (TH1D*)g->Get("h12");
    TH1D* h15 = (TH1D*)g->Get("h15");
    TH1D* h16 = (TH1D*)g->Get("h16");

    */

    std::vector < TH1D* > h = {h11, h12, h15, h16};
    std::vector < TString > y = {"2011", "2012", "2015", "2016"};
    /*
    TCanvas* c1 = new TCanvas();
    TCanvas* c2 = new TCanvas();
    TCanvas* c3 = new TCanvas();
    TCanvas* c4 = new TCanvas();
    */
    std::vector < TCanvas* > c = {c1, c2, c3, c4};
    TLine* tl;

    for (int i=0; i<4; i++) {
        //c[i] = new TCanvas();
        c[i]->cd();
        h[i]->SetLineColor(kBlue+2);
        h[i]->SetLineWidth(3);
        h[i]->SetTitle(y[i]+" Data: B mass from m(D^{0}) < 1700 MeV/c^{2} sideband, D0_FD_ZSIG>2");
        h[i]->GetXaxis()->SetTitle("m(B) [MeV/c^{2}]");
        h[i]->GetYaxis()->SetTitle("Events");
        h[i]->Draw();
        tl = new TLine(5279.29, 0, 5279.29, h[i]->GetMaximum());
        tl->SetLineColor(kRed);
        tl->SetLineWidth(3);
        tl->Draw();
        c[i]->SaveAs("/home/colmmurphy/Plots/B_mass_D_central_sideband_"+y[i]+"_.pdf", "pdf");
    }
}
