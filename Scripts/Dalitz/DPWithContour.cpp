Double_t dal(Double_t* x, Double_t* par) {

    Double_t mParent = par[0];
    Double_t mi = par[1];
    Double_t mj = par[2];
    Double_t mk = par[3];
    
    Double_t mikSq = x[0];
    Double_t mjkSq = x[1];
    Double_t mik = TMath::Sqrt(mikSq);
    Double_t mjk = TMath::Sqrt(mjkSq);
    
    Double_t ejcmsik = (mParent*mParent-mj*mj-mik*mik)/(2.0*mik);
    Double_t ekcmsik = (mik*mik+mk*mk-mi*mi)/(2.0*mik);
    if (ekcmsik<mk || ejcmsik<mj) return 2.0;
    
    Double_t pj = TMath::Sqrt(ejcmsik*ejcmsik-mj*mj);
    Double_t pk = TMath::Sqrt(ekcmsik*ekcmsik-mk*mk);
    Double_t coshelik = (mjk*mjk - mk*mk - mj*mj - 2.0*ejcmsik*ekcmsik)/(2.0*pj*pk);
    
    Double_t coshelikSq = coshelik*coshelik;
    return coshelikSq;

}

void DPWithContour () {

    /*TFile* f = TFile::Open("/home/colmmurphy/Plots/MC_2012_B2DPi_D2KPiPi0_Dalitz.root");
    TCanvas* c1 = (TCanvas*)f->Get("c");
    c1->Draw();
    */
    
    std::string bach = "Pi";
    gStyle->SetOptStat(0);
    
    TString Bach_cut;
    if (bach == "Pi") Bach_cut = "Bach_PIDK < 12";
    else Bach_cut = "Bach_PIDK > 12";

    TString other_cuts = "(Bu_L0Global_TIS == 1 || Bu_L0HadronDecision_TOS == 1) && fabs(D0_M - 1864) < 50 && K0_PIDK > 2 && P0_PIDK < -2 && "+Bach_cut+" && Bu_M<5900 && Bu_M>5080 && Pi0_M>115 && Pi0_M < 155 && Pi0_PT > 500 && Pi0_P > 1000 && Bach_PT > 500 && Bach_PT < 10000 && Bach_P > 5000 && Bach_P < 100000 && K0_PT > 250 && P0_PT > 250 && D0_FD_ZSIG > 2";
    //TString other_cuts = "";
    TString TMCut = "K0_MC_MOTHER_ID == 421 && P0_MC_MOTHER_ID == 421 && Pi0_MC_MOTHER_ID == 421 && abs(K0_MC_GD_MOTHER_ID) == 521 && abs(P0_MC_GD_MOTHER_ID) == 521 && abs(Pi0_MC_GD_MOTHER_ID) == 521 && K0_MC_MOTHER_KEY == P0_MC_MOTHER_KEY && P0_MC_MOTHER_KEY == Pi0_MC_MOTHER_KEY && K0_MC_GD_MOTHER_KEY == P0_MC_GD_MOTHER_KEY && P0_MC_GD_MOTHER_KEY == Pi0_MC_GD_MOTHER_KEY && abs(Bach_MC_MOTHER_ID) == 521 && abs(D0_MC_MOTHER_ID) == 521 && Bach_MC_MOTHER_KEY == D0_MC_MOTHER_KEY && D0_MC_MOTHER_KEY == K0_MC_GD_MOTHER_KEY";

    TFile* f1 = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/2012/MagCombined/CombinedCharge/Both_neutBDT_B2D%s_D2KPiPi0.root", bach.c_str()));
    //TFile* f1 = TFile::Open(Form("~/TruthMatchedTrees/2012/MagCombined/CombinedCharge/TM_B2D%s_D2KPiPi0_MC.root", bach.c_str()));
    TTree* tm = (TTree*)f1->Get("DecayTree");
    tm->Draw("M_KP:M_KPi0>>h1(25, 300000, 3200000, 25, 300000, 3500000)", (TString)(other_cuts+"&& neutBDT>0.1 && neut_othersBDT>0.1") , "colz");
    TH2D* h = (TH2D*)gDirectory->Get("h1");
    
    Double_t m1 = 139.570; // Pi^{\pm} mass
    Double_t m2 = 134.977; // Pi0 mass
    Double_t m3 = 493.677; // K^{\pm} mass
    Double_t mParent = 1864.84; // D0 mass
    
    Double_t m13SqMin = (m1+m3)*(m1+m3); // Minimum is when energy shared equally
    Double_t m23SqMin = (m2+m3)*(m2+m3);
    Double_t m13SqMax = (mParent-m2)*(mParent-m2); // Maximum when m2 only takes away min
    Double_t m23SqMax = (mParent-m1)*(mParent-m1);
    
    Double_t xMin = m13SqMin-1.0;
    Double_t xMax = m13SqMax+1.0;
    Double_t yMin = m23SqMin-1.0;
    Double_t yMax = m23SqMax+1.0;
    
    Int_t npar = 4;
    TF2 * f2 = new TF2("contour", &dal, xMin, xMax, yMin, yMax, npar);
    
    f2->SetParameter(0, mParent);
    f2->SetParameter(1, m1);
    f2->SetParameter(2, m2);
    f2->SetParameter(3, m3);
    
    Double_t b[] = {1.0};
    f2->SetContour(1, b);
    
    Int_t nbins = 1000;
    
    f2->SetNpx(nbins);
    f2->SetNpy(nbins);
    
    f2->SetLineWidth(3);
    f2->SetLineStyle(kSolid);
    
    TPaveText *title = new TPaveText(.6,.65,.9,.8, "NDCBR");
    title->AddText("2012 Data: B^{#pm}#rightarrow[K^{#pm}#pi^{#mp}#pi^{0}]_{D^{0}}K^{#pm}");
    //title->AddText("");
    //title->AddText("With truth matching");
    title->AddText("BDT > 0.1");
    title->Draw(); 
    
    h->SetTitle("Dalitz Plot");
    h->GetXaxis()->SetTitle("m^{2}(K^{#pm}#pi^{#mp})");
    h->GetYaxis()->SetTitleOffset(1.2);
    h->GetYaxis()->SetTitle("m^{2}(K^{#pm}#pi^{0})");
    h->GetYaxis()->SetTitleSize(0.04);
    h->GetXaxis()->SetTitleSize(0.04);
     
    f2->DrawCopy("same");
    
    gPad->Update();

    
    delete f2;
    
}



