#include "RooIpatia.cxx"
#include "RooHILLdini.C"
#include "RooHORNSdini.C"

#include "myDecay.cxx"

double FitTM_MC (int year, double bdt_cut=0.1) {

    // Simple script to fit variables
    // in truth matched MC
    
    // Use the fit parameters found
    // here for data fits!
    
    using namespace RooFit;
    
    char yield3_text[40];
    char yield4_text[40];
    char yield5_text[40];
    
    //int year = 2011;
    std::string tuple = "B2DPi_D2KPiPi0";
    std::string var = "Data_B_DPVCFIT";
    
    int begin = 5000;
    int end   = 5900;
    double nbins = double(end - begin)/10;
    //double nbins = 30;
    
    bool sim = false;
    
    double lumi = 0;
    double lumierr = 0;
    
    //=== Generate truth matched MC tree ===//
    myDecay a_decay = myDecay(tuple, year);
    if (!sim) {
        a_decay.findLumi();
        lumi = a_decay.GetLumi();
        lumierr = a_decay.GetLumiErr();
    }
    //TTree* tree = decay.mc_t->CopyTree(decay.GetTMCuts());
    
    TString tm_cuts = a_decay.GetTMCuts();
    // Save a reduced TTree for 2012, with a BDT cut
    // ready to fit and get background data
    
    
    //======================================
    // Uncomment to create a new histogram
    // and save it to a scratch file
    // so it can be fitted more quickly
    // in future
    ///*

    //TFile* f = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/MagCombined/CombinedCharge/FullBDT_%s_MC.root", year, tuple.c_str()));
    //TTree* tree = (TTree*)f->Get("DecayTree");
    //TFile* g = TFile::Open("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0.root");
    //TTree* tree2 = (TTree*)g->Get("DecayTree");
    
    
    
    ///*
    //TFile* s = new TFile("~/scratch_run1.root", "recreate");
    
    ///*
    //TChain* tree = new TChain("DecayTree");
    //tree->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2011/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC.root");
    //tree->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Full_B2DPi_D2KPiPi0_2.root");
    /*
    tree->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2015/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC_actual.root");
    tree->Add("/data/lhcb/users/colmmurphy/ReducedTrees/2016/MagCombined/CombinedCharge/FullBDT_B2DPi_D2KPiPi0_MC_actual.root");
    */
    
    //tree->Draw(Form("Bu_DPVCFIT_M>>h1(%f,%i,%i)", nbins, begin, end), Form("Bu_M<5900 && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && Bach_PIDK<4 && K0_PIDK>2 && P0_PIDK<-2 && (Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && D0_FD_ZSIG>2 && fabs(D0_M-1865)<50 && fabs(D0_SWAP_M-1865)>30 && FullBDT_2011 > %f", bdt_cut));
    //tree->Draw(Form("Bu_DPVCFIT_M>>h1(%f,%i,%i)", nbins, begin, end), Form("Bu_M<5900 && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && Bach_PIDK<4 && K0_PIDK>2 && P0_PIDK<-2 && (Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && D0_FD_ZSIG>2 && fabs(D0_M-1865)<50 && fabs(D0_SWAP_M-1865)>30"));
    //tree->Draw("Bu_DPVCFIT_M>>h1(90,5000,5900)", Form("Bu_M<5900 && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && Bach_PIDK<12 && K0_PIDK>4 && P0_PIDK<-4 && (Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && D0_FD_ZSIG>2 && FullBDT > %f", bdt_cut));
    
    // ID method
    //if (sim) tree->Draw(Form("Bu_DPVCFIT_M>>h1(%f,%i,%i)", nbins, begin, end), "Bu_M<5900 &&"+tm_cuts);
    // BKGCAT method
    //if (sim) tree->Draw(Form("Bu_DPVCFIT_M>>h1(%f,%i,%i)", nbins, begin, end), "Bu_M<5900 && Bu_BKGCAT==0 && D0_BKGCAT==0 && Pi0_BKGCAT==0");
    // && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && Bach_PIDK<12 && K0_PIDK>4 && P0_PIDK<-4 && (Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && D0_FD_ZSIG>2 && fabs(D0_M-1865)<50 && fabs(D0_SWAP_M-1865)>30 && FullBDT_2011>-0.1 
    
    //TH1D* h = (TH1D*)gDirectory->Get("h1");
    //h->Write();
    //*/
    //======================================
    //**************************************
    //======================================
    // Uncomment to plot previously
    // created histogram
    TFile* f = TFile::Open("~/scratch_run1.root"); TH1D* h = (TH1D*)f->Get("h1");
    //======================================
    
    
    //================================================//
    //=== Set up the canvas, and pad for residuals ===//
    //================================================//
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
    
    //=========================//
    //=== Setting up RooFit ===//
    //=========================//
    RooRealVar x("x", "x", begin, end); // B mass
    //RooRealVar mean("mean", "mean of gaussian", 5290, 5240, 5320);  // B mass
    //RooRealVar sigma("sigma", "mean width of gaussian", 2, 1, 300); // B mass
    //RooRealVar x("x", "x", 1700, 2500); // D mass
    //RooRealVar mean("mean", "mean of gaussian", 1850, 1700, 1900);  // D mass
    //RooRealVar x("x", "x", 100, 180); // Pi0 mass
    //RooRealVar mean("mean", "mean of gaussian", 135, 125, 145);  // D mass
    //RooRealVar sigma("sigma", "mean width of gaussian", 20, 1, 100); // D mass
    RooRealVar slope("slope", "slope", -0.1, -1., 0.);
    RooRealVar nsig("nsig", "nsig", 50000, 0, 1000000);
    RooRealVar nexp("nexp", "nexp", 50000, 0, 1000000);
    RooRealVar nbgg("nbgg", "nbgg", 1000, 0, 1000000);
    
    // I want to fix the yields of horn with respect to
    // hill, as the fraction of decays going to pi0 versus
    // a gamma are fixed according to the PDG values
    RooRealVar nhorn("nhorn", "nhorn", 50000, 0, 1000000);
    RooConstVar yield_ratio("yield_ratio", "yield_ratio", 0.33);
    RooFormulaVar nhill("nhill", "nhill", "@0*@1", RooArgList(nhorn, yield_ratio));
    //RooRealVar nhill("nhill", "nhill", 50000, 0, 1000000);
    RooRealVar ncb("ncb", "ncb", 50000, 0, 1000000);
    
    
    //=== Ipatia parameters ===//
    ///*
    RooRealVar L("l", "l", -2.5, -5, 5);
    RooRealVar a1("a1", "a1", 0.5, -5, 5);
    RooRealVar a2("a2", "a2", 9.1, 0, 10);
    RooRealVar n1("n1", "n1", 0.05, 0, 5);
    RooRealVar n2("n2", "n2", 0.1, 0, 5);
    RooRealVar sigma("sigma", "sigma", 20, 1, 100);
    RooRealVar sigma2("sigma2", "sigma2", 10, 1, 100);
    RooRealVar mean("mean", "mean", 5270, 5200, 5400);
    //*/
    /*
    RooRealVar L("l", "l", -2.4423);
    RooRealVar a1("a1", "a1", 0.012132);
    RooRealVar a2("a2", "a2", 0.89707);
    RooRealVar n1("n1", "n1", 0.024158);
    RooRealVar n2("n2", "n2", 1.3231);
    RooRealVar sigma("sigma", "sigma", 23.067);
    RooRealVar mean("mean", "mean", 5271.8);
    */
    RooRealVar x1("x1", "x1", 1, 0, 5);
    
    RooRealVar co1("co1", "co1", 1, 0, 100);
    RooRealVar co2("co2", "co2", 1, 0, 100);
    RooPolynomial p1("p1", "p1", x, RooArgList(co1, co2));
    
    //=== HILLdini parameters ===//
    /*
    RooRealVar hill_a("hilla", "hilla", 4750, 4000, 5050);
    RooRealVar hill_b("hillb", "hillb", 5080, 5000, 5250);
    RooRealVar hill_csi("hillcsi", "hillcsi", -2.5, -5, 5);
    RooRealVar hill_shift("hillsh", "hillsh", 1.2, 0, 500);
    RooRealVar hill_sigma("hillsigma", "hillsigma", 5, 1, 300);
    RooRealVar hill_ratio_sigma("hillrs", "hillrs", 0.8, 0, 10);
    RooRealVar hill_fract_sigma("hillfs", "hillfs", 0.1, 0, 0.2);
    RooRealVar hill_shiftg("hillshg", "hillshg", 1.1, 0, 500);
    */
    ///*
    RooRealVar hill_a("hilla", "hilla", 4600.3);
    RooRealVar hill_b("hillb", "hillb", 5089.4);
    RooRealVar hill_csi("hillcsi", "hillcsi", -3.6412);
    RooRealVar hill_shift("hillsh", "hillsh", 0.23293);
    RooRealVar hill_sigma("hillsigma", "hillsigma", 11.336);
    RooRealVar hill_ratio_sigma("hillrs", "hillrs", 8.4673);
    RooRealVar hill_fract_sigma("hillfs", "hillfs", 0.19996);
    RooRealVar hill_shiftg("hillshg", "hillshg", 0.0132);
    //*/
    //=== HORNdini parameters ===//
    ///*
    //RooRealVar a("a", "a", 5015, 5000, 5030);
    //RooRealVar b("b", "b", 5100, 5050, 5160);
    //RooRealVar csi("csi", "csi", 0.1, -5, 5);
    //RooRealVar shift("sh", "sh", 1.2, 0, 500);
    //RooRealVar horn_sigma("hornsigma", "hornsigma", 5, 1, 300);
    //RooRealVar ratio_sigma("rs", "rs", 4.8, 0, 10);
    RooRealVar fract_sigma("fs", "fs", 0.3, 0, 5);
    //RooRealVar shiftg("shg", "shg", 0.1, 0, 50);
    //*/
    ///*
    RooRealVar a("a", "a", 5001.);
    RooRealVar b("b", "b", 5102.4);
    RooRealVar csi("csi", "csi", 0.58351);
    RooRealVar shift("sh", "sh", 10.369);
    RooRealVar ratio_sigma("rs", "rs", 9.7688);
    //RooRealVar fract_sigma("fs", "fs", 0.15271);
    RooRealVar shiftg("shg", "shg", 0.076293);
    RooRealVar horn_sigma("hornsigma", "hornsigma", 16.509);
    //*/
    //=== RooCBShape parameters ===//
    RooRealVar cbmean("cbmean", "cbmean", 5320, 5300, 5400);
    RooRealVar cbsigma("cbsigma", "cbsigma", 20, 0, 100);
    RooRealVar alpha("alpha", "alpha", 2.2, 0, 5);
    RooRealVar alpha2("alpha2", "alpha", 1, 0, 5);
    RooRealVar cb_n("cb_n", "cb_n", 3.8, -5, 10);
    RooRealVar cb_n2("cb_n2", "cb_n", 1, -5, 10);
    /*
    RooRealVar alpha("alpha", "alpha", 4.9074);
    RooRealVar alpha2("alpha2", "alpha", 3.5106);
    RooRealVar cb_n("cb_n", "cb_n", 1.6459);
    RooRealVar cb_n2("cb_n2", "cb_n", 2.4258);
    */
    //=== Background broad gaussian ===//
    RooRealVar bg_mean("bg_mean", "bg_mean", 5130, 5100, 5200);
    RooRealVar bg_sigma("bg_sigma", "bg_sigma", 150,100,300);
    RooGaussian bg_gauss("bg_gauss", "bg_gauss", x, bg_mean, bg_sigma);
    
    //=== Create distributions ===//
    RooCBShape CBShape("CBShape", "CBShape", x, mean, sigma, alpha, cb_n);
    RooIpatia ipat("ipat", "ipat", x, L, a1, n1, sigma, mean, a2, n2);
    RooHORNSdini horn("horn", "horn", x, a, b, csi, shift, horn_sigma, ratio_sigma, fract_sigma, shiftg );
    RooHILLdini hill("hill", "hill", x, hill_a, hill_b, hill_csi, hill_shift, hill_sigma, hill_ratio_sigma, hill_fract_sigma, hill_shiftg );   
    
    //=== Double Gaussian ===//
    /*
    RooRealVar sigma("sigma", "sigma", 49.075);
    RooRealVar sigma2("sigma2", "sigma", 17.740);
    RooRealVar mean("mean", "mean", 5280.2);
    */
   // RooGaussian gauss1("gauss", "gauss", x, mean, sigma);
   // RooGaussian gauss2("gauss2", "gauss", x, mean, sigma2);
    RooRealVar sigFrac("sigFrac", "sigFrac", 0.2, 0., 1.);
    /*
    //RooRealVar sigFrac("sigFrac", "sigFrac", 0.19228);
    */
    //=== Double CB ===//
    /*
    RooRealVar sigma("sigma", "sigma", 17.640);
    RooRealVar sigma2("sigma2", "sigma", 47.746);
    RooRealVar mean("mean", "mean", 5280.2);
    */
    RooGaussian gauss1("gauss", "gauss", x, mean, sigma);
    RooGaussian gauss2("gauss2", "gauss", x, mean, sigma2);
    /*
    //RooRealVar sigFrac("sigFrac", "sigFrac", 0.8, 0., 1.);
    RooRealVar sigFrac("sigFrac", "sigFrac", 0.80152);
    */
    // make double gaussian
    RooAddPdf db_gauss("bd_gauss", "db_gauss", RooArgList(gauss1, gauss2), sigFrac);
    //RooGaussian bg_gauss("bg_gauss", "bg_gauss", x, bg_mean, bg_sigma);
    // make double crystal ball
    //RooCBShape CBShape("CBShape", "CBShape", x, mean, sigma, alpha, cb_n);
    //RooCBShape cb2("cb2", "CBShape", x, mean, sigma2, alpha2, cb_n2);
    //RooAddPdf db_cb("db_cb", "db_cb", RooArgList(CBShape, cb2), sigFrac);
    
    
    
    RooExponential exp("exp", "exp", x, slope);
    
    RooArgList functions;
      //  functions.add(CBShape);
      //  functions.add(db_gauss);
        functions.add(ipat);
        functions.add(exp);
      //  functions.add(CBShape);
        functions.add(horn);
        functions.add(hill);
        functions.add(bg_gauss);
      //  functions.add(p1);
    RooArgList yields;
        yields.add(nsig);
        yields.add(nexp);
     //   yields.add(ncb);
        yields.add(nhorn);
        yields.add(nhill);
        yields.add(nbgg);
    RooAddPdf model("model", "model", functions, yields);
    
    RooPlot* x_comb = x.frame(Title("B^{#pm} Mass"));
            RooDataHist h_combined("B", "B", x, Import(*h));
            //=== Fit the data with the gaussian (signal) and exponential (background) ===//
            //=== Save signal yields (i.e. number of particles fitted to gaussian)     ===//
            RooFitResult* r3 = model.fitTo(h_combined, Save());
            Double_t yield_3 = nsig.getVal();
            Double_t bac1 = nexp.getVal();
            Double_t bac2 = nhorn.getVal();
            Double_t bac3 = nhill.getVal();
            Double_t bac4 = nbgg.getVal();
            Double_t nbac = bac1+bac2+bac3+bac4;
            // For hills+horns plot, just use
            // those two values!
            //Double_t nbac = bac2+bac3;
           // Double_t nbac = bac1;
            
            h_combined.plotOn(x_comb);
            
            model.plotOn(x_comb, LineColor(kRed), Components(horn));
            model.plotOn(x_comb, LineColor(kBlack), Components(exp));
            model.plotOn(x_comb, LineColor(kGreen), Components(ipat));
            model.plotOn(x_comb, LineColor(kGreen+2), Components(bg_gauss));
            model.plotOn(x_comb, LineColor(kRed+2), Components(hill));
            model.plotOn(x_comb, LineColor(kGreen+3), Components(CBShape));
            model.plotOn(x_comb, LineColor(kRed+2), Components(gauss1));
            model.plotOn(x_comb, LineColor(kGreen+3), Components(gauss2));
            model.plotOn(x_comb, LineColor(kGreen+3), Components(p1));
            model.plotOn(x_comb);
            double s_over_b = double(nbac)/yield_3;
        
            pad1->SetLeftMargin(0.15);
            pad2->SetLeftMargin(0.15);
            x_comb->GetYaxis()->SetTitleOffset(1.6); 
            //x_comb->GetXaxis()->SetTitle("m [MeV/c^2]");
            
            //==========================//
            //=== Text to go on plot ===//
            //==========================//
            // Yields
            sprintf(yield3_text, "Signal yield:  %.0f", yield_3);
            sprintf(yield4_text, "Background yield:  %.0f", nbac);
            sprintf(yield5_text, "#frac{bkg}{sig} =  %.2f", s_over_b);
            TPaveText* pt3 = new TPaveText(0.5,0.1,0.89,0.6,"NDCNB");
            pt3->SetFillStyle(0);
            pt3->SetLineColor(kWhite);
            
            // LaTeX of decay process
            TText * t0 = new TText();
            if (tuple == "B2DPi_D2KPiPi0") t0 = pt3->AddText("B^{#pm}#rightarrow[K^{#pm}#pi^{#mp}#pi^{0}]_{D}#pi^{#pm}");
            if (tuple == "B2DK_D2KPiPi0") t0 = pt3->AddText("B^{#pm}#rightarrow[K^{#pm}#pi^{#mp}#pi^{0}]_{D}K^{#pm}");
            
            t0->SetTextFont(42);
            t0->SetTextAlign(30);
            
            // Year
            TText* t2 = pt3->AddText(Form("Year:  %i", year));
            //TText* t2 = pt3->AddText("All MC Years");
            t2->SetTextAlign(30);
            t2->SetTextFont(42);
            
            
            // Luminosity value
            if (!sim) {
                
               // TText* t6 = pt3->AddText(Form("L = %0.2f #pm %0.2f pb^{-1}", lumi, lumierr));
               // t6->SetTextAlign(30);
               // t6->SetTextFont(42);
            }
                
            TText *t1 = pt3->AddText(yield3_text);
            t1->SetTextAlign(30);
            t1->SetTextFont(42);
            TText *t3 = pt3->AddText(yield4_text);
            pt3->AddText("");
            TText *t4 = pt3->AddText(yield5_text);
            pt3->AddText("");
            //TText * t3 = pt3->AddText("BKGCAT truth match");
            t3->SetTextAlign(30);
            t3->SetTextFont(42);
            t4->SetTextAlign(30);
            t4->SetTextFont(42);
            // BDT cut value
            TText *t5 = pt3->AddText(Form("FullBDT > %0.2f", bdt_cut));
            t5->SetTextAlign(30);
            t5->SetTextFont(42);
            
            
            // Residuals
            RooHist* hresid = x_comb->pullHist();
            //hresid->GetXaxis()->SetLimits(5000, 5900);
            
            //pt3->AddText(latex_comb);
            x_comb->Draw();
            pt3->Draw();
            
            x_comb->SetMinimum(0.1);
            pad1->Update();
            //TLine *line3 = new TLine(5300, pad1->GetUymin(), 5300, pad1->GetUymax());
            //line3->Draw();
            
   
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
   hresid->GetXaxis()->SetLimits(begin+1, end-1);
   hresid->SetTitle("");
   hresid->GetXaxis()->SetTitle("m [MeV/c^2]");
   hresid->GetYaxis()->SetTitle("Pull");
   std::cout << "Pad 1 text size: " << x_comb->GetXaxis()->GetTitleSize() << std::endl;
   std::cout << "Pad 2 text size: " << hresid->GetXaxis()->GetTitleSize() << std::endl;
   pad2->Update();
   TLine *line = new TLine(pad2->GetUxmin(), 0., pad2->GetUxmax(), 0.);
   //TLine *line2 = new TLine(5300, pad2->GetUymin(), 5300, pad2->GetUymax());
   line->SetLineWidth(3);
   line->SetLineColor(kRed);
   line->Draw();
   //line2->Draw();
            
   c1->SaveAs(Form("~/Plots/%i_%s_bdt_%0.2f_B_mass_fit.root", year, tuple.c_str(), bdt_cut), "root");
   c1->SaveAs(Form("~/Plots/%i_%s_bdt_%0.2f_B_mass_fit.png", year, tuple.c_str(), bdt_cut), "png");
   
   r3->Print();
   return s_over_b;
    
}
