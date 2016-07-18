static TCanvas * c = 0;

BitsetMaker * tryOne(int i = 0, const char * branch = "short1", double thresh = 0.35, BitsetMaker * maker = 0, double tmin = 0.2e-6, double tmax =-1, bool crop = false ) 
{

  TFile f("data_in_tree_format.root"); 

  TTree * tree = (TTree*) f.Get("shortdata"); 


  TGraph * g = 0; 
  tree->SetBranchAddress(branch,&g); 
  tree->GetEntry(i); 

  if (!maker) maker = new BitsetMaker(thresh); 

  maker->process(g,tmin,tmax,crop); 
  TGraph * gbits = maker->getLast()->graph(); 
 

  if (!c) 
  {
    c = new TCanvas("try","blahblahblah",1000,500); 
    c->Divide(1,2); 
  }
  c->cd(1); 

  g->Draw("alp"); 
  TGraph *tg_p = new TGraph(2); 
  TGraph *tg_n = new TGraph(2); 

  tg_p->SetPoint(0, g->GetX()[0], thresh); 
  tg_n->SetPoint(0, g->GetX()[0], -thresh); 
  tg_p->SetPoint(1, g->GetX()[g->GetN()-1], thresh); 
  tg_n->SetPoint(1, g->GetX()[g->GetN()-10], -thresh); 
  tg_p->SetLineColor(3); 
  tg_n->SetLineColor(3); 

  tg_p->Draw("lsame"); 
  tg_n->Draw("lsame"); 
  c->cd(2); 
  gbits->Draw("alp"); 

  return maker; 


}
