#include "../inc/heart_model.hpp"
#include "heart_model.hpp"


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
    NodeTable NT_editable_copy = m_node_table;
    // PathTable temp_path = m_path_table;
    
    //**************************************************
    // a temp path table that can be updated by node automata
    PathTable temp_path_node = m_path_table;
    //***************************************************
    std::vector<bool> temp_act(m_node_table.node_table.size());

    for (int i = 0; i < m_node_table.node_table.size(); ++i) {
        //---------------------------------
        // update parameters for each node
        // [m_node_table(i,:),temp_path_node]=node_automatron(NT_editable_copy(i,:),path_ind,term_ind,temp_path_node);
        Node &node = m_node_table.node_table[i];
        node.setIndex_of_path_activate_the_node(-1);
        node.node_automaton(temp_path_node);
        // create local variables for node activation signals
        // for ease of use and manipulation
        // temp_act[i] = m_node_table{i,9};
        temp_act[i] = node.getParameters().activation;
    }
    
    
    for (int i = 0; i < m_path_table.path_table.size(); ++i) {
        // update parameters for each path
        // [m_path_table(i,:),node_act_1,node_act_2]=path_automatron(path_table(i,:),NT_editable_copy{path_table{i,3},9},NT_editable_copy{path_table{i,4},9});
        Path &path = m_path_table.path_table[i];
        path.path_automaton(NT_editable_copy);
        // update the local node activation signals of the two nodes
        // connecting to the path by using "OR" operation
        // if NT_editable_copy{path_table{i,3},2}~=2{
        const int entry_index = path.getParameters().entry_node_index;
        if(NT_editable_copy.node_table[entry_index].getParameters().node_state_index != ERP){
            // temp_act{path_table{i,3}}=temp_act{path_table{i,3}} || node_act_1;
            temp_act[entry_index] = temp_act[entry_index] || NT_editable_copy.node_table[entry_index].getParameters().activation;
            //-------------------------------------
            // store the path that activated the node
            // if node_act_1==1
                // m_node_table{path_table{i,3},11}=i;
            if(NT_editable_copy.node_table[entry_index].getParameters().activation == true)
                m_node_table.node_table[entry_index].setIndex_of_path_activate_the_node(i);
                //-------------------------------------
        }else{
            // temp_act{path_table{i,3}}=false;
            temp_act[entry_index] = false;
            // NT_editable_copy{path_table{i,3},3}=NT_editable_copy{path_table{i,3},4};
            NT_editable_copy.node_table[entry_index].setTERP_current(
                NT_editable_copy.node_table[entry_index].getParameters().TERP_default);
        }
        const int exit_index = path.getParameters().exit_node_index;
        // if NT_editable_copy{path_table{i,4},2}~=2{
        if (NT_editable_copy.node_table[exit_index].getParameters().node_state_index != ERP){
            // temp_act{path_table{i,4}}=temp_act{path_table{i,4}} || node_act_2;
            temp_act[exit_index] = temp_act[exit_index] || NT_editable_copy.node_table[exit_index].getParameters().activation;
            //-------------------------------------
            // store the path that activated the node
            // if node_act_2==1
            if(NT_editable_copy.node_table[exit_index].getParameters().activation == true)
                // m_node_table{path_table{i,4},11}=i;
                m_node_table.node_table[exit_index]
                .setIndex_of_path_activate_the_node(i);
            //-------------------------------------
        }
        else{
            // temp_act{path_table{i,4}}=false;
            temp_act[exit_index] = false;
            // NT_editable_copy{path_table{i,4},3}=NT_editable_copy{path_table{i,4},4};
            NT_editable_copy.node_table[exit_index].setTERP_current(
                NT_editable_copy.node_table[exit_index].getParameters().TERP_default);
        }
    }
       // update the parameters to global variables
    //    NT_editable_copy=[m_node_table(:,1:8),temp_act',m_node_table(:,10:12)]; // TODO Uncomment this line
    for (int i = 0; i < m_node_table.node_table.size(); ++i) {
        m_node_table.node_table[i].setActivation(temp_act[i]);
    }
        // find changes in the default antegrade conduction of the path table

        // ind = find(cell2mat(temp_path_node( :, 9)) ~ = cell2mat(m_path_table( :, 9)));
    for (int i = 0; i < m_path_table.path_table.size(); ++i){
        if (temp_path_node.path_table[i].getParameters().forward_timer_default !=
              m_path_table.path_table[i].getParameters().forward_timer_default){
            std::cout << "tmp_path doesnt equal tmp_path_node" << std::endl;
            m_path_table.path_table[i].setForwardTimerDefault(
                temp_path_node.path_table[i].getParameters().forward_timer_default);
            if (temp_path_node.path_table[i].getParameters().path_state_index == Idle){
                m_path_table.path_table[i].setForwardTimerCurrent(
                    m_path_table.path_table[i].getParameters().forward_timer_default);
            }
        }
        if(temp_path_node.path_table[i].getParameters().backward_timer_default !=
            m_path_table.path_table[i].getParameters().backward_timer_default){
            m_path_table.path_table[i].setBackwardTimerDefault(
                temp_path_node.path_table[i].getParameters().backward_timer_default);
            if (temp_path_node.path_table[i].getParameters().path_state_index == Idle){
                m_path_table.path_table[i].setBackwardTimerCurrent(
                    m_path_table.path_table[i].getParameters().backward_timer_default);
            }
        }
    }

    //    for i=1:length(ind)
    //        // update the value
    //       m_path_table{ind(i),9}=temp_path_node{ind(i),9};
    //       // if the path is still in idle state, also update the current
    //       // value
    //       if temp_path_node{ind(i),2}==1
    //           m_path_table{ind(i),8}=m_path_table{ind(i),9};
    //       end
    //    end
       // find changes in the default retrograde conduction of the path table
    //    ind=find(cell2mat(temp_path_node(:,11))~=cell2mat(m_path_table(:,11)));
    //    for i=1:length(ind)
    //       m_path_table{ind(i),11}=temp_path_node{ind(i),11};
    //       if temp_path_node{ind(i),2}==1
    //           m_path_table{ind(i),10}=m_path_table{ind(i),11};
    //       end
    //    end
    //    path_table=temp_path;  // on finish, this will be redundant
    return;
}