#include <sstream>
#include <iostream>
#include <string>
{
    // データファイルを読み込む
	
   TString file_name_input, file_name_output;
	string strContents, strVal, strNameVal[6];
	char cNameVal[6][12];
	ifstream file_input, file_list;
    // データを格納するための変数
    float val1, val2, val3, val4, val5, val6;
	std::cout << "  Enter file name\n  >> ";
	std::cin >> file_name_input;
	bool flag_file_list = 0;
	TTree *tree = new TTree("T1", "TimeResolved");
		
//	TFile *file_output;

	if(file_name_input.Contains("list")||file_name_input.Contains("LIST")||file_name_input.Contains("List")){
		file_list.open(file_name_input);
		flag_file_list = 1;
		file_list>>file_name_input;
	}

	do{		
		cout <<"	>> Processing : " << file_name_input << endl;
		file_name_output = file_name_input;
		file_name_output.Append(".root");
		file_input.open(file_name_input);
		strContents.clear();
		strVal.clear();
		for(int i =0; i<6; i++) {
			strNameVal[i].clear();
			memset(cNameVal[i], '\0', sizeof(cNameVal[i]));
		}

		//Read Header
		for(int i = 0; i<10; i++){
			std::getline(file_input, strContents);
			if(i==2){
				stringstream linestream(strContents);
				for(int j=0; j<6; j++){
					std::getline(linestream, strNameVal[j], ',');
					std::strcpy(cNameVal[j], strNameVal[j].c_str());
					for (int l=0; l<strlen(cNameVal[j])-1;l++){
						cNameVal[j][l] = cNameVal[j][l+1];
						if(l == strlen(cNameVal[j])-2) cNameVal[j][l] = '\0';
					}
				}
			}
		}
	//return 0;
		// TTreeを作成する
		// TTree::Branch(...)を使って、各変数のブランチを作成する
		// 第一引数：ブランチ名；なんでも良い；用意した変数名と違っていても構わない
		// 第二引数：変数のアドレス；変数が実体の場合は、&を先頭につけてアドレスを指定する；事前に変数を用意しておかないと怒られる
		// 第三引数：変数の型；"変数／型"の形で記述する；int型はI, float型はF, double型はFなど
	TTree *tree = new TTree("T1", "TimeResolved");
		tree->Branch("Time", &val1, "val1/F");
		tree->Branch(cNameVal[1], &val2, "val2/F");
		tree->Branch(cNameVal[2], &val3, "val3/F");
		tree->Branch(cNameVal[3], &val4, "val4/F");
		tree->Branch(cNameVal[4], &val5, "val5/F");
		tree->Branch(cNameVal[5], &val6, "val6/F");
		
		// C++でファイルを読み込むときの常套手段
		
		while (std::getline(file_input, strContents)) {
			stringstream linestream(strContents);
			std::getline(linestream, strVal, ',');
			val1 = stof(strVal);
			std::getline(linestream, strVal, ',');
			val2 = stof(strVal);
			std::getline(linestream, strVal, ',');
			val3 = stof(strVal);
			std::getline(linestream, strVal, ',');
			val4 = stof(strVal);
			std::getline(linestream, strVal, ',');
			val5 = stof(strVal);
			std::getline(linestream, strVal, ',');
			val6 = stof(strVal);
				//std::cout << val1 << " " << val2 << " "<< val2 << " "<< val3 << " "<< val4 << " "<< val5 << " " << std::endl;
			if(val6<1.)tree->Fill();  // データのエントリの区切りで必ずTTree::Fill()する
		}
	
		// 作成したTTreeを保存するためのROOTファイルを準備する
		TFile *file_output = new TFile(file_name_output, "recreate");
		file_name_output.Clear();
		file_name_input.Clear();
		tree->Write();  // TFileを開いた状態で、TTree::Write()すれば書き込みできる；違うファイルに書き込みたい場合は後述するかも
		file_output->Close();  // 最後にファイルを閉じる；プログラム（やマクロ）終了時に勝手に閉じてくれるらしいが一応
		file_input.close();
		tree->Reset();
	}while(file_list>>file_name_input);
    return 0;
}
