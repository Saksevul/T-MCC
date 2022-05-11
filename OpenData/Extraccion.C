
TString TAOD = "Jet";
TString nAOD = "0001";


void CloneBranch() {   Float_t PI=TMath::Pi(); Int_t nprint=1;

    printf("\t\t I) Iniciando clonación  CaloTowersSorted_towerMaker__RECO.\n");

  //Abrimos los archivos que nos interesan y los árboles que queremos leer.
  TFile *InputFile = new TFile("/home/saksevul/CMS_1.3.0/CMS_Run2011A/"+TAOD+"_20000/"+nAOD+".root");
  // Gwt old tree
  TTree *Eventos = (TTree*)InputFile->Get("Events");

  // Deactive all branches
  Eventos -> SetBranchStatus("*",0);

  // Activate branhes only
  Eventos -> SetBranchStatus("CaloTowersSorted_towerMaker__RECO.",1);
  Eventos -> SetBranchStatus("CaloTowersSorted_towerMaker__RECO.obj.obj.phi_",1);
  Eventos -> SetBranchStatus("CaloTowersSorted_towerMaker__RECO.obj.obj.eta_",1);
  Eventos -> SetBranchStatus("CaloTowersSorted_towerMaker__RECO.obj.obj.emE_",1);
  Eventos -> SetBranchStatus("CaloTowersSorted_towerMaker__RECO.obj.obj.hadE_",1);

    printf("\t\t II) Clonación en proceso\n");

  // Create a neu file + a clone of old tree header. Do not copy events
  TFile *NewFile = new TFile("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/CT-"+nAOD+".root","recreate");
  auto newtree = Eventos->CloneTree();

  newtree -> Print();

  // Flush to disk
  NewFile -> Write();

  // Clean
  delete InputFile;
  delete NewFile;

    printf("\t\t III) Clonación finalizada\n");

}




