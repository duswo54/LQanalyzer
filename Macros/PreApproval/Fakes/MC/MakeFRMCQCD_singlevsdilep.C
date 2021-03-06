#include "Macro.h"
#include "CMS_lumi.h"

void MakeFRMCQCD_singlevsdilep(){
  
  TString path_qcd= "/home/jalmond/Analysis/LQanalyzer/data/output/ElectronFakes/FakeRateCalculator_El_SKQCD_5_3_14.root";
  

  setTDRStyle();
  gStyle->SetPalette(1);
  
  TFile * fqcd = new TFile(path_qcd);
  
  vector<TString> list_of_regions;
  list_of_regions.push_back("Dilep");
 
  
  vector<TString> list_of_names;
  list_of_names.push_back("closebjet");
  //list_of_names.push_back("awayjet_pt");
  //list_of_names.push_back("noclosejet_nbjet");
  list_of_names.push_back("pt");
  list_of_names.push_back("cb_pt");
  list_of_names.push_back("nocb_pt");
  list_of_names.push_back("eta");
  list_of_names.push_back("cb_eta");
  list_of_names.push_back("nocb_eta");
  list_of_names.push_back("njets");
  list_of_names.push_back("ht");
  list_of_names.push_back("cb_ht");
  list_of_names.push_back("nocb_ht");
  list_of_names.push_back("st");
  list_of_names.push_back("cb_st");
  list_of_names.push_back("nocb_st");
  list_of_names.push_back("finebins_ht");
  list_of_names.push_back("nbjet");


  vector<TString> xaxis_label;
  xaxis_label.push_back("Has Closebjet");
  //  xaxis_label.push_back("awayjet P_{T} GeV");
  //xaxis_label.push_back("noclosejet N(bjets)");
  xaxis_label.push_back("P_{T} GeV");
  xaxis_label.push_back("closejet P_{T} GeV");
  xaxis_label.push_back("no closejet P_{T} GeV");
  xaxis_label.push_back("#eta");
  xaxis_label.push_back("closejet #eta");
  xaxis_label.push_back("noclosejet #eta");

  xaxis_label.push_back("N(jets)");
  xaxis_label.push_back("H_{T} GeV");
  xaxis_label.push_back("closejet H_{T} GeV");
  xaxis_label.push_back("noclosejet H_{T} GeV");
  xaxis_label.push_back("S_{T} GeV");
  xaxis_label.push_back("closejet S_{T} GeV");
  xaxis_label.push_back("noclosejet S_{T} GeV");
  xaxis_label.push_back("H_{T} GeV");
  xaxis_label.push_back("N(bjets)");
  
  
 
  for(unsigned int j = 0; j < list_of_regions.size(); j++){
    for(unsigned int i = 0; i < list_of_names.size(); i++){

      
      TCanvas* c1 = new TCanvas((("Plot")+ list_of_regions.at(j )+list_of_names.at(i)).Data(), "Plot", 1600, 1200);
      
      cout << "Plotting " << (list_of_regions.at(j ) + "_MCLooseEl_"+list_of_names.at(i)) << endl;
      TH1F* h_qcd_d_loose= (TH1F*)fqcd->Get(((list_of_regions.at(j ) + "_MCLooseEl_"+list_of_names.at(i)).Data()));
      TH1F* h_qcd_d_tight= (TH1F*)fqcd->Get(((list_of_regions.at(j ) + "_MCTightEl_"+list_of_names.at(i)).Data()));
      
      TH1F* h_qcd_loose= (TH1F*)fqcd->Get(("Singlelep_MCLooseEl_"+list_of_names.at(i)).Data());
      TH1F* h_qcd_tight= (TH1F*)fqcd->Get(("Singlelep_MCTightEl_"+list_of_names.at(i)).Data());

      bool useqcd_d = (h_qcd_d_tight);
      bool useqcd = (h_qcd_tight);

      if(!h_qcd_tight) continue;
      h_qcd_tight->SetMarkerStyle(20);
      h_qcd_tight->SetMarkerSize(2.);
      h_qcd_tight->SetLineColor(kRed);
      h_qcd_tight->SetMarkerColor(kRed);
      
      h_qcd_tight->SetLineWidth(2.);
      
      if(useqcd_d){
	h_qcd_d_tight->SetMarkerStyle(21);
	h_qcd_d_tight->SetMarkerSize(2.);
	h_qcd_d_tight->SetMarkerColor(kCyan);
	h_qcd_d_tight->SetLineColor(kCyan);
	h_qcd_d_tight->SetLineWidth(2.);
      }

      if(list_of_names.at(i).Contains("eta")){
	h_qcd_tight->Rebin(5.);
	h_qcd_loose->Rebin(5.);
	if(useqcd_d){
	  h_qcd_d_tight->Rebin(5.);
	  h_qcd_d_loose->Rebin(5.);
	}
      }
      
      h_qcd_tight->Divide(h_qcd_loose);
      if(useqcd_d)h_qcd_d_tight->Divide(h_qcd_d_loose);

      h_qcd_tight->GetYaxis()->SetTitle("#epsilon (T/L)" );
      h_qcd_tight->GetXaxis()->SetTitle(xaxis_label.at(i));
      
      h_qcd_tight->GetYaxis()->SetRangeUser(0., .5);
      if(xaxis_label.at(i).Contains("Has")){
	h_qcd_tight->GetXaxis()->SetBinLabel(1,"!bjet: #DeltaR< 0.4");
	h_qcd_tight->GetXaxis()->SetBinLabel(2,"bjet:  #DeltaR< 0.4");
      }
      h_qcd_tight->Draw("p");
      if(useqcd_d)h_qcd_d_tight->Draw("psame");
      
      
      
      TLegend* legendH = new TLegend(0.7, 0.7, 0.9, 0.9);
      legendH->SetFillColor(kWhite);
      legendH->SetTextFont(42);
      legendH->SetTextSize(0.03);
      legendH->AddEntry(h_qcd_tight, "QCD (single e)");
      if(useqcd_d)legendH->AddEntry(h_qcd_d_tight, "QCD (di-el)");
      legendH->Draw();
            
      CMS_lumi( c1, 2, 11 );
      c1->Update();
      c1->RedrawAxis();

      c1->SaveAs(("/home/jalmond/WebPlots/PreApproval/Fakes/MCFake_qcd_" + list_of_regions.at(j ) + "_" + list_of_names.at(i) + ".pdf").Data());
    }
    
  }
}


