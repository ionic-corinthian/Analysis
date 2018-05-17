#include "RooIpatia.cxx"

void B_fit() {
    // Fit B mass to truth matched MC
    // with double crystal ball

    using namespace RooFit;

    // Options
    TString shape = "ipat";
    bool preselection = true;
    int run=3; // 1: Run 1, 2: Run 2, 3: Run 1 + Run 2

    // Create a canvas to split into pads,
    // for fit and residuals below
    TCanvas* c1 = new TCanvas("c1", "c1", 800, 800);
    TPad* pad1 = new TPad("pad1", "pad1", 0, 0.33, 1, 1);
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.33);
    pad1->SetBottomMargin(0.00001);
    pad1->SetBorderMode(0);
    pad2->SetTopMargin(0.00001);
    pad2->SetBottomMargin(0.2);
    pad2->SetBorderMode(0);
    pad1->Draw();
    pad2->Draw();
    pad1->cd();

    // B mass range parameters
    double lower = 5000;
    double upper = 5900;
    double nbins = double(upper-lower)/10;

    TString sel_cuts = " && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && Bach_PIDK<12 && K0_PIDK>4 && P0_PIDK<-4 && (Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && D0_FD_ZSIG>2 && fabs(D0_M-1865)<50 && fabs(D0_SWAP_M-1865)>30 && FullBDT_2011>-0.1";
    // Open B mass MC;
    TChain* t = new TChain("DecayTree");
    if (run==1 || run==3) {
        t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
        t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    }
    if (run==2 || run==3) { 
        t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
        t->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_MC.root");
    }
    if (preselection)  t->Draw(Form("Bu_DPVCFIT_M>>h(%f, %f, %f)", nbins, lower, upper), "Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0");
    if (!preselection) t->Draw(Form("Bu_DPVCFIT_M>>h(%f, %f, %f)", nbins, lower, upper), "Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0"+sel_cuts);
    TH1D* h1 = (TH1D*)gDirectory->Get("h");
    //=========================
    //=== RooFit parameters ===
    //=========================
    //Double Gaussian
    RooRealVar x("x", "x", lower, upper);
    RooRealVar mean1("mean1", "mean1", 5280, 5200, 5400);
    RooRealVar mean2("mean2", "mean2", 5280, 5200, 5400);
    RooRealVar sigma1("sigma1", "sigma1", 70, 0, 150);
    RooRealVar sigma2("sigma2", "sigma2", 20, 0, 100);
    RooRealVar nsig("nsig", "nsig", 50000, 0, 2000000);
    RooRealVar frac("frac", "frac", 0.2, 0.01, 0.99);

    RooGaussian gauss1("gauss1", "gauss1", x, mean1, sigma1);
    RooGaussian gauss2("gauss2", "gauss2", x, mean2, sigma2);

    RooAddPdf dg("dg", "dg", RooArgList(gauss1, gauss2), frac);

    //Ipatia
    RooRealVar L("l", "l", -2.5, -5, 5);
    RooRealVar a_ip1("a_ip1", "a_ip1", 0.5, -5, 5);
    RooRealVar a2("a2", "a2", 9.1, 0, 10);
    RooRealVar n1("n1", "n1", 0.05, 0, 5);
    RooRealVar n2("n2", "n2", 0.1, 0, 5);
    RooRealVar sig_ip("sig_ip", "sig_ip", 20, 1, 100);
    RooRealVar sig_ip2("sig_ip2", "sig_ip2", 10, 1, 100);
    RooRealVar mean_ip("mean_ip", "mean_ip", 5270, 5200, 5400);
 
    RooIpatia ipat("ipat", "ipat", x, L, a_ip1, n1, sig_ip, mean_ip, a2, n2);

    // Background polynomial
    RooRealVar a0("a0", "a0", 1);
    RooRealVar a1("a1", "a1", 0, -1, 1);
    RooRealVar nbkg("nbkg", "nbkg", 2000, 1, 1000000);

    RooPolynomial p1("p1", "p1", x, RooArgList(a0, a1), 0);

    // Total Model
    //RooAddPdf model("model", "model", RooArgList(dg, p1), RooArgList(nsig, nbkg));
    //RooAddPdf model("model", "model", RooArgList(ipat, p1), RooArgList(nsig, nbkg));
    RooAddPdf model("model", "model", RooArgList(ipat), RooArgList(nsig));

    // Create plot
    RooPlot* xplot = x.frame(Title("B^{#pm} Mass (Ipatia fit) [MeV/c^{2}]"));
    RooDataHist dhist("B", "B", x, Import(*h1));
    RooFitResult *r1 = model.fitTo(dhist, Save());
    double sig_yield = nsig.getVal(); 
    double bkg_yield = nbkg.getVal();
    double s_over_b = double(sig_yield)/bkg_yield;

    // Draw plot
    dhist.plotOn(xplot);
    /*
    model.plotOn(xplot, LineColor(kRed+2), Components(gauss1));
    model.plotOn(xplot, LineColor(kGreen+2), Components(gauss2));
    */
    model.plotOn(xplot, LineColor(kRed), Components(ipat));
    model.plotOn(xplot, LineColor(kBlack), Components(p1));
    model.plotOn(xplot, LineColor(kBlue+2));
    xplot->Draw();
    r1->Print();

    // Draw residuals
    RooHist* hresid = xplot->pullHist();
    pad2->cd();
    hresid->Draw();

    hresid->GetYaxis()->SetTitleOffset(0.8);
    hresid->GetYaxis()->SetNdivisions(5,4,0);
    hresid->GetXaxis()->SetTitleOffset(1.2);
    hresid->GetXaxis()->SetTitleSize(0.07);
    hresid->GetYaxis()->SetTitleSize(0.07);
    hresid->GetYaxis()->SetTitleOffset(0.8);
    hresid->GetXaxis()->SetLabelSize(0.07);
    hresid->GetYaxis()->SetLabelSize(0.07);
    hresid->GetXaxis()->SetLimits(lower+1, upper-1);
    hresid->SetTitle("");
    hresid->GetXaxis()->SetTitle("m [MeV/c^2]");
    hresid->GetYaxis()->SetTitle("Pull");
    pad2->Update();
    TLine *line = new TLine(pad2->GetUxmin(), 0., pad2->GetUxmax(), 0.);
    line->SetLineWidth(3);
    line->SetLineColor(kRed);
    line->Draw();

    // Display yields
    pad1->cd();
    //  char* sig_yield_s;
    //  char* bkg_yield_s;
    //  char* s_over_b_s;
    //  sprintf(sig_yield_s, "", sig_yield);
    //  sprintf(bkg_yield_s, "", bkg_yield);
    //  sprintf(s_over_b_s, "", s_over_b);
    //  
    TPaveText* text_info = new TPaveText(0.5,0.02,0.89,0.5,"NDCNB");
    text_info->SetFillStyle(0);
    text_info->SetLineColor(kWhite);
    TText* t0 = text_info->AddText("B^{#pm}#rightarrow[K^{#pm}#pi^{#mp}#pi^{0}]_{D}#pi^{#pm}");
    TText* t1 = text_info->AddText("BKGCAT truth matched");
    TText* t2 = new TText();
    if (preselection ) t2 = text_info->AddText("Preselection");
    if (!preselection) t2 = text_info->AddText("Postselection");
    text_info->AddText(Form("Signal yield:  %.0f", sig_yield));
    //text_info->AddText(Form("Background yield:  %.0f", bkg_yield));
    //text_info->AddText(Form("#frac{S}{B}:  %.2f", s_over_b));

    TPaveText* run_info = new TPaveText(0.5, 0.51, 0.89, 0.89, "NDC");
    if (run==1) run_info->AddText("Run 1 MC");
    if (run==2) run_info->AddText("Run 2 MC");
    if (run==3) run_info->AddText("Both Runs MC");
    //run_info->AddText(" ");
    
    TText* sig1_t;
    TText* sig2_t;
    TText* mean1_t;
    TText* mean2_t;

    if (shape=="dg") {
    sig1_t = run_info->AddText(Form("#sigma_{1} = %0.2f #pm %0.5f", sigma1.getVal(), sigma1.getError()));
    sig2_t = run_info->AddText(Form("#sigma_{2} = %0.2f #pm %0.5f", sigma2.getVal(), sigma2.getError()));
    run_info->AddText(" ");
    mean1_t = run_info->AddText(Form("#bar{x}_{1} = %0.2f #pm %0.5f", mean1.getVal(), mean1.getError()));
    mean2_t = run_info->AddText(Form("#bar{x}_{2} = %0.2f #pm %0.5f", mean2.getVal(), mean2.getError()));
    // Colour code TPaveText entries based on
    // component plot colours
    sig1_t->SetTextColor(kRed+2);
    mean1_t->SetTextColor(kRed+2);
    sig2_t->SetTextColor(kGreen+2);
    mean2_t->SetTextColor(kGreen+2);
    }
    if (shape=="ipat") {
    sig1_t = run_info->AddText(Form("#sigma = %0.2f #pm %0.5f", sig_ip.getVal(), sig_ip.getError()));
    mean1_t = run_info->AddText(Form("#bar{x} = %0.2f #pm %0.5f", mean_ip.getVal(), mean_ip.getError()));
    sig1_t->SetTextColor(kRed+2);
    mean1_t->SetTextColor(kRed+2);
    }
    text_info->Draw();
    run_info->Draw();

    const char* run_s;
    const char* sel_s;
    if (run==1) run_s = "1";
    else if (run==2) run_s = "2";
    else if (run==3) run_s = "1and2";
    if (preselection) sel_s = "Pre";
    else if (!preselection) sel_s = "Post";
    c1->SaveAs(Form("/home/colmmurphy/Plots/Kpipi0/Ipatia_run%s_%sselection.pdf", run_s, sel_s), "pdf");
}

