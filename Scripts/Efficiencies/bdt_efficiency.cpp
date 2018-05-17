#include "myDecay.cxx"

void bdt_efficiency() {
    // This script generates plots of efficiency
    // vs BDT cut value, for the following
    // decay:
    TString TupleName = "B2DPi_D2KPiPi0";
    TString mag = "Combined";
    TString charge = "Combined";
    int year = 2016;

    // Open file corresponding to decay detailed
    // above, and get its TTree.
    TFile* f = TFile::Open(Form("/data/lhcb/users/colmmurphy/ReducedTrees/%i/Mag%s/%sCharge/FullBDT_%s_MC.root", year, mag.Data(), charge.Data(), TupleName.Data()), "READ");
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
    std::vector < double > bdt_cuts;
    std::vector < double > :: const_iterator cut_iter;
    std::vector < double > efficiencies;
    std::vector < double > :: const_iterator eff_iter;
    
    // Variables to hold value of number of entries
    // after TM and BDT cut
    int n_bdt;
    
    // Fill the BDT cut vector
    int j=0;
    for (double i=-0.3; i<0.35; i+=0.005) {
    // j gives the length of this vector
    // which needs to be given to the 
    // TGraph
        bdt_cuts.push_back(i);
        j++;
    }
    
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
    for (iter = bdt_cuts.begin(); iter != bdt_cuts.end(); iter++) {
        n_bdt = t->GetEntries(cuts+Form("&& FullBDT>%f", *iter));
        // Uncomment following line to see actual values
        //std::cout << Form("BDT\t%0.2f\t:\t", *iter) << n_bdt << std::endl;
        // Fill vector
        efficiencies.push_back(double(n_bdt)/n_tm);
    }

    // Create TGraph to fill
    TGraph* gr = new TGraph(j, &(bdt_cuts[0]), &(efficiencies[0]));
    gr->SetLineWidth(4);
    gr->SetLineColor(kRed+2);
    gr->SetTitle("BDT Cut Efficiency for "+a.latex_comb+Form(" (%i MC)", year));
    gr->GetXaxis()->SetTitle("BDT Cut Value");
    gr->GetYaxis()->SetTitle("Efficiency");
    
    // Add a TPave to show
    // which BDT was used
    TPaveText* t1 = new TPaveText(0.2, 0.3, 0.4, 0.4, "NDC");
    t1->AddText("Full BDT");
    
    gr->Draw("AC");
    t1->Draw("br");
    
    
    f->Close();
    
    

}
