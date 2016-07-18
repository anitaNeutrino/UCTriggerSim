
void makeAllTriggerBits(double thresh = 0.35, double tmin = 0.2e-6, double tmax = -1)
{
  TFile f("data_in_tree_format.root"); 
  TTree * tree = (TTree*) f.Get("shortdata"); 

  TGraph * g0 = 0; 
  TGraph * g1 = 0; 

  TriggerBits * b0 = 0; 
  TriggerBits * b1 = 0; 

  int linpol;
  double snr1, snr2; 
  double angle; 
  int timestamp; 
  int index; 

  tree->SetBranchAddress("short1",&g0); 
  tree->SetBranchAddress("short2",&g1); 
  tree->SetBranchAddress("linpol",&linpol); 
  tree->SetBranchAddress("snr1",&snr1); 
  tree->SetBranchAddress("snr2",&snr2); 
  tree->SetBranchAddress("angle",&angle); 
  tree->SetBranchAddress("timestamp",&timestamp); 

  TFile of(TString::Format("bitstreams/thresh_%g.root", thresh), "RECREATE"); 

  TTree * out = new TTree("bitstream","Bitstream"); 

  out->Branch("b1",&b0); 
  out->Branch("b2",&b1); 
  out->Branch("linpol",&linpol); 
  out->Branch("snr1",&snr1); 
  out->Branch("snr2",&snr2); 
  out->Branch("angle",&angle); 
  out->Branch("timestamp",&timestamp); 
  out->Branch("thresh",&thresh); 

  BitsetMaker m1(thresh); 
  BitsetMaker m2(thresh); 

  for (int i = 0; i < tree->GetEntries(); i++)
  {
    if (i % 100 == 0) printf("%d\n",i); 
    index = i; 
    tree->GetEntry(i); 
    b0 = m1.process(g0, tmin, tmax); 
    b1 = m2.process(g1, tmin, tmax); 
    out->Fill(); 
  }

  out->Write(); 

}

void makeNoiseTriggers(double thresh_start = 0.2, double thresh_stop = 1, double thresh_step = 0.02)
{

  for (double thresh = thresh_start; thresh <= thresh_stop; thresh += thresh_step)
  {
    makeAllTriggerBits(thresh); 
  }
}

