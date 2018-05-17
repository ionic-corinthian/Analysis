
void myDecay::autoFit() {

    //============================================================//
    //=== FUNCTION : autoFit                                   ===//
    //============================================================//
    //=== This function takes two histograms (one for B+       ===//
    //=== and one for B-) and uses RooFit to fit them.         ===//
    //=== TupleName, latex_p and latex_m are used to label     ===//
    //=== the plots correctly and save the output.             ===//
    //============================================================//
    //=== TupleName: The name of the dtt, e.g. "B2DK_D2KPiPi0" ===//
    //===            N.B. A suffix of _MC should be added for  ===//
    //===            simulation data, e.g. "B2DK_D2KPiPi0_MC"  ===//
    //=== latex_p  : ROOT Latex command for B+ decay           ===//
    //=== latex_m  : ROOT Latex command for B- decay           ===//
    //=== h_p      : TH1D histogram of B+ mass distribution    ===//
    //=== h_m      : TH1D histogram of B- mass distribution    ===//
    //============================================================//

    //=== Define char arrays for printing the signal yields onto plots ===//

    char yield1_text[40];
    char yield2_text[40];
    char yield3_text[40];

    //======================================================//
    //=== Define the variables needed for the fitting:   ===//
    //======================================================//
    //=== The mass (which goes on the x axis)  :   x     ===//
    //=== The mean of the mass distribution    :   mean  ===//
    //=== The gradient of the background exp.  :   slope ===//
    //=== # events matched to signal           :   nsig  ===//
    //=== # events matched to background       :   nbac  ===//
    //======================================================//

    //=================================================================================//
    //=== WARNING: Eventually I will split by charge of B meson, i.e make two plots ===//
    //=== side by side to measure CPV. However for now I will just make one plot,   ===//
    //=== and will not split by B mass to save on time. To make two plots just copy ===//
    //=== the following plotting code, but for two RooPlot objects, one for plus    ===//
    //=== and one for minus. Fit and plot each one seperately, i.e. duplicate most  ===//
    //=== lines below, starting from RooFitResult* r1....                           ===//
    //=== I will also need to add an extra argument for the second histogram, i.e   ===//
    //=== TH1D * h_m.                                                               ===//
    //=== When I bring back this feature I could save both the B+ and B- plot       ===//
    //=== seperately, as well as the overall plot and overall canvas.               ===//
    //=================================================================================//

    RooRealVar x("x", "x", 5000, 6000);
    RooRealVar mean("mean", "mean of gaussian", 5300, 5100, 5400);
    RooRealVar sigma("sigma", "mean width of gaussian", 30, 2, 300);
    RooRealVar slope("slope", "slope", -0.1, -1., 0.);
    RooRealVar nsig("nsig", "nsig", 50000, 0, 1000000);
    RooRealVar nbac("nbac", "nbac", 50000, 0, 1000000);

    //======================================//
    //=== Setup RooFit frames and models ===//
    //======================================//

    //=== Now create a canvas to plot the fits for B+ and B- ===//
    TCanvas* c1 = new TCanvas(Form("B_Mass_%s", TupleName.Data()), "B Mass", 800, 400);
    //=== If both charges are to be drawn then the canvas
    if (charge == "Both") c1->Divide(2);

    //=== Build gaussian pdf (for signal) in terms of x, mean and sigma ===//
    RooGaussian gauss("gauss", "gauss", x, mean, sigma);
    RooExponential exp("exp", "exp", x, slope);
    RooArgList functions;
    functions.add(gauss);
    functions.add(exp);
    RooArgList yields;
    yields.add(nsig);
    yields.add(nbac);
    RooAddPdf model("model", "model", functions, yields);

    //=== Read in the data to be fitted, i.e. the B mass histograms ===//
    switch (eCharge) {

        case Charge::Both :
        case Charge::Plus :
            {
                if (charge == "Both") c1->cd(2);
                RooPlot* x_plus = x.frame(Title("B+ Mass"));
                RooDataHist h_plus("B+", "B+", x, Import(*h_p));  
                //=== Fit the data with the gaussian (signal) and exponential (background) ===//
                //=== Save signal yields (i.e. number of particles fitted to gaussian)     ===//
                RooFitResult* r1 = model.fitTo(h_plus, Save());
                Double_t yield_1 = nsig.getVal();
                h_plus.plotOn(x_plus);
                model.plotOn(x_plus);
                model.plotOn(x_plus, LineColor(kRed), Components(gauss));
                model.plotOn(x_plus, LineColor(kBlack), Components(exp));

                gPad->SetLeftMargin(0.15); x_plus->GetYaxis()->SetTitleOffset(1.6);
                x_plus->GetXaxis()->SetTitle("m [MeV/c^2]");
                sprintf(yield1_text, "Signal yield: %f", yield_1);
                TPaveText* pt2 = new TPaveText(0.5,0.7,0.9,0.9,"NDC");
                pt2->AddText(yield1_text);
                pt2->AddText(latex_p);
                x_plus->Draw();
                pt2->Draw();
                if (charge == "Plus") break;
            }

        case Charge::Minus :
            {
                if (charge == "Both") c1->cd(1);
                RooPlot* x_minus = x.frame(Title("B- Mass"));
                RooDataHist h_minus("B-", "B-", x, Import(*h_m));
                //=== Fit the data with the gaussian (signal) and exponential (background) ===//
                //=== Save signal yields (i.e. number of particles fitted to gaussian)     ===//
                RooFitResult* r2 = model.fitTo(h_minus, Save());
                Double_t yield_2 = nsig.getVal();
                h_minus.plotOn(x_minus);
                model.plotOn(x_minus);
                model.plotOn(x_minus, LineColor(kRed), Components(gauss));
                model.plotOn(x_minus, LineColor(kBlack), Components(exp));

                gPad->SetLeftMargin(0.15); x_minus->GetYaxis()->SetTitleOffset(1.6); 
                x_minus->GetXaxis()->SetTitle("m [MeV/c^2]");
                sprintf(yield2_text, "Signal yield: %f", yield_2);
                TPaveText* pt = new TPaveText(0.5,0.7,0.9,0.9,"NDC");
                pt->AddText(yield2_text);
                pt->AddText(latex_m);
                x_minus->Draw();
                pt->Draw();
                break;
            }
        case Charge::Combined :
            {
                RooPlot* x_comb = x.frame(Title("B^{#pm} Mass"));
                RooDataHist h_combined("B", "B", x, Import(*h_comb));
                //=== Fit the data with the gaussian (signal) and exponential (background) ===//
                //=== Save signal yields (i.e. number of particles fitted to gaussian)     ===//
                RooFitResult* r3 = model.fitTo(h_combined, Save());
                Double_t yield_3 = nsig.getVal();
                h_combined.plotOn(x_comb);
                model.plotOn(x_comb);
                model.plotOn(x_comb, LineColor(kRed), Components(gauss));
                model.plotOn(x_comb, LineColor(kBlack), Components(exp));

                gPad->SetLeftMargin(0.15); x_comb->GetYaxis()->SetTitleOffset(1.6); 
                x_comb->GetXaxis()->SetTitle("m [MeV/c^2]");
                sprintf(yield3_text, "Signal yield: %f", yield_3);
                TPaveText* pt3 = new TPaveText(0.5,0.7,0.9,0.9,"NDC");
                pt3->AddText(yield3_text);
                pt3->AddText(latex_comb);
                x_comb->Draw();
                pt3->Draw();
                break;
            }
    }

    //=== Save canvas as an image and a ROOT file ===// 
    c1->Print(Form("/data/lhcb/users/colmmurphy/Plots/%i/Mag%s/%sCharge/Bu_M_Fit_%s.png", year, mag.Data(), charge.Data(), TupleName.Data()), "png");
    c1->Print(Form("/data/lhcb/users/colmmurphy/Plots/%i/Mag%s/%sCharge/Bu_M_Fit_%s.root", year, mag.Data(), charge.Data(), TupleName.Data()), "root");
}


