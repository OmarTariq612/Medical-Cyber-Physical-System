#ifndef _PATH_MODEL_HPP
#define _PATH_MODEL_HPP
#include <string>
#include <vector>
#include <unordered_map> 
#include "GLOBAL_ENUMS.hpp"
#include "data.hpp"
class NodeTable;    // Forward declaration of NodeTable to avoid circular dependency

class Path{
public:
    Path(
        const std::string &path_name,
        const std::vector<int> &path_integer_parameters,
        const std::vector<float> &path_float_parameters
    );
    // ~Path();
    // void path_automaton(NodeTable& NT);
    std::pair<bool, bool> path_automaton(NodeTable& NT);

    // Getter methods
    const path_parameters& getParameters() const { return _path_para; }
    // Setter methods
    void setParameters(const path_parameters& parameters) { _path_para = parameters; }
    void setPathName(const std::string &path_name) { _path_para.path_name = path_name; }
    void setPathStateIndex(const path_state_index_enum &path_state_index) { _path_para.path_state_index = path_state_index; }
    void setEntryNodeIndex(const int &entry_node_index) { _path_para.entry_node_index = entry_node_index; }
    void setExitNodeIndex(const int &exit_node_index) { _path_para.exit_node_index = exit_node_index; }
    void setAmplitudeFactor(const int &amplitude_factor) { _path_para.amplitude_factor = amplitude_factor; }
    void setForwardSpeed(const float &forward_speed) { _path_para.forward_speed = forward_speed; }
    void setBackwardSpeed(const float &backward_speed) { _path_para.backward_speed = backward_speed; }
    void setForwardTimerCurrent(const float &forward_timer_current) { _path_para.forward_timer_current = forward_timer_current; }
    void setForwardTimerDefault(const float &forward_timer_default) { _path_para.forward_timer_default = forward_timer_default; }
    void setBackwardTimerCurrent(const float &backward_timer_current) { _path_para.backward_timer_current = backward_timer_current; }
    void setBackwardTimerDefault(const float &backward_timer_default) { _path_para.backward_timer_default = backward_timer_default; }
    void setPathLength(const float &path_length) { _path_para.path_length = path_length; }
    void setPathSlope(const float &path_slope) { _path_para.path_slope = path_slope; }


private:
    path_parameters _path_para;
};


// root singleton class for the path table
class PathTable{
public:
    PathTable(
        const std::vector<std::string> &path_names,
        const std::vector<std::vector<int>> &path_integer_parameters,
        const std::vector<std::vector<float>> &path_float_parameters);
    // ~PathTable();
    std::vector<std::vector<path_terminal_pair>> path_terminal_pairs_per_point_list; // TODO: make this private
    std::vector<Path> path_table;       // TODO prevent direct access to this variable

    // pathe_terminal_pairs_per_point getter
    // const std::vector<std::vector<path_terminal_pair>>& getPathTerminalPairsPerPoint() const { return path_terminal_pairs_per_point_list; }
};
#endif // _PATH_MODEL_HPP
