#include <iostream>
// #include <memory>
// #include <string>
// #include <vector>
#include "../inc/heart_model.hpp"
int main(){
    // Test HeartModel
    Heart *heart_model = Heart::getInstance();
    std::cout << "Number of nodes: " << heart_model->getNodeTable().node_table.size() << std::endl;
    std::cout << "Number of paths: " << heart_model->getPathTable().path_table.size() << std::endl;
    heart_model->heart_automaton();
    std::cout << "Number of nodes: " << heart_model->getNodeTable().node_table.size() << std::endl;
    std::cout << "Number of paths: " << heart_model->getPathTable().path_table.size() << std::endl;
    
    return 0;
}


// TODO: In the heart automatro, do it in the easy way and make copies of the node_table and path_table and then update the original tables then optimize it later by deviding the tmp tables into In-Object state duplications for previous state