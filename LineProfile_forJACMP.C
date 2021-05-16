
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
    TString fileName = "LineProfile_calc_meaured_comp_10mm.csv";

    TCanvas *canv_lineProfile = new TCanvas("canv_lineProfile", "Line profile", canvasWidth, canvasHeight);
    TGraph *gr_measured = new TGraph(fileName, "%lg, %lg, %*lg, %*lg");
    TGraph *gr_simulated = new TGraph(fileName, "%lg, %*lg, %lg, %*lg");
    TGraph *gr_simulated_shift = new TGraph(fileName, "%lg, %*lg, %*lg, %lg");

    // gr_simulated->SetMarkerStyle(5);
    // gr_simulated->GetYaxis()->SetNdivisions(505);
    gr_simulated_shift->SetMarkerStyle(21);
    gr_simulated->SetMarkerStyle(22);
    gr_simulated_shift->SetMarkerColor(kRed);
    gr_simulated->SetMarkerColor(kBlue);
    gr_simulated_shift->SetMarkerSize(2);
    gr_simulated->SetMarkerSize(2);
    gr_measured->SetFillColor(kGray + 2);
    gr_measured->SetLineWidth(0);
    gr_simulated->SetLineWidth(0);
    gr_simulated_shift->SetLineWidth(0);
    gr_measured->GetYaxis()->SetTitle("Spot dose [Gy]");
    gr_measured->GetXaxis()->SetTitle("Beam position [mm]");
    gr_measured->Draw("AB");
    gr_simulated->Draw("PSAME");
    gr_simulated_shift->Draw("PSAME");

    TLegend *leg1 = new TLegend(0.63, 0.75, 0.91, 0.91);
    leg1->AddEntry(gr_measured, "Measured spot dose");
    leg1->AddEntry(gr_simulated, "Simulated spot dose (center = 0)");
    leg1->AddEntry(gr_simulated_shift, "Simulated spot dose (center = 0.4)");
    leg1->Draw();
}