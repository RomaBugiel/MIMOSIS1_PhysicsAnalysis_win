#include "MIMOSIS1_TreeReader.h"

std::ostream &operator<<(std::ostream &os, const MIMOSIS1_TreeReader &treereader) 
{
	os << std::setw(30) << " TreeReader is run with: " 		<< std::endl;
	os << std::setw(30) << "_param_1 : " 					<< treereader._param_1 			<< std::endl;
	os << std::setw(30) << "_param_2 : " 					<< treereader._param_2 			<< std::endl;
	os << std::setw(30) << "_param_3 : " 					<< treereader._param_3 			<< std::endl;
	//os << std::setw(30) << "_param_1_value : " 				<< treereader._param_1_value 	<< std::endl;
	//os << std::setw(30) << "_param_2_value : " 				<< treereader._param_2_value 	<< std::endl;
	//os << std::setw(30) << "_param_3_value : " 				<< treereader._param_3_value 	<< std::endl;
	os << std::setw(30) << "_enable_scan : " 				<< treereader._enable_scan 		<< std::endl;
	os << std::setw(30) << "_frames_in_run : " 				<< treereader._frames_in_run 		<< std::endl;
	os << std::setw(30) << "_val_min : " 					<< treereader._val_min 			<< std::endl;
	os << std::setw(30) << "_val_max : " 					<< treereader._val_max 			<< std::endl;
	os << std::setw(30) << "_step : " 						<< treereader._step 			<< std::endl;
	os << std::setw(30) << "_incl : " 						<< treereader._incl 			<< std::endl;
	os << std::setw(30) << "_input_tree_file_path : " 		<< treereader._input_tree_file_path 		<< std::endl;
	os << std::setw(30) << "_input_tree_file_name_core : " 	<< treereader._input_tree_file_name_core 	<< std::endl;
	os << std::setw(30) << "_input_tree_name : " 			<< treereader._input_tree_name 				<< std::endl;
	
									
	return os;                          
}

/*
 * Takes parameter min, max and step values and also inclusion list to prepare the list of parameter values used futher in the analysis
 */
 
int MIMOSIS1_TreeReader::extract_scan_values_list()
{	
	std::string param;
	
	switch(_enable_scan) 
	{
		case 1: { param = "Back bias"; 			break;}
		case 2: { param = "Pulse height"; 		break;}
		case 3: { param = "Threshold voltage"; 	break;}
		default:{ param = "WRONG PARAM"; MSG(WARN, "No scan enable!" ); break;}
	}
	
	
	if(_val_min == _val_max) 
	{
		v_param_values.push_back(_val_min);
	}
	else 
	{
		for(double p = _val_min; p <= _val_max ; p += _step)
		{	
			v_param_values.push_back(p);
		}
	}

	if(_incl >= 0) v_param_values.push_back(_incl);

	MSG(INFO, param + " analysis scan is enabled from " + std::to_string(_val_min) + " to " + std::to_string(_val_max) + " in " + std::to_string(v_param_values.size()) << " steps: ");
	for(auto &i : v_param_values) std::cout <<  i << std::endl;

	return 0;

}


void MIMOSIS1_TreeReader::init(const char * config_file)
{
	
	if(config.ReadFile(config_file, kEnvUser) < 0)
	{ 
		MSG(ERR, "Config file not loaded --> exit." );
		exit(0);
	}
	else
	{
	
		_param_1 		=	config.GetValue("_param_1", "");
		_param_2 		=   config.GetValue("_param_2", "");
		_param_3 		=   config.GetValue("_param_3", "");
		                    
		_param_1_value	=   config.GetValue("_param_1_value", 0);
		_param_2_value	=   config.GetValue("_param_2_value", 0);
		_param_3_value	=   config.GetValue("_param_3_value", 0);
	                       
		_enable_scan	=	config.GetValue("_enable_scan", 0);
		_frames_in_run	=	config.GetValue("_frames_in_run", -1);

		_val_min		=	config.GetValue("_val_min", 0);
		_val_max		=   config.GetValue("_val_max", 0);
		_step			=   config.GetValue("_step", 0);
		_incl			=   config.GetValue("_incl", -1);		
		
		_input_tree_file_path		=	config.GetValue("_input_tree_file_path", "");		
		_input_tree_file_name_core 	=   config.GetValue("_input_tree_file_name_core", "");	
		_input_tree_name			=   config.GetValue("_input_tree_name", "");			
	
		_row_start				=	config.GetValue("_row_start", 0);
		_row_end				=	config.GetValue("_row_end", 0);
		_column_start			=	config.GetValue("_column_start", 0);
		_column_end				=	config.GetValue("_column_end", 0);
	}
	
	extract_scan_values_list();
}

/*
 * Load input TFiles
 */


