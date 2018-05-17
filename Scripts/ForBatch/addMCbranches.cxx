#include "myDecay.cxx"

void addMCbranches(int i, int j) {

// Arguments are for a bash submission
// script. 
// i gives year
// j gives true or false

std::cout << "--- Parameters:\n"
          << "    i = " << i << '\n'
          << "    j = " << j << std::endl;

if (i == 1) {
    myDecay DK_2011 = myDecay("B2DK_D2KPiPi0", 2011);
    if (j == 0) DK_2011.addBranchesMC(false);
    if (j == 1) DK_2011.addBranchesMC(true);
}
if (i == 2) {
    myDecay DK_2012 = myDecay("B2DK_D2KPiPi0", 2012);
    if (j == 0) DK_2012.addBranchesMC(false);
    if (j == 1) DK_2012.addBranchesMC(true);
}
if (i == 3) {
    myDecay DK_2015 = myDecay("B2DK_D2KPiPi0", 2015);
    if (j == 0) DK_2015.addBranchesMC(false);
    if (j == 1) DK_2015.addBranchesMC(true);
}
if (i == 4) {
    myDecay DK_2016 = myDecay("B2DK_D2KPiPi0", 2016);
    if (j == 0) DK_2016.addBranchesMC(false);
    if (j == 1) DK_2016.addBranchesMC(true);
}
if (i == 5) {
    myDecay DPi_2011 = myDecay("B2DPi_D2KPiPi0", 2011);
    if (j == 0) DPi_2011.addBranchesMC(false);
    if (j == 1) DPi_2011.addBranchesMC(true);
}
if (i == 6) {
    myDecay DPi_2012 = myDecay("B2DPi_D2KPiPi0", 2012);
    if (j == 0) DPi_2012.addBranchesMC(false);
    if (j == 1) DPi_2012.addBranchesMC(true);
}
if (i == 7) {
    myDecay DPi_2015 = myDecay("B2DPi_D2KPiPi0", 2015);
    if (j == 0) DPi_2015.addBranchesMC(false);
    if (j == 1) DPi_2015.addBranchesMC(true);
}
if (i == 8) {
    myDecay DPi_2016 = myDecay("B2DPi_D2KPiPi0", 2016);
    if (j == 0) DPi_2016.addBranchesMC(false);
    if (j == 1) DPi_2016.addBranchesMC(true);
}

std::cout << "Done!\n";

}
