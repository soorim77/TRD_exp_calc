
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
{
    bool flag_projection = 0;

    gStyle->SetLabelSize(0.05, "x");
    gStyle->SetLabelSize(0.05, "y");
    gStyle->SetTitleSize(0.05, "x");
    gStyle->SetTitleSize(0.05, "y");
    gStyle->SetLineWidth(1);
    gStyle->SetPadTopMargin(0.15);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadLeftMargin(0.13);
    gStyle->SetOptFit(1001);
    const int canvasWidth = 1200, canvasHeight = 800;
    ROOT::Math::MinimizerOptions::SetDefaultMinimizer("Minuit2");
    TString fileName = "Beam_size_position/RID157_1,0";
    TString fileNameX = fileName + "_x.txt";
    TString fileNameY = fileName + "_y.txt";
    TString LineOrProjection;
    if (0)
    {
        LineOrProjection = "%*lg %lg %lg %*lg";
    }
    else
    {
        LineOrProjection = "%*lg %lg %*lg %lg";
    }

    TCanvas *canv_lineProfile = new TCanvas("canv_lineProfile", "Line profile", canvasWidth, canvasHeight);
    TGraph *gr_x_profile = new TGraph(fileNameX, LineOrProjection);
    TGraph *gr_y_profile = new TGraph(fileNameY, LineOrProjection);

    gr_x_profile->SetMarkerStyle(5);
    gr_x_profile->GetYaxis()->SetNdivisions(505);
    gr_y_profile->SetMarkerStyle(26);
    gr_x_profile->Draw("AP");
    gr_y_profile->Draw("PSAME");
    TF1 *func_gaussx = new TF1("xFit", "gaus", -30, 30);
    TF1 *func_gaussy = new TF1("yFit", "gaus", -30, 30);
    func_gaussx->SetParLimits(1, -2, 2);
    func_gaussy->SetParLimits(1, -2, 2);
    func_gaussx->SetParLimits(2, 0.5, 8);
    func_gaussy->SetParLimits(2, 0.5, 8);
    func_gaussx->SetParLimits(0, 0, 10);
    func_gaussy->SetParLimits(0, 0, 10);
    func_gaussy->SetLineColor(kBlue);
    gr_x_profile->Fit("xFit", "RBM");
    gr_y_profile->Fit("yFit", "RBM");
}