void DrawHist(TH3F *h_3Dem, TH3F *h_3Dhad, Int_t ev, Int_t nJets[3]) {

  gStyle->SetOptStat(0);
  // gStyle->SetAxisColor(0, "xy");
  gStyle->SetPalette(kGreyScale);
  // gStyle->SetPalette(kSolar);
  // gStyle->SetTitleFontSize(0.06);
  // gStyle->SetPalette(kCherry);
  // gStyle->SetPalette(kAvocado);

  TColor::InvertPalette();

  Int_t nBinPhi_ECal = h_3Dem->GetNbinsX();  Int_t nBinEta_ECal = h_3Dem->GetNbinsY();
  Int_t nBinPhi_HCal = h_3Dhad->GetNbinsX(); Int_t nBinEta_HCal = h_3Dhad->GetNbinsY();

  Float_t LimPhi_ECal = h_3Dem->GetXaxis()->GetXmax();
  Float_t LimEta_ECal = h_3Dem->GetYaxis()->GetXmax();
  Float_t LimPhi_HCal = h_3Dhad->GetXaxis()->GetXmax();
  Float_t LimEta_HCal = h_3Dhad->GetYaxis()->GetXmax();

  // Creamos los histogramas necesarios
  TH2F *h_em  = new TH2F("em",  "Distribucion de energ#acute{i}a en el ECal; #phi; #eta; Energ#acute{i}a", nBinPhi_ECal, -LimPhi_ECal, LimPhi_ECal, nBinEta_ECal, -LimEta_ECal, LimEta_ECal);
  TH2F *h_had = new TH2F("had", "Distribucion de energ#acute{i}a en el HCal; #phi; #eta; Energ#acute{i}a", nBinPhi_HCal, -LimPhi_HCal, LimPhi_HCal, nBinEta_HCal, -LimEta_HCal, LimEta_HCal);

  // Llenar los histogramas rotados adecuadamente
  for (size_t eta=nBinEta_ECal; eta>=1; eta--) {
    for (size_t phi=1; phi<=nBinPhi_ECal; phi++) {
      // if (h_3Dem->GetBinContent(phi,eta,ev+1) >= 1.0)
        h_em -> SetBinContent(phi,eta, log(h_3Dem->GetBinContent(phi,eta,ev+1)+1.0));
      if (phi<=nBinPhi_HCal && eta<=nBinEta_HCal/* && phi,eta,h_3Dhad->GetBinContent(phi,eta,ev+1) >= 1.0*/)
        h_had -> SetBinContent(phi,eta, log(h_3Dhad->GetBinContent(phi,eta,ev+1)+1.0));
    }
  }
  // Convertir el número de Jets en una cadena.
  Int_t M = 3;
  // Sin, Cortes+Umbrales, Energía
  char m[M], mE[M], mEE[M];
  gcvt(nJets[0], M, m);
  gcvt(nJets[1], M, mE);
  gcvt(nJets[2], M, mEE);
  TString s=m, sE=mE, sEE=mEE;

  // Gráfico del ECal
  TCanvas* ECal_Canvas = new TCanvas("ECal_Canvas","ECal_Canvas",0,0,405,270);
  // h_em->SetTitle("Energ#acute{i}a en el ECal   (N_{Jets}= "+sEE+" / "+sE+" / "+s+"); #phi; #eta; ");
  h_em->SetTitle("; #phi; #eta; E");
  h_em->GetZaxis()->SetTitleOffset(0.75);
  h_em->GetXaxis()->CenterTitle(true);  //h_em->GetYaxis()->CenterTitle(true);
  h_em->GetXaxis()->SetTickLength(0.0); h_em->GetYaxis()->SetTickLength(0.0);
  // h_em->SetStats(0);
  h_em->Draw("COLZ");
  ECal_Canvas -> Print("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/Hist3D/"+nAOD+".pdf(","pdf");
  ECal_Canvas -> Print("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/Hist3D/ECal-"+nAOD+".jpg","jpg");
  // ECal_Canvas -> Close();

  // Gráfico del HCal
  TCanvas* HCal_Canvas = new TCanvas("HCal_Canvas","HCal_Canvas",0,0,405,270);
  // h_had->SetTitle("Energ#acute{i}a en el HCal   (N_{Jets}= "+sEE+" / "+sE+" / "+s+"); #phi; #eta; ");
  h_had->SetTitle("; #phi; #eta; E");
  h_had->GetZaxis()->SetTitleOffset(0.75);
  h_had->GetXaxis()->CenterTitle(true);  //h_had->GetYaxis()->CenterTitle(true);
  h_had->GetXaxis()->SetTickLength(0.0); h_had->GetYaxis()->SetTickLength(0.0);
  // h_had->SetStats(0);
  h_had->Draw("COLZ");
  HCal_Canvas -> Print("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/Hist3D/"+nAOD+".pdf","pdf");
  HCal_Canvas -> Print("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/Hist3D/HCal-"+nAOD+".jpg","jpg");
  // HCal_Canvas -> Close();

  // projecting histograms
  // the projections always contain double values !
  TH1* h_phi = h_em->ProjectionX(); // ! TH1D, not TH1F
  TH1* h_eta = h_em->ProjectionY(); // ! TH1D, not TH1F
  h_phi->SetMarkerColor(4); h_eta->SetMarkerColor(4);


  // Gráfico de las proyecciones en phi y eta del ECal
  TCanvas* Proj_Canvas = new TCanvas("Proj_Canvas","Proj_Canvas",0,0,3240,2160);
  TPad *Phi_ECalProj = new TPad("Phi_ECalProj","", 0, 0.5, 0.5, 1); Phi_ECalProj->SetMargin(0.08,0,0.12,0); Phi_ECalProj->Draw();
  TPad *Eta_ECalProj = new TPad("Eta_ECalProj","", 0.5, 0.5, 1, 1); Eta_ECalProj->SetMargin(0.08,0,0.12,0); Eta_ECalProj->Draw();
  TPad *Phi_HCalProj = new TPad("Phi_HCalProj","", 0, 0, 0.5, 0.5); Phi_HCalProj->SetMargin(0.08,0,0.12,0); Phi_HCalProj->Draw();
  TPad *Eta_HCalProj = new TPad("Eta_HCalProj","", 0.5, 0, 1, 0.5); Eta_HCalProj->SetMargin(0.08,0,0.12,0); Eta_HCalProj->Draw();
  Phi_ECalProj->cd();
  Float_t mean = h_phi->GetMean(), meanE = h_phi->GetMeanError();
  m[M], mE[M];
  gcvt(mean, M, m);
  gcvt(meanE, M, mE);
  s = m, sE = mE;
  h_phi->SetTitle("Proy_{#phi} del ECal   (#mu = "+s+" #pm "+sE+"); #phi; Energ#acute{i}a");
  h_phi->GetXaxis()->SetTickLength(0.0); h_phi->GetYaxis()->SetTickLength(0.0);
  h_phi->DrawCopy("P");
  Eta_ECalProj->cd();
  mean = h_eta->GetMean(), meanE = h_eta->GetMeanError();
  gcvt(mean, M, m);
  gcvt(meanE, M, mE);
  s = m, sE = mE;
  h_eta->SetTitle("Proy_{#eta} del ECal   (#mu = "+s+" #pm "+sE+"); #eta; Energ#acute{i}a");
  h_eta->GetXaxis()->SetTickLength(0.0); h_eta->GetYaxis()->SetTickLength(0.0);
  h_eta->DrawCopy("P");

  // Ahora con el tro calorimetro.
  h_phi = h_had->ProjectionX(); h_eta = h_had->ProjectionY();
  h_phi->SetMarkerStyle(21); h_eta->SetMarkerStyle(21);
  h_phi->SetMarkerColor(4); h_eta->SetMarkerColor(4);
  h_phi->SetMarkerSize(2); h_eta->SetMarkerSize(2);

  Phi_HCalProj->cd();
  mean = h_phi->GetMean(), meanE = h_phi->GetMeanError();
  gcvt(mean, M, m);
  gcvt(meanE, M, mE);
  s = m, sE = mE;
  h_phi->SetTitle("Proy_{#phi} del HCal   (#mu = "+s+" #pm "+sE+"); #phi; Energ#acute{i}a");
  h_phi->GetXaxis()->SetTickLength(0.0); h_phi->GetYaxis()->SetTickLength(0.0);
  h_phi->DrawCopy("P");

  Eta_HCalProj->cd();
  mean = h_eta->GetMean(), meanE = h_eta->GetMeanError();
  gcvt(mean, M, m);
  gcvt(meanE, M, mE);
  s = m, sE = mE;
  h_eta->SetTitle("Proy_{#eta} del HCal   (#mu = "+s+" #pm "+sE+"); #eta; Energ#acute{i}a");
  h_eta->GetXaxis()->SetTickLength(0.0); h_eta->GetYaxis()->SetTickLength(0.0);
  h_eta->DrawCopy("P");
  Proj_Canvas -> Print("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/Hist3D/"+nAOD+".pdf)","pdf");
  // Proj_Canvas -> Close();
}




