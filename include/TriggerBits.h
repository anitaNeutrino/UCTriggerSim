#ifndef TRIGGER_BITS_H
#define TRIGGER_BITS_H

#include "TBits.h" 

class TGraph; 
class TriggerBits  
{

  public: 
    TriggerBits() { g = 0; bits = 0;} 
    TriggerBits (const TBits & bit, double t0, double dt) 
      :  g(0), t0(t0), dt(dt) { bits = new TBits(bit);}
    TriggerBits (const TriggerBits & other); 
    
    TriggerBits suppressPastFirst() const; 
    TriggerBits holdoff(int n) const; 
    TriggerBits prolongTrigger(int n) const; 
    TriggerBits coincidence(const TriggerBits & other) const ; 
    double getT0() const { return t0; } 
    double getDt() const { return dt; } 


    TGraph * graph() ;
    const TBits * getBits() const { return bits; } 
    virtual ~TriggerBits(); 

  private: 
    mutable TGraph * g;  //!
    TBits * bits; 
    double t0; 
    double dt; 

    ClassDef(TriggerBits,1); 
};



#endif
