#ifndef _NODE_MODEL_HPP
#define _NODE_MODEL_HPP
#include <string>
#include <vector>
#include "GLOBAL_ENUMS.hpp"
#include "data.hpp"
#include "path_model.hpp"


class Node{
public:
    Node(const std::string &node_name,
         const std::vector<int> &node_integer_parameters,
         const std::vector<path_terminal_pair> &path_terminal_pair,
         const node_position &node_pos);
    // ~Node();
    // void node_automatron(node_parameters &node_para, int& path_ind, int& term_ind);

    // Getter methods
    const node_parameters& getParameters() const { return _node_para; }
    // Setter methods
    void setParameters(const node_parameters& parameters) { _node_para = parameters; }
private:
    node_parameters _node_para;
};


// root singleton class for the node table
class NodeTable{
private:
    NodeTable(
        std::vector<std::string> node_names,
        std::vector<std::vector<int>> node_int_parameters,
        std::vector<node_position> node_positions
    );
public:
    // ~NodeTable();
    static NodeTable *getInstance();
    std::vector<Node> node_table;
    static NodeTable* instance;

};

#endif // _NODE_MODEL_HPP