void WriteTxt(TH3F *h_em, TH3F *h_had, Int_t ev, TString path) {

  Int_t nBinPhi_ECal = h_em->GetNbinsX();  Int_t nBinEta_ECal = h_em->GetNbinsY();
  Int_t nBinPhi_HCal = h_had->GetNbinsX(); Int_t nBinEta_HCal = h_had->GetNbinsY();

  // Archivos de salida para las imagenes
  FILE *ECalF;  ECalF = fopen("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/"+path+"/ECal-"+nAOD+".txt", "w");
  FILE *HCalF;  HCalF = fopen("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/"+path+"/HCal-"+nAOD+".txt", "w");

  // Escribir en los archivos de salida
  for (size_t e=1; e<=ev; e++) {
    for (size_t eta=nBinEta_ECal; eta>=1; eta--) {
      for (size_t phi=1; phi<=nBinPhi_ECal; phi++) {
        fprintf(ECalF, "%f ", log(h_em->GetBinContent(phi,eta,e)+1.0));
        if (phi<=nBinPhi_HCal && eta<=nBinEta_HCal)
          fprintf(HCalF, "%f ", log(h_had->GetBinContent(phi,eta,e)+1.0));
      }
    }
    // // Escribe una lienea en blanco para diferenciar cada evento
    fprintf(ECalF, "\n");
    fprintf(HCalF, "\n");
  }

  // Cerrar los archivos creados
  fclose(ECalF);
  fclose(HCalF);
}



bool DiJet(TLeaf* Jets_pt_, TLeaf* Jets_phi_, TLeaf* Jets_eta_, Int_t e, Int_t i, Int_t first, Int_t second) {
  Float_t PI=TMath::Pi();
  Float_t dPhi;
  // topología Di-Jet
  if (i>first && second<0 && Jets_pt_->GetValue(i)>50) second = i;
  if (second<0)  return false;
  // atlas
  if (Jets_pt_->GetValue(first)/Jets_pt_->GetValue(second)>1.5)  return false;
  // cms
  if (fabs(Jets_eta_->GetValue(first))>1.3 || fabs(Jets_eta_->GetValue(second))>1.3)  return false;
  dPhi = fabs(Jets_phi_->GetValue(first)-Jets_phi_->GetValue(second));
  if (dPhi > PI) dPhi = 2*PI-dPhi;
  if (dPhi < 2.7)  return false;
  if (i>second && Jets_pt_->GetValue(i)>5 && Jets_pt_->GetValue(i)*2/(Jets_pt_->GetValue(first)+Jets_pt_->GetValue(second))>0.2)  return false;

  return true;
}



bool MultiJet(TLeaf* Jets_pt_, TLeaf* Jets_phi_, TLeaf* Jets_eta_, Int_t i, Int_t first, Int_t second) {
  Float_t PI=TMath::Pi();
  Float_t dPhi;
  // topología multijet
  // Los jets están ordenados de manera decreciente respecto al pT
  if (Jets_pt_->GetValue(first) < 250 || fabs(Jets_eta_->GetValue(first)) > 1.3)  return false;
  // recoil system
  if (i <= first || fabs(Jets_eta_->GetValue(i)) > 1.479)  return false;
  dPhi = fabs(Jets_phi_->GetValue(first)-Jets_phi_->GetValue(i));
  if (dPhi > PI) dPhi = 2*PI-dPhi;
  if (dPhi < 2.8)  return false;
  if (25>Jets_pt_->GetValue(i) || Jets_pt_->GetValue(i)>750)  return false;

  return true;
}




