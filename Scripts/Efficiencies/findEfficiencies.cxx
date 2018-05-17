#include "efficienciesMC.cpp"

void findEfficiencies () {

    std::vector < TString > Tuples = {"B2DK_D2KPiPi0", "B2DPi_D2KPiPi0"};
    std::vector < int > Years = {2011, 2012, 2015, 2016};
    std::vector < myDecay > decays;
    std::vector < double > effs;
    std::vector < double > n_trim;

    std::vector < TString > ::const_iterator tn_iter;
    std::vector < int > ::const_iterator yr_iter;
    std::vector < myDecay > ::const_iterator md_iter;
    std::vector < double > ::const_iterator ef_iter;    

    for (yr_iter = Years.begin(); yr_iter != Years.end(); yr_iter++) {
        for (tn_iter = Tuples.begin(); tn_iter != Tuples.end(); tn_iter++) {
            decays.push_back( myDecay(*tn_iter, *yr_iter) );
        }
    }
    
    for (md_iter = decays.begin(); md_iter != decays.end(); md_iter++) {
       effs.push_back(efficienciesMC(*md_iter));
    }
std::cout << '\n';
    for (ef_iter = effs.begin(); ef_iter != effs.end(); ef_iter+=2) {
        std::cout << "result: " << double(*ef_iter/(*(ef_iter+1))) << std::endl;
    }
    
    
}
