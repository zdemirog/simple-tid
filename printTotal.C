void printTotal(){
  TFile *fin=TFile::Open("tgtFlangeRadLevelV0.root","READ");

  const string hNm[4]={"g","em","ep","n"};
  for(int i=0;i<4;i++){
    TH1F *hD=(TH1F*)fin->Get(Form("det5530/d5530_energyDS_allPZ_%s",hNm[i].c_str()));
    TH1F *hU=(TH1F*)fin->Get(Form("det5530/d5530_energyUS_allPZ_%s",hNm[i].c_str()));

    TH1F *hd = (TH1F*)hU->Clone("hDummy");
    hd->Add(hD);
    cout<<hd->GetTitle()<<" "<<hd->Integral()<<endl;
    delete hd;
  }
  fin->Close();
}
