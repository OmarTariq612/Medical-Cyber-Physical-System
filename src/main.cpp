#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../inc/GLOBAL_ENUMS.hpp"
#include "../inc/data.hpp"
#include "../inc/node_model.hpp"
#include "../inc/path_model.hpp"

int main(){
    // Test PathTable
    PathTable *path_table = PathTable::getInstance();
    std::cout << "Number of paths: " << path_table->path_table.size() << std::endl;
    std::cout << "Path name: " << 
                    path_table->instance->path_table[0].getParameters().path_name << std::endl;
    


    // Test NodeTable
    NodeTable *node_table = NodeTable::getInstance();
    std::cout << "Number of nodes: " << node_table->node_table.size() << std::endl;
    std::cout << "Node name: " << node_table->node_table[0].getParameters().node_name << std::endl;

    return 0;
}


// TODO: fill the node positions in the NodeList constructor
// TODO: implement the logic of the node and path