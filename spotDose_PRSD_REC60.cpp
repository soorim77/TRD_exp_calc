
{
    gStyle->SetLabelSize(0.05, "x");
    gStyle->SetLabelSize(0.05, "y");
    gStyle->SetTitleSize(0.05, "x");
    gStyle->SetTitleSize(0.05, "y");
    gStyle->SetLineWidth(1);
    gStyle->SetPadTopMargin(0.15);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadLeftMargin(0.13);
    const int canvasWidth = 1200, canvasHeight = 800;
    TString spot_result_file_name = "resultREC60_E9_90mm_20191210.csv.root.txt";
    TCanvas *canv_spotIntegration = new TCanvas("canv_spot_dose", "Spot Dose", canvasWidth, canvasHeight);
    double xmin_spot_TRD = 200, xmax_spot_TRD = 425, ymin_spot_TRD = -0.5, ymax_spot_TRD = 10, scaling_spot_CumulSpot = 0.075;

    TMultiGraph *mg = new TMultiGraph();
    TGraph *gr_spot_dose = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %*lg %*lg %lg %*lg");
    // TGraph *gr_spot_DSN = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %lg %*lg");
    TGraph *gr_PRSD = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
    TGraph *gr_time = new TGraph(spot_result_file_name, "%lg %lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg");
    TGraph *gr_slice = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %lg %*lg %*lg %*lg %*lg");
    gr_spot_dose->GetYaxis()->SetNdivisions(-502);
    gr_spot_dose->GetXaxis()->SetNdivisions(-502);
    gr_spot_dose->SetFillColor(kRed);
    gr_PRSD->SetFillColor(kBlue);
    gr_spot_dose->SetLineColor(kRed);
    gr_PRSD->SetLineColor(kBlue);
    gr_spot_dose->SetLineWidth(1);
    gr_PRSD->SetLineWidth(1);
    for (size_t i = 0; i < gr_PRSD->GetN() - 1; i++)
    {
        if (gr_time->GetY()[i + 1] - gr_time->GetY()[i] > 0.1)
        {
            gr_spot_dose->SetPointY(i, 0);
            gr_PRSD->SetPointY(i, 0);
        }
        if (gr_slice->GetY()[i] == 1 || gr_slice->GetY()[i] == 3 || gr_slice->GetY()[i] == 4 || gr_slice->GetY()[i] == 6)
        {
            gr_PRSD->GetY()[i] = gr_PRSD->GetY()[i] / 3.3;
        }
    }
    gr_spot_dose->SetTitle("Spot Dose");
    gr_PRSD->SetTitle("PR-spot dose");
    // gr_spot_dose->SetFillStyle(0);
    gr_PRSD->SetFillStyle(0);
    mg->Add(gr_spot_dose);
    mg->Add(gr_PRSD);
    mg->GetXaxis()->SetRangeUser(xmin_spot_TRD, xmax_spot_TRD);
    mg->Draw("AB");
    TLegend *leg1 = new TLegend(0.63, 0.75, 0.91, 0.91);
    leg1->AddEntry(gr_spot_dose);
    leg1->AddEntry(gr_PRSD);
    leg1->Draw();
}