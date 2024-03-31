#ifndef _HEART_MODEL_HPP
#define _HEART_MODEL_HPP


#include "path_model.hpp"
#include "node_model.hpp"

class Heart{
private:
    Heart();
    NodeTable *node_table;
    PathTable *path_table;
public:
    static Heart *getInstance();
    void heart_automatron();
};







#endif // _HEART_MODEL_HPP