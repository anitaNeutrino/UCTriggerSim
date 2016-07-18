#include "TGraph.h" 
#include "TriggerBits.h" 


UCTrigger::TriggerBits UCTrigger::TriggerBits::prolongTrigger(int n) const
{
  TBits copy = *bits; 

  for (int i = 0; i < n; i++)
  {
    copy |= copy << 1; 
  }

  return TriggerBits(copy, t0, dt); 
}


UCTrigger::TriggerBits::TriggerBits(const TriggerBits & other)
  : t0(other.t0), dt(other.dt)
{
  g = 0; 
  bits = new TBits(*other.bits); 
}

UCTrigger::TriggerBits UCTrigger::TriggerBits::suppressPastFirst() const
{
  return holdoff(1); 
}



UCTrigger::TriggerBits UCTrigger::TriggerBits::holdoff(int n) const
{
  TBits new_bits = *bits; 

  for (int i = 1; i <= n; i++)
  {
    new_bits &= ~(*bits << i); 
  }

  return TriggerBits(new_bits, t0, dt); 
}


UCTrigger::TriggerBits UCTrigger::TriggerBits::coincidence(const TriggerBits & other) const
{
  if ( dt != other.getDt())
  {
    fprintf(stderr, "Can only do concidence if dt's match!\n"); 
    return TriggerBits (TBits(bits->GetNbits()), t0, dt);
  }

  double offset = (other.getT0() - t0) / dt; 
  if (offset != int(offset))
  {

    fprintf(stderr, "Can only do concidence if integer period offset !\n"); 
    return TriggerBits (TBits(bits->GetNbits()), t0, dt);
  }


  TBits new_bits = *other.getBits(); 

  if (offset < 0) new_bits >> int(-offset); 
  if (offset > 0) new_bits << int(offset); 
  new_bits &= *bits; 

  return TriggerBits(new_bits, t0,dt); 
}




TGraph * UCTrigger::TriggerBits::graph() 
{

  if (g) return g; 


  g= new TGraph(); 

  int N = bits->GetNbits(); 
  int last = -1; 
  for (int i = 0; i < N; i++) 
  {
    int now = bits->TestBitNumber(i) ? 1 : 0; 
    if (now != last) 
    {

      if (last >= 0) 
      {
        g->SetPoint(g->GetN(), i * dt + t0, last); 
      }
      last = now;
      g->SetPoint(g->GetN(), i * dt + t0, last);
    }
  }

  g->SetPoint(g->GetN(), N * dt + t0, last); 

  return g; 
}


UCTrigger::TriggerBits::~TriggerBits() 
{
  if (bits) delete bits; 
  if (g) delete g; 
}


