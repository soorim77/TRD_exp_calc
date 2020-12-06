{
    const int canvasWidth = 1200, canvasHeight = 800;
    TCanvas *canv_dose_frequency = new TCanvas("canv_dose_frequency", "Dose-Frequency", canvasWidth, canvasHeight);
    TGraph *gr_SD_freq = new TGraph("resultREC60_E9x3_20191210.csv.root.txt", "%*lg %lg %*lg %*lg %*lg %*lg %*lg %lg %*lg");
    // gr_SD_freq->GetXaxis()->SetRangeUser(gr_SD_freq->GetXmin(), gr_SD_freq->GetXmax());
    // gr_SD_freq->GetYaxis()->SetRangeUser(gr_SD_freq->GetYmin(), gr_SD_freq->GetYmax());
    for (size_t i = 0; i < gr_SD_freq->GetN(); i++)
    {
        if (!i)
        {
            ;
        }
        else if (gr_SD_freq->GetX()[i] - gr_SD_freq->GetX()[i - 1] > 0.02)
        {
            gr_SD_freq->GetY()[i - 1] = 0;
            // cout << "aaa" << endl;
        }
        else
            ;
    }
    // gr_SD_freq->GetYaxis()->SetNdivisions(506);
    // gr_SD_freq->GetXaxis()->SetNdivisions(506);
    // gr_SD_freq->GetZaxis()->SetNdivisions(506);
    // gr_SD_freq->Draw();
    TH1D *hist_SD_freq = new TH1D("hist_SD_freq", "SD_Frequency", 700, -0.002, 0.033);
    for (size_t i = 0; i < gr_SD_freq->GetN(); i++)
    {

        hist_SD_freq->Fill(gr_SD_freq->GetY()[i]);
    }
    canv_dose_frequency->SetLogy();
    hist_SD_freq->Draw("hist c");

    // TCanvas *canv_dose_frequency2 = new TCanvas("canv_dose_frequency2", "Dose-Frequency2", canvasWidth, canvasHeight);
    // TH1D *hist_SD_freqSD = new TH1D("hist_SD_freqSD", "SD_FrequencySD", 350, -0.002, 0.033);
    // for (size_t i = 0; i < gr_SD_freq->GetN(); i++)
    // {
    //     hist_SD_freqSD->Fill(gr_SD_freq->GetY()[i], gr_SD_freq->GetY()[i]);
    // }
    // canv_dose_frequency2->SetLogy();
    // hist_SD_freqSD->Draw("hist c");
}