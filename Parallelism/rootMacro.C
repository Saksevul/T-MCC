
void EnergyQuotient() {

  //Abrimos el archivo que nos interesa y el árbol que queremos leer.
  TFile *HtoGGFile = new TFile("/home/saksevul/T/Open-Data/HToGG-0002.root");


  TH2F *Calo_Em_2D_HHtoGG =(TH2F*)HtoGGFile->Get("CaloTowers_2D_em"); //Calo_Em_2D_HHtoGG->Scale(1.0/Calo_Em_2D_HHtoGG->Integral()); //Calo_Em_2D_HHtoGG->Rebin();
  TH2F *Calo_Had_2D_HHtoGG =(TH2F*)HtoGGFile->Get("CaloTowers_2D_had"); //Calo_Had_2D_HHtoGG->Scale(1.0/Calo_Had_2D_HHtoGG->Integral()); //Calo_Had_2D_HHtoGG->Rebin();


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // gStyle->SetPalette(kCherry);
  gStyle->SetPalette(kAvocado);
  TColor::InvertPalette();

  Float_t EtaLim_ECal;

  TCanvas* Calo_Em_2D__HHtoGG = new TCanvas("Calo_Em_2D__HHtoGG","Calo_Em_2D__HHtoGG",0,0,3240,2160);
  TPad *H__Em_2D__HHtoGG = new TPad("H__Em_2D__HHtoGG","", 0, 0, 1, 1);  H__Em_2D__HHtoGG->SetMargin(0.08,0.12,0.1,0.1);  //H__Em_2D__HHtoGG->SetLogz();
  H__Em_2D__HHtoGG->Draw();  H__Em_2D__HHtoGG->cd();
  Calo_Em_2D_HHtoGG->SetTitle("Dep#acute{o}sitos de energ#acute{i}a en el ECAL; #phi [Rad]; #eta; Energ#acute{i}a (GeV)");
  Calo_Em_2D_HHtoGG->GetXaxis()->SetTitleSize(0.04);  Calo_Em_2D_HHtoGG->GetXaxis()->SetTitleOffset(0.9);  Calo_Em_2D_HHtoGG->GetXaxis()->SetLabelSize(0.03);
  Calo_Em_2D_HHtoGG->GetYaxis()->SetTitleSize(0.04);  Calo_Em_2D_HHtoGG->GetYaxis()->SetTitleOffset(0.9);  Calo_Em_2D_HHtoGG->GetYaxis()->SetLabelSize(0.03);
  Calo_Em_2D_HHtoGG->GetZaxis()->SetTitleSize(0.04);  Calo_Em_2D_HHtoGG->GetZaxis()->SetTitleOffset(0.7);  Calo_Em_2D_HHtoGG->GetZaxis()->SetLabelSize(0.03);
  // Calo_Em_2D_HHtoGG->SetAxisRange(-1.479, 1.479, "Y");
  Calo_Em_2D_HHtoGG->SetContour(100);
  Calo_Em_2D_HHtoGG->DrawCopy("colz");
  Calo_Em_2D__HHtoGG->SaveAs("/home/saksevul/T/Parallelism/Calo_2D.pdf("); Calo_Em_2D__HHtoGG->Close();


  gStyle->SetPalette(kCherry);
  // gStyle->SetPalette(kAvocado);
  TColor::InvertPalette();

  TCanvas* Calo_Had_2D__HHtoGG = new TCanvas("Calo_Had_2D__HHtoGG","Calo_Had_2D__HHtoGG",0,0,3240,2160);
  TPad *H__Had_2D__HHtoGG = new TPad("H__Had_2D__HHtoGG","", 0, 0, 1, 1);  H__Had_2D__HHtoGG->SetMargin(0.08,0.12,0.1,0.1);  //H__Had_2D__HHtoGG->SetLogz();
  H__Had_2D__HHtoGG->Draw();  H__Had_2D__HHtoGG->cd();
  Calo_Had_2D_HHtoGG->SetTitle("Dep#acute{o}sitos de energ#acute{i}a en el HCAL; #phi [Rad]; #eta; Energ#acute{i}a (GeV)");
  Calo_Had_2D_HHtoGG->GetXaxis()->SetTitleSize(0.04);  Calo_Had_2D_HHtoGG->GetXaxis()->SetTitleOffset(0.9);  Calo_Had_2D_HHtoGG->GetXaxis()->SetLabelSize(0.03);
  Calo_Had_2D_HHtoGG->GetYaxis()->SetTitleSize(0.04);  Calo_Had_2D_HHtoGG->GetYaxis()->SetTitleOffset(0.9);  Calo_Had_2D_HHtoGG->GetYaxis()->SetLabelSize(0.03);
  Calo_Had_2D_HHtoGG->GetZaxis()->SetTitleSize(0.04);  Calo_Had_2D_HHtoGG->GetZaxis()->SetTitleOffset(0.7);  Calo_Had_2D_HHtoGG->GetZaxis()->SetLabelSize(0.03);
  // Calo_Had_2D_HHtoGG->SetAxisRange(-1.479, 1.479, "Y");
  Calo_Had_2D_HHtoGG->SetContour(100);
  Calo_Had_2D_HHtoGG->DrawCopy("colz");
  Calo_Had_2D__HHtoGG->SaveAs("/home/saksevul/T/Parallelism/Calo_2D.pdf)"); Calo_Had_2D__HHtoGG->Close();




  // gStyle->SetPalette(kCherry);
  gStyle->SetPalette(kAvocado);
  TColor::InvertPalette();

  TCanvas* Calo_Em__HHtoGG = new TCanvas("Calo_Em__HHtoGG","Calo_Em__HHtoGG",0,0,364,198);
  TPad *H__Em__HHtoGG = new TPad("H__Em__HHtoGG","", 0, 0, 1, 1, 0, 0, -1);  H__Em__HHtoGG->SetMargin(0.0,0.0,0.0,0.0);  //H__Em__HHtoGG->SetLogz();
  H__Em__HHtoGG->Draw();  H__Em__HHtoGG->cd();
  Calo_Em_2D_HHtoGG->SetTitle(" ; ; ; ");
  Calo_Em_2D_HHtoGG->GetXaxis()->SetTitleSize(0.0);  Calo_Em_2D_HHtoGG->GetXaxis()->SetTitleOffset(0.0);  Calo_Em_2D_HHtoGG->GetXaxis()->SetLabelSize(0.0);
  Calo_Em_2D_HHtoGG->GetYaxis()->SetTitleSize(0.0);  Calo_Em_2D_HHtoGG->GetYaxis()->SetTitleOffset(0.0);  Calo_Em_2D_HHtoGG->GetYaxis()->SetLabelSize(0.0);
  Calo_Em_2D_HHtoGG->GetZaxis()->SetTitleSize(0.0);  Calo_Em_2D_HHtoGG->GetZaxis()->SetTitleOffset(0.0);  Calo_Em_2D_HHtoGG->GetZaxis()->SetLabelSize(0.0);
  // Calo_Em_2D_HHtoGG->SetAxisRange(-1.479, 1.479, "Y");
  Calo_Em_2D_HHtoGG->SetContour(100);
  Calo_Em_2D_HHtoGG->GetXaxis()->SetTickLength(0.0);  Calo_Em_2D_HHtoGG->GetYaxis()->SetTickLength(0.0);  //Calo_Em_2D_HHtoGG->SetAxisColor(0);
  Calo_Em_2D_HHtoGG->DrawCopy("Col");  Calo_Em_2D_HHtoGG->DrawCopy("ColSame");
  Calo_Em__HHtoGG->SaveAs("/home/saksevul/T/Parallelism/Calo_Em.png");  //Calo_Em__HHtoGG->Close();


  gStyle->SetPalette(kCherry);
  // gStyle->SetPalette(kAvocado);
  TColor::InvertPalette();

  TCanvas* Calo_Had__HHtoGG = new TCanvas("Calo_Had__HHtoGG","Calo_Had__HHtoGG",0,0,364,198);
  TPad *H__Had__HHtoGG = new TPad("H__Had__HHtoGG","", 0, 0, 1, 1, 0, 0, -1);  H__Had__HHtoGG->SetMargin(0.0,0.0,0.0,0.0);  //H__Had__HHtoGG->SetLogz();
  H__Had__HHtoGG->Draw();  H__Had__HHtoGG->cd();
  Calo_Had_2D_HHtoGG->SetTitle(" ; ; ; ");
  Calo_Had_2D_HHtoGG->GetXaxis()->SetTitleSize(0.0);  Calo_Had_2D_HHtoGG->GetXaxis()->SetTitleOffset(0.0);  Calo_Had_2D_HHtoGG->GetXaxis()->SetLabelSize(0.0);
  Calo_Had_2D_HHtoGG->GetYaxis()->SetTitleSize(0.0);  Calo_Had_2D_HHtoGG->GetYaxis()->SetTitleOffset(0.0);  Calo_Had_2D_HHtoGG->GetYaxis()->SetLabelSize(0.0);
  Calo_Had_2D_HHtoGG->GetZaxis()->SetTitleSize(0.0);  Calo_Had_2D_HHtoGG->GetZaxis()->SetTitleOffset(0.0);  Calo_Had_2D_HHtoGG->GetZaxis()->SetLabelSize(0.0);
  // Calo_Had_2D_HHtoGG->SetAxisRange(-1.479, 1.479, "Y");
  Calo_Had_2D_HHtoGG->SetContour(100);
  Calo_Had_2D_HHtoGG->GetXaxis()->SetTickLength(0.0);  Calo_Had_2D_HHtoGG->GetYaxis()->SetTickLength(0.0);  //Calo_Had_2D_HHtoGG->SetAxisColor(0);
  Calo_Had_2D_HHtoGG->DrawCopy("Col");
  Calo_Had_2D_HHtoGG->DrawCopy("ColSame");
  Calo_Had__HHtoGG->SaveAs("/home/saksevul/T/Parallelism/Calo_Had.png");  H__Had__HHtoGG->Delete();  Calo_Had__HHtoGG->Close();


  TCanvas* Calo__HHtoGG = new TCanvas("Calo__HHtoGG","Calo__HHtoGG",0,0,728,396);
  TPad *H___HHtoGG = new TPad("H___HHtoGG","", 0, 0, 1, 1, 1, 0, -1);  H___HHtoGG->SetMargin(0.0,0.0,0.0,0.0);
  Calo__HHtoGG -> Draw();  H___HHtoGG->Draw();  H___HHtoGG->cd();
  gStyle->SetPalette(kGreyScale);

  Calo_Em_2D_HHtoGG->DrawCopy("Col");
  Calo_Em_2D_HHtoGG->DrawCopy("ColSame");
  Calo__HHtoGG->SaveAs("/home/saksevul/T/Parallelism/Calo_2.png");

  Calo_Had_2D_HHtoGG->DrawCopy("Col");
  Calo_Had_2D_HHtoGG->DrawCopy("ColSame");
  Calo__HHtoGG->SaveAs("/home/saksevul/T/Parallelism/Calo_1.png");

  H___HHtoGG->Close();  Calo__HHtoGG->Close();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void rootMacro(){

  // Le damos algo de personalización al las gráficas..
  gStyle->SetOptStat(0);

  // Define color map
  // gStyle->SetPalette(kDeepSea);
  // gStyle->SetPalette(kGreyScale);
  // gStyle->SetPalette(kDarkBodyRadiator);
  // gStyle->SetPalette(kBlueYellow);
  // gStyle->SetPalette(kRainBow);
  // gStyle->SetPalette(kInvertedDarkBodyRadiator);
  // gStyle->SetPalette(kBird);
  // gStyle->SetPalette(kCubehelix);
  // gStyle->SetPalette(kGreenRedViolet);
  // gStyle->SetPalette(kBlueRedYellow);
  // gStyle->SetPalette(kOcean);
  // gStyle->SetPalette(kColorPrintableOnGrey);
  // gStyle->SetPalette(kAlpine);
  // gStyle->SetPalette(kAquamarine);
  // gStyle->SetPalette(kArmy);
  // gStyle->SetPalette(kAtlantic);
  // gStyle->SetPalette(kAurora);
  // gStyle->SetPalette(kAvocado);
  // gStyle->SetPalette(kBeach);
  // gStyle->SetPalette(kBlackBody);
  // gStyle->SetPalette(kBlueGreenYellow);
  // gStyle->SetPalette(kBrownCyan);
  // gStyle->SetPalette(kCMYK);
  // gStyle->SetPalette(kCandy);
  // gStyle->SetPalette(kCherry);
  // gStyle->SetPalette(kCoffee);
  // gStyle->SetPalette(kDarkRainBow);
  // gStyle->SetPalette(kDarkTerrain);
  // gStyle->SetPalette(kFall);
  // gStyle->SetPalette(kFruitPunch);
  // gStyle->SetPalette(kFuchsia);
  // gStyle->SetPalette(kGreyYellow);
  // gStyle->SetPalette(kGreenBrownTerrain);
  // gStyle->SetPalette(kGreenPink);
  // gStyle->SetPalette(kIsland);
  // gStyle->SetPalette(kLake);
  // gStyle->SetPalette(kLightTemperature);
  // gStyle->SetPalette(kLightTerrain);
  // gStyle->SetPalette(kMint);
  // gStyle->SetPalette(kNeon);
  // gStyle->SetPalette(kPastel);
  // gStyle->SetPalette(kPearl);
  // gStyle->SetPalette(kPigeon);
  // gStyle->SetPalette(kPlum);
  // gStyle->SetPalette(kRedBlue);
  // gStyle->SetPalette(kRose);
  // gStyle->SetPalette(kRust);
  // gStyle->SetPalette(kSandyTerrain);
  // gStyle->SetPalette(kSienna);
  // gStyle->SetPalette(kSolar);
  // gStyle->SetPalette(kSouthWest);
  // gStyle->SetPalette(kStarryNight);
  // gStyle->SetPalette(kSunset);
  // gStyle->SetPalette(kTemperatureMap);
  // gStyle->SetPalette(kThermometer);
  // gStyle->SetPalette(kValentine);
  // gStyle->SetPalette(kVisibleSpectrum);
  // gStyle->SetPalette(kWaterMelon);
  // gStyle->SetPalette(kCool);
  // gStyle->SetPalette(kCopper);
  // gStyle->SetPalette(kGistEarth);
  // gStyle->SetPalette(kViridis);
  // gStyle->SetPalette(kCividis);


  // Corremos los macros que nos interesan.
  EnergyQuotient();

}
