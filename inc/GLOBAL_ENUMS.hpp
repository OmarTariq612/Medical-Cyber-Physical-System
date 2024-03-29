#ifndef _H_GLOBAL_ENUMS_HPP
#define _H_GLOBAL_ENUMS_HPP
#include <string>
#include <vector>


struct node_position
{
    float x, y;
};

enum node_state_index_enum{
    rest = 1,
    ERP = 2,
    RRP = 3
};

// a struct that has path_idx, terminal pair for each path connected to the node
struct path_terminal_pair{
    int path_idx;
    bool terminal;
};


struct node_parameters{
        std::string node_name;
        node_state_index_enum node_state_index = rest; // 1 for rest, 2 for ERP, 3 for RRP
        
        // The total refractory period is comprised of the (1) Rset, (2) Conduction, (3) ERP, (4) RRP, and (5) Trest periods. Starting or Ending with a Rest period. 
        int TERP_current; 
        int TERP_default;   // The ERP (Effective Refractory Period) refers to the period during which cardiac cells are unable to respond to a new electrical stimulus, regardless of its strength.
        int TRRP_current; 
        int TRRP_default;   // The RRP (Relative Refractory Period) follows the ERP and represents a phase during which cardiac cells have partially recovered their excitability. While the cells are still in a refractory state, they are more responsive to strong stimuli compared to during the ERP.RRP is shorter than that of the ERP.
        int Trest_current; 
        int Trest_default;  // The Trest (Rest) period is the period during which the cell is not excitable and is not responsive to any stimulus.
        bool activation;    // 1 for activated, 0 for not activated
        int Terp_min;       // The minimum effective refractory period
        int Terp_max;       // The maximum effective refractory period
        int index_of_path_activate_the_node;    // The index of the path that activates the node [Haven't found the use of this parameter yet] #TODO TOFIND
        std::vector<path_terminal_pair> connected_paths; // The paths that are connected to the node
        node_position node_pos; // The position of the node in the grid
    };

#endif // _H_GLOBAL_ENUMS_HPP