#include "FitTM_MC.cxx"

void purity_plots() {

  double purities[30];
  double bdt_cuts[30];
  
  int j=0;
  for (double i=0.1; i<0.2; i+=0.02) {
  
    bdt_cuts[j] = i;
    
    double ans = FitTM_MC(i);
    
    purities[j] = ans;
    
    std::cout << "--- BDT Cut: " << i << std::endl;
    std::cout << "--- Purity : " << ans << std::endl;
    j++;
  
  }
  
  
  
  TGraph * gr = new TGraph(j, bdt_cuts, purities);
  gr->Draw("AC*");

}