void Jets() {
  Float_t PI=TMath::Pi();
  Int_t nprint=1;

  //Abrimos los archivos que nos interesan y los árboles que queremos leer.
  TFile *InputFile = new TFile("/media/saksevul/HDD/CMS_Run2011A/"+TAOD+"_20000/"+nAOD+".root");
  // TFile *InputFile = new TFile("/media/saksevul/Pawahtun/CMS_Run2011A/"+TAOD+"_20000/"+nAOD+".root");

  //Crear el archivo de salida que queremos. Tiene que ir después del inputFile.
  // TFile *OutputFile = new TFile("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/Jets/"+nAOD+".root", "RECREATE");

  // jets file
  FILE *diJetF;  diJetF = fopen("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/DiJet/Jet-"+nAOD+".txt", "w");
  FILE *multiJetF;  multiJetF = fopen("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/MultiJet/Jet-"+nAOD+".txt", "w");
  FILE *randomJetF;  randomJetF = fopen("/home/saksevul/T-MCC/OpenData/"+TAOD+"_20000/RandomJet/Jet-"+nAOD+".txt", "w");

  // Cargamos el átbol de eventos
  TTree *Eventos = (TTree*)InputFile->Get("Events");


    printf("\n\t JCA: ak5PF, InputFile: "+TAOD+"_20000/"+nAOD+".root \n\n\t\t %i) Archivos abiertos\n", nprint++);


  // PFJets
  TLeaf *PFJets_pt_                        = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.pt_");
  TLeaf *PFJets_eta_                       = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.eta_");
  TLeaf *PFJets_phi_                       = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.phi_");
  TLeaf *PFJets_mChargedHadronEnergy       = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mChargedHadronEnergy");
  TLeaf *PFJets_mNeutralHadronEnergy       = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mNeutralHadronEnergy");
  TLeaf *PFJets_mElectronEnergy            = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mElectronEnergy");
  TLeaf *PFJets_mPhotonEnergy              = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mPhotonEnergy");
  TLeaf *PFJets_mMuonEnergy                = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mMuonEnergy");
  TLeaf *PFJets_mChargedHadronMultiplicity = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mChargedHadronMultiplicity");
  TLeaf *PFJets_mNeutralHadronMultiplicity = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mNeutralHadronMultiplicity");
  TLeaf *PFJets_mPhotonMultiplicity        = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mPhotonMultiplicity");
  TLeaf *PFJets_mElectronMultiplicity      = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mElectronMultiplicity");
  TLeaf *PFJets_mMuonMultiplicity          = Eventos->GetLeaf("recoPFJets_ak5PFJets__RECO.obj.m_specific.mMuonMultiplicity");
  // CaloJets
  TLeaf *CaloJets_pt_         = Eventos->GetLeaf("recoCaloJets_ak5CaloJets__RECO.obj.pt_");
  TLeaf *CaloJets_eta_        = Eventos->GetLeaf("recoCaloJets_ak5CaloJets__RECO.obj.eta_");
  TLeaf *CaloJets_phi_        = Eventos->GetLeaf("recoCaloJets_ak5CaloJets__RECO.obj.phi_");
  TLeaf *CaloJets_emEnergyEB  = Eventos->GetLeaf("recoCaloJets_ak5CaloJets__RECO.obj.m_specific.mEmEnergyInEB");
  TLeaf *CaloJets_hadEnergyHB = Eventos->GetLeaf("recoCaloJets_ak5CaloJets__RECO.obj.m_specific.mHadEnergyInHB");
  TLeaf *CaloJets_hadEnergyHO = Eventos->GetLeaf("recoCaloJets_ak5CaloJets__RECO.obj.m_specific.mHadEnergyInHO");
  // Colosimetros
  TLeaf *CaloTowers_eta_          = Eventos->GetLeaf("CaloTowersSorted_towerMaker__RECO.obj.obj.eta_");
  TLeaf *CaloTowers_phi_          = Eventos->GetLeaf("CaloTowersSorted_towerMaker__RECO.obj.obj.phi_");
  TLeaf *CaloTowers_emEnergy      = Eventos->GetLeaf("CaloTowersSorted_towerMaker__RECO.obj.obj.emE_");
  TLeaf *CaloTowers_hadEnergy     = Eventos->GetLeaf("CaloTowersSorted_towerMaker__RECO.obj.obj.hadE_");


    printf("\t\t %i) Ramas leidas\n", nprint++);


  // Numero de eventos elegidos
  Int_t nEv = 256,   nJet = 8;
  Int_t nEvDi = nEv,  nEvMulti = nEv,  nEvRandom = nEv;
  // multilplicidad de jets
  Int_t jetMult[8]{0, 0, 0, 0, 0, 0, 0, 0};

  Float_t nRbE=5, nRbH=1;
  Float_t dPhi_ECal=2*PI/360.0,  dEta_ECal=0.0174,  dPhi_HCal=5*dPhi_ECal,  dEta_HCal=5*dEta_ECal;
  Float_t LimPhi_ECal=PI,  LimEta_ECal=1.479,  LimPhi_HCal=LimPhi_ECal,  LimEta_HCal=LimEta_ECal;
  Int_t nBinPhi_ECal=(2*LimPhi_ECal/dPhi_ECal)/nRbE,  nBinEta_ECal=(2*LimEta_ECal/dEta_ECal)/nRbE,
        nBinPhi_HCal=(2*LimPhi_HCal/dPhi_HCal)/nRbH,  nBinEta_HCal=(2*LimEta_HCal/dEta_HCal)/nRbH;
  Int_t JetCoord[nEv][10][10];

  // Jets
  TH1F *h_Jets__Multiplicity  = new TH1F("Jets_Multiplicity", "Multiplicidad, de PFJet por Evento; Multilicidad; Frecuencia", 24, 0, 24);
  // Calorimetros
  TH3F *h_CaloTowers_3D_emDi  = new TH3F("CaloTowers_3D_emDi",  "Distribucion 3D en el ECal; #phi; #eta; Ocurrencia", nBinPhi_ECal, -LimPhi_ECal, LimPhi_ECal, nBinEta_ECal, -LimEta_ECal, LimEta_ECal, nEv, 0, nEv);
  TH3F *h_CaloTowers_3D_hadDi = new TH3F("CaloTowers_3D_hadDi", "Distribucion 3D en el HCal; #phi; #eta; Ocurrencia", nBinPhi_HCal, -LimPhi_HCal, LimPhi_HCal, nBinEta_HCal, -LimEta_HCal, LimEta_HCal, nEv, 0, nEv);

  TH3F *h_CaloTowers_3D_emMulti = new TH3F("CaloTowers_3D_emMulti",  "Distribucion 3D en el ECal; #phi; #eta; Ocurrencia", nBinPhi_ECal, -LimPhi_ECal, LimPhi_ECal, nBinEta_ECal, -LimEta_ECal, LimEta_ECal, nEv, 0, nEv);
  TH3F *h_CaloTowers_3D_hadMulti= new TH3F("CaloTowers_3D_hadMulti", "Distribucion 3D en el HCal; #phi; #eta; Ocurrencia", nBinPhi_HCal, -LimPhi_HCal, LimPhi_HCal, nBinEta_HCal, -LimEta_HCal, LimEta_HCal, nEv, 0, nEv);

  TH3F *h_CaloTowers_3D_emRandom = new TH3F("CaloTowers_3D_emRandom",  "Distribucion 3D en el ECal; #phi; #eta; Ocurrencia", nBinPhi_ECal, -LimPhi_ECal, LimPhi_ECal, nBinEta_ECal, -LimEta_ECal, LimEta_ECal, nEv, 0, nEv);
  TH3F *h_CaloTowers_3D_hadRandom= new TH3F("CaloTowers_3D_hadRandom", "Distribucion 3D en el HCal; #phi; #eta; Ocurrencia", nBinPhi_HCal, -LimPhi_HCal, LimPhi_HCal, nBinEta_HCal, -LimEta_HCal, LimEta_HCal, nEv, 0, nEv);


    printf("\t\t %i) Histogramas creados\n", nprint++);


  Float_t etaMax = 1.3, MaxValue = 0.0, dPhi;
  Int_t e, MaxValue_EventID=0, nJets[3], jetSize;
  Int_t ev = 0;
  Int_t evDi = ev,  evMulti = ev,  evRandom = ev;

  Bool_t diJet, multiJet, randomJet, acceptDi, acceptMulti;
  Int_t first, second, recoil;

  Int_t JetMultiplicity[2];
  Float_t Jet_E, ChargedPartMultiplicity, JetInternalMult, ChargedEMEnergy, Ck=1.0;

  // Hacemos un loop respecto a todo el conjunto de datos de interés
  for (e=0; e<Eventos->GetEntries(); e++) {
    if (evDi>=nEv && evMulti>=nEv && evRandom>=nEv) break;

    //  Cargamos las ramas cuya información se analizará
    PFJets_pt_        ->GetBranch()->GetEntry(e);
    PFJets_eta_       ->GetBranch()->GetEntry(e);
    PFJets_phi_       ->GetBranch()->GetEntry(e);
    PFJets_mMuonEnergy         ->GetBranch()->GetEntry(e);  PFJets_mMuonMultiplicity         ->GetBranch()->GetEntry(e);
    PFJets_mPhotonEnergy       ->GetBranch()->GetEntry(e);  PFJets_mPhotonMultiplicity       ->GetBranch()->GetEntry(e);
    PFJets_mElectronEnergy     ->GetBranch()->GetEntry(e);  PFJets_mElectronMultiplicity     ->GetBranch()->GetEntry(e);
    PFJets_mNeutralHadronEnergy->GetBranch()->GetEntry(e);  PFJets_mNeutralHadronMultiplicity->GetBranch()->GetEntry(e);
    PFJets_mChargedHadronEnergy->GetBranch()->GetEntry(e);  PFJets_mChargedHadronMultiplicity->GetBranch()->GetEntry(e);

    // Redefinimoslos valores de las topologías
    diJet = false;  multiJet = false;  randomJet = false;  acceptDi = true;  acceptMulti = true;  recoil = 0;
    // Multiplicidades
    JetMultiplicity[0] = 0;    JetMultiplicity[1] = 0;

    // vectores de jets considerados
    vector<Int_t> Jet_id;
    Jet_id.clear();


    // Ciclo sobre el i-ésimo jet del e-ésimo evento para calcular la multiplicidad de jets en un evento
    for (size_t i=0; i<PFJets_pt_->GetLen(); i++) {
      // Corte respecto a eta del jet utilizando PF Jets
      if ( fabs(PFJets_eta_->GetValue(i)) >= etaMax ) continue;

      // Definir las variables necesarias
      Jet_E = PFJets_mMuonEnergy->GetValue(i)+PFJets_mPhotonEnergy->GetValue(i)+PFJets_mElectronEnergy->GetValue(i)+PFJets_mNeutralHadronEnergy->GetValue(i)+PFJets_mChargedHadronEnergy->GetValue(i);
      ChargedPartMultiplicity = PFJets_mMuonMultiplicity->GetValue(i)+PFJets_mElectronMultiplicity->GetValue(i)+PFJets_mChargedHadronMultiplicity->GetValue(i);
      JetInternalMult = PFJets_mMuonMultiplicity->GetValue(i)+PFJets_mPhotonMultiplicity->GetValue(i)+PFJets_mElectronMultiplicity->GetValue(i)+PFJets_mNeutralHadronMultiplicity->GetValue(i)+PFJets_mChargedHadronMultiplicity->GetValue(i);
      // Charged electromagnetic.
      ChargedEMEnergy = PFJets_mMuonEnergy->GetValue(i) + PFJets_mElectronEnergy->GetValue(i);

      // Se neceitan 3 cortes para la toma de datos (según el criterio):
      // Fraccion máxima de la energía de un tipo de partículas: 90%.
        if ( PFJets_mPhotonEnergy->GetValue(i)/Jet_E >= 0.99 ) continue;
        if ( PFJets_mNeutralHadronEnergy->GetValue(i)/Jet_E >= 0.99 ) continue;
      // Número de constituyentes mínimo > 1.
      if ( JetInternalMult < 2 ) continue;
      // Frácción mínima de la energía de hadrones cargados > 0%.
        if ( PFJets_mChargedHadronMultiplicity->GetValue(i) == 0 ) continue;
      // Frácción de partículas electromagnéticas cargadas < 99%.
        if ( ChargedEMEnergy/Jet_E >= 0.99 ) continue;
      // Número de constituyentes cargados > 0.
      if ( ChargedPartMultiplicity < 1 ) continue;


      // // Insertar en el vector
      // Jet_id.push_back(i);

      // EVENTOS DIJET O MULTIJET
      // if (PFJets_pt_->GetValue(i)<50)  continue;

      // recalcular las banderas en función del TAOD y las restricciones cinemáticas
      // if (TAOD=="Jet"/* && DiJet(PFJets_pt_, PFJets_phi_, PFJets_eta_, e, i, first, second)*/){
      // Los jets están ordenados de manera decreciente respecto al pT
        // tomar el jet con mayor pt como el first, ie, first = 0
        // tomar el siguiente como second, ie, second = 1
        // topología Di-Jet
        // atlas
        if (PFJets_pt_->GetValue(0)<50 || PFJets_pt_->GetValue(1)<50)  acceptDi = false;
        if (PFJets_pt_->GetValue(0)/PFJets_pt_->GetValue(1)>1.5)  acceptDi = false;
        // cms
        if (fabs(PFJets_eta_->GetValue(0))>1.3 || fabs(PFJets_eta_->GetValue(1))>1.3)  acceptDi = false;
        // if (fabs(PFJets_eta_->GetValue(i)) > 1.3)  continue;
      if (fabs(PFJets_eta_->GetValue(i)) < 1.3) {
        dPhi = fabs(PFJets_phi_->GetValue(0)-PFJets_phi_->GetValue(1));
        if (dPhi > PI) dPhi = fabs(2*PI-dPhi);
        if (dPhi < 2.7)  acceptDi = false;
        if (i>1 && PFJets_pt_->GetValue(i)>5 && PFJets_pt_->GetValue(i)*2/(PFJets_pt_->GetValue(0)+PFJets_pt_->GetValue(1))>0.2)
          acceptDi = false;
        if (acceptDi)
          JetMultiplicity[1] += 1;
      }
      // else if (TAOD=="MultiJet"/* && MultiJet(PFJets_pt_, PFJets_phi_, PFJets_eta_, i, first, second)*/) {
        // topología multijet
          // pt's 250, 25, 750
        if (PFJets_pt_->GetValue(0) < 250 || fabs(PFJets_eta_->GetValue(0)) > 1.3)  acceptMulti = false;
        // recoil system
      if (fabs(PFJets_eta_->GetValue(i)) < 1.3) {
        dPhi = fabs(PFJets_phi_->GetValue(0)-PFJets_phi_->GetValue(i));
        if (dPhi > PI) dPhi = fabs(2*PI-dPhi);
        if (dPhi > 2.8) {
          if (i > 0)
            if(25>PFJets_pt_->GetValue(i) || PFJets_pt_->GetValue(i)>750) {
              acceptMulti = false;
            }
          if (acceptMulti)
            recoil += 1;
        }
      }


    }  // End of the first i loop.


    // EVENTOS DIJET O MULTIJET
    // Restricciones respecto a los Jets
    if (/*TAOD == "Jet" &&*/ acceptDi && JetMultiplicity[1]>1)
      diJet = true;
    if (/*TAOD == "MultiJet" &&*/ acceptMulti && recoil >= 2 /*&& JetMultiplicity[1]>2*/)
      multiJet = true;
    if (!diJet && !multiJet)
      randomJet = true;

    // hacer que sean mutuamente exclusivos
    if (diJet && multiJet)  continue;

    // restringir el numero de eventos
    if (diJet && evDi>=nEvDi) continue;
    else if (multiJet && evMulti>=nEvMulti) continue;
    else if (evRandom >=nEvRandom) continue;



    //  Cargamos las ramas cuya información se analizará
    CaloJets_pt_        ->GetBranch()->GetEntry(e);
    CaloJets_eta_       ->GetBranch()->GetEntry(e);
    CaloJets_phi_       ->GetBranch()->GetEntry(e);
    CaloJets_emEnergyEB ->GetBranch()->GetEntry(e);
    CaloJets_hadEnergyHB->GetBranch()->GetEntry(e);
    CaloJets_hadEnergyHO->GetBranch()->GetEntry(e);

    // Multiplicidades
    JetMultiplicity[0] = 0;    JetMultiplicity[1] = 0;

    // Ciclo sobre el i-ésimo jet del e-ésimo evento para calcular la multiplicidad de jets en un evento
    for (size_t i=0; i<CaloJets_pt_->GetLen(); i++) {
      // Corte respecto a eta del jet utilizando PF Jets
      if ( CaloJets_pt_->GetValue(i) < 12.0 ) continue;
      if ( fabs(CaloJets_eta_->GetValue(i)) > etaMax ) continue;
      JetMultiplicity[0] += 1;

      // Frácción mínima de la energía de hadrones cargados > 0%.
      if (CaloJets_hadEnergyHB->GetValue(i)+CaloJets_hadEnergyHO->GetValue(i) <= 0.0) continue;
      // agregar las partículas al evento.
      Jet_id.push_back(i);
      JetMultiplicity[1] += 1;
    }  // End of the first i loop.


    jetSize=(Int_t)Jet_id.size();
    // restingir el número de Jets por evento
    if (jetSize > nJet) continue;

    // printf("%2d", jetSize);

    // blancear las clases
    if (randomJet && jetSize==0 && jetMult[0]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==1 && jetMult[1]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==2 && jetMult[2]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==3 && jetMult[3]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==4 && jetMult[4]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==5 && jetMult[5]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==6 && jetMult[6]>=nEvRandom/nJet) continue;
    if (randomJet && jetSize==7 && jetMult[7]>=nEvRandom/nJet) continue;


    // // Multiplicidad de jets.
    // h_Jets__Multiplicity -> Fill(JetMultiplicity[1]);

    // completar la información, de ser necesario
    if (randomJet && jetSize<nJet)
      jetMult[jetSize] += 1;

    // completar la información, de ser necesario
    for (size_t i=0; i<nJet; i++)
      Jet_id.push_back(-1);


    // Ciclo sobre los jets que cumplen las restricciones dadas.
    for (size_t i=0; i<nJet; i++){
      if (Jet_id[i] == -1) {
        if (diJet)
          fprintf(diJetF, "9.999 9.999 ");
        else if (multiJet)
          fprintf(multiJetF, "9.999 9.999 ");
        else if (randomJet)
          fprintf(randomJetF, "9.999 9.999 ");
      } else {
        if (diJet)
          fprintf(diJetF, "%.3f %.3f ", CaloJets_phi_->GetValue(Jet_id[i]), CaloJets_eta_->GetValue(Jet_id[i]));
        else if (multiJet)
          fprintf(multiJetF, "%.3f %.3f ", CaloJets_phi_->GetValue(Jet_id[i]), CaloJets_eta_->GetValue(Jet_id[i]));
        else if (randomJet)
          fprintf(randomJetF, "%.3f %.3f ", CaloJets_phi_->GetValue(Jet_id[i]), CaloJets_eta_->GetValue(Jet_id[i]));
      }
    }
    if (diJet)
      fprintf(diJetF, "\n");
    else if (multiJet)
      fprintf(multiJetF, "\n");
    else if (randomJet)
      fprintf(randomJetF, "\n");



    //  Información de los hits en los calorímetros
    CaloTowers_phi_     ->GetBranch()->GetEntry(e);
    CaloTowers_eta_     ->GetBranch()->GetEntry(e);
    CaloTowers_emEnergy ->GetBranch()->GetEntry(e);
    CaloTowers_hadEnergy->GetBranch()->GetEntry(e);

    // Ciclo sobre todos los hits en los calorimetros.
    // Este valor (CaloTowers_phi_->GetLen()) es el mismo para ambos calorímetros, en phi, eta, emE y hadE.
    for (size_t i=0; i<CaloTowers_phi_->GetLen(); i++) {
      if (fabs(CaloTowers_eta_->GetValue(i)) > 1.479)  continue;
      // LLeno unos histogramas correspondientes a EM
      if (diJet) {
        h_CaloTowers_3D_emDi  -> Fill( CaloTowers_phi_->GetValue(i), CaloTowers_eta_->GetValue(i), evDi, CaloTowers_emEnergy->GetValue(i) );
        h_CaloTowers_3D_hadDi -> Fill( CaloTowers_phi_->GetValue(i), CaloTowers_eta_->GetValue(i), evDi, CaloTowers_hadEnergy->GetValue(i) );
      } else if (multiJet) {
        h_CaloTowers_3D_emMulti  -> Fill( CaloTowers_phi_->GetValue(i), CaloTowers_eta_->GetValue(i), evMulti, CaloTowers_emEnergy->GetValue(i) );
        h_CaloTowers_3D_hadMulti -> Fill( CaloTowers_phi_->GetValue(i), CaloTowers_eta_->GetValue(i), evMulti, CaloTowers_hadEnergy->GetValue(i) );
      } else if (randomJet) {
        h_CaloTowers_3D_emRandom  -> Fill( CaloTowers_phi_->GetValue(i), CaloTowers_eta_->GetValue(i), evRandom, CaloTowers_emEnergy->GetValue(i) );
        h_CaloTowers_3D_hadRandom -> Fill( CaloTowers_phi_->GetValue(i), CaloTowers_eta_->GetValue(i), evRandom, CaloTowers_hadEnergy->GetValue(i) );
      }
    }  // End of the second i loop.

    // if (had_Mult < 40) continue;

    // // Seleccionar el evento a imprimir al final
    // if (em_E+had_E > MaxValue) {
    //   MaxValue = em_E+had_E;
    //   MaxValue_EventID = evDi;
    //   nJets[0] = PFJets_pt_->GetLen();
    //   nJets[1] = JetMultiplicity[0];
    //   nJets[2] = JetMultiplicity[1];
    // }

    // Eventos considerados
    if (diJet)
      evDi += 1;
    else if (multiJet)
      evMulti += 1;
    else if (randomJet)
      evRandom += 1;
  }  // End of Event loop.


  // // Información del Jet
  // h_Jets__Multiplicity-> Write();
  // // Información de los calorímetros
  // h_CaloTowers_3D_emDi  -> Write();
  // h_CaloTowers_3D_hadDi -> Write();
  // h_CaloTowers_3D_emMulti -> Write();
  // h_CaloTowers_3D_hadMulti-> Write();


    printf("\t\t %i) Datos extraidos\n", nprint++);
    printf("\t\t   %i.1) Proporción de extracción: %i-%i-%i / %i / %lld\n", nprint, evDi, evMulti, evRandom, e, Eventos->GetEntries());
    printf("\t\t   %i.2) %i, %i, %i, %i, %i, %i, %i, %i \n", nprint, jetMult[0], jetMult[1], jetMult[2], jetMult[3], jetMult[4], jetMult[5], jetMult[6], jetMult[7]);


  // Dibujar los histogramas en un pdf
  // DrawHist(h_CaloTowers_3D_em, h_CaloTowers_3D_had, MaxValue_EventID, nJets);


  //   printf("\t\t %i) Histogramas dibujados en .pdf\n", nprint++);


  // Escribir en un archio txt
  WriteTxt(h_CaloTowers_3D_emDi, h_CaloTowers_3D_hadDi, evDi, "DiJet");
  WriteTxt(h_CaloTowers_3D_emMulti, h_CaloTowers_3D_hadMulti, evMulti, "MultiJet");
  WriteTxt(h_CaloTowers_3D_emRandom, h_CaloTowers_3D_hadRandom, evRandom, "RandomJet");


    printf("\t\t %i) 'Imagenes' generadas en .txt. Eventos totales: %d\n", nprint++, ev);


  // Cerramos y eliminamos los archivos abierto y creado.
  InputFile->Close();
  // delete OutputFile;
  fclose(diJetF);
  fclose(multiJetF);
  fclose(randomJetF);

    printf("\t\t %i) Extracción terminada!\n\n", nprint++);

}



//###################################################################################################################################################################################
//###################################################################################################################################################################################



void Extraccion() {
  Jets();
  // Random();
  // CloneBranch();
}
