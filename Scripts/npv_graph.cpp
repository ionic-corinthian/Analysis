#include "myDecay.cxx"

void npv_graph(int year) {

  // A simple script to plot nPV
  // using a TGraph and GetEntries()
  // approach (TChain didn't seem to
  // work properly!)

  myDecay a = myDecay("B2DPi_D2KPiPi0", year);

  double x[20], y[20];
  int n = 10;

  std::cout << '\n';

  for (int i=0; i<n; i++) {
    std::cout << "Calculating n_entries for nPV == " << i << std::endl;
    x[i] = i;
    y[i] = a.t->GetEntries(Form("nPV==%i", i));

  }

  TGraph * gr = new TGraph(n, x, y);
  gr->Draw("AB");
  gr->SetFillColor(kRed+2);
  //gr->SetFillStyle(3004);
  gr->SetTitle(Form("%i: nPV TGraph", year));
  gr->GetXaxis()->SetTitle("nPV");
  gr->GetYaxis()->SetTitle("Events");
 
  gPad->Update();
}
