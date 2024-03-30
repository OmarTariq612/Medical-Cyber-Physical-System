#ifndef _NODE_MODEL_CPP
#define _NODE_MODEL_CPP
#include <iostream>
#include "../inc/node_model.hpp"


// Constructor
Node::Node(
    const std::string &node_name_, 
    const std::vector<int> &node_integer_parameters, 
    const std::vector<path_terminal_pair> &path_terminal_pair, 
    const node_position &node_pos
    )
:
    _node_para{
        node_name_,                                                     // node_name
        static_cast<node_state_index_enum>(node_integer_parameters[0]), // 1 for rest, 2 for ERP, 3 for RRP
        node_integer_parameters[1],                                     // TERP_current
        node_integer_parameters[2],                                     // TERP_default
        node_integer_parameters[3],                                     // TRRP_current
        node_integer_parameters[4],                                     // TRRP_default
        node_integer_parameters[5],                                     // Trest_current
        node_integer_parameters[6],                                     // Trest_default
        static_cast<bool>(node_integer_parameters[7]),                  // activation
        node_integer_parameters[8],                                     // Terp_min
        node_integer_parameters[9],                                     // Terp_max
        node_integer_parameters[10],                                    // index_of_path_activate_the_node #TODO TOFIND
        path_terminal_pair,                                             // connected_paths
        node_pos                                                        // node_pos
    }                        
{}

// Initialize pointer to zero so that it can be initialized in first call to getInstance
NodeTable* NodeTable::instance = nullptr;

NodeTable::NodeTable(
    std::vector<std::string> node_names_,
    std::vector<std::vector<int>> node_integer_parameters_,
    std::vector<node_position> node_positions_
    )
{
    for (int i = 0; i < node_names_.size(); ++i){
        std::vector<path_terminal_pair> path_terminal_pair1 = PathTable::getInstance()->path_terminal_pairs_per_point_list[i];
        Node node1(node_names_[i], node_integer_parameters_[i], path_terminal_pair1, node_positions_[i]);
        node_table.push_back(node1);
    }
}


NodeTable *NodeTable::getInstance(){
    if (instance == nullptr){
        instance = new NodeTable(node_names, node_integer_parameters, node_positions);
    }
    return instance;
}


// Destructor
// Node::~Node() = default // TODO 
/*
// Method to simulate the node
void Node::node_automatron(node_parameters &node_para, int& path_ind, int& term_ind, path_table){
    temp_act=0;

if node_para{9} % if node is activated
    temp=node_para{10};
    switch node_para{2}
        
        case 1 %Rest
           
            % set ERP to longest
            node_para{4}=temp(2);
            node_para{3}=node_para{4}+round((rand-0.5)*0*node_para{4});
            
           
            % reset path conduction speed
            for i=1:length(path_ind)
                % if at terminal 1, only affect antegrade conduction; 2 for
                % retrograde conduction
                if term_ind(i)==1
                    path_table{path_ind(i),9}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),6});
                else
                    path_table{path_ind(i),11}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),7});
                end
            end
     
            
            % Reset Trest
            node_para{7}=round(node_para{8}*(1+(rand-0.5)*0));
            % change state to ERP
            node_para{2}=2;
        case 2 %ERP
         
            % set ERP to the lowest
            node_para{4}=temp(1);
      
            % set conduction speed to the slowest
            for i=1:length(path_ind)
                if term_ind(i)==1
                    path_table{path_ind(i),9}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),6}*(node_para{12}+1));
           
                  
                else
                    path_table{path_ind(i),11}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),7}*3);
                   
                end
            
            end
        
            % reset TERP
            node_para{3}=round((1+(rand-0.5)*0)*node_para{4});
        case 3 %RRP
            
            % calculate the ratio of early activation
            ratio=node_para{5}/node_para{6};
           
            % calculate the ERP timer for the next round
            
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            % don't get mad Paja, only AV node has different response
            % pattern. so just change the reaction function of node AV to
            % the first one
            if node_para{12}==1
                node_para{4}=temp(2)+round((1+(rand-0.5)*0)*(1-(1-ratio)^3)*(temp(1)-temp(2)));
            else
                node_para{4}=temp(1)+round((1+(rand-0.5)*0)*(1-ratio^3)*(temp(2)-temp(1)));
            end
            %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
            
            node_para{3}=round((1+(rand-0.5)*0)*node_para{4});
            
      
            % change the conduction speed of connecting path
            for i=1:length(path_ind)
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
                % same here, only AV node has faster trend
                if node_para{12}==1
                    if term_ind(i)==1
                        path_table{path_ind(i),9}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),6}*(1+ratio*3));

                    else
                        path_table{path_ind(i),11}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),7}*(1+ratio*3));

                    end
                else
                    if term_ind(i)==1
                        path_table{path_ind(i),9}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),6}*(1+ratio^2*3));

                    else
                        path_table{path_ind(i),11}=round((1+(rand-0.5)*0)*path_table{path_ind(i),12}/path_table{path_ind(i),7}*(1+ratio^2*3));

                    end
                    
                end
                %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        
              
            end
            
           
            
            % reset TRRP
            node_para{5}=round((1+(rand-0.5)*0)*node_para{6});
            % change state to ERP
            node_para{2}=2;
    end
   
else % if node is not activated
    switch node_para{2}
        case 1 %Rest
            if node_para{7}==0 % self depolarize
                % change state to ERP
                node_para{2}=2;
                % reset Trest timer
                node_para{7}=round((1+(rand-0.5)*0)*node_para{8});
                % activate the node
                temp_act=1;
            else
                % timer
                node_para{7}=node_para{7}-1;
            end
        case 2 %ERP
            if node_para{3}==0 %timer running out
                % change state to RRP
                node_para{2}=3;
                % reset TERP timer
                node_para{3}=round((1+(rand-0.5)*0)*node_para{4});
            else
                % timer
                node_para{3}=node_para{3}-1;
            end
        case 3 % RRP
            if node_para{5}==0 % timer running out
                % change state to rest
                node_para{2}=1;
                % reset TRRP timer
                node_para{5}=round((1+(rand-0.5)*0)*node_para{6});
            else
                % timer
                node_para{5}=node_para{5}-1;
            end
    end
end
%--------------------------------------
temp=[node_para(1:8),temp_act,node_para(10:12)];
%---------------------------------------
return
}
*/
#endif // _NODE_MODEL_CPP
