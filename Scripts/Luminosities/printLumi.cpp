#include "myDecay.cxx"
#include "StripSameCand.cpp"
void printLumi() {

// This script prints out a table of luminosities
// and their associated errors, for the different
// years.

// Run 1
myDecay d11 = myDecay("B2DPi_D2KPiPi0", 2011);
myDecay d12 = myDecay("B2DPi_D2KPiPi0", 2012);
myDecay d1 = myDecay("B2DPi_D2KPiPi0", 1);
// Run 2
myDecay d15 = myDecay("B2DPi_D2KPiPi0", 2015);
myDecay d16 = myDecay("B2DPi_D2KPiPi0", 2016);
myDecay d17 = myDecay("B2DPi_D2KPiPi0", 2017);
myDecay d2 = myDecay("B2DPi_D2KPiPi0", 2);

// Calculate luminosity information
d11.findLumi();
d12.findLumi();
d15.findLumi();
d16.findLumi();
d17.findLumi();
d1.findLumi();
d2.findLumi();

// Get lumi and error values
double lumi11 = d11.GetLumi();
double lumi11err = d11.GetLumiErr();
double lumi12 = d12.GetLumi();
double lumi12err = d12.GetLumiErr();
double lumi15 = d15.GetLumi();
double lumi15err = d15.GetLumiErr();
double lumi16 = d16.GetLumi();
double lumi16err = d16.GetLumiErr();
double lumi17 = d17.GetLumi();
double lumi17err = d17.GetLumiErr();

double lumi1 = d1.GetLumi();
double lumi1err = d1.GetLumiErr();
double lumi2 = d2.GetLumi();
double lumi2err = d2.GetLumiErr();

double bef11 = d11.mc_t->GetEntries();
double bef12 = d12.mc_t->GetEntries();
double bef15 = d15.mc_t->GetEntries();
double bef16 = d16.mc_t->GetEntries();

double aft11 = StripSameCand(d11.mc_t);
double aft12 = StripSameCand(d12.mc_t);
double aft15 = StripSameCand(d15.mc_t);
double aft16 = StripSameCand(d16.mc_t);

/*
double bef1 = d1.mc_t->GetEntries();
double aft1 = StripSameCand(d1.mc_t);

double bef2 = d2.mc_t->GetEntries();
double aft2 = StripSameCand(d2.mc_t);
*/

// Print table
std::cout << std::endl;
std::cout << "--------------------------------------------------" << '\n'
          << "- LUMINOSITIES                                   -" << '\n'
          << "--------------------------------------------------" << std::endl;
std::cout << "-Year:\t2011\tLuminosity:\t" << lumi11 << " pm " << lumi11err << '\n' 
          << "-Year:\t2012\tLuminosity:\t" << lumi12 << " pm " << lumi12err << '\n'
          << "-Year:\t2015\tLuminosity:\t" << lumi15 << " pm " << lumi15err << '\n'
          << "-Year:\t2016\tLuminosity:\t" << lumi16 << " pm " << lumi16err << '\n'
          << "-Year:\t2017\tLuminosity:\t" << lumi17 << " pm " << lumi17err << std::endl;
std::cout << "--------------------------------------------------" << '\n'
          << "-RUN 1:\t\tLuminosity:\t" << lumi1 << " pm " << lumi1err << '\n'
          << "--------------------------------------------------" << '\n'
          << "-RUN 2:\t\tLuminosity:\t" << lumi2 << " pm " << lumi2err << '\n'
          << "--------------------------------------------------" << std::endl;   

std::cout << std::endl;
std::cout << "--------------------------------------------------" << '\n'
          << "- MULTIPLE CANDIDATES                            -" << '\n'
          << "--------------------------------------------------" << std::endl;
std::cout << "-Year:\t2011\tBefore: " << bef11 << "\tAfter: " << aft11 << '\n'
          << "-Year:\t2012\tBefore: " << bef12 << "\tAfter: " << aft12 << '\n'
          << "-Year:\t2015\tBefore: " << bef15 << "\tAfter: " << aft15 << '\n'
          << "-Year:\t2016\tBefore: " << bef16 << "\tAfter: " << aft16 << std::endl;
std::cout << "--------------------------------------------------" << '\n';
/*
          << "-RUN 1:\t\tBefore:\t" << bef1 << "\tAfter: " << aft1 << '\n'
          << "--------------------------------------------------" << '\n'
          << "-RUN 2:\t\tBefore:\t" << bef2 << "\tAfter: " << aft2 << '\n'
          << "--------------------------------------------------" << std::endl;  
*/
}
