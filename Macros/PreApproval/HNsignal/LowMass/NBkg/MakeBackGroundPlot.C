#include "Macro.h"
#include "CMS_lumi.h"
#include "TGraphAsymmErrors.h"


TH1F* makeHist(TString m, TString hist, int col, int style);
TH1F* makeRefHist(TString m);
TH1F* makehist(TString hist);
TH1F* makenpHist(TString hist,  int col, int style);
TH1F* makecfHist(TString hist,  int col, int style);
TH1F* makemcHist(TString hist,  int col, int style);
void MakeCutEffHist(TCanvas* c, TString hist, TH1F* thist, TH1F* thist_presel);


void MakeBackGroundPlot(){
  
  setTDRStyle();
  gStyle->SetPalette(1);

  TH1F* tight_anal1 = makehist("PreSelection_lowmass");
  TH1F* tight_anal2 = makehist("PreSelection_medium_lowmass");
  TH1F* tight_anal3 = makehist("PreSelection_tight_lowmass");
  TH1F* tight_anal4 = makehist("PreSelection_iso_b10_e10_lowmass");


  TLegend* legendH = new TLegend(0.6, 0.7, 0.9, 0.9);
  legendH->SetFillColor(kWhite);
  legendH->SetTextSize(0.03);

  
  tight_anal4->GetXaxis()->SetTitle("m_{N} GeV");
  tight_anal4->GetYaxis()->SetTitle("NBkg");
 
  tight_anal1->SetMarkerColor(kBlue);
  tight_anal1->SetMarkerStyle(20.);
  tight_anal2->SetMarkerColor(kRed);
  tight_anal2->SetMarkerStyle(21.);
  tight_anal3->SetMarkerColor(kRed);
  tight_anal3->SetMarkerStyle(22.);

  tight_anal4->SetMarkerColor(kRed);
  tight_anal4->SetMarkerStyle(20.);



  legendH->AddEntry(tight_anal1, "Analysis", "p");
  legendH->AddEntry(tight_anal2, "Medium POG", "p");
  legendH->AddEntry(tight_anal3, "Tight POG", "p");
  legendH->AddEntry(tight_anal4, "Tight POG + IP", "p");

  tight_anal4->GetYaxis()->SetRangeUser(0., 80.);
  tight_anal4->Draw("p");
  tight_anal2->Draw("psame");
  tight_anal3->Draw("psame");
  tight_anal1->Draw("psame");

  legendH->Draw();

  TGraphAsymmErrors * g = new TGraphAsymmErrors(heff);
  g->SetLineWidth(2.0);
  g->SetMarkerSize(2.);
  //  g->Draw( "9pXsame" );
  
  
  CMS_lumi( c1, 2, 11 );
  c1->Update();
  c1->RedrawAxis();
  
  
  c1->SaveAs(("/home/jalmond/WebPlots/PreApproval/SignalPlots/NBkg_presel_lowmass.pdf" ));
  return;

  MakeCutEffHist(c1,"Zveto", tight_analb, tight_anala);
  MakeCutEffHist(c1,"2015", tight_analc, tight_analb);
  MakeCutEffHist(c1,"trig", tight_anal1, tight_analc);
  MakeCutEffHist(c1,"met", tight_anal2, tight_anal1);
  MakeCutEffHist(c1,"bjetveto", tight_anal3, tight_anal2);
  MakeCutEffHist(c1,"jj", tight_anal4, tight_anal3);
  MakeCutEffHist(c1,"3lep", tight_anal5, tight_anal4);
  MakeCutEffHist(c1,"eecut", tight_anal6, tight_anal5);
  MakeCutEffHist(c1,"meejj", tight_anal7, tight_anal6);

  
  
}

void MakeCutEffHist(TCanvas* c, TString hist, TH1F* thist, TH1F* thist_presel){

  TH1F* h_zveto_eff = (TH1F*)thist->Clone(hist);
  h_zveto_eff->Divide(thist_presel);
  h_zveto_eff->GetYaxis()->SetTitle("Cut efficiency");
  h_zveto_eff->Draw("p");
  
  c->SaveAs(("/home/jalmond/WebPlots/PreApproval/SignalPlots/Punzi_" + hist + ".pdf" ));
  

}


