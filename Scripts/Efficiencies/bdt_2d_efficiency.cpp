#include "myDecay.cxx"

void bdt_2d_efficiency(int year) {
    // This script generates plots of efficiency
    // vs BDT cut value, for the following
    // decay:
    TString TupleName = "B2DPi_D2KPiPi0";
    TString mag = "Combined";
    TString charge = "Combined";

    // Open file corresponding to decay detailed
    // above, and get its TTree.
    TFile* f = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/Full_%s_MC.root", year, mag.Data(), charge.Data(), TupleName.Data()), "READ");
    TTree* t = (TTree*)f->Get("DecayTree");
    
    // Get truth matching cuts
    myDecay a = myDecay( TupleName , year);
    TString cuts = a.GetTMCuts();
    
    // Initial number
    int n = t->GetEntries();
    // Number after truth match
    int n_tm = t->GetEntries(cuts);
    // Number after bdt cut will be calculated in 
    // a loop, then put into a TGraph of efficiency
    // vs. BDT cut value
    //
    // Create a vector (and an iterator) to hold
    // the BDT cut values, and the efficiency
    // values
    std::vector < double > neut_cut;
    std::vector < double > :: const_iterator neut_cut_it;
    std::vector < double > other_cut;
    std::vector < double > :: const_iterator other_cut_it;
    std::vector < std::vector <double> > eff;
    std::vector < std::vector <double> > :: const_iterator eff_iter;
    
    // Variables to hold value of number of entries
    // after TM and BDT cut
    int n_bdt;
    
    // Fill the BDT cut vector
    int j=0;
    for (double i=-0.05; i<0.15; i+=0.01) {
    // j gives the length of this vector
    // which needs to be given to the 
    // TGraph
        neut_cut.push_back(i);
        other_cut.push_back(i);
        j++;
    }
    std::cout << "\nSIZE: " << j*j << std::endl; 

    // Print out the initial value of the size of
    // the MC tree,
    // and the size of the tree after truth match
    std::cout << std::endl;
    std::cout << "Initial \t:\t" << n << '\n'
              << "After TM\t:\t" << n_tm << '\n';
              
    // Loop over the different bdt cut values and find
    // the number of entries in the MC tree after this
    // cut.
    // Then plot results in a TGraph of efficiency vs.
    // BDT cut

    TCanvas* c1 = new TCanvas("c1", "c1", 800,800);
    TGraph2D* gr = new TGraph2D(j*j);
    //gr->SetNpx(10);
    //gr->SetNpy(10);
    int i=0;
    for (neut_cut_it = neut_cut.begin(); neut_cut_it != neut_cut.end(); neut_cut_it++) {
        for (other_cut_it = other_cut.begin(); other_cut_it != other_cut.end(); other_cut_it++) {
            n_bdt = t->GetEntries(cuts+Form("&& neutBDT>%f && neut_othersBDT>%f", *neut_cut_it, *other_cut_it));
            std::cout << "Point # " << i << "   neutBDT: " << *neut_cut_it << "   otherBDT: " << *other_cut_it << "    EFF: " << double(n_bdt)/n_tm << std::endl;
            // Uncomment following line to see actual values
            //std::cout << Form("BDT\t%0.2f\t:\t", *iter) << n_bdt << std::endl;
            // Fill vector
            //eff[x_pos].push_back(double(n_bdt)/n_tm);
            gr->SetPoint(i, (*neut_cut_it), (*other_cut_it), double(n_bdt)/n_tm);
            i++;
        }
        
    }

    std::cout << "Final i value: " << i << std::endl;
    // Create TGraph to fill
    //TGraph2D* gr = new TGraph2D(j, &(neut_cut[0]), &(other_cut[0]), &(eff[0][0]));
    gr->SetLineWidth(4);
    gr->SetLineColor(kRed+2);
    gr->SetTitle("BDT Cut Efficiency for "+a.latex_comb+Form(" (%i MC); Neutral BDT Cut; Remainder BDT Cut; Efficiency", year));
    
    // Add a TPave to show
    // which BDT was used
    TPaveText* t1 = new TPaveText(0.2, 0.3, 0.4, 0.4, "NDC");
    t1->AddText("Full BDT");
    c1->SetMargin(0.15,0.15,0.15,0.1);
    gr->GetHistogram()->GetYaxis()->SetTitleOffset(1.5);
    gr->GetHistogram()->GetZaxis()->SetTitleOffset(1.45);
    gr->Draw("colz");
    gPad->Update();
    TPaletteAxis* pa = (TPaletteAxis*)gr->GetHistogram()->GetListOfFunctions()->FindObject("palette");
    pa->SetTitleOffset(1.8);
    
    //t1->Draw("br");
    
    c1->SaveAs(Form("/home/colmmurphy/Plots/2d_eff_%i.pdf", year), "pdf");
    
    f->Close();
    
    

}
