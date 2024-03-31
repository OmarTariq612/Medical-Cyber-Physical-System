#ifndef _NODE_MODEL_CPP
#define _NODE_MODEL_CPP
// #include <iostream>
#include "../inc/node_model.hpp"

// Create a random number generator
std::default_random_engine generator(time(0));
// Create a uniform real distribution between 0 and 1
std::uniform_real_distribution<double> _rand(0.0, 1.0);

// Constructor
Node::Node(
    const std::string &node_name_,
    const std::vector<int> &node_integer_parameters,
    const std::vector<path_terminal_pair> &path_terminal_pair,
    const node_position &node_pos)
    : _node_para{
          node_name_,                                                     // node_name
          static_cast<node_state_index_enum>(node_integer_parameters[0]), // 1 for rest, 2 for ERP, 3 for RRP
          node_integer_parameters[1],                                     // TERP_current
          node_integer_parameters[2],                                     // TERP_default
          node_integer_parameters[3],                                     // TRRP_current
          node_integer_parameters[4],                                     // TRRP_default
          node_integer_parameters[5],                                     // Trest_current
          node_integer_parameters[6],                                     // Trest_default
          static_cast<bool>(node_integer_parameters[7]),                  // activation
          node_integer_parameters[8],                                     // Terp_min
          node_integer_parameters[9],                                     // Terp_max
          node_integer_parameters[10],                                    // index_of_path_activate_the_node
          static_cast<AVness_enum>(node_integer_parameters[11]),          // AVness (1) AV, (2) non_AV
          path_terminal_pair,                                             // connected_paths
          node_pos                                                        // node_pos
      }
{
}

// Initialize pointer to zero so that it can be initialized in first call to getInstance
NodeTable *NodeTable::instance = nullptr;

NodeTable::NodeTable(
    std::vector<std::string> node_names_,
    std::vector<std::vector<int>> node_integer_parameters_,
    std::vector<node_position> node_positions_)
{
    for (int i = 0; i < node_names_.size(); ++i)
    {
        std::vector<path_terminal_pair> path_terminal_pair1 = PathTable::getInstance()->path_terminal_pairs_per_point_list[i];
        Node node1(node_names_[i], node_integer_parameters_[i], path_terminal_pair1, node_positions_[i]);
        node_table.push_back(node1);
    }
}

NodeTable *NodeTable::getInstance()
{
    if (instance == nullptr)
    {
        instance = new NodeTable(node_names, node_integer_parameters, node_positions);
    }
    return instance;
}

// Destructor
// Node::~Node() = default // TODO

