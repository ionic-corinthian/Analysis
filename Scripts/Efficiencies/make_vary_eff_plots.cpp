#include "vary_cut_eff.cpp"

void make_vary_eff_plots() {

  info a;

  // Add desired cuts and their ranges
             //                                                   S 
             //                                                   T         S
             //                                                   A   E     T     L P
             //                                                   R   N     E     E O
             //                                                   T   D     P     G S
/*  filler (a, "D0 Flight Significance", "D0_FD_ZSIG>%f"          , 0 , 5   , 0.1 , "TR");
  filler (a, "D0 Swapped Mass Window", "fabs(D0_SWAP_M-1865)>%f", 10, 100 , 2   , "TR");
  filler (a, "D0 Mass Window"        , "fabs(D0_M-1865)<%f"     , 10, 100 , 2   , "MR");
  filler (a, "Bach PIDK"             , "Bach_PIDK<%f"           , 0 , 15  , 0.5 , "MR");
  filler (a, "BDT"                   , "FullBDT_2011>%f"        , -1, 0.4 , 0.05, "ML");
  filler (a, "Pi0 Mass Window"       , "fabs(Pi0_M-135)<%f"     , 7 , 25  , 1   , "MR");
  filler (a, "K0 PIDK"               , "K0_PIDK>%f"             , 0 , 5   , 0.1 , "TR");
  filler (a, "P0 PIDK"               , "P0_PIDK<%f"             , -5, 0.5 , 0.1 , "TL");
*/
  filler (a, "Full BDT"              , "FullBDT_2011>%f"        , -0.05, 0.25 , 0.05, "ML");
  filler (a, "Neutral BDT"           , "neutBDT>%f"             , -0.05, 0.25 , 0.05, "ML");
  filler (a, "Remainder BDT"         , "neut_othersBDT>%f"      , -0.05, 0.25 , 0.05, "ML");

  vary_cut_eff(a);

  // Clean up
  std::get<0>(a).clear();
  std::get<1>(a).clear();
  std::get<2>(a).clear();
  std::get<3>(a).clear();
  std::get<4>(a).clear();
  std::get<5>(a).clear();
}
