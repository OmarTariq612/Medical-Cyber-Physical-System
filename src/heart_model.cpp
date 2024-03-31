#include "../inc/heart_model.hpp"


Heart::Heart(
    const std::vector<std::string> &h_node_names,
    const std::vector<std::vector<int>> &h_node_int_parameters,
    const std::vector<node_position> &h_node_positions,
    const std::vector<std::string> &h_path_names,
    const std::vector<std::vector<int>> &h_path_integer_parameters,
    const std::vector<std::vector<float>> &h_path_float_parameters
)
    : m_path_table(
        h_path_names,
        h_path_integer_parameters,
        h_path_float_parameters
    ),
    m_node_table(
        h_node_names,
        h_node_int_parameters,
        h_node_positions,
        m_path_table
    )
{
}

// initialize the instance to nullptr
Heart *Heart::instance = nullptr;


Heart *Heart::getInstance(){
    if(instance == nullptr){
        instance = new Heart(
            Data.node_names,
            Data.node_int_parameters,
            Data.node_positions,
            Data.path_names,
            Data.path_int_parameters,
            Data.path_float_parameters);
    }
    return instance;
}