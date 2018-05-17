#include "truthMatcher.cxx"

void DoAllTruthMatching () {

    std::vector < TString > Tuples = {"B2DK_D2KPiPi0", "B2DPi_D2KPiPi0"};
    std::vector < int > Years = {2011, 2012, 2015, 2016};
    
    std::vector < TString > ::const_iterator tn_iter;
    std::vector < int > ::const_iterator yr_iter;
    
    for (tn_iter = Tuples.begin(); tn_iter != Tuples.end(); tn_iter++) {
        for (yr_iter = Years.begin(); yr_iter != Years.end(); yr_iter++) {
            std::cout << "Performing truth matching for " << *tn_iter << " in year " << *yr_iter << std::endl;
            truthMatcher(*tn_iter, *yr_iter);
        }
    }

}
