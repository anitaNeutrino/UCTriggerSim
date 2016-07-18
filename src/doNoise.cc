#include "TFile.h"
#include "TriggerBits.h"
#include "TMath.h"
#include "TH1.h" 
#include "CombinatoricTrigger.h" 
#include "TTree.h"
#include "TEventList.h" 


void doNoise(double thresh, int linpol = 0) 
{
  TFile f(TString::Format("bitstreams/thresh_%g.root",thresh)); 
  TTree * t = (TTree*) f.Get("bitstream"); 


  std::vector<TriggerBits> top_raw[16][2]; 
  std::vector<TriggerBits> middle_raw[16][2]; 
  std::vector<TriggerBits> bottom_raw[16][2]; 
  std::vector<TriggerBits> top[16][2]; 
  std::vector<TriggerBits> middle[16][2]; 
  std::vector<TriggerBits> bottom[16][2]; 





  t->Draw(">>elist",TString::Format("linpol == %d",linpol)); 

  TEventList * elist = (TEventList*) gDirectory->Get("elist"); 
  t->SetEventList(elist); 

  t->Draw("b1.getBits()->CountBits() + b2.getBits()->CountBits() / ( b1.getDt() * b1.getBits()->GetNbits())","","goff"); 

  TH1 * htemp = (TH1 *) gDirectory->Get("htemp"); 

  printf("L1 Rate: %f Hz\n", htemp->GetMean()); 

  
  TriggerBits * b1 = 0; 
  TriggerBits * b2 = 0; 

  t->SetBranchAddress("b1",&b1); 
  t->SetBranchAddress("b2",&b2); 
  t->GetEntry(0); 
  double dt = b1->getDt(); 
  double nbits = b1->getBits()->GetNbits(); 

  int phi_sector = 0; 

  //just to make sure we get the same number in all of them! 
  int N = t->GetEntries() / 48; 
  long max = N * 48; 


  for (int i = 0; i < max; i++) 
  {

    t->GetEntry(i); 
    
    if(i % 3 == 0)
    {

      top_raw[phi_sector][0].push_back(*b1); 
      top_raw[phi_sector][1].push_back(*b2); 
      top[phi_sector][0].push_back(*b1); 
      top[phi_sector][1].push_back(*b2); 
    }
    else if ( i % 3 == 1)
    {
      middle_raw[phi_sector][0].push_back(*b1); 
      middle_raw[phi_sector][1].push_back(*b2); 
      middle[phi_sector][0].push_back(b1->prolongTrigger(2)); 
      middle[phi_sector][1].push_back(b2->prolongTrigger(2)); 
    }
    else
    {
      bottom_raw[phi_sector][0].push_back(*b1); 
      bottom_raw[phi_sector][1].push_back(*b2); 
      bottom[phi_sector][0].push_back(b1->prolongTrigger(3)); 
      bottom[phi_sector][1].push_back(b2->prolongTrigger(3)); 
    }

    phi_sector = (phi_sector+1) % 16; 
  }


  const TBits* bits[linpol ? 3 : 6]; 
  
  int counter = 0; //L2 counter
  int nL2 = 0; 
  int nL3; 
  int idx = 0;  //L3 counter 
  int last_idx = -100;//used for L3 holdoff



  TriggerBits  * L2 = new TriggerBits[16]; 

  for (int itop= 0; itop< N; itop++)
  {
    for (int imid = 0; imid < N; imid++)
    {
      for (int ibot = 0; ibot < N; ibot++)
      {

        for (int j = 0; j < 16; j++)
        {

          counter++; 

          if (linpol)
          {
            TBits answer(top[j][0][itop].getBits()->GetNbits()); 

            for (int pol = 0; pol < 2; pol++)
            {
              bits[0] = top[j][pol][itop].getBits(); 
              bits[1] = middle[j][pol][imid].getBits(); 
              bits[2] = bottom[j][pol][ibot].getBits(); 

              answer |= combinatoric_and(bits, 2, 3); 
            }

            new (&L2[j]) TriggerBits(answer, top[j][0][itop].getT0(), top[j][0][itop].getDt()); 
          }
          else
          {
            bits[0] = top[j][0][itop].getBits(); 
            bits[1] = top[j][1][itop].getBits(); 
            bits[2] = middle[j][0][imid].getBits(); 
            bits[3] = middle[j][1][imid].getBits(); 
            bits[4] = bottom[j][0][ibot].getBits(); 
            bits[5] = bottom[j][1][ibot].getBits(); 

            TBits answer = combinatoric_and(bits,4,6); 
            new (&L2[j]) TriggerBits(answer, top[j][0][itop].getT0(), top[j][0][itop].getDt()); 
          }

          TriggerBits countL2 = L2[j].suppressPastFirst(); 

          int ntriggers = countL2.getBits()->CountBits(); 

          nL2 += ntriggers; 
        }

        for (int j = 0; j < 16; j++)
        {
          if (L2[j].coincidence(L2[ (j+1) % 16] ).getBits()->CountBits() && idx - 32 > last_idx)
          {
            nL3 ++; 
            break; 
            last_idx = idx; 
          }
        }

        for (int j = 0; j < 16; j++)
        {
          L2[j].~TriggerBits(); 
        }
        

        idx++; 
      }
    }
  }

  printf("%d L2 triggers out of %d phi-sector-period combinations considered (%f Hz)\n", nL2, counter, nL2 / (counter * dt * nbits)); 
  printf("%d L3 triggers out of %d global period combinations considered (%f Hz)\n", nL3, idx, nL3 / (idx * dt * nbits)); 
  printf("Expected L3 from L2: %f\n",  16 * 2*TMath::Power(nL2 / (counter * nbits*dt),2) * dt); 
}

int main (int nargs, char ** args) 
{

  if (nargs > 2) 
  {
    doNoise(atof(args[1]), atoi(args[2]));
  }
  else
  {
    doNoise(atof(args[1])); 
  }


  
  return 0; 
}
