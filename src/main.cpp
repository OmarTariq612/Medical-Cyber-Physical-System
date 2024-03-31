#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../inc/GLOBAL_ENUMS.hpp"
#include "../inc/data.hpp"
// #include "../inc/node_model.hpp"
#include "../inc/path_model.hpp"

int main(){
    // Test PathTable
    std::unique_ptr<PathTable> path_table2 = std::make_unique<PathTable>(
        path_names,
        path_int_parameters,
        path_float_parameters);
    std::cout << "Number of paths: " << path_table2->path_table.size() << std::endl;
    std::cout << "Path name: " << 
                    path_table2->path_table[0].getParameters().path_name << std::endl;

    // Test NodeTable
    // NodeTable *node_table = NodeTable::getInstance();
    // std::cout << "Number of nodes: " << node_table->node_table.size() << std::endl;
    // std::cout << "Node name: " << node_table->node_table[0].getParameters().node_name << std::endl;

    return 0;
}


// TODO: implement the logic of the node and path
// TODO: exclude the path that activates the node from the list of paths that the node affects