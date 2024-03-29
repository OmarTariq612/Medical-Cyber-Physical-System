#ifndef _H_PATH_MODEL_HPP
#define _H_PATH_MODEL_HPP
#include <string>
#include <vector>



class Path{
public:
    Path();
    ~Path();
    void path_automatron();

    std::string path_name;
    int path_state_index, entry_node_index,
        exit_node_index, amplitude_factor, forward_speed,
        backward_speed, forward_timer_current, forward_timer_default,
        backward_timer_current, backward_timer_default;
    float path_length,
        path_slope;

private:
    // create a struct to store the parameters of the node
    int path_para;
    int path_ind, term_ind;
};








#endif // _H_PATH_MODEL_HPP