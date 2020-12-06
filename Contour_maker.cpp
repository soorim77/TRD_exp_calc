
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

    TCanvas *canv_spotContour = new TCanvas("canv_spot_contour", "Dose delivered from each spot point", canvasHeight + 50, canvasHeight);
    canv_spotContour->cd();
    canv_spotContour->SetLeftMargin(0.13);
    canv_spotContour->SetRightMargin(0.16);
    canv_spotContour->SetTopMargin(0.13);
    TGraph2D *gr2d_spotdose_from_each_psn = new TGraph2D("result_RID157_0,0_2.csv.root.txt", "%*lg %*lg %lg %lg %*lg %*lg %*lg %lg");
    gStyle->SetPalette(55);
    for (size_t i = 0; i < gr2d_spotdose_from_each_psn->GetN(); i++)
    {
        gr2d_spotdose_from_each_psn->GetX()[i] = gr2d_spotdose_from_each_psn->GetX()[i] * 72.51406659 - 162.1109085;
        gr2d_spotdose_from_each_psn->GetY()[i] = gr2d_spotdose_from_each_psn->GetY()[i] * 73.56867389 - 162.6660815;
        if (gr2d_spotdose_from_each_psn->GetZ()[i] > 0.00015)
        {
            gr2d_spotdose_from_each_psn->GetZ()[i] = 100 * sqrt((0.00002 / gr2d_spotdose_from_each_psn->GetZ()[i]) * (0.00002 / gr2d_spotdose_from_each_psn->GetZ()[i]) + 0.021 * 0.021);
        }
        else
        {
            gr2d_spotdose_from_each_psn->GetZ()[i] = 12;
        }

        // gr_PRSD->GetX()[i] = gr_PRSD->GetX()[i] * 72.51406659 - 162.1109085;
    }
    // gr2d_spotdose_from_each_psn->GetXaxis()->SetRangeUser(1.7, 2.5);
    // gr2d_spotdose_from_each_psn->GetYaxis()->SetRangeUser(1.7, 2.5);
    Int_t palette[10];
    palette[0] = kRed;
    palette[1] = kOrange + 7;
    palette[2] = kOrange;
    palette[3] = kYellow + 1;
    palette[4] = kSpring + 6;
    palette[5] = kGreen - 4;
    palette[6] = kGreen + 1;
    palette[7] = kGreen + 2;
    palette[8] = kCyan + 2;
    palette[9] = kBlue + 3;
    // palette[10] = kBlue + 3;
    // palette[11] = kBlue + 3;
    // palette[12] = kBlue + 3;
    // palette[13] = kBlue + 3;
    // palette[14] = kBlue + 3;
    // palette[15] = kBlue + 3;
    // palette[16] = kBlue + 3;
    // palette[17] = kBlue + 3;
    // palette[18] = 4;
    // palette[19] = 4;
    gStyle->SetPalette(10, palette);
    gr2d_spotdose_from_each_psn->GetYaxis()->SetNdivisions(505);
    gr2d_spotdose_from_each_psn->GetXaxis()->SetNdivisions(505);
    gr2d_spotdose_from_each_psn->Draw("colz"); //text45
    // canv_spotContour->SetLogz();
    // gr2d_spotdose_from_each_psn->GetContourList(0.002);
    // gr2d_spotdose_from_each_psn->GetContourList(0.001);
    // gr2d_spotdose_from_each_psn->Draw("cont4 same");
    // gPad->Update();
    // gr2d_spotdose_from_each_psn->SetLineColor(kRed);
}