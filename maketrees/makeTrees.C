#include "TFile.h" 
#include <stdio.h>
#include "TTree.h" 
#include "TGraph.h" 

#include "analyze_trigtest_data.C" 

void makeTree()
{

  FILE * csvfile = fopen("data_info.csv","r"); 
  char buf[1024]; 
  //get first line
  fgets(buf, 1024, csvfile); 


  TFile of("data_in_tree_format.root","RECREATE"); 

  TTree * tree = new TTree("shortdata","Short Data"); 

  TGraph * raw1 = 0; 
  TGraph * raw2 = 0; 
  TGraph * short1 = 0; 
  TGraph * short2 = 0; 

  int linpol; 
  int timestamp;
  double snr1; 
  double snr2; 
  double angle; 


  tree->Branch("raw1",&raw1);
  tree->Branch("raw2",&raw2);
  tree->Branch("short1",&short1);
  tree->Branch("short2",&short2);
  tree->Branch("snr1",&snr1);
  tree->Branch("snr2",&snr2);
  tree->Branch("linpol",&linpol); 
  tree->Branch("timestamp",&timestamp); 
  tree->Branch("angle",&angle); 



  while(fgets(buf,1024,csvfile))
  {
    printf("%s\n", buf); 
    sscanf(buf,"%d,%d,%lf,%lf,%lf", &timestamp, &linpol, &snr1, &snr2, &angle); 

    TString fname = TString::Format("../../data/tds6804_%d.bin", timestamp);
    WaveformSet waves = readBinaryFile(fname.Data()); 

    for (int i = 0; i < waves.numberOfWaveforms()/4; i++)
    {
      raw1 = waves.getGraph(4*i+1); 
      raw1->SetTitle(TString::Format("raw %s" , linpol ? "HPOL" : "LCP")); 
      raw1->SetName(TString::Format("raw_%s" , linpol ? "hpol" : "lcp")); 
      short1 = waves.getGraph(4*i+2); 
      short1->SetTitle(TString::Format("short %s" , linpol ? "HPOL" : "LCP")); 
      short1->SetName(TString::Format("short_%s" , linpol ? "hpol" : "lcp")); 
      raw2 = waves.getGraph(4*i+3); 
      raw2->SetTitle(TString::Format("raw %s" , linpol ? "VPOL" : "RCP")); 
      raw2->SetName(TString::Format("raw_%s" , linpol ? "vpol" : "rcp")); 
 
      short2 = waves.getGraph(4*i+4); 
      short2->SetTitle(TString::Format("short %s" , linpol ? "VPOL" : "RCP")); 
      short2->SetName(TString::Format("short_%s" , linpol ? "vpol" : "rcp")); 
 
      tree->Fill(); 

      delete raw1;
      delete raw2;
      delete short1;
      delete short2;
    }
  }

  tree->Write(); 



}
