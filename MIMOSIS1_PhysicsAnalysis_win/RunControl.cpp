/*! @file RunControl.cpp
 *  @brief main() 
 * 	\todo Configurator class should be written with all parameters that are in config. MIMOSIS1_TreeReader and LabTest should implement Configurator object
 * and use the values stored by it. Not load param_1 ... each seperetly. 
 */


#include "globals.h"
#include "cout_msg.h"

#include "MIMOSIS1_TreeReader.h"
#include "LabTest.h"



int main() 
{
	
	MSG(INFO, "\n ------- MIMOSIS-1 ANALYSIS ------- \n ");

	MIMOSIS1_TreeReader *tree_reader =  new MIMOSIS1_TreeReader();
	tree_reader		->		init("config_file.cfg");
	std::cout 		<< 		*tree_reader << std::endl;
	tree_reader		->		load_intput_files();
	
	std::cout << std::endl;
	LabTest *test_vph 		= 		new LabTest(
									tree_reader -> get_integrated_frames(), 
									tree_reader -> get_scan_values() );
	test_vph->_scaned_param = 		tree_reader -> get_scanned_parameter_name() ;
	std::cout << "here: " << (tree_reader->get_integrated_frames()).size() << std::endl;
	std::cout << "here2: " << (test_vph->_v_MIM_int_frame).size() << std::endl;
	std::cout << "Dupa " << ( (test_vph->_v_MIM_int_frame)[2]).nb_of_frames << std::endl;
	std::cout << "Dupa2 " << (((test_vph->_v_MIM_int_frame)[2]).h2_hit_map_ob).GetEntries() << std::endl;


	test_vph		->		init("config_file.cfg");
	std::cout 		<< 		*test_vph << std::endl;
	test_vph		->		open_output_tree();
	test_vph		->		init_histo();
	test_vph		->		copy_integrated_frames_histos_to_output();
	test_vph		->		plot_S_curves();
	test_vph		->		close_output_tree();
	return 0;

}
