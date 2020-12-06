
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
    TString spot_result_file_name = "result_RID157_0,0.csv.root.txt";
    TCanvas *canv_spotIntegration = new TCanvas("canv_spot_dose", "Spot Dose", canvasWidth, canvasHeight);
    double xmin_spot_TRD = 200, xmax_spot_TRD = 425, ymin_spot_TRD = -0.5, ymax_spot_TRD = 10, scaling_spot_CumulSpot = 0.075;

    TMultiGraph *mg = new TMultiGraph();
    TGraph *gr_0 = new TGraph("result_RID157_0,0_2.csv.root.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %lg %*lg");
    // TGraph *gr_spot_DSN = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %lg %*lg");
    TGraph *gr_m1 = new TGraph("result_RID157_0,-1.csv.root.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
    TGraph *gr_m05 = new TGraph("result_RID157_0,-0.5.csv.root.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
    TGraph *gr_1 = new TGraph("result_RID157_0,1.csv.root.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
    TGraph *gr_05 = new TGraph("result_RID157_0,0.5.csv.root.txt", "%lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg %lg");
    TGraph *gr_time = new TGraph(spot_result_file_name, "%lg %lg %*lg %*lg %*lg %*lg %*lg %*lg %*lg");
    TGraph *gr_slice = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %lg %*lg %*lg %*lg %*lg");
    gr_0->GetYaxis()->SetNdivisions(502);
    gr_0->GetXaxis()->SetNdivisions(502);
    gr_0->SetLineColor(kBlack);
    gr_m1->SetLineColor(kRed);
    gr_m05->SetLineColor(kOrange + 1);
    gr_05->SetLineColor(kBlue);
    gr_1->SetLineColor(kBlue);
    gr_0->SetLineWidth(2);
    gr_m1->SetLineWidth(2);
    gr_1->SetLineWidth(2);
    gr_0->SetLineStyle(1);
    gr_m1->SetLineStyle(2);
    gr_1->SetLineStyle(3);
    gr_0->SetTitle("0");
    gr_m1->SetTitle("-1");
    gr_1->SetTitle("1");
    gr_05->SetTitle("0.5");
    gr_m05->SetTitle("-0.5");
    gr_0->SetFillStyle(0);
    gr_m1->SetFillStyle(0);
    gr_1->SetFillStyle(0);
    gr_m05->SetFillStyle(0);
    gr_05->SetFillStyle(0);

    mg->Add(gr_0);
    mg->Add(gr_m1);
    // mg->Add(gr_m05);
    mg->Add(gr_1);
    // mg->Add(gr_05);
    mg->GetXaxis()->SetRangeUser(xmin_spot_TRD, xmax_spot_TRD);
    mg->Draw("AL");
    TLegend *leg1 = new TLegend(0.73, 0.75, 0.91, 0.91);
    leg1->AddEntry(gr_0);
    leg1->AddEntry(gr_m1);
    leg1->AddEntry(gr_1);
    leg1->Draw();

    // TCanvas *canv_position = new TCanvas("canv_position", "Position", canvasWidth, canvasHeight);
    // TGraph2D *gr2d = new TGraph2D();
    // for (size_t i = 0; i < 625; i++)
    // {
    //     gr2d->SetPoint(i, gr_m1->GetX()[i], -1, gr_m1->GetY()[i]);
    //     gr2d->SetPoint(625 + i, gr_1->GetX()[i], 1, gr_1->GetY()[i]);
    //     gr2d->SetPoint(625 * 2 + i, gr_05->GetX()[i], 0.5, gr_05->GetY()[i]);
    //     gr2d->SetPoint(625 * 3 + i, gr_m05->GetX()[i], -0.5, gr_m05->GetY()[i]);
    //     gr2d->SetPoint(625 * 4 + i, gr_0->GetX()[i], 0, gr_0->GetY()[i]);
    // }
    // gr2d->GetXaxis()->SetRangeUser(200, 415);
    // gr2d->Draw("line");
}