TH1F* makehist(TString hist){


    TH1F* np_40 = makenpHist(hist, 414,0);
    TH1F* np_50 = makenpHist(hist,402,0);
    TH1F* np_60 = makenpHist(hist,616,0);
    TH1F* np_70 = makenpHist(hist,432,0);
    TH1F* np_80 = makenpHist(hist,600,0);
    TH1F* np_90 = makenpHist(hist,600,0);


    TH1F* cf_40 = makecfHist(hist, 414,0);
    TH1F* cf_50 = makecfHist(hist,402,0);
    TH1F* cf_60 = makecfHist(hist,616,0);
    TH1F* cf_70 = makecfHist(hist,432,0);
    TH1F* cf_80 = makecfHist(hist,600,0);
    TH1F* cf_90 = makecfHist(hist,600,0);

    TH1F* mc_40 = makemcHist(hist, 414,0);
    TH1F* mc_50 = makemcHist(hist,402,0);
    TH1F* mc_60 = makemcHist(hist,616,0);
    TH1F* mc_70 = makemcHist(hist,432,0);
    TH1F* mc_80 = makemcHist(hist,600,0);
    TH1F* mc_90 = makemcHist(hist,600,0);

    
  TH1F* heff = new TH1F("heff","heff", 10, 0., 10.);
  
  cout << np_40->GetBinContent(2)  << " " << cf_40->GetBinContent(2) << " " << mc_40->GetBinContent(2) << endl;
  heff->SetBinContent(1, np_40->GetBinContent(2) + cf_40->GetBinContent(2) + mc_40->GetBinContent(2));
  heff->SetBinContent(2, np_50->GetBinContent(2) + cf_50->GetBinContent(2) + mc_50->GetBinContent(2));
  heff->SetBinContent(3, np_60->GetBinContent(2) + cf_60->GetBinContent(2) + mc_60->GetBinContent(2));
  heff->SetBinContent(4, np_70->GetBinContent(2) + cf_70->GetBinContent(2) + mc_70->GetBinContent(2));
  heff->SetBinContent(5, np_80->GetBinContent(2) + cf_80->GetBinContent(2) + mc_80->GetBinContent(2));
  heff->SetBinContent(6, np_90->GetBinContent(2) + cf_90->GetBinContent(2) + mc_90->GetBinContent(2));

  /*heff->SetBinContent(7, (hn_100->GetBinContent(2)/href_100->GetBinContent(2)));
  heff->SetBinContent(8, (hn_200->GetBinContent(2)/href_200->GetBinContent(2)));
  heff->SetBinContent(9, (hn_300->GetBinContent(2)/href_300->GetBinContent(2)));
  heff->SetBinContent(10, (hn_500->GetBinContent(2)/href_500->GetBinContent(2)));*/

  heff->GetXaxis()->SetBinLabel(1,"40");
  heff->GetXaxis()->SetBinLabel(2,"50");
  heff->GetXaxis()->SetBinLabel(3,"60");
  heff->GetXaxis()->SetBinLabel(4,"70");
  heff->GetXaxis()->SetBinLabel(5,"80");
  heff->GetXaxis()->SetBinLabel(6,"90");
  /*heff->GetXaxis()->SetBinLabel(7,"100");
  heff->GetXaxis()->SetBinLabel(8,"200");
  heff->GetXaxis()->SetBinLabel(9,"300");*/

  
  return heff;
}


TH1F* makecfHist(TString hist,  int col, int style){
  TString  path = "/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKchargeflip_dilep_5_3_14.root";
  
  cout << hist << endl;
  TFile* file = new TFile(path);
  TH1F* h = (TH1F*)file->Get(hist);
  
  cout << h << endl;
  return h;

}

TH1F* makemcHist(TString hist,  int col, int style){
  TString  path = "/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_mc_5_3_14.root";

  cout << hist << endl;
  TFile* file = new TFile(path);
  TH1F* h = (TH1F*)file->Get(hist);

  cout << h << endl;
  return h;

}


TH1F* makenpHist(TString hist,  int col, int style){
  TString  path = "/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKnonprompt_dilep_5_3_14.root";

  cout << hist << endl;
  TFile* file = new TFile(path);
  TH1F* h = (TH1F*)file->Get(hist);

  cout << h << endl;
  return h;

}

TH1F* makeHist(TString m, TString hist,  int col, int style){
  TString  path = "/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKHNee" + m + "_nocut_5_3_14.root";

  cout << hist << endl;
  TFile* file = new TFile(path);
  TH1F* h = (TH1F*)file->Get(hist);

  cout << h << endl;
  return h;
}


TH1F* makeRefHist(TString m){
  TString  path = "/home/jalmond/Analysis/LQanalyzer/data/output/SSElectron/HNDiElectron_SKHNee" + m + "_nocut_5_3_14.root";
  
  TFile* file = new TFile(path);
  TH1F* h = (TH1F*)file->Get("Efficiency/eff_electronRef");
  return h;
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







