
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
{
    //Set default ROOT style
    //gROOT->LoadMacro("subStyleSetting.C");
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

    //Read default settings
    //gROOT->LoadMacro("subDefaultSettingReader.C");
    TString file_name_default_settings_str = "defaultSettings.txt";
    ifstream file_default_settings(file_name_default_settings_str.Data());
    TString file_name_data_for_time_shift_str, file_name_data_analysis_str, file_name_data_simulation_str;
    TString variable_name_default_str, variable_parameter_default_str;
    string temp;
    bool bool_display_times_shift, bool_display_raw_data, bool_display_spot_dose, bool_display_cumulated_dose, bool_display_simulated_TRD;
    int no_spot_in_pattern;
    double variable_value_default, calibaration_factor_V_to_Gy = 0;
    float time_shift_for_plot_f = 0;
    for (int i = 0; i < 20; i++)
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
            if (variable_name_default_str == "file_name_data_simulation")
            {
                file_name_data_simulation_str.Form("%s", variable_parameter_default_str.Data());
            }
            if (variable_name_default_str == "no_spot_in_pattern")
            {
                no_spot_in_pattern = variable_parameter_default_str.Atoi();
            }
            if (variable_name_default_str == "display_time_shift")
            {
                if (variable_parameter_default_str.Atoi() == 0)
                    bool_display_times_shift = false;
                else
                    bool_display_times_shift = true;
            }
            if (variable_name_default_str == "display_spot_dose")
            {
                if (variable_parameter_default_str.Atoi() == 0)
                    bool_display_spot_dose = false;
                else
                    bool_display_spot_dose = true;
            }
            if (variable_name_default_str == "display_raw_data")
            {
                if (variable_parameter_default_str.Atoi() == 0)
                    bool_display_raw_data = false;
                else
                    bool_display_raw_data = true;
            }
            if (variable_name_default_str == "display_cumulated_dose")
            {
                if (variable_parameter_default_str.Atoi() == 0)
                    bool_display_cumulated_dose = false;
                else
                    bool_display_cumulated_dose = true;
            }
            if (variable_name_default_str == "display_simulated_TRD")
            {
                if (variable_parameter_default_str.Atoi() == 0)
                    bool_display_simulated_TRD = false;
                else
                    bool_display_simulated_TRD = true;
            }
            if (variable_name_default_str == "time_shift_for_plot")
            {
                time_shift_for_plot_f = variable_parameter_default_str.Atoi();
            }
            if (variable_name_default_str == "calibaration_factor_V_to_Gy")
            {
                calibaration_factor_V_to_Gy = variable_parameter_default_str.Atof();
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
        time_base_calc = 0.2, base_DSN = 0, base_TRD = 0, max_TRD = -20, max_DSN = -20,
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
            if (processing_baseline_calc < no_baseline_calc * 0.1)
                processing_baseline_calc++;
            else if (processing_baseline_calc < 1.1 * no_baseline_calc)
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
    if (bool_display_times_shift)
    {
        TCanvas *canvas_time_shift_perX = new TCanvas("canv_time_shift_perX", "Time Shift2", canvasWidth, canvasHeight);
        float time_bin1_f_perX[int(max_bins_const / 20)], DSN_bin1_f_perX[int(max_bins_const / 20)], TRD_bin1_f_perX[int(max_bins_const / 20)];
        int bins_Xaverage = 100, perX_counter = 0;
        for (int i = 0; i < no_points_branch; i++)
        {
            time_bin1_f_perX[perX_counter] += (time_bin1_f[i]) / bins_Xaverage;
            TRD_bin1_f_perX[perX_counter] += TRD_bin1_f[i] / bins_Xaverage;
            DSN_bin1_f_perX[perX_counter] += DSN_bin1_f[i] / bins_Xaverage;
            if (((i + 1) % bins_Xaverage == 0) && (i + 1))
                perX_counter++;
        }
        cout << "perX counter" << perX_counter;
        TGraph *gr_DSN_time_shift_perX = new TGraph(perX_counter, time_bin1_f_perX, DSN_bin1_f_perX);
        TGraph *gr_TRD_time_shift_perX = new TGraph(perX_counter, time_bin1_f_perX, TRD_bin1_f_perX);
        gr_TRD_time_shift_perX->SetLineColor(kRed);
        gr_TRD_time_shift_perX->Draw("AL");
        gr_TRD_time_shift_perX->GetXaxis()->SetRangeUser(0, 15);
        gr_TRD_time_shift_perX->GetYaxis()->SetRangeUser(-1, 6);
        gr_TRD_time_shift_perX->GetXaxis()->SetTitle("Time [s]");
        gr_TRD_time_shift_perX->GetYaxis()->SetTitle("Voltage [V]");
        gr_TRD_time_shift_perX->GetYaxis()->SetNdivisions(505);
        gr_TRD_time_shift_perX->GetXaxis()->SetNdivisions(505);
        gr_DSN_time_shift_perX->SetLineColor(kBlack);
        gr_DSN_time_shift_perX->Draw("same");
        TLegend *leg_time_shift = new TLegend(0.63, 0.75, 0.91, 0.91);
        leg_time_shift->AddEntry(gr_DSN_time_shift_perX, "Dose monitor");
        leg_time_shift->AddEntry(gr_TRD_time_shift_perX, " TRD");
        leg_time_shift->Draw();
    }
    //--Matching
    //----get matching point
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
    if (bool_display_times_shift)
    {
        TCanvas *canv_Short = new TCanvas("canv_time_shift_zoom", "Before and After Time Shift", canvasWidth, canvasHeight);
        canv_Short->cd();
        gr_short_DSN->SetLineWidth(2);
        gr_short_DSN->Draw();
        gr_short_DSN->GetYaxis()->SetNdivisions(505);
        gr_short_DSN->GetXaxis()->SetNdivisions(505);
        gr_short_DSN->GetXaxis()->SetTitle("Time [s]");
        gr_short_DSN->GetYaxis()->SetTitle("Output voltage [V] or A.U.");
        gr_scaled_TRD_matching->SetLineColor(kRed);
        gr_scaled_TRD_matching->SetLineWidth(2);
        gr_scaled_TRD_matching->Draw("same");
        gr_scaled_shifted_TRD->SetLineColor(kBlue);
        //        gr_scaled_shifted_TRD->Draw("same");
        TLegend *leg_time_shift_zoom = new TLegend(0.63, 0.75, 0.91, 0.91);
        leg_time_shift_zoom->AddEntry(gr_short_DSN, "Dose monitor");
        leg_time_shift_zoom->AddEntry(gr_scaled_TRD_matching, " Normalized TRD");
        leg_time_shift_zoom->Draw();
    }

    //Spot_divide
    //gROOT->LoadMacro("subSpotDivide.C");
    cout << "========  1  ========" << endl;
    cout << " Data for time shift\t: " << file_name_data_analysis_str << endl;
    float PSNX_f, PSNY_f, PSNX_bin1_f[max_bins_const], PSNY_bin1_f[max_bins_const],
        timespan_spotchange = 70.e-6, timespan_during_spot = 70.e-6,
        diff_PSN_spotchange = 13e-3, PSNX_spot_average = 0, PSNY_spot_average = 0,
        diff_PSNX_from_spot_average, diff_PSNY_from_spot_average,
        timespan_slicechange = 5.e-3, timediff_DSN_PSN = -20.e-6,
        spot_pos_x[max_bins_const], spot_pos_y[max_bins_const];
    double spot_data_bin2_d[4][max_bins_const]; //[0:time], [1;slice no], [2:DSN], [3:TRD]
    char branch_name_PSNX_c[6] = "POS_X", branch_name_PSNY_c[6] = "POS_Y";
    int flag_spot_change_bin1[max_bins_const] = {0},
        counter_spotchange, counter_during_spot, counter_slicechange = 0,
        flag_slice_change = 0, bin_shift_spot_dividing = bin_shift + int(timediff_DSN_PSN / sampling_time_f),
        no_slice = 0, no_spot = 1, counter_delay_spotchange = 0;
    cout << "bin shift from PSN : " << bin_shift_spot_dividing << endl;
    const int binspan_spotchange = int(timespan_spotchange / sampling_time_f),
              binspan_during_spot = int(timespan_during_spot / sampling_time_f),
              binspan_slicechange = int(timespan_slicechange / sampling_time_f),
              binspan_determine_spot = binspan_during_spot + binspan_spotchange,
              threshold_during_spot = int(binspan_during_spot * 0.8),
              threshold_spotchange = int(binspan_spotchange * 0.5),
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
        time_bin1_f[i] = time_f + time_shift_for_plot_f;
        //        time_shifted_bin1_f[i]=time_f+bin_shift*sampling_time_f;
        TRD_bin1_f[i] = -TRD_f;
        DSN_bin1_f[i] = DSN_f;
        PSNX_bin1_f[i] = PSNX_f;
        PSNY_bin1_f[i] = PSNY_f;
        //--dose integration
        counter_during_spot = 0;
        counter_spotchange = 0;
        //--detect spot change
        if (i < binspan_determine_spot)
            ; //spot dividing is not possible
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
                    PSNX_spot_average += PSNX_bin1_f[i - binspan_during_spot - j];
                    PSNY_spot_average += PSNY_bin1_f[i - binspan_during_spot - j];
                }
                PSNX_spot_average = PSNX_spot_average / (binspan_during_spot + 1);
                PSNY_spot_average = PSNY_spot_average / (binspan_during_spot + 1);
                if (counter_during_spot >= threshold_during_spot)
                {
                    no_slice++;
                    flag_slice_change = 0;
                    counter_slicechange = 0;
                    flag_spot_change_bin1[i - binspan_spotchange - 1 - bin_shift_spot_dividing] = 1;
                    spot_data_bin2_d[0][no_spot] = time_bin1_f[i - 9];
                    spot_data_bin2_d[1][no_spot] = no_slice;
                    spot_pos_x[no_spot - 1] = PSNX_spot_average;
                    spot_pos_y[no_spot - 1] = PSNY_spot_average;
                    no_spot++;
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
                    continue; //if there is no plateau, next bin
                for (int j = 0; j < binspan_spotchange; j++)
                {
                    if ((PSNX_bin1_f[i - binspan_spotchange + j] - PSNX_spot_average > diff_PSN_spotchange) || (-PSNX_bin1_f[i - binspan_spotchange + j] + PSNX_spot_average > diff_PSN_spotchange))
                    {
                        counter_spotchange++;
                    }
                    if ((PSNY_bin1_f[i - binspan_spotchange + j] - PSNY_spot_average > diff_PSN_spotchange) || (-PSNY_bin1_f[i - binspan_spotchange + j] + PSNY_spot_average > diff_PSN_spotchange))
                    {
                        counter_spotchange++;
                    }
                }
                if (counter_spotchange < threshold_spotchange)
                    continue;
                flag_spot_change_bin1[i - binspan_spotchange - 1 - bin_shift_spot_dividing] = 1; //reference event time:DSN
                spot_data_bin2_d[0][no_spot] = time_bin1_f[i - binspan_spotchange - 1] + bin_shift_spot_dividing * sampling_time_f;
                spot_data_bin2_d[1][no_spot] = no_slice;
                counter_delay_spotchange = 0;
                spot_pos_x[no_spot - 1] = PSNX_spot_average;
                spot_pos_y[no_spot - 1] = PSNY_spot_average;
                no_spot++;
            }
            spot_pos_x[no_spot - 1] = PSNX_spot_average;
            spot_pos_y[no_spot - 1] = PSNY_spot_average;
            counter_delay_spotchange++;
            counter_slicechange = 0;
            if (!no_spot)
                continue;
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
    sampling_time_f = int((time_bin1_f[1] - time_bin1_f[0]) * 1.e6) * 1.e-6;
    calibaration_factor_V_to_Gy = calibaration_factor_V_to_Gy * sampling_time_f;
    int spot_counter = 0;
    for (int i = 0; i < no_points_branch; i++)
    {
        spot_data_bin2_d[2][spot_counter] += (DSN_bin1_f[i - binspan_spotchange] - base_DSN); //DSN
        spot_data_bin2_d[3][spot_counter] += (TRD_bin1_f[i - binspan_spotchange] - base_TRD); //TRD
        if (flag_spot_change_bin1[i])
            spot_counter++;
    }

    cout << "Numbers of Slices : " << no_slice << endl
         << "Default number of spots and identified spots : " << no_spot_in_pattern << ", " << no_spot << endl;
    TString spot_result_file_name = "result" + file_name_data_analysis_str + ".txt";
    ofstream ofs_result_spot("result" + file_name_data_analysis_str + ".txt");
    ofs_result_spot << "No time PSNX PSNY SliceNo spotDSN spotTRD" << endl;
    for (int i = 0; i < no_spot; i++)
    {
        //[0:time] [spot pos x] [spot pos y] [1;slice no] [2:DSN] [3:TRD]
        ofs_result_spot << i + 2 - (no_spot - no_spot_in_pattern) << " " << setprecision(9) << spot_data_bin2_d[0][i] << " "
                        << spot_pos_x[i] << " " << spot_pos_y[i] << " "
                        << spot_data_bin2_d[1][i] << " " << spot_data_bin2_d[2][i] << " " << spot_data_bin2_d[3][i] << endl;
        // ofs_result_spot << i +1 - (no_spot - no_spot_in_pattern) << " " << setprecision(9) << spot_data_bin2_d[0][i] << " " << spot_data_bin2_d[1][i] << " " << spot_data_bin2_d[2][i] << " " << spot_data_bin2_d[3][i] << endl;
    }
    if (bool_display_raw_data)
    {
        float TRD_divided_point_bin1_f[max_bins_const / 100], TRD_divided_time_bin1_f[max_bins_const / 100];
        float ymin_TRD_raw = -1, ymax_TRD_raw = 6, scaling_TRD_PSN = 12, xmin_TRD_raw = 0.5, xmax_TRD_raw = 1.3;
        TCanvas *canv_raw_data = new TCanvas("canv_spot_dividing", "Spot dividing", canvasWidth, canvasHeight);
        canv_raw_data->cd();
        TGraph *gr_TRD_raw = new TGraph(no_points_branch, time_bin1_f, TRD_bin1_f);
        gr_TRD_raw->SetTitle("Spot dividing");
        TGraph *gr_PSNX_raw = new TGraph(no_points_branch, time_bin1_f, PSNX_bin1_f);
        TGraph *gr_PSNY_raw = new TGraph(no_points_branch, time_bin1_f, PSNY_bin1_f);
        for (int i = 0; i < no_points_branch; i++)
        { // matching time axis and
            gr_PSNX_raw->GetX()[i] += (bin_shift_spot_dividing * sampling_time_f);
            gr_PSNX_raw->GetY()[i] = (gr_PSNX_raw->GetY()[i] - 2) * scaling_TRD_PSN;
        }
        for (int i = 1; i < no_spot - 1; i++)
        {
            TRD_divided_time_bin1_f[i] = spot_data_bin2_d[0][i];
            TRD_divided_point_bin1_f[i] = gr_PSNX_raw->Eval(spot_data_bin2_d[0][i]);
        }
        TGraph *gr_divide_point = new TGraph(no_spot, TRD_divided_time_bin1_f, TRD_divided_point_bin1_f);
        gr_TRD_raw->GetYaxis()->SetRangeUser(ymin_TRD_raw, ymax_TRD_raw);
        gr_TRD_raw->GetXaxis()->SetRangeUser(xmin_TRD_raw, xmax_TRD_raw);
        gr_TRD_raw->GetYaxis()->SetNdivisions(505);
        gr_TRD_raw->GetXaxis()->SetNdivisions(505);
        gr_TRD_raw->GetXaxis()->SetTitle("Time [s]");
        gr_TRD_raw->GetYaxis()->SetTitle("TRD output [V]");
        gr_TRD_raw->Draw("AL");
        gr_PSNX_raw->SetLineColor(kGreen + 1);
        gr_PSNX_raw->Draw("SAME");
        TGaxis *axis_PSN = new TGaxis(xmax_TRD_raw, ymin_TRD_raw, xmax_TRD_raw, ymax_TRD_raw, ymin_TRD_raw / scaling_TRD_PSN + 2, ymax_TRD_raw / scaling_TRD_PSN + 2, 505, "+L");
        axis_PSN->SetLabelColor(kGreen + 2);
        axis_PSN->SetLineColor(kGreen + 2);
        axis_PSN->SetVertical();
        axis_PSN->SetTitle("Position monitor output [V]");
        axis_PSN->SetTitleColor(kGreen + 2);
        axis_PSN->SetTitleFont(42);
        axis_PSN->SetLabelFont(42);
        axis_PSN->SetTitleSize(0.05);
        axis_PSN->Draw();
        gr_divide_point->SetMarkerStyle(2);
        gr_divide_point->SetMarkerSize(1);
        gr_divide_point->SetMarkerColor(kRed);
        gr_divide_point->Draw("PSAME");
        TLegend *leg_divide_point = new TLegend(0.63, 0.75, 0.91, 0.91);
        leg_divide_point->AddEntry(gr_TRD_raw, "TRD output");
        leg_divide_point->AddEntry(gr_PSNX_raw, "Position monitor (X)", "l");
        leg_divide_point->AddEntry(gr_divide_point, "Spot dividing point", "p");
        leg_divide_point->Draw();
        //        TGraph * gr_DSN_raw = new TGraph(no_points_branch, time_bin1_f, DSN_bin1_f);
    }
    if (bool_display_spot_dose)
    {
        TCanvas *canv_spotIntegration = new TCanvas("canv_spot_dose", "Spot Dose", canvasWidth, canvasHeight);
        double xmin_spot_TRD = 200, xmax_spot_TRD = 425, ymin_spot_TRD = -0.05, ymax_spot_TRD = 0.7, scaling_spot_IntegSpot = 0.075;
        canv_spotIntegration->cd();
        TGraph *gr_spot_dose = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %*lg %lg");
        TGraph *gr_spot_DSN = new TGraph(spot_result_file_name, "%lg %*lg %*lg %*lg %*lg %lg %*lg");
        TGraph *gr_Integrated_Dose = new TGraph();
        for (int i = 0; i < gr_spot_dose->GetN(); i++)
            gr_spot_dose->GetY()[i] *= calibaration_factor_V_to_Gy * 1000;
        for (int i = 0; i < gr_spot_dose->GetN(); i++)
            gr_spot_DSN->GetY()[i] *= 0.001;
        gr_Integrated_Dose->SetPoint(0, gr_spot_dose->GetX()[0], gr_spot_dose->GetY()[0] * scaling_spot_IntegSpot);
        for (int i = 1; i < gr_spot_dose->GetN(); i++)
            gr_Integrated_Dose->SetPoint(i, gr_spot_dose->GetX()[i], gr_spot_dose->GetY()[i] * scaling_spot_IntegSpot + gr_Integrated_Dose->GetY()[i - 1]);
        //cout << "Calibration factor (" <<calibaration_factor_V_to_Gy<<") was applied"<< endl;
        gr_spot_dose->SetMarkerStyle(6);
        gr_spot_dose->SetMarkerSize(5);
        gr_spot_DSN->SetLineColor(kRed);
        gr_spot_dose->GetXaxis()->SetTitle();
        //gr_spot_dose->GetXaxis()->SetRangeUser(1,no_spot_in_pattern);
        gr_spot_dose->GetXaxis()->SetRangeUser(xmin_spot_TRD, xmax_spot_TRD);
        gr_spot_dose->GetYaxis()->SetRangeUser(ymin_spot_TRD, ymax_spot_TRD);
        gr_spot_dose->SetTitle("Spot Dose");
        gr_spot_dose->GetYaxis()->SetNdivisions(505);
        gr_spot_dose->GetXaxis()->SetNdivisions(505);
        gr_spot_dose->SetFillColor(kBlack);
        gr_spot_dose->SetLineWidth(3);
        gr_spot_dose->Draw("AB");
        // gr_spot_DSN->Draw("LSAME");
        gr_spot_dose->GetXaxis()->SetTitle("Number of spot");
        gr_spot_dose->GetYaxis()->SetTitle("Spot Dose [10^{-3} Gy]");
        TLegend *leg_Integrated_dose = new TLegend(0.63, 0.75, 0.91, 0.91);
        leg_Integrated_dose->AddEntry(gr_spot_dose, "Spot Dose", "l");

        if (bool_display_cumulated_dose)
        {
            gr_Integrated_Dose->SetLineColor(kGreen + 1);
            gr_Integrated_Dose->SetLineWidth(3);
            gr_Integrated_Dose->Draw("LSAME");
            TGaxis *axis_Integrated_dose = new TGaxis(xmax_spot_TRD, ymin_spot_TRD, xmax_spot_TRD, ymax_spot_TRD, ymin_spot_TRD / scaling_spot_IntegSpot, ymax_spot_TRD / scaling_spot_IntegSpot, 505, "+L");
            axis_Integrated_dose->SetLabelColor(kGreen + 2);
            axis_Integrated_dose->SetLineColor(kGreen + 2);
            axis_Integrated_dose->SetVertical();
            axis_Integrated_dose->SetTitle("Cumulative Dose [10^{-3} Gy]");
            axis_Integrated_dose->SetTitleColor(kGreen + 2);
            axis_Integrated_dose->SetTitleFont(42);
            axis_Integrated_dose->SetLabelFont(42);
            axis_Integrated_dose->SetTitleSize(0.05);
            axis_Integrated_dose->Draw();
            leg_Integrated_dose->AddEntry(gr_Integrated_Dose, "Cumulated Dose", "l");
        }
        if (bool_display_simulated_TRD)
        {
            //TCanvas *canvtemp = new TCanvas();
            TGraph *gr_Integrated_dose_simulation = new TGraph(file_name_data_simulation_str, "%lg %*lg %*lg %*lg %*lg %*lg %lg");
            float scaling_measurement_simulation = gr_Integrated_Dose->GetY()[gr_Integrated_Dose->GetN() - 1] / gr_Integrated_dose_simulation->GetY()[gr_Integrated_dose_simulation->GetN() - 1];
            for (int i = 0; i < gr_Integrated_dose_simulation->GetN(); i++)
            {
                gr_Integrated_dose_simulation->GetY()[i] *= scaling_measurement_simulation;
                gr_Integrated_dose_simulation->GetX()[i] = gr_Integrated_dose_simulation->GetX()[i] * 3.14e-3 - 1.5;
            }
            gr_Integrated_dose_simulation->SetLineColor(kRed);
            gr_Integrated_dose_simulation->SetLineWidth(2);
            gr_Integrated_dose_simulation->SetLineStyle(2);
            gr_Integrated_dose_simulation->Draw("Lsame");
            leg_Integrated_dose->AddEntry(gr_Integrated_dose_simulation, "Simulation", "l");
        }
        if (bool_display_simulated_TRD)
        {
            //            TCanvas *canvtemp = new TCanvas();
            TGraph *gr_spot_dose_simulation = new TGraph(file_name_data_simulation_str, "%lg %*lg %*lg %*lg %*lg %lg %*lg");
            for (int i = 0; i < gr_spot_dose_simulation->GetN(); i++)
            {
                gr_spot_dose_simulation->GetY()[i] *= 0.029;
                gr_spot_dose_simulation->GetX()[i] = gr_spot_dose_simulation->GetX()[i] * 3.14e-3 - 1;
            }
            gr_spot_dose_simulation->SetLineColor(kRed);
            gr_spot_dose_simulation->SetLineWidth(3);
            gr_spot_dose_simulation->SetLineStyle(2);
            gr_spot_dose_simulation->Draw("Lsame");
            leg_Integrated_dose->AddEntry(gr_spot_dose_simulation, "Simulation", "l");
        }
        leg_Integrated_dose->Draw();
        if (1)
        {
            TCanvas *canv_dose_probability_dist = new TCanvas("canv_dose_probability_dist", "Dose probability", canvasWidth, canvasHeight);
            TH1F *h1_dose_probability_dist = new TH1F("h1_dose_probability_dist", "Dose probability distribution", 100, -0.1, 0.9);
            TH1F *h1_dose_effect_probability_dist = new TH1F("h1_dose_effect_probability_dist", "Effective Dose probability distribution", 100, -0.1, 0.9);
            for (int i = 0; i < gr_spot_dose->GetN(); i++)
            {
                h1_dose_probability_dist->Fill(gr_spot_dose->GetY()[i], 1.0);
                h1_dose_effect_probability_dist->Fill(gr_spot_dose->GetY()[i], double(gr_spot_dose->GetY()[i]) * 100); ///
            }
            canv_dose_probability_dist->SetLogy();
            // h1_dose_effect_probability_dist->SetLineColor(kGreen);
            h1_dose_effect_probability_dist->Draw();
            h1_dose_effect_probability_dist->GetYaxis()->SetRangeUser(0.1, 1e6);
            h1_dose_probability_dist->Draw("same");
        }
    }

    // ofstream ofs_spot_dose();
    if (1)
    {
        TCanvas *canv_spotContour = new TCanvas("canv_spot_contour", "Dose delivered from each spot point", canvasHeight, canvasHeight);
        canv_spotContour->cd();
        TGraph2D *gr2d_spotdose_from_each_psn = new TGraph2D(spot_result_file_name, "%*lg %*lg %lg %lg %*lg %*lg %lg");
        gStyle->SetPalette(55);
        // gr2d_spotdose_from_each_psn->GetXaxis()->SetRangeUser(1.7, 2.5);
        // gr2d_spotdose_from_each_psn->GetYaxis()->SetRangeUser(1.7, 2.5);
        gr2d_spotdose_from_each_psn->Draw("colz"); //text45
    }
}