void myDecay::setBranches() {

    //============================================================//
    //=== Function : setBranches                               ===//
    //============================================================//
    //=== This function reads in a TBranch and sets up the     ===//
    //=== appropriate branch addresses depending on the        ===//
    //=== TupleName given, i.e. which channel/mode is being    ===//
    //=== considered.                                          ===//
    //============================================================//
    //=== t        : TChain containing all of the MagUp and    ===//
    //===            MagDown entries for the given TupleName   ===//
    //=== TupleName: The name of the dtt, e.g. "B2DK_D2KPiPi0" ===//
    //===            N.B. A suffix of _MC should be added for  ===//
    //===            simulation data, e.g. "B2DK_D2KPiPi0_MC"  ===//
    //============================================================//

    //============================================================//
    //=== Numbering Naming Convention                          ===//
    //============================================================//
    //=== Instead of using string comparisons within loops, I  ===//
    //=== am instead opting to use flags, and a numbering con- ===//
    //=== vention to refer to each mode.                       ===//
    //===------------------------------------------------------===//
    //=== In this scheme the leading "B2D" is ignored as this  ===//
    //=== is always the same.                                  ===//
    //=== Similarly, the trailing "Pi0" is ignored.            ===//
    //=== This leaves the middle three particles, which can    ===//
    //=== vary.                                                ===//
    //===------------------------------------------------------===//
    //=== K: 1      Pi: 2                                      ===//
    //===------------------------------------------------------===//
    //=== B2DK_D2KPiPi0:   112                                 ===//
    //=== B2DK_D2PiKPi0:   121                                 ===//
    //=== B2DK_D2PiPiPi0:  122                                 ===//
    //=== B2DK_D2KKPi0:    111                                 ===//
    //===                                                      ===//
    //=== B2DPi_D2KPiPi0:  212                                 ===//
    //=== B2DPi_D2PiKPi0:  221                                 ===//
    //=== B2DPi_D2PiPiPi0: 222                                 ===//
    //=== B2DPi_D2KKPi0:   211                                 ===//
    //============================================================//

    //===============================//
    //=== Global branch addresses ===//
    //===============================//
    t->SetBranchAddress("Bu_L0Global_TIS",          &Bu_L0Global_TIS);
    t->SetBranchAddress("Bu_L0HadronDecision_TOS",  &Bu_L0HadronDecision_TOS);
    t->SetBranchAddress("D0_M",                     &D0_M);
    t->SetBranchAddress("Bach_PIDK",                &Bach_PIDK);
    t->SetBranchAddress("Bu_M",                     &Bu_M);
    t->SetBranchAddress("Bu_DPVCFIT_M",             &Bu_DPVCFIT_M);
    t->SetBranchAddress("Bu_ID",                    &Bu_ID);

    t->SetBranchAddress("Bu_ENDVERTEX_Z",           &Bu_ENDVERTEX_Z);
    t->SetBranchAddress("Bu_ENDVERTEX_ZERR",        &Bu_ENDVERTEX_ZERR);
    t->SetBranchAddress("D0_ENDVERTEX_Z",           &D0_ENDVERTEX_Z);
    t->SetBranchAddress("D0_ENDVERTEX_ZERR",        &D0_ENDVERTEX_ZERR);

    t->SetBranchAddress("eventNumber",              &eventNumber);
    t->SetBranchAddress("runNumber",                &runNumber);


    //===============================//
    //=== Decay specific branches ===//
    //===============================//
    //=== KPiPi0 ===//
    if (TupleCode == 112 || TupleCode == 121 || TupleCode == 212 || TupleCode == 221) {
        t->SetBranchAddress("K0_PIDK", &K0_PIDK);
        t->SetBranchAddress("P0_PIDK", &P0_PIDK);
    }

    //=== PiPiPi0 ===//
    if (TupleCode == 122 || TupleCode == 222) { 
        t->SetBranchAddress("P0_PIDK", &P0_PIDK);
        t->SetBranchAddress("P1_PIDK", &P1_PIDK);
    }

    //=== KKPi0 ===//
    if (TupleCode == 111 || TupleCode == 211) {
        t->SetBranchAddress("K0_PIDK", &K0_PIDK);
        t->SetBranchAddress("K1_PIDK", &K1_PIDK);
    }

}