// Method to simulate the node
void Node::node_automatron()
{
    int t_activation = 0; // temporary_activation

    if (_node_para.activation == true)
    { // if node is activated
        int t_Terp_min = _node_para.Terp_min;
        int t_Terp_max = _node_para.Terp_max;

        switch (_node_para.node_state_index)
        {

        case rest: // Rest
            // set ERP to longest
            _node_para.TERP_default = t_Terp_max;
            // the diff between MatLab and C++ round() is that in C++ round() rounds to the nearest integer value "round half to even", while in MatLab it rounds towards the nearest integer value "round half away from zero"
            _node_para.TERP_current = _node_para.TERP_default + std::round((_rand(generator) - 0.5) * 0 * _node_para.TERP_default);

            // reset path conduction speed
            // for the paths connected to the node
            for (int i = 0; i < _node_para.connected_paths.size(); ++i)
            {
                int path_idx = _node_para.connected_paths[i].path_idx;
                const path_parameters &path_parameters = PathTable::getInstance()->path_table[path_idx].getParameters();
                float original_forward_speed = path_parameters.forward_speed;   // path_table{path_idx, 6}
                float original_backward_speed = path_parameters.backward_speed; // path_table{path_idx, 7}
                float original_path_length = path_parameters.path_length;       // path_table{path_idx, 12}
                // if at __entry, only affect antegrade conduction; __exit for retrograde conduction
                if (_node_para.connected_paths[i].terminal == __entry)
                { // TODO: investigate the multiplication by 0
                    float tmp_forward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_forward_speed);
                    PathTable::getInstance()->path_table[path_idx].setForwardTimerDefault(tmp_forward_timer_default);
                }
                else
                {
                    float tmp_backward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_backward_speed);
                    PathTable::getInstance()->path_table[path_idx].setBackwardTimerDefault(tmp_backward_timer_default);
                }
            }

            // Reset Trest
            _node_para.Trest_current = std::round(_node_para.Trest_default * (1 + (_rand(generator) - 0.5) * 0));
            // change state to ERP
            _node_para.node_state_index = ERP;

            break;
        case ERP: // ERP

            // set ERP to the lowest
            _node_para.TERP_default = t_Terp_min;

            // set conduction speed to the slowest
            for (int idx = 0; idx < _node_para.connected_paths.size(); ++idx)
            {
                int path_idx = _node_para.connected_paths[idx].path_idx;
                const path_parameters &path_parameters = PathTable::getInstance()->path_table[path_idx].getParameters();
                float original_forward_speed = path_parameters.forward_speed;   // path_table{path_idx, 6}
                float original_backward_speed = path_parameters.backward_speed; // path_table{path_idx, 7}
                float original_path_length = path_parameters.path_length;       // path_table{path_idx, 12}
                // if at __entry, only affect antegrade conduction; __exit for retrograde conduction
                if (_node_para.connected_paths[idx].terminal == __entry)
                { // TODO: investigate the multiplication by 0
                    float tmp_forward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_forward_speed * (_node_para.AVness + 1));
                    PathTable::getInstance()->path_table[path_idx].setForwardTimerDefault(tmp_forward_timer_default);
                }
                else
                {
                    float tmp_backward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_backward_speed * 3);
                    PathTable::getInstance()->path_table[path_idx].setBackwardTimerDefault(tmp_backward_timer_default);
                }
            }

            // reset TERP
            _node_para.TERP_current = std::round((1 + (_rand(generator) - 0.5) * 0) * _node_para.TERP_default);
            break;
        case RRP: // RRP

            // calculate the ratio of early activation
            float ratio = _node_para.TRRP_current / _node_para.TRRP_default;
            // calculate the ERP timer for the next round

            //////////////////////////////////////////////////////////////
            // don't get mad Paja, only AV node has different response
            // pattern. so just change the reaction function of node AV to
            // the first one. COMMENT BY COMMIT OWNER: https://github.com/zhihaoj
            if (_node_para.AVness == AV)
                _node_para.TERP_default = t_Terp_max + std::round((1 + (_rand(generator) - 0.5) * 0) * (1 - (1 - ratio) * (1 - ratio) * (1 - ratio)) * (t_Terp_min - t_Terp_max));
            else
                _node_para.TERP_default = t_Terp_min + std::round((1 + (_rand(generator) - 0.5) * 0) * (1 - (ratio * ratio * ratio)) * (t_Terp_max - t_Terp_min));
            ///////////////////////////////////////////////////////////////////////////////////////////

            _node_para.TERP_current = std::round((1 + (_rand(generator) - 0.5) * 0) * _node_para.TERP_default);

            // change the conduction speed of connecting path
            for (int idx = 0; idx < _node_para.connected_paths.size(); ++idx)
            {
                /////////////////////////////////////////////////
                // same here, only AV node has faster trend
                int path_idx = _node_para.connected_paths[idx].path_idx;
                const path_parameters &path_parameters = PathTable::getInstance()->path_table[path_idx].getParameters();
                float original_path_length = path_parameters.path_length;       // path_table{path_idx, 12}
                float original_forward_speed = path_parameters.forward_speed;   // path_table{path_idx, 6}
                float original_backward_speed = path_parameters.backward_speed; // path_table{path_idx, 7}
                if (_node_para.AVness == AV)
                {
                    if (_node_para.connected_paths[idx].terminal == __entry)
                    {
                        float tmp_forward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_forward_speed * (1 + ratio * 3));
                        PathTable::getInstance()->path_table[path_idx].setForwardTimerDefault(tmp_forward_timer_default);
                    }
                    else
                    {
                        float tmp_backward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_backward_speed * (1 + ratio * 3));
                        PathTable::getInstance()->path_table[path_idx].setBackwardTimerDefault(tmp_backward_timer_default);
                    }
                }
                else
                {
                    if (_node_para.connected_paths[idx].terminal == __entry)
                    {
                        float tmp_forward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_forward_speed * (1 + ratio * ratio * 3));
                        PathTable::getInstance()->path_table[path_idx].setForwardTimerDefault(tmp_forward_timer_default);
                    }
                    else
                    {
                        float tmp_backward_timer_default = std::round((1 + (_rand(generator) - 0.5) * 0) * original_path_length / original_backward_speed * (1 + ratio * ratio * 3));
                        PathTable::getInstance()->path_table[path_idx].setBackwardTimerDefault(tmp_backward_timer_default);
                    }
                }
            }

            // reset TRRP
            _node_para.TRRP_current = std::round((1 + (_rand(generator) - 0.5) * 0) * _node_para.TRRP_default);
            // change state to ERP
            _node_para.node_state_index = ERP;
            break;
        }
    }
    else
    { // if node is not activated
        switch (_node_para.node_state_index)
        {
        case rest: // Rest
            if (_node_para.Trest_current == 0)
            { // self depolarize
                // change state to ERP
                _node_para.node_state_index = ERP;
                // reset Trest timer
                _node_para.Trest_current = std::round((1 + (_rand(generator) - 0.5) * 0) * _node_para.Trest_default);
                // activate the node
                t_activation = 1;
            }
            else // timer
                _node_para.Trest_current--;
            break;
        case ERP: // ERP
            if (_node_para.TERP_current == 0)
            { // timer running out
                // change state to RRP
                _node_para.node_state_index = RRP;
                // reset TERP timer
                _node_para.TERP_current = round((1 + (_rand(generator) - 0.5) * 0) * _node_para.TERP_default);
            }
            else // timer
                _node_para.TERP_current--;
            break;
        case RRP: // RRP
            if (_node_para.TRRP_current == 0)
            { // timer running out
                // change state to rest
                _node_para.node_state_index = rest;
                // reset TRRP timer
                _node_para.TRRP_current = round((1 + (_rand(generator) - 0.5) * 0) * _node_para.TRRP_default);
            }
            else // timer
                _node_para.Trest_current--;
            break;
        }
    }
    //--------------------------------------
    _node_para.activation = t_activation;
    //---------------------------------------
    return;
}

#endif // _NODE_MODEL_CPP