CMS_lumi( TPad* pad, int iPeriod, int iPosX )
{
  bool outOfFrame    = false;
  if( iPosX/10==0 )
    {
      outOfFrame = true;
    }
  int alignY_=3;
  int alignX_=2;
  if( iPosX/10==0 ) alignX_=1;
  if( iPosX==0    ) alignY_=1;
  if( iPosX/10==1 ) alignX_=1;
  if( iPosX/10==2 ) alignX_=2;
  if( iPosX/10==3 ) alignX_=3;
  int align_ = 10*alignX_ + alignY_;

  float H = pad->GetWh();
  float W = pad->GetWw();
  float l = pad->GetLeftMargin();
  float t = pad->GetTopMargin();
  float r = pad->GetRightMargin();
  float b = pad->GetBottomMargin();
  float e = 0.025;

  pad->cd();

  TString lumiText;
  if( iPeriod==1 )
    {
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==2 )
    {
      lumiText += " (8 TeV)";
    }
  else if( iPeriod==3 )
    {
      lumiText = lumi_8TeV;
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
    }
  else if ( iPeriod==4 )
    {
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
    }
  else if ( iPeriod==7 )
    {
      if( outOfFrame ) lumiText += "#scale[0.85]{";
      lumiText += lumi_13TeV;
      lumiText += " (13 TeV)";
      lumiText += " + ";
      lumiText += lumi_8TeV;
      lumiText += " (8 TeV)";
      lumiText += " + ";
      lumiText += lumi_7TeV;
      lumiText += " (7 TeV)";
      if( outOfFrame) lumiText += "}";
    }
  else if ( iPeriod==12 )
    {
      lumiText += "8 TeV";
    }

  cout << lumiText << endl;

  TLatex latex;
  latex.SetNDC();
  latex.SetTextAngle(0);
  latex.SetTextColor(kBlack);

  float extraTextSize = extraOverCmsTextSize*cmsTextSize;

  latex.SetTextFont(42);
  latex.SetTextAlign(31);
  latex.SetTextSize(lumiTextSize*t);
  latex.DrawLatex(1-r,1-t+lumiTextOffset*t,lumiText);

  if( outOfFrame )
    {
      latex.SetTextFont(cmsTextFont);
      latex.SetTextAlign(11);
      latex.SetTextSize(cmsTextSize*t);
      latex.DrawLatex(l,1-t+lumiTextOffset*t,cmsText);
    }

  pad->cd();

  float posX_;
  if( iPosX%10<=1 )
    {
      posX_ =   l + relPosX*(1-l-r);
    }
  else if( iPosX%10==2 )
    {
      posX_ =  l + 0.5*(1-l-r);
    }
  else if( iPosX%10==3 )
    {
      posX_ =  1-r - relPosX*(1-l-r);
    }
  float posY_ = 1-t - relPosY*(1-t-b);
  if( !outOfFrame )
    {
      if( drawLogo )
        {
          posX_ =   l + 0.045*(1-l-r)*W/H;
          posY_ = 1-t - 0.045*(1-t-b);
          float xl_0 = posX_;
          float yl_0 = posY_ - 0.15;
          float xl_1 = posX_ + 0.15*H/W;
          float yl_1 = posY_;
          TASImage* CMS_logo = new TASImage("CMS-BW-label.png");
          TPad* pad_logo = new TPad("logo","logo", xl_0, yl_0, xl_1, yl_1 );
          pad_logo->Draw();
          pad_logo->cd();
          CMS_logo->Draw("X");
          pad_logo->Modified();
          pad->cd();
        }
      else
        {
          latex.SetTextFont(cmsTextFont);
          latex.SetTextSize(cmsTextSize*t);
          latex.SetTextAlign(align_);
          latex.DrawLatex(posX_, posY_, cmsText);
          if( writeExtraText )
            {
              latex.SetTextFont(extraTextFont);
              latex.SetTextAlign(align_);
              latex.SetTextSize(extraTextSize*t);
              latex.DrawLatex(posX_, posY_- relExtraDY*cmsTextSize*t, extraText);
            }
        }
    }
  else if( writeExtraText )
    {
      if( iPosX==0)
        {
          posX_ =   l +  relPosX*(1-l-r);
          posY_ =   1-t+lumiTextOffset*t;
        }
      latex.SetTextFont(extraTextFont);
      latex.SetTextSize(extraTextSize*t);
      latex.SetTextAlign(align_);
      latex.DrawLatex(posX_, posY_, extraText);
    }
  return;
}





