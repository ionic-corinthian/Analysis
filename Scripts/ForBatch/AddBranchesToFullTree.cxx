#include "myDecay.cxx"

void  AddBranchesToFullTree  (TString decay, Int_t year, TString mag, TString charge) {

    //=========================================================//
    //=== This function is intended to be run on all years, ===//
    //=== mag combinations, and charge combinations. It     ===//
    //=== applies trigger + PIDK cuts to reduce the number  ===//
    //=== of events in a given Tree. It then runs code to   ===//
    //=== generate and add the D0_FD_ZSIG branch to TTree.  ===//
    //=== NB "sameCand()" function should only be applied   ===//
    //=== at the very end of all selection!                 ===//
    //=========================================================//

           std::cout << "--- INFO: AddBranchesToFullTree called with parameters\n"; 
           std::cout << "          Decay  " << decay << '\n'
                     << "          Year   " << year << '\n'
                     << "          Mag    " << mag << '\n'
                     << "          Charge " << charge << '\n';
    
           myDecay a = myDecay(decay, year, mag, charge);
           
           std::cout << "--- INFO: Decay class constructed successfully\n";
           std::cout << "--- INFO: Performing event loop to add branches" << std::endl;
           a.addBranches();
           


        
}
