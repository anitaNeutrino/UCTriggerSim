#ifndef _UC_TRIGGER_COMBINATORIC_TRIGGER_H
#define _UC_TRIGGER_COMBINATORIC_TRIGGER_H 

#include <vector>
#include <algorithm>
#include "TBits.h" 

namespace UCTrigger
{

TBits combinatoric_and(const TBits ** in, size_t M, size_t N)
{

  std::vector<bool> selector(N); 
  std::fill(selector.end()- M, selector.end(), true); 

  TBits answer; 
  //loop over all permutations of selector 
  //see e.g. http://stackoverflow.com/questions/9430568/generating-combinations-in-c
  
  do 
  {
    TBits attempt; 
    attempt=~attempt;  // set to all true

    for (size_t i = 0; i < N; i++) 
    {
      if (selector[i])
      {
        attempt &= *in[i]; 
      }
    }

    answer |= attempt; 

  } while (std::next_permutation(selector.begin(), selector.end())); 

  return answer; 
};
}





#endif