void MIMOSIS1_TreeReader::load_intput_files()
{
	TString inname 	{""};
	TFile	* infile{ nullptr };
	TTree	* tree {nullptr};
	TH2D	* h2_hit_map {nullptr};

	MIMOSIS1_Integrated_Frame	integrated_frame;	
	std::map<std::string, int> 	run_param;

	const int nb_of_bins_x  {(_column_end-_column_start)+1};
	const int nb_of_bins_y  {(_row_end-_row_start)+1};

	// for (long unsigned int i = 0; i <5; i++)
	for(long unsigned int i = 0; i < v_param_values.size(); i++) 
	{		
		// Depending on which parameter scan was made, prepare a proper name and proper parameters info for data structure
		// to do: might be probably shorter
		switch( _enable_scan ) 
		{
			case 1: { _param_1_value = v_param_values[i]; break;	}
			case 2: { _param_2_value = v_param_values[i]; break;	}
			case 3: { _param_3_value = v_param_values[i]; break;	}
			default:{ break;}
		}
		
		run_param.insert ( std::pair<std::string,int>(_param_1,_param_1_value) 	);
		run_param.insert ( std::pair<std::string,int>(_param_2,_param_2_value) 	);
		run_param.insert ( std::pair<std::string,int>(_param_3,_param_3_value) 	);

		inname = _input_tree_file_path +"\\"+_input_tree_file_name_core+"_"+ _param_1 +"_" + TString::Itoa(_param_1_value, 10) + "_" + _param_2 +"_" + TString::Itoa(_param_2_value, 10) + "_" + _param_3 + "_" + TString::Itoa(_param_3_value, 10) + ".root";
		infile = new TFile(inname);
		
		if (infile->IsOpen() == true)
		{
			std::cout << "File:" << inname << " IS OPEN " << std::endl;

			//If file is open proceed with opening tree and loading data
			tree = (TTree*) infile->Get((TString)_input_tree_name);

			//Histo 1 read 
			if ((TH2D*)infile->Get("h2_integrated_frame_matrix") != nullptr)
			{
				h2_integrated_frame_matrix = (TH2D*)infile->Get("h2_integrated_frame_matrix");
				MSG(DEB, "Entries in -- h2_integrated_frame_matrix--: " << h2_integrated_frame_matrix -> GetEntries());
			}
			else std::cout << "\tNullptr h2_integrated_frame_matrix histogram" << std::endl;
			
			//Histo 2 read
			if ((TH2D*)infile->Get("h2_integrated_frame_part") != nullptr)
			{
				h2_integrated_frame_part = (TH2D*)infile->Get("h2_integrated_frame_part");
				MSG(DEB, "Entries in -- h2_integrated_frame_part --: " << h2_integrated_frame_part->GetEntries());
			}
			else std::cout << "\tNullptr h2_integrated_frame_matrix histogram" << std::endl;

			//Histo 3 read
			if ((TH1D*)infile->Get("h_fired_pixels_int_frame") != nullptr)
			{
				h_fired_pixels_int_frame = (TH1D*)infile->Get("h_fired_pixels_int_frame");
				MSG(DEB, "\t[CNTRL] Entries in -- h_fired_pixels_int_frame --: " << h_fired_pixels_int_frame->GetEntries());
			}
			else std::cout << "\tNullptr h_fired_pixels_int_frame histogram" << std::endl;
		
			//Fill MIMOSIS1_Integrated_Frame structure
			integrated_frame.h2_hit_map = h2_integrated_frame_matrix; // !!! part or full here?
				MSG(CNTR, "Number of entries in structure during integrated frame vector generation: " << (integrated_frame.h2_hit_map)->GetEntries() );
			integrated_frame.h2_hit_map_ob = *h2_integrated_frame_matrix; // !!! part or full here?
					
			integrated_frame.nb_of_frames = _frames_in_run;
			integrated_frame.run_param = run_param;
			
			//Fill the vector
			v_MIM_int_frame.push_back(integrated_frame);
		}
		else
		{
			std::cout << "File: " << inname <<" is not open." << std::endl;		
			exit(0);
		}
		
		infile->Close();
		h2_hit_map 	= nullptr;
		//infile		->Close();
		//infile 		= nullptr;
		tree 		= nullptr;
		run_param.clear();

	}
	
	std::cout << "MIMOSIS1_TreeReader built vector from: "  << std::to_string(v_MIM_int_frame.size()) << " runs. (" + std::to_string(v_MIM_int_frame.size()) << " integrated frames loaded)" << std::endl;
	
	delete h2_hit_map;
	delete infile;
	delete tree;

}

/*
 * Return vector of MIMOSIS0_Int_Frame structures
 */

std::vector<MIMOSIS1_Integrated_Frame> MIMOSIS1_TreeReader::get_integrated_frames()
{
	return v_MIM_int_frame;
}

/*
 * Return vector of values used in parameters scan
 */

std::vector<int> MIMOSIS1_TreeReader::get_scan_values()
{
	return v_param_values;
}


/*
 * Return vector of values used in parameters scan
 */
TString MIMOSIS1_TreeReader::get_scanned_parameter_name()
{
	TString _param {"WRONG PARAM NAME"};
	
	switch( _enable_scan ) 
		{
			case 1: { _param = static_cast<TString>(_param_1); break;	}
			case 2: { _param = static_cast<TString>(_param_2); break;	}
			case 3: { _param = static_cast<TString>(_param_3); break;	}
			default:{ break;}
		}
	
	return _param;
}
