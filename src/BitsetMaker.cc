#include "BitsetMaker.h" 
#include "TGraph.h" 
#include <algorithm>



TriggerBits * BitsetMaker::process(const TGraph * g, double tmin, double tmax, bool crop) 
{


  int min_i = 0; 
  int max_i = g->GetN(); 

  if (tmin > 0) min_i = std::lower_bound(g->GetX(), g->GetX() + g->GetN(), tmin) - g->GetX(); 
  if (tmax > 0) max_i = std::lower_bound(g->GetX(), g->GetX() + g->GetN(), tmax) - g->GetX(); 

  int nbits = crop ? (max_i - min_i) / factor : (g->GetN()) / factor; 

  TBits bits(nbits); 

  bool clear = true; 
  for (int i = 0; i < nbits; i++) 
  {
    int n_above = 0; 
    int n_below = 0; 

    for (int j = 0; j < factor; j++) 
    {
      int index = factor * i + j; 
      if (crop) index += min_i; 
      if (index > max_i || index < min_i) continue; 
      double val = g->GetY()[index];  
      if (val > thresh) n_above++; 
      else if (val < -thresh) n_below++; 
    }


    if ( (n_above > min || n_below > min)) 
    {
      if (clear) 
      {
        bits.SetBitNumber(i, true); 
        clear = false; 
      }
    }
    else
    {
      clear = true; 
    }
  }

  tbits.~TriggerBits();
  new (&tbits) TriggerBits (bits, g->GetX()[crop ? min_i : 0], factor * (g->GetX()[1]-g->GetX()[0])); 
  return &tbits; 
}



