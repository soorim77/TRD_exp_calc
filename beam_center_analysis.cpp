
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
    TString fileName[6];
    fileName[1] = "Beam_size_position/RID157_-1,0_x.txt";
    fileName[2] = "Beam_size_position/RID157_-0.5,0_x.txt";
    fileName[3] = "Beam_size_position/RID157_0,0_x.txt";
    fileName[4] = "Beam_size_position/RID157_0.5,0_x.txt";
    fileName[5] = "Beam_size_position/RID157_1,0_x.txt";
    // TString fileNameX = fileName + "_x.txt";
    // TString fileNameY = fileName + "_y.txt";
    TString LineOrProjection;
    if (flag_projection)
    {
        LineOrProjection = "%*lg %lg %lg %*lg";
    }
    else
    {
        LineOrProjection = "%*lg %lg %*lg %lg";
    }

    TCanvas *canv_lineProfile = new TCanvas("canv_lineProfile", "Line profile", canvasWidth, canvasHeight);
    TGraph *gr_profile[6];
    TF1 *func_gauss[6];
    TString fitName[6] = {"Fit",
                          "Fit",
                          "Fit",
                          "Fit",
                          "Fit",
                          "Fit"};
    int colorNo[6] = {7,
                      1,
                      2,
                      4,
                      6,
                      8};
    for (size_t i = 1; i < 6; i++)
    {
        gr_profile[i] = new TGraph(fileName[i], LineOrProjection);
        gr_profile[i]->SetMarkerStyle(2);
        gr_profile[i]->GetXaxis()->SetNdivisions(505);
        gr_profile[i]->GetYaxis()->SetNdivisions(505);
        gr_profile[i]->SetMarkerColor(colorNo[i]);
        if (i == 1)
            gr_profile[i]->Draw("AP");
        else
            gr_profile[i]->Draw("PSAME");
        fitName[i] = fitName[i] + i;
        func_gauss[i] = new TF1(fitName[i], "gaus", -30, 30);
        func_gauss[i]->SetParLimits(1, -2, 2);
        func_gauss[i]->SetParLimits(2, 0.5, 8);
        func_gauss[i]->SetParLimits(0, 0, 10);
        func_gauss[i]->SetLineColor(colorNo[i]);
        gr_profile[i]->Fit(fitName[i], "RBM");
    }
    gr_profile[1]->GetYaxis()->SetTitle("Projection dose [Gy]");
    gr_profile[1]->GetXaxis()->SetTitle("Beam position [mm]");

    // gr_x_profile->SetMarkerStyle(5);
    // gr_x_profile->GetYaxis()->SetNdivisions(505);
    // gr_y_profile->SetMarkerStyle(26);
    // gr_x_profile->Draw("AP");
    // gr_y_profile->Draw("PSAME");
    // TF1 *func_gaussx = new TF1("xFit", "gaus", -30, 30);
    // TF1 *func_gaussy = new TF1("yFit", "gaus", -30, 30);
    // func_gaussx->SetParLimits(1, -2, 2);
    // func_gaussy->SetParLimits(1, -2, 2);
    // func_gaussx->SetParLimits(2, 0.5, 8);
    // func_gaussy->SetParLimits(2, 0.5, 8);
    // func_gaussx->SetParLimits(0, 0, 10);
    // func_gaussy->SetParLimits(0, 0, 10);
    // func_gaussy->SetLineColor(kBlue);
    // gr_x_profile->Fit("xFit", "RBM");
    // gr_y_profile->Fit("yFit", "RBM");
}