{
    const int canvasWidth = 1200, canvasHeight = 800;
    TCanvas *canv_lineProfile = new TCanvas("canv_Presetcount", "PresetCount", canvasWidth, canvasHeight);
    TGraph2D *gr_2d = new TGraph2D("HandN_presetcount.txt", "%lg %lg %lg");
    gr_2d->GetXaxis()->SetRangeUser(gr_2d->GetXmin(), gr_2d->GetXmax());
    gr_2d->GetYaxis()->SetRangeUser(gr_2d->GetYmin(), gr_2d->GetYmax());
    gr_2d->GetYaxis()->SetNdivisions(506);
    gr_2d->GetXaxis()->SetNdivisions(506);
    gr_2d->GetZaxis()->SetNdivisions(506);
    gr_2d->Draw("lego1");
}