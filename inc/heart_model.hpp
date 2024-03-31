#ifndef _HEART_MODEL_HPP
#define _HEART_MODEL_HPP


#include "path_model.hpp"
#include "node_model.hpp"
#include <vector>
#include <string>
#include "GLOBAL_ENUMS.hpp"
#include "data.hpp"

class Heart{
private:
    Heart(
        const std::vector<std::string> &h_node_names,
        const std::vector<std::vector<int>> &h_node_int_parameters,
        const std::vector<node_position> &h_node_positions,
        const std::vector<std::string> &h_path_names,
        const std::vector<std::vector<int>> &h_path_integer_parameters,
        const std::vector<std::vector<float>> &h_path_float_parameters
    );
    NodeTable m_node_table;
    PathTable m_path_table;
public:
    static Heart *getInstance();    
    // void heart_automaton();
    static Heart *instance;
    // getter methods
    const NodeTable& getNodeTable() const { return m_node_table; }
    const PathTable& getPathTable() const { return m_path_table; }
    // setter methods
    NodeTable &setNodeTable() { return m_node_table; }
    PathTable &setPathTable() { return m_path_table; }
};







#endif // _HEART_MODEL_HPP