void myDecay::histFiller(TString charge) {
    
    //=============================================================//
    //=== Function : histFiller                                 ===//
    //=============================================================//
    //=== This function takes a TChain, its associate TupleName ===//
    //=== and the charge of the B being considered, and fills a ===//
    //=== histogram of the B mass distribution. It then saves   ===//
    //=== these histograms for later access/analysis.           ===//
    //=============================================================//
    //=== t        : TChain containing all of the MagUp and     ===//
    //===            MagDown entries for the given TupleName    ===//
    //=== TupleName: The name of the dtt, e.g. "B2DK_D2KPiPi0"  ===//
    //===            N.B. A suffix of _MC should be added for   ===//
    //===            simulation data, e.g. "B2DK_D2KPiPi0_MC"   ===//
    //=== charge   : Specifies the charge of the B meson to be  ===//
    //===            considered. Either "B+" or "B-"            ===//
    //=============================================================//
    
    //=== Define the histogram to be filled ===//
    TH1D* h = new TH1D(Form("h_%s_%s", TupleName.Data(), charge.Data()), charge.Data(), 100, 5000, 6000);
    
    //=== Make 2D mass comparison plots ===//
    //=== Pi0 VS D0 ===//
    TH2D* h_a = new TH2D(Form("MassComp_%s_%s","D0_vs_Pi0", TupleName.Data()), "D0 vs. Pi0", 100, 100, 170, 100, 1600, 2200 );
    //=== Pi0 VS Bu ===//
    TH2D* h_b = new TH2D(Form("MassComp_%s_%s","Bu_vs_Pi0", TupleName.Data()), "Bu vs. Pi0", 100, 100, 170, 100, 4500, 7000 );
    //=== D0  VS Bu ===//
    TH2D* h_c = new TH2D(Form("MassComp_%s_%s","Bu_vs_D0",  TupleName.Data()), "Bu vs. D0", 100, 1600, 2200,100, 4500, 7000 );    

    //=== Fill the histogram if cuts are passed ===//

    std::cout << "Starting loop:" << TupleName.Data() << std::endl;
        //=== TIME THE EVENT LOOP ===//
        int start_s = get_wall_time();
    for (Int_t i = 0; i < 500000; i++) {
        
        Double_t n_entries_before_cuts                        = t->GetEntries();
        Double_t n_entries_after_trigger_cuts                 = 0;    
        
        t->GetEntry(i);
        
        std::cout << "Bu_M " << Bu_M << std::endl;
        
        if (i%100000 == 0) std::cout << "--- INFO: Entry number = " << i << std::endl;
        
        //=========================//
        //=== Trigger selection ===//
        //=========================//
        if ((Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && 
            fabs(D0_M - 1864) < 50) {
    
            n_entries_after_trigger_cuts += 1; // Increment this counter by 1
            
            //=================================//
            //=== PID selection (data only) ===//
            //=================================//
            
            //=== Does event FAIL the cuts applied for specific tuples? (reverse cut condition) ===//
            //=== KPiPi0 ===//
            if (strcmp(TupleName, "B2DK_D2KPiPi0") == 0 || strcmp(TupleName, "B2DPi_D2KPiPi0") == 0 ||
                strcmp(TupleName, "B2DK_D2PiKPi0") == 0 || strcmp(TupleName, "B2DPi_D2PiKPi0") == 0) {
                 if (K0_PIDK < 2 || P0_PIDK > -2) continue;
            }
    
            //=== PiPiPi0 ===//
            if (strcmp(TupleName, "B2DK_D2PiPiPi0") == 0 || strcmp(TupleName, "B2DPi_D2PiPiPi0") == 0) { 
                if (P0_PIDK > -2 || P1_PIDK > -2) continue;
            }
    
            //=== KKPi0 ===//
            if (strcmp(TupleName, "B2DK_D2KKPi0") == 0 || strcmp(TupleName, "B2DPi_D2KKPi0") == 0) {
                if (K0_PIDK < 2 || K1_PIDK < 2) continue;
            }
            
            //==============================//
            //=== Bachelor PID selection ===//
            //==============================//
            
            //=== Bach = K ===//
            if (strcmp(TupleName, "B2DK_D2KPiPi0") == 0 || strcmp(TupleName, "B2DK_D2PiPiPi0") == 0 || strcmp(TupleName, "B2DK_D2KKPi0") == 0 || strcmp(TupleName, "B2DK_D2PiKPi0") ) {
                if (Bach_PIDK < 4) continue;
            }   
            //=== Bach = Pi ===//
            if (strcmp(TupleName, "B2DPi_D2KPiPi0") == 0 || strcmp(TupleName, "B2DPi_D2PiPiPi0") == 0 || strcmp(TupleName, "B2DPi_D2KKPi0") == 0 || strcmp(TupleName, "B2DPi_D2PiKPi0") ) {
                if (Bach_PIDK > 4) continue; 
            }
            
            //=== Fill the histogram (with kinematic fit B mass) ===//
            h->Fill(Bu_DPVCFIT_M);
            //=== Fill the 2D mass comparison plots ===//
           // h_a->Fill(Pi0_M, D0_M);
           // h_b->Fill(Pi0_M, Bu_M);
           // h_c->Fill(D0_M,  Bu_M);
       }
       
    }
    int stop_s = get_wall_time();
    std::cout << "--- INFO: TIME ELAPSED = " << (stop_s - start_s) << std::endl;
    
    //======================//
    //=== Save histogram ===//
    //======================//
    TFile f1(Form("h_%s_%s.root", TupleName.Data(), charge.Data()), "recreate");
    h->Write();
    
    TFile f2(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/2D_Mass_%s_%s.root", year, mag.Data(), charge.Data(), TupleName.Data(), "D0_vs_Pi0"), "recreate");
    h_a->Write();
    TFile f3(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/2D_Mass_%s_%s.root", year, mag.Data(), charge.Data(), TupleName.Data(), "Bu_vs_Pi0"), "recreate");
    h_b->Write();
    TFile f4(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/2D_Mass_%s_%s.root", year, mag.Data(), charge.Data(), TupleName.Data(), "Bu_vs_D0"), "recreate");
    h_c->Write();

    std::cout << "Finished loop." << std::endl;
    if (charge == "Plus" ) h_p = h;
    else if (charge == "Minus" ) h_m = h;
    else std::cout << "ERROR: '" << charge.Data() << "' is not an acceptable charge. Please enter 'Plus' or 'Minus'!" << std::endl; 

}
