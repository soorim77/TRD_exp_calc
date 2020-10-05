{
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>

    //Set default ROOT style
    //gROOT->LoadMacro("subStyleSetting.C");
    gStyle->SetLabelSize(0.05, "x");
    gStyle->SetLabelSize(0.05, "y");
    gStyle->SetTitleSize(0.05, "x");
    gStyle->SetTitleSize(0.05, "y");
    gStyle->SetPadTopMargin(0.15);
    gStyle->SetPadBottomMargin(0.13);
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadLeftMargin(0.13);

    //Read default settings
    //gROOT->LoadMacro("subDefaultSettingReader.C");
    TString file_name_default_settings_str = "defaultSettings.txt";
    ifstream file_default_settings(file_name_default_settings_str.Data());
    TString file_name_data_for_time_shift_str, file_name_data_analysis_str;
    TString variable_name_default_str, variable_parameter_default_str;
    string temp;
    int no_spot_in_pattern;
    double variable_value_default;
    for (int i = 0; i < 10; i++)
    {
        variable_parameter_default_str.Clear();
        variable_name_default_str.Clear();
        if (file_default_settings >> variable_name_default_str >> variable_parameter_default_str)
        {
            if (variable_name_default_str == "file_name_data_for_time_shift")
            {
                file_name_data_for_time_shift_str.Form("%s", variable_parameter_default_str.Data());
            }
            if (variable_name_default_str == "file_name_data_analysis")
            {
                file_name_data_analysis_str.Form("%s", variable_parameter_default_str.Data());
            }
            if (variable_name_default_str == "no_spot_in_pattern")
            {
                no_spot_in_pattern = variable_parameter_default_str.Atoi();
                cout << "there is default spot no" << endl;
            }
        }
        else
            continue;
    }

    if (file_name_data_for_time_shift_str == "")
    {
        cout << "Enter the reference file for time shifting calculation >> ";
        cin >> temp;
        file_name_data_for_time_shift_str.Form("%s", temp.c_str());
    }
    TFile file_data_for_time_shift(file_name_data_for_time_shift_str);
    if (file_name_data_analysis_str == "")
    {
        cout << "Enter the reference file for time shifting calculation >> ";
        cin >> temp;
        file_name_data_analysis_str.Form("%s", temp.c_str());
    }
    TFile file_data_analysis(file_name_data_analysis_str);

    const int max_bins_const = 10000000;

    //Time shifting calculation from (TFile file_data_for_time_shift)
    //gROOT->LoadMacro("subTimeShiftingCalculation.C");
    //--max, average, base
    cout << "========  0  ========" << endl;
    cout << " Data for time shift\t: " << file_name_data_for_time_shift_str << endl;
    TTree *tree_time_shift = (TTree *)file_data_for_time_shift.Get("T1");
    char branch_name_Time_c[5] = "Time", branch_name_DSN_c[5] = "DSNM", branch_name_TRD_c[7] = "Markus";
    float DSN_f, TRD_f, time_f,
        time_bin1_f[max_bins_const], TRD_bin1_f[max_bins_const], DSN_bin1_f[max_bins_const],
        sampling_time_f,
        time_base_calc = 2, base_DSN = 0, base_TRD = 0, max_TRD = -20, max_DSN = -20,
        time_avg_calc = 0.3, delay_avg_calc = 0.01, avg_DSN = 0, avg_TRD = 0;
    int no_points_branch, processing_baseline_calc, no_baseline_calc,
        processing_avg_calc, no_avg_calc, bin_no_center_IRR_start;
    tree_time_shift->SetBranchAddress(branch_name_Time_c, &time_f);
    tree_time_shift->SetBranchAddress(branch_name_DSN_c, &DSN_f);
    tree_time_shift->SetBranchAddress(branch_name_TRD_c, &TRD_f);
    no_points_branch = tree_time_shift->GetEntries();
    for (int i = 0; i < 2; i++)
    {
        tree_time_shift->GetEntry(i);
        time_bin1_f[i] = time_f;
    }
    //sampling time calculation
    sampling_time_f = int((time_bin1_f[1] - time_bin1_f[0]) * 1.e6) * 1.e-6;
    no_baseline_calc = int((time_base_calc / sampling_time_f));
    cout << sampling_time_f << " " << no_baseline_calc << endl;
    no_avg_calc = time_avg_calc / sampling_time_f;
    processing_avg_calc = -int(delay_avg_calc / sampling_time_f);

    for (int i = 2; i < no_points_branch; i++)
    {
        tree_time_shift->GetEntry(i);
        time_bin1_f[i] = time_f;
        TRD_bin1_f[i] = -TRD_f;
        DSN_bin1_f[i] = DSN_f;
        //Base caculation
        if (DSN_bin1_f[i] < 0.1)
        {
            if (processing_baseline_calc < no_baseline_calc)
                processing_baseline_calc++;
            else if (processing_baseline_calc < 2 * no_baseline_calc)
            {
                base_DSN += DSN_bin1_f[i];
                base_TRD += TRD_bin1_f[i];
                processing_baseline_calc++;
            }
            else
                ;
        }
        //DSN-avg calculation
        if (DSN_bin1_f[i] > 0.5 && TRD_bin1_f[i] > 0.3)
        {
            if (processing_avg_calc < 0)
            {
                processing_avg_calc++;
            }
            else if (processing_avg_calc < no_avg_calc)
            {
                avg_DSN += DSN_bin1_f[i];
                avg_TRD += TRD_bin1_f[i];
                processing_avg_calc++;
            }
            else
                ;
            //maximum calculation
            if (DSN_bin1_f[i] > max_DSN)
                max_DSN = DSN_bin1_f[i];
            if (TRD_bin1_f[i] > max_TRD)
                max_TRD = TRD_bin1_f[i];
        }
    }
    avg_DSN = avg_DSN / no_avg_calc;
    avg_TRD = avg_TRD / no_avg_calc;
    base_TRD = base_TRD / no_baseline_calc;
    base_DSN = base_DSN / no_baseline_calc;
    cout << "DSN and TRD maximum values are " << max_DSN << " " << max_TRD << endl;
    cout << "Base value of DSN and TRD are  " << base_DSN << " " << base_TRD << endl;
    cout << "Average value of DSN and TRD are  " << avg_DSN << " " << avg_TRD << endl;
    TCanvas *canv_Full = new TCanvas();
    TGraph *gr_DSN = new TGraph(no_points_branch, time_bin1_f, DSN_bin1_f);
    TGraph *gr_TRD = new TGraph(no_points_branch, time_bin1_f, TRD_bin1_f);
    gr_DSN->Draw();
    gr_DSN->GetYaxis()->SetRangeUser(-1, 10);
    gr_TRD->SetLineColor(kRed);
    gr_TRD->Draw("same");
    //--Matching
    //----get matching point
    TCanvas *canv_Short = new TCanvas();
    canv_Short->cd();
    for (int i = 0; i < no_points_branch; i++)
    {
        if (!bin_no_center_IRR_start)
        {
            if (TRD_bin1_f[i] > max_TRD / 2)
                bin_no_center_IRR_start = i;
        }
    }
    float delay_matching_start = 0.02, time_matching_start = delay_matching_start + time_bin1_f[bin_no_center_IRR_start],
          timespan_matching = 0.01, time_matching_end = time_matching_start + timespan_matching;
    int bin_no_matching_start = bin_no_center_IRR_start + delay_matching_start / sampling_time_f,
        bin_no_matching_end = bin_no_center_IRR_start + (delay_matching_start + timespan_matching) / sampling_time_f,
        additional_bin_no_matching = 50;
    const int no_bin_matching = 25000;
    float time_DSNmatching_bin1_f[no_bin_matching], time_TRDmatching_bin1_f[no_bin_matching],
        DSN_matching_bin1_f[no_bin_matching], TRD_matching_bin1_f[no_bin_matching],
        time_shifted_bin1_f[no_bin_matching], TRD_scaled_bin1_f[no_bin_matching];
    int no_bin_counting = 0;
    //----Prepare TGraph
    for (int i = bin_no_matching_start - additional_bin_no_matching; i < bin_no_matching_end + additional_bin_no_matching; i++)
    {
        time_DSNmatching_bin1_f[no_bin_counting] = time_bin1_f[i];
        DSN_matching_bin1_f[no_bin_counting] = DSN_bin1_f[i] - base_DSN;
        no_bin_counting++;
    }
    int no_bin_matching_DSN = no_bin_counting;
    TGraph *gr_short_DSN = new TGraph(no_bin_matching_DSN, time_DSNmatching_bin1_f, DSN_matching_bin1_f);
    float scaling_factor_matching_DoverT = (avg_DSN - base_DSN) / (avg_TRD - base_TRD);
    no_bin_counting = 0;
    for (int i = bin_no_matching_start; i < bin_no_matching_end; i++)
    {
        time_TRDmatching_bin1_f[no_bin_counting] = time_bin1_f[i];
        TRD_matching_bin1_f[no_bin_counting] = TRD_bin1_f[i] - base_TRD;
        TRD_scaled_bin1_f[no_bin_counting] = TRD_matching_bin1_f[no_bin_counting] * scaling_factor_matching_DoverT;
        no_bin_counting++;
    }
    int no_bin_matching_TRD = no_bin_counting;
    TGraph *gr_scaled_TRD_matching = new TGraph(no_bin_matching_TRD, time_TRDmatching_bin1_f, TRD_scaled_bin1_f);
    double diff_start_sum_bin_shift = 9999999999, diff_start_sum_us_shift = 9999999999,
           diff_sum_bin_shift, diff_sum_us_shift = 0;
    int bin_shift, us_shift;
    for (int i = 0; i < 2 * additional_bin_no_matching; i++)
    {
        diff_sum_bin_shift = 0;
        for (int j = 4; j < no_bin_matching_TRD - 4; j++)
        {
            diff_sum_bin_shift += TMath::Sq(TRD_scaled_bin1_f[j] - gr_short_DSN->Eval(time_DSNmatching_bin1_f[i + j]));
        }
        if (diff_start_sum_bin_shift > diff_sum_bin_shift)
        {
            bin_shift = i;
            diff_start_sum_bin_shift = diff_sum_bin_shift;
        }
    }
    for (int i = -100; i < 100; i++)
    {
        diff_sum_us_shift = 0;
        for (int j = 0; j < no_bin_matching_TRD; j++)
        {
            diff_sum_us_shift += TMath::Sq(TRD_scaled_bin1_f[j] - gr_short_DSN->Eval(time_TRDmatching_bin1_f[j] + (i)*sampling_time_f / 10));
        }
        if (diff_start_sum_us_shift > diff_sum_us_shift)
        {
            us_shift = int(i * sampling_time_f / 1e-6 / 10);
            diff_start_sum_us_shift = diff_sum_us_shift;
        }
    }
    bin_shift = bin_shift - additional_bin_no_matching;
    us_shift = us_shift;
    cout << "bin shift : " << bin_shift << " bins" << endl;
    cout << "us shift  : " << us_shift << " us" << endl;
    no_bin_counting = 0;
    for (int i = bin_no_matching_start; i < bin_no_matching_end; i++)
    {
        time_shifted_bin1_f[no_bin_counting] = time_bin1_f[i + bin_shift];
        no_bin_counting++;
    }
    TGraph *gr_scaled_shifted_TRD = new TGraph(no_bin_matching_TRD, time_shifted_bin1_f, TRD_scaled_bin1_f);
    gr_short_DSN->Draw();
    gr_scaled_TRD_matching->SetLineColor(kRed);
    gr_scaled_TRD_matching->Draw("same");
    gr_scaled_shifted_TRD->SetLineColor(kBlue);
    gr_scaled_shifted_TRD->Draw("same");

    //Spot_cut
    cout << "========  1  ========" << endl;
    cout << " Data for time shift\t: " << file_name_data_analysis_str << endl;
    float PSNX_f, PSNY_f, PSNX_bin1_f[max_bins_const], PSNY_bin1_f[max_bins_const],
        timespan_spotchange = 110.e-6, timespan_during_spot = 70.e-6,
        diff_PSN_spotchange = 12e-3, PSNX_spot_average = 0, PSNY_spot_average = 0,
        diff_PSNX_from_spot_average, diff_PSNY_from_spot_average,
        timespan_slicechange = 5.e-3,
        spot_pos_x[max_bins_const], spot_pos_y[max_bins_const];
    double spot_data_bin2_d[4][max_bins_const]; //[0:time], [1;slice no], [2:DSN], [3:TRD]
    char branch_name_PSNX_c[6] = "POS_X", branch_name_PSNY_c[6] = "POS_Y";
    int flag_spot_change_bin1[max_bins_const],
        counter_spotchange, counter_during_spot, counter_slicechange = 0,
                                                 flag_slice_change = 0,
                                                 no_slice = 0, no_spot = 0, counter_delay_spotchange = 0;
    const int binspan_spotchange = int(timespan_spotchange / sampling_time_f),
              binspan_during_spot = int(timespan_during_spot / sampling_time_f),
              binspan_slicechange = int(timespan_slicechange / sampling_time_f),
              binspan_determine_spot = binspan_during_spot + binspan_spotchange,
              threshold_during_spot = int(binspan_during_spot * 0.6),
              threshold_spotchange = int(binspan_spotchange * 0.3),
              delay_spotchange_detect = int(binspan_determine_spot + 2);
    TTree *tree_analysis = (TTree *)file_data_analysis.Get("T1");
    tree_analysis->SetBranchAddress(branch_name_Time_c, &time_f);
    tree_analysis->SetBranchAddress(branch_name_DSN_c, &DSN_f);
    tree_analysis->SetBranchAddress(branch_name_TRD_c, &TRD_f);
    tree_analysis->SetBranchAddress(branch_name_PSNX_c, &PSNX_f);
    tree_analysis->SetBranchAddress(branch_name_PSNY_c, &PSNY_f);
    no_points_branch = tree_analysis->GetEntries();
    for (int i = 0; i < no_points_branch; i++)
    {
        tree_analysis->GetEntry(i);
        time_bin1_f[i] = time_f;
        TRD_bin1_f[i] = -TRD_f;
        DSN_bin1_f[i] = DSN_f;
        PSNX_bin1_f[i] = PSNX_f;
        PSNY_bin1_f[i] = PSNY_f;
        //--dose integration
        counter_during_spot = 0;
        counter_spotchange = 0;
        //--detect spot change
        flag_spot_change_bin1[i] = 0;
        if (i < binspan_determine_spot)
            ; //spot cutting is not possible
        else if (PSNX_bin1_f[i - binspan_spotchange] > 0.31 && PSNY_bin1_f[i - binspan_spotchange] > 0.31)
        {
            //----1st spot after slice change
            if (flag_slice_change == 1)
            {
                counter_during_spot = 0;
                for (int j = 0; j < binspan_during_spot; j++)
                {
                    //cout << PSNY_bin1_f[i-j] <<" " << PSNY_bin1_f[i-j-binspan_spotchange] << " " << diff_PSN_spotchange << endl;
                    if ((PSNY_bin1_f[i - j] - PSNY_bin1_f[i - j - binspan_during_spot] < diff_PSN_spotchange) && (-PSNY_bin1_f[i - j] + PSNY_bin1_f[i - j - binspan_during_spot] < diff_PSN_spotchange) && (PSNX_bin1_f[i - j] - PSNX_bin1_f[i - j - binspan_during_spot] < diff_PSN_spotchange) && (-PSNX_bin1_f[i - j] + PSNX_bin1_f[i - j - binspan_during_spot] < diff_PSN_spotchange))
                    {
                        counter_during_spot++;
                    }
                }
                if (counter_during_spot >= threshold_during_spot)
                {
                    no_spot++;
                    no_slice++;
                    flag_slice_change = 0;
                    counter_slicechange = 0;
                    flag_spot_change_bin1[i - 9] = 1;
                    spot_data_bin2_d[0][no_spot] = time_bin1_f[i - 9];
                    spot_data_bin2_d[1][no_spot] = no_slice;
                }
            }
            //----2nd and next spots after slice change
            else if (counter_delay_spotchange > delay_spotchange_detect && flag_slice_change == 0)
            {
                PSNX_spot_average = 0;
                PSNY_spot_average = 0;
                for (int j = 0; j < binspan_during_spot; j++)
                {
                    if ((PSNX_bin1_f[i - binspan_determine_spot + j] - PSNX_bin1_f[i - binspan_spotchange] < diff_PSN_spotchange) && (-PSNX_bin1_f[i - binspan_determine_spot + j] + PSNX_bin1_f[i - binspan_spotchange] < diff_PSN_spotchange) && (PSNY_bin1_f[i - binspan_determine_spot + j] - PSNY_bin1_f[i - binspan_spotchange] < diff_PSN_spotchange) && (-PSNY_bin1_f[i - binspan_determine_spot + j] + PSNY_bin1_f[i - binspan_spotchange] < diff_PSN_spotchange))
                    {
                        counter_during_spot++;
                    }
                    PSNX_spot_average += PSNX_bin1_f[i - binspan_determine_spot + j];
                    PSNY_spot_average += PSNY_bin1_f[i - binspan_determine_spot + j];
                }
                PSNX_spot_average = PSNX_spot_average / binspan_during_spot;
                PSNY_spot_average = PSNY_spot_average / binspan_during_spot;
                if (counter_during_spot < threshold_during_spot)
                    continue; //if there is no plateau, break
                for (int j = 0; j < binspan_spotchange; j++)
                {
                    if ((PSNX_bin1_f[i - binspan_spotchange + j] - PSNX_spot_average > diff_PSN_spotchange) || (-PSNX_bin1_f[i - binspan_spotchange + j] + PSNX_spot_average > diff_PSN_spotchange) || (PSNY_bin1_f[i - binspan_determine_spot + j] - PSNY_spot_average > diff_PSN_spotchange) || (-PSNY_bin1_f[i - binspan_determine_spot + j] + PSNY_spot_average > diff_PSN_spotchange))
                    {
                        counter_spotchange++;
                    }
                }
                if (counter_spotchange < threshold_spotchange)
                    continue;
                no_spot++;
                flag_spot_change_bin1[i - binspan_spotchange + 1] = 1;
                spot_data_bin2_d[0][no_spot] = time_bin1_f[i - binspan_spotchange + 1];
                spot_data_bin2_d[1][no_spot] = no_slice;
                counter_delay_spotchange = 0;
                spot_pos_x[no_spot] = PSNX_spot_average;
                spot_pos_y[no_spot] = PSNY_spot_average;
            }
            counter_delay_spotchange++;
            counter_slicechange = 0;
            if (!no_spot)
                continue;
            spot_data_bin2_d[2][no_spot + 1] += (DSN_bin1_f[i - binspan_spotchange] - base_DSN);
            spot_data_bin2_d[3][no_spot + 1] += (TRD_bin1_f[i - binspan_spotchange] - base_TRD);
        }
        else if (!flag_slice_change)
        { //no data PSNX/Y
            counter_during_spot = 0;
            counter_spotchange = 0;
            counter_delay_spotchange = 0;
            counter_slicechange++;
            if (counter_slicechange > binspan_slicechange)
            {
                flag_slice_change = 1;
                counter_slicechange = 0;
            }
        }
    }
    cout << "Numbers of Slices : " << no_slice << endl
         << "Default number of spots and identified spots : " << no_spot_in_pattern << ", " << no_spot << endl;
    TString spot_result_file_name = "result" + file_name_data_analysis_str + ".txt";
    ofstream ofs_result_spot("result" + file_name_data_analysis_str + ".txt");
    ofs_result_spot << "No time PSNX PSNY SliceNo spotDSN spotTRD" << endl;
    for (int i = 1; i < no_spot + 1; i++)
    {
        //[0:time] [spot pos x] [spot pos y] [1;slice no] [2:DSN] [3:TRD]
        ofs_result_spot << i - (no_spot - no_spot_in_pattern) << " " << setprecision(9) << spot_data_bin2_d[0][i] << " "
                        << spot_pos_x[i] << " " << spot_pos_y[i] << " "
                        << spot_data_bin2_d[1][i] << " " << spot_data_bin2_d[2][i] << " " << spot_data_bin2_d[3][i] << endl;
    }
    TCanvas *canv_spotIntegration = new TCanvas();
    canv_spotIntegration->cd();
    TGraph *gr_spotIntegration = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %*lg %lg");
    gr_spotIntegration->SetMarkerStyle(7);
    gr_spotIntegration->SetMarkerSize(5);
    gr_spotIntegration->Draw("AP");

    TCanvas *canv_spotContour = new TCanvas();
    canv_spotContour->cd();
    // TH2D h2_spotdose_from_each_psn = new TH2D("h2_spotdose", "Spot dose from each position", )
}