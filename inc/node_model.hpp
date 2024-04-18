#ifndef _NODE_MODEL_HPP
#define _NODE_MODEL_HPP
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "GLOBAL_ENUMS.hpp"
#include "data.hpp"
class PathTable;  // Forward declaration of PathTable to avoid circular
                  // dependency

class Node {
 public:
  Node(const std::string &node_name,
       const std::vector<int> &node_integer_parameters,
       const std::vector<path_terminal_pair> &path_terminal_pair,
       const node_position &node_pos);
  // ~Node();
  void node_automaton(PathTable &PT);

  // Getter methods
  const node_parameters &getParameters() const { return _node_para; }
  // Setter methods
  void setParameters(const node_parameters &parameters) {
    _node_para = parameters;
  }
  void setNodeStateIndex(const node_state_index_enum &node_state_index) {
    _node_para.node_state_index = node_state_index;
  }
  void setTERP_current(const int &TERP_current) {
    _node_para.TERP_current = TERP_current;
  }
  void setTERP_default(const int &TERP_default) {
    _node_para.TERP_default = TERP_default;
  }
  void setTRRP_current(const int &TRRP_current) {
    _node_para.TRRP_current = TRRP_current;
  }
  void setTRRP_default(const int &TRRP_default) {
    _node_para.TRRP_default = TRRP_default;
  }
  void setTrest_current(const int &Trest_current) {
    _node_para.Trest_current = Trest_current;
  }
  void setTrest_default(const int &Trest_default) {
    _node_para.Trest_default = Trest_default;
  }
  void setActivation(const bool &activation) {
    _node_para.activation = activation;
  }
  void setTerp_min(const int &Terp_min) { _node_para.Terp_min = Terp_min; }
  void setTerp_max(const int &Terp_max) { _node_para.Terp_max = Terp_max; }
  void setIndex_of_path_activate_the_node(
      const int &index_of_path_activate_the_node) {
    _node_para.index_of_path_activate_the_node =
        index_of_path_activate_the_node;
  }
  void setAVness(const AVness_enum &AVness) { _node_para.AVness = AVness; }
  void setConnectedPaths(
      const std::vector<path_terminal_pair> &connected_paths) {
    _node_para.connected_paths = connected_paths;
  }
  void setNodePos(const node_position &node_pos) {
    _node_para.node_pos = node_pos;
  }

 private:
  node_parameters _node_para;
};

// root singleton class for the node table
class NodeTable {
 public:
  NodeTable(const std::vector<std::string> &node_names_,
            const std::vector<std::vector<int>> &node_int_parameters_,
            const std::vector<node_position> &node_positions_,
            const PathTable &PathTable);
  // ~NodeTable();
  std::vector<Node> node_table;
};

#endif  // _NODE_MODEL_HPP