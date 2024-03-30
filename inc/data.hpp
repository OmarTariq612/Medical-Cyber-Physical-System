#ifndef _DATA_HPP
#define _DATA_HPP 
    /* Nodes Data */
    /* matlab name: node_table */
    //  Consider this vector as a column to the left of the node_integer_parameters
    const std::vector<std::string> node_names = {
        "SA", "CT", "AV", "OS", "His_p", "His_m", "His_d", "Bach", "LA_a", "LA",
        "RBB_m", "RBB", "LBB_m", "LBB", "RVA", "LVA", "RV_m", "RV", "LV_m", "LV",
        "CT_a", "RA_a", "RA", "SEP_RV_m", "SEP_RV", "SEP_LV_m", "SEP_LV", "CS_LV", 
        "CS_LA", "slow_b", "slow_a", "fast", "fast_b"
    };
    const std::vector<std::vector<int>> node_integer_parameters = {
        {1, 220, 220, 10, 20, 700, 700, 0, 150, 300, 0, 2},
        {1, 220, 220, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 250, 250, 9999, 9999, 0, 350, 230, 0, 1},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 100, 200, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 330, 440, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 180, 290, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 180, 290, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 180, 290, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 180, 290, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 180, 290, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 180, 290, 0, 2},
        {1, 220, 220, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 360, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 300, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 300, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 300, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 300, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 300, 0, 2},
        {1, 320, 320, 120, 120, 9999, 9999, 0, 150, 300, 0, 2},
        {1, 320, 320, 200, 200, 9999, 9999, 0, 250, 150, 0, 1},
        {1, 320, 320, 300, 300, 9999, 9999, 0, 250, 150, 0, 1},
        {1, 320, 320, 10, 10, 9999, 9999, 0, 500, 380, 0, 1},
        {1, 320, 320, 10, 10, 9999, 9999, 0, 500, 380, 0, 1},
    };
    
    /*matlab name: node_pos*/
    const std::vector<node_position> node_positions = {
        {135.500000000000, 295.500000000000},
        {134.500000000000, 161.500000000000},
        {170.500000000000, 216.500000000000},
        {165.500000000000, 263.500000000000},
        {204.500000000000, 248.500000000000},
        {236.500000000000, 250.500000000000},
        {262.500000000000, 220.500000000000},
        {245.500000000000, 344.500000000000},
        {313.500000000000, 349.500000000000},
        {338.500000000000, 325.500000000000},
        {304.500000000000, 161.500000000000},
        {382.500000000000, 127.500000000000},
        {319.500000000000, 172.500000000000},
        {392.500000000000, 149.500000000000},
        {394.500000000000, 113.500000000000},
        {410.500000000000, 144.500000000000},
        {280.500000000000, 96.5000000000000},
        {187.500000000000, 147.500000000000},
        {398.500000000000, 235.500000000000},
        {358.500000000000, 303.500000000000},
        {112.500000000000, 243.500000000000},
        {175.500000000000, 295.500000000000},
        {198.500000000000, 269.500000000000},
        {286.500000000000, 149.500000000000},
        {207.500000000000, 228.500000000000},
        {327.500000000000, 189.500000000000},
        {254.500000000000, 263.500000000000},
        {312.500000000000, 316.500000000000},
        {296.500000000000, 334.500000000000},
        {151.500000000000, 213.500000000000},
        {150.500000000000, 243.500000000000},
        {167.500000000000, 243.500000000000},
        {168.500000000000, 230.500000000000},
    };


    /* Path Data */
    /* matlab name: path_table */
    const std::vector<std::string> path_names = {
        "SA_CT_a", "CT", "slow_AV", "SA_OS", "fast_AV", "SA_Bach", "Bach_LA_a", 
        "LA", "AV_His", "His_p", "His_d", "His_RBB", "His_LBB", "RBB", "LBB", 
        "RBB_RV", "LBB_LV", "RV_LV", "RV_m", "LV_m", "RV", "LV", "SA_RA_a",
        "RA", "SEP_RV", "SEP_RV_m", "SEP_LV", "SEP_LV_m", "CS_LV", "CS_LA",
        "slow", "OS_slow", "OS_fast", "fast",
    };

    const std::vector<std::vector<int>> path_int_parameters = {

        {1, 1, 21, 10},
        {1, 21, 2, 10},
        {1, 30, 3, 5},
        {1, 1, 4, 5},
        {1, 33, 3, 5},
        {1, 1, 8, 5},
        {1, 8, 9, 10},
        {1, 9, 10, 10},
        {1, 3, 5, 2},
        {1, 5, 6, 2},
        {1, 6, 7, 2},
        {1, 7, 11, 2},
        {1, 7, 13, 2},
        {1, 11, 12, 2},
        {1, 13, 14, 2},
        {1, 12, 15, 10},
        {1, 14, 16, 10},
        {1, 15, 16, 10},
        {1, 15, 17, 10},
        {1, 16, 19, 10},
        {1, 17, 18, 10},
        {1, 19, 20, 10},
        {1, 1, 22, 10},
        {1, 22, 23, 10},
        {1, 24, 25, 10},
        {1, 15, 24, 10},
        {1, 26, 27, 10},
        {1, 16, 26, 10},
        {1, 27, 28, 10},
        {1, 23, 29, 5},
        {1, 31, 30, 10},
        {1, 4, 31, 10},
        {1, 4, 32, 10},
        {1, 32, 33, 10}};


    const std::vector<std::vector<float>> path_float_parameters = {
        {1,	                1,	                57,	57,	57,	57,	56.8594759033180,	2.26086956521739},
        {1,	                1,	                85,	85,	85,	85,	84.8999411071645,	-3.72727272727273},
        {1,	                1,	                19,	19,	19,	19,	19.2353840616713,	1.52777777777778},
        {1,	                1,	                44,	44,	44,	44,	43.8634243989226,	-1.06666666666667},
        {1,	                1,	                47,	47,	47,	47,	14.1421356237310,	-7},
        {3.01050244975818,	3.01050244975818,	40,	40,	40,	40,	120.420097990327,	0.445454545454545},
        {1,	                1,	                68,	68,	68,	68,	68.1835757349231,	0.0735294117647059},
        {1,	                1,	                35,	35,	35,	35,	34.6554469023269,	-0.960000000000000},
        {1,	                1,	                47,	47,	47,	47,	46.6904701197150,	0.941176470588235},
        {2,	                2,	                16,	16,	16,	16,	32.0624390837628,	0.0625000000000000},
        {2,	                2,	                20,	20,	20,	20,	39.6988664825584,	-1.15384615384615},
        {2,	                2,	                36,	36,	36,	36,	72.4223722339996,	-1.40476190476190},
        {2,	                2,	                37,	37,	37,	37,	74.5184540902453,	-0.842105263157895},
        {4.25440947723653,	4.25440947723653,	20,	20,	20,	20,	85.0881895447306,	-0.435897435897436},
        {3.82687862363049,	3.82687862363049,	20,	20,	20,	20,	76.5375724726098,	-0.315068493150685},
        {1,	                1,              	18,	18,	18,	18,	18.4390889145858,	-1.16666666666667},
        {1,	                1,              	19,	19,	19,	19,	18.6815416922694,	-0.277777777777778},
        {2,	                2,              	17,	17,	17,	17,	34.8855270850248,	1.93750000000000},
        {5.76302871761021,	5.76302871761021,	20,	20,	20,	20,	115.260574352204,	0.149122807017544},
        {4.58938993767146,	4.58938993767146,	20,	20,	20,	20,	91.7877987534291,	-7.58333333333333},
        {5.30330085889911,	5.30330085889911,	20,	20,	20,	20,	106.066017177982,	-0.548387096774194},
        {3.94461658466320,	3.94461658466320,	20,	20,	20,	20,	78.8923316932641,	-1.70000000000000},
        {1,	                1,	                40,	40,	40,	40,	40,	                0},
        {1,	                1,	                35,	35,	35,	35,	34.7131099154196,	-1.13043478260870},
        {2,	                2,	                56,	56,	56,	56,	111.722871427475,	-1},
        {2,	                2,	                57,	57,	57,	57,	113.841995766062,	-0.333333333333333},
        {2,	                2,	                52,	52,	52,	52,	103.947101931704,	-1.01369863013699},
        {2,	                2,	                47,	47,	47,	47,	94.4139820153774,	-0.542168674698795},
        {2,	                2,	                39,	39,	39,	39,	78.5684415016615,	0.913793103448276},
        {2,	                2,	                59,	59,	59,	59,	117.596768663089,	0.663265306122449},
        {0.0900000000000000,	0.0900000000000000,	330,330,330,330,30.0166620396073,	-30},
        {2,	                2,	                13,	13,	13,	13,	25,	                1.33333333333333},
        {2,	                2,	                10,	10,	10,	10,	20.0997512422418,	-10},
        {2,	                2,	                7,	7,	7,	7,	13.0384048104053,	-13}};



#endif // _DATA_HPP