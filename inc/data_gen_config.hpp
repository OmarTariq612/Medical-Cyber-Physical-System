#ifndef _DATA_GEN_CONFIG_HPP
#define _DATA_GEN_CONFIG_HPP

#define CASE 1
#if CASE == 1
#include "../inc/aflutter_data_gen.hpp"
#define Data AflutterData
#elif CASE == 2
#include "../inc/ep_avnrt_data_gen.hpp"
#define Data Ep_avnrtData
#elif CASE == 3
#include "../inc/wenckbach_data_gen.hpp"
#define Data WenckbachData
#else
#error "No case selected"
#endif

#endif  // DATA_GEN_CONFIG_HPP
