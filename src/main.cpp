#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../inc/GLOBAL_ENUMS.hpp"
#include "../inc/heart_model.hpp"
int main(){
    // Test HeartModel
    Heart *heart_model = Heart::getInstance();
    std::cout << "Number of nodes: " << heart_model->getNodeTable().node_table.size() << std::endl;
    std::cout << "Number of paths: " << heart_model->getPathTable().path_table.size() << std::endl;
    

    return 0;
}


// TODO: exclude the path that activates the node from the list of paths that the node affects