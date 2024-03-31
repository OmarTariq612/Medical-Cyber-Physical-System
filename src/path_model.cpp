#ifndef _PATH_MODEL_CPP
#define _PATH_MODEL_CPP
#include "../inc/path_model.hpp"
#include "../inc/node_model.hpp"

// Constructor
Path::Path(
    const std::string &path_name_,
    const std::vector<int> &path_integer_parameters,
    const std::vector<float> &path_float_parameters)
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
{
}

PathTable::PathTable(
    const std::vector<std::string> &path_names_,
    const std::vector<std::vector<int>> &path_integer_parameters_,
    const std::vector<std::vector<float>> &path_float_parameters_)
{
    for (int i = 0; i < path_names_.size(); ++i)
    {
        Path path1(path_names_[i], path_integer_parameters_[i], path_float_parameters_[i]);
        path_table.push_back(path1);
    }

    std::unordered_map<int, std::vector<path_terminal_pair>> node_idx_paths_terminals_umap;
    for (int path_idx = 0; path_idx < path_table.size(); ++path_idx)
    {
        int entry_idx = path_table[path_idx].getParameters().entry_node_index;
        int exit_idx = path_table[path_idx].getParameters().exit_node_index;
        node_idx_paths_terminals_umap[entry_idx].push_back(path_terminal_pair{path_idx, __entry});
        node_idx_paths_terminals_umap[exit_idx].push_back(path_terminal_pair{path_idx, __exit});
    }
    int node_count = Data.node_names.size();
    // made this instead of using the node_table.size() because the node_table is not initialized yet and it will go into a cyclic dependency problem
    for (int node_idx = 0; node_idx < node_count; ++node_idx)
    {
        path_terminal_pairs_per_point_list.push_back(node_idx_paths_terminals_umap[node_idx]);
    }
}

// Path automaton
void Path::path_automaton(NodeTable& NT){
    bool temp_node1_activation = false;
    bool temp_node2_activation = false;
    const int entry_node_index = _path_para.entry_node_index;
    const int exit_node_index = _path_para.exit_node_index;
    Node &entry_node = NT.node_table[entry_node_index];
    Node &exit_node = NT.node_table[exit_node_index];
    switch (_path_para.path_state_index)
    {
    case Idle: // Idle
        // if activation coming from entry node
        if (entry_node.getParameters().activation == true)
            // Antegrade conduction
            _path_para.path_state_index = Antegrade_conduction;
        // if activation coming from exit node
        else if (exit_node.getParameters().activation == true)
            // Retrograde conduction
            _path_para.path_state_index = Retrograde;
        break;
    case Antegrade_conduction: // Antegrade conduction
        // if activation coming from exit node
        if (exit_node.getParameters().activation == true)
        { // double
            _path_para.path_state_index = Double;
            // // reset timer
            // _path_para.forward_timer_current = _path_para.forward_timer_default;
        }
        else
        {
            // if timer running out
            if (_path_para.forward_timer_current == 0)
            {
                // reset timer
                _path_para.forward_timer_current = _path_para.forward_timer_default;
                // activate exit node
                temp_node2_activation = true;
                // go to conflict state
                _path_para.path_state_index = Conflict;
            }
            else // timer
                _path_para.forward_timer_current--;
        }
        break;
    case Retrograde: // Retro
        // if activation coming from entry node
        if (entry_node.getParameters().activation == true)
        { // conflict
            // go to Double state
            _path_para.path_state_index = Double;
            // // reset timer
            // _path_para.backward_timer_current = _path_para.backward_timer_default;
        }
        else
        {
            // if timer runs out
            if (_path_para.backward_timer_current == 0)
            {
                // reset timer
                _path_para.backward_timer_current = _path_para.backward_timer_default;
                // activate the entry node
                temp_node1_activation = true;
                // change state to conflict
                _path_para.path_state_index = Conflict;
            }
            else // timer
                _path_para.backward_timer_current--;
        }
        break;
    case Conflict: // Conflict
        // go to Idle state
        _path_para.path_state_index = Idle;
        break;
    case Double: // double
        if (_path_para.backward_timer_current == 0)
        {
            // reset timer
            _path_para.backward_timer_current = _path_para.backward_timer_default;
            // activate the entry node
            temp_node1_activation = true;
            // change state to conflict
            // path_para{2}=2; TODO: check this
            _path_para.path_state_index = Conflict;
            // Update the path nodes activation before returning
            entry_node.setActivation(temp_node1_activation);
            exit_node.setActivation(temp_node2_activation);
            return;
        }
        if (_path_para.forward_timer_current == 0)
        {
            // reset timer
            _path_para.forward_timer_current = _path_para.forward_timer_default;
            // activate exit node
            temp_node2_activation = true;
            // go to conflict state
            // path_para{2}=3; TODO: check this too
            _path_para.path_state_index = Conflict;
            // Update the path nodes activation before returning
            entry_node.setActivation(temp_node1_activation);
            exit_node.setActivation(temp_node2_activation);
            return;
        }
        if (std::abs(1 - _path_para.forward_timer_current / _path_para.forward_timer_default - _path_para.backward_timer_current / _path_para.backward_timer_default) < (0.9 / std::min(_path_para.forward_timer_default, _path_para.backward_timer_default)))
        {
            _path_para.backward_timer_current = _path_para.backward_timer_default;
            _path_para.forward_timer_current = _path_para.forward_timer_default;
            _path_para.path_state_index = Conflict;
        }
        else
        {
            _path_para.forward_timer_current--;
            _path_para.backward_timer_current--;
        }
        break;
    }
    return;
}

#endif // _PATH_MODEL_CPP