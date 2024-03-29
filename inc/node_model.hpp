#ifndef _H_NODE_MODEL_HPP
#define _H_NODE_MODEL_HPP
#include <string>
#include <vector>
#include "../inc/GLOBAL_ENUMS.hpp"



class Node{
public:
    Node(const std::string &node_name, const std::vector<int> &node_integer_parameters, 
         const std::vector<path_terminal_pair> &path_terminal_pair,
         const node_position &node_pos)
    {}
    // ~Node();
    void node_automatron(node_parameters &node_para, int& path_ind, int& term_ind);
    // create a struct to store the parameters of the node
    node_parameters node_para;
};




// root class for the node table
class NodeTable{
public:
    NodeTable();
    ~NodeTable();
    static NodeTable* getInstance();
    std::vector<Node> node_table;
    static NodeTable* instance;
};
#endif // _H_NODE_MODEL_HPP