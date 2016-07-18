#ifndef BITSET_MAKER_H
#define BITSET_MAKER_H

#include "TBits.h" 
#include "TriggerBits.h" 


class BitsetMaker 
{

  public: 

    BitsetMaker (double threshold,  double fraction_required_above_threshold = 0.5,  int decimation_factor = 20, bool suppress_after = true) 
      : thresh(threshold), fraction(fraction_required_above_threshold), factor(decimation_factor), suppress(suppress_after) 
    {
      min = int(fraction * factor + 0.5); 
    }

    TriggerBits * process(const TGraph * g, double tmin = 0, double tmax = -1, bool crop = true); 

    TriggerBits *  getLast() { return &tbits; }

  private: 
    double thresh; 
    double fraction; 
    int min; 
    int factor; 
    TriggerBits tbits; 
    bool suppress; 
}; 





#endif
