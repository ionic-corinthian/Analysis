void mass_plots_2d() {

    TFile* f11 = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0.root");

    TTree* t11 = (TTree*)f11->Get("DecayTree");

    TCanvas* c1 = new TCanvas();
    TFile* g = new TFile("/home/colmmurphy/Plots/Bu_vs_D0_mass_2011.root", "recreate");
    // Bu vs D0
    TH2D* h11 = new TH2D("h11", "h11", 10, 1800, 1900, 100, 4500, 7000);
    // Cuts: don't include D window, or B window
TString cut = "(Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && fabs(Pi0_M - 135) < 20 && K0_PIDK > 2 && P0_PIDK < -2 && Bach_PIDK < 4 && Bach_P > 5000 && Bach_P < 100000 && Bach_PT > 500 && Bach_PT < 10000 && fabs(D0_SWAP_M - 1865) > 30 && D0_FD_ZSIG>2";
    t11->Draw("Bu_DPVCFIT_M:D0_M");
    h11->Write();
    c1->SaveAs("/home/colmmurphy/Plots/Bu_vs_D0_mass_2011.pdf", "pdf");
    g->Close();
}
