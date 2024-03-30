#ifndef _PATH_MODEL_CPP
#define _PATH_MODEL_CPP
#include "../inc/path_model.hpp"
#include <iostream>



// Constructor
Path::Path(
    const std::string &path_name_, 
    const std::vector<int> &path_integer_parameters, 
    const std::vector<float> &path_float_parameters
    )
:
    _path_para{
        path_name_,
        static_cast<path_state_index_enum>(path_integer_parameters[0]), // (1) Idle, (2) Antegrade conduction, (3) Retrograde, (4) Conflict, and (5) Double
        path_integer_parameters[1],                                     // entry_node_index = 0 (originally 1)
        path_integer_parameters[2],                                     // exit_node_index = 1 (originally 2)
        path_integer_parameters[3],                                     // amplitude_factor
        path_float_parameters[0],                                       // forward_speed
        path_float_parameters[1],                                       // backward_speed
        path_float_parameters[2],                                       // forward_timer_current
        path_float_parameters[3],                                       // forward_timer_default
        path_float_parameters[4],                                       // backward_timer_current
        path_float_parameters[5],                                       // backward_timer_default
        path_float_parameters[6],                                       // path_length
        path_float_parameters[7]                                        // path_slope
    }
{}

// Initialize pointer to zero so that it can be initialized in first call to getInstance
PathTable* PathTable::instance = nullptr;

PathTable::PathTable(
    std::vector<std::string> path_names_,
    std::vector<std::vector<int>> path_integer_parameters_,
    std::vector<std::vector<float>> path_float_parameters_
    )
{
    for (int i = 0; i < path_names_.size(); ++i){
        Path path1(path_names_[i], path_integer_parameters_[i], path_float_parameters_[i]);
        path_table.push_back(path1);
    }
    
    std::unordered_map<int, std::vector<path_terminal_pair>> node_idx_paths_terminals_umap;
    for (int path_idx = 0; path_idx < path_table.size(); ++path_idx){
        int entry_idx = path_table[path_idx].getParameters().entry_node_index;
        int exit_idx = path_table[path_idx].getParameters().exit_node_index;
        node_idx_paths_terminals_umap[entry_idx].push_back(path_terminal_pair{path_idx, __entry});
        node_idx_paths_terminals_umap[exit_idx].push_back(path_terminal_pair{path_idx, __exit});
    }
    int node_count = node_names.size();
    // made this instead of using the node_table.size() because the node_table is not initialized yet and it will go into a cyclic dependency problem
    for (int node_idx = 0; node_idx < node_count; ++node_idx){
        path_terminal_pairs_per_point_list.push_back(node_idx_paths_terminals_umap[node_idx]);
    }
}


PathTable *PathTable::getInstance(){
    if (instance == nullptr) {
        instance = new PathTable(path_names, path_int_parameters, path_float_parameters);
    }
    return instance;
}


#endif // _PATH_MODEL_CPP