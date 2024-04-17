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


void Heart::heart_automaton()
{
// function [node_table,path_table]=heart_model(node_table,path_table)
// The function update the parameters for nodes and paths in one time stamp
//
// Inputs:
// node_table: Cell array, each row contains parameters for one node
//
//    format: {'node name', node_state_index, TERP_current,
//            TERP_default, TRRP_current, TRRP_default, Trest_current,
//            Trest_default, node_activation,[Terp_min,Terp_max],index_of_path_activate_the_node} 
//
// path_para: Cell array, each row contains parameters for one path
//
//    format: {'path_name',path_state_index, entry_node_index,
//            exit_node_index, amplitude_factor, forward_speed,
//            backward_speed, forward_timer_current, forward_timer_default,
//            backward_timer_current, backward_timer_default, path_length,
//            path_slope}

/*
    in the original NT_editable_copy state we:
    1. get node activation signals
    2. get node state index
    3. reset TERP_current to TERP_default


    m_node_table represents the updated node table which is the actual updated nodes
    NT_editable_copy is a local copy of the node table editable by the node automaton
    
    path_table is the original copy of the path table that we don't want to change
    temp_path is the updated path table which is the actual updated paths
    
    
    path_table is just used to get the entry and exit node indexes

    temp_path is updated by the path automaton
    temp_path_node is updated by the node automaton
    */

    // local temp node & path table

    NodeTable temp_node = m_node_table;
    PathTable temp_path = m_path_table;
    PathTable path_table = m_path_table;
    NodeTable node_table = m_node_table;
    // PathTable temp_path = m_path_table;

    //**************************************************
    // a temp path table that can be updated by node automata
    PathTable temp_path_node = m_path_table;
    //***************************************************
    std::vector<bool> temp_act(m_node_table.node_table.size());

    for (int i = 0; i < m_node_table.node_table.size(); ++i) {
        Node &temp_node_elem = temp_node.node_table[i];
        temp_node_elem.setIndex_of_path_activate_the_node(-1);
        temp_node_elem.node_automaton(temp_path_node);
        temp_act[i] = temp_node_elem.getParameters().activation;
    }


    for (int i = 0; i < m_path_table.path_table.size(); ++i) {
        // update parameters for each path
        // [m_path_table(i,:),node_act_1,node_act_2]=path_automatron(path_table(i,:),NT_editable_copy{path_table{i,3},9},NT_editable_copy{path_table{i,4},9});
        auto [node_act_1, node_act_2] = temp_path.path_table[i].path_automaton(node_table);
        const Path& path = path_table.path_table[i];
        const int entry_index = path.getParameters().entry_node_index;
        if (node_table.node_table[entry_index].getParameters().node_state_index != ERP){
            temp_act[entry_index] = temp_act[entry_index] || node_act_1;
            if (node_act_1 == true)
                temp_node.node_table[entry_index].setIndex_of_path_activate_the_node(i);
        }
        else{
            temp_act[entry_index] = false;
            node_table.node_table[entry_index].setTERP_current(
                node_table.node_table[entry_index].getParameters().TERP_default);
        }
        const int exit_index = path.getParameters().exit_node_index;
        if (node_table.node_table[exit_index].getParameters().node_state_index != ERP){
            temp_act[exit_index] = temp_act[exit_index] || node_act_2;
            if (node_act_2 == true)
                temp_node.node_table[exit_index].setIndex_of_path_activate_the_node(i);
        }
        else{
            temp_act[exit_index] = false;
            node_table.node_table[exit_index].setTERP_current(
                node_table.node_table[exit_index].getParameters().TERP_default);
        }

    }
       // update the parameters to global variables
    //    NT_editable_copy=[m_node_table(:,1:8),temp_act',m_node_table(:,10:12)]; // TODO Uncomment this line
    for (int i = 0; i < m_node_table.node_table.size(); ++i) {
        temp_node.node_table[i].setActivation(temp_act[i]);
    }
    // m_node_table.node_table = temp_node.node_table;
    m_node_table = temp_node;
    

        // find changes in the default antegrade conduction of the path table

        // ind = find(cell2mat(temp_path_node( :, 9)) ~ = cell2mat(m_path_table( :, 9)));
    for (int i = 0; i < temp_path.path_table.size(); ++i){

        // std::cout << temp_path_node.path_table[i].getParameters().forward_timer_default << " " << temp_path.path_table[i].getParameters().forward_timer_default << " ";

        if (temp_path_node.path_table[i].getParameters().forward_timer_default !=
            temp_path.path_table[i].getParameters().forward_timer_default){
            std::cout << "tmp_path doesnt equal tmp_path_node 1" << std::endl;
            temp_path.path_table[i].setForwardTimerDefault(
                temp_path_node.path_table[i].getParameters().forward_timer_default);
            if (temp_path_node.path_table[i].getParameters().path_state_index == Idle){
                temp_path.path_table[i].setForwardTimerCurrent(
                    temp_path.path_table[i].getParameters().forward_timer_default);
            }
        }
        if(temp_path.path_table[i].getParameters().backward_timer_default !=
            m_path_table.path_table[i].getParameters().backward_timer_default){
            std::cout << "tmp_path doesnt equal tmp_path_node 2" << std::endl;
            m_path_table.path_table[i].setBackwardTimerDefault(
                temp_path.path_table[i].getParameters().backward_timer_default);
            if (temp_path.path_table[i].getParameters().path_state_index == Idle){
                m_path_table.path_table[i].setBackwardTimerCurrent(
                    m_path_table.path_table[i].getParameters().backward_timer_default);
            }
        }
    }
    // std::cout << std::endl;
    m_path_table = temp_path;
    // std::cout << temp_node.node_table[0].getParameters().node_state_index << std::endl;
    // for (int j = 0; j < temp_node.node_table.size(); ++j){
    //     std::cout << temp_node.node_table[j].getParameters().activation << " ";
    // }
    // std::cout << std::endl;
    return;
}
