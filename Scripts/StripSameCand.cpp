std::pair <TTree*, TH1D*> StripSameCand(TTree* tree, bool verbose=false) {
    // Remove multiple candidates
    // by picking one at random
    
    // VERBOSE: Set to true to print out map values
    
    //=============//
    //=== Setup ===//
    //=============//
    
    // Must use these types, the numbers
    // involved are very large!
    ULong64_t   eventNumber = 0;
    UInt_t      runNumber = 0;
    
    // Only need these two numbers
    tree->SetBranchAddress("runNumber", &runNumber);
    tree->SetBranchAddress("eventNumber", &eventNumber);
    
    // Initialise map to add run/event numbers
    std::map <ULong64_t, std::map <ULong64_t, std::vector<Int_t>>>   candMap;

    //====================//
    //=== Fill the map ===//
    //====================//
    
    for ( int i=0 ; i<tree->GetEntries() ; i++) {
        tree->GetEntry(i);
        candMap[runNumber][eventNumber].push_back(i); 
    }
    
    
    TTree * newTree = tree->CloneTree(0);
    
    //=== Fill + Save a histogram with multiplicity information, i.e. size of 3rd map element ===//  
    TH1D * h1 = new TH1D("h_Multiplicity", "Event Number Multiplicity", 20, 0, 20);
    
    //=== Want to copy the input chain, and output a chain where duplicate Bs have been rand- ===//
    //=== only selected.
    
    int max_vec_len = 0; // Use this to set range of multiplicity histogram later
    typedef std::map <ULong64_t, std::map <ULong64_t, std::vector<Int_t>>>::const_iterator MapIterator;
    if (verbose) std::cout << "=== Map values ===" << std::endl;
    for (MapIterator it = candMap.begin() ; it != candMap.end(); it++) {
        
        if (verbose) std::cout << "Run Number: " << it->first << std::endl << "------------------";
        
        typedef std::map <ULong64_t, std::vector<Int_t>>::const_iterator InnerIterator;
        
        for (InnerIterator it2 = it->second.begin() ; it2 != it->second.end(); it2++) {
            if (verbose) std::cout << std::endl << "Event Number: " << it2->first;
            int vec_len = it2->second.size();
            h1->Fill(vec_len);       
            if (vec_len > max_vec_len) max_vec_len = vec_len;
            
            //=== Copy Tree if size of vector is 1 (i.e. one B cand. only!) ===//
            if (vec_len == 1) {
                
                tree->GetEntry(it2->second[0]);
                newTree->Fill();
                
                }
            
            //=== If there is more than one candidate, pick one at random ===//
            else if (vec_len > 1) {
            
                //=== Generate a random element index ===//
                int random = 0;
                random = rand() % vec_len;
                
                tree->GetEntry(it2->second[random]);
                newTree->Fill();
            
            }
            
            //=== Iterate over vector contents! ===//
            typedef std::vector <Int_t>::const_iterator VectorIterator;
            
            if (verbose) {
                std:: cout << " Locations: ";
                for (VectorIterator it3 = it2->second.begin(); it3 != it2->second.end(); it3++) {
                
                    std::cout << " " << *it3;
                
                }
            }
        }
        if (verbose) std::cout << std::endl;
    }

h1->SetTitle("Candidate Multiplicity");
h1->GetXaxis()->SetTitle("Multiplicity");
h1->GetXaxis()->SetRangeUser(0, max_vec_len+2);
h1->GetYaxis()->SetTitle("Entries");

std::cout << "\n--- INFO: Number of entries after removing multiple candidates: " << newTree->GetEntries() << std::endl;
return std::make_pair(newTree, h1);


}
