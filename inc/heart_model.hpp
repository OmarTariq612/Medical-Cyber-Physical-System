#ifndef _HEART_MODEL_HPP
#define _HEART_MODEL_HPP

#include <string>
#include <vector>

#include "GLOBAL_ENUMS.hpp"
#include "ep_avnrt_data_gen.hpp"
#include "node_model.hpp"
#include "path_model.hpp"

class Heart {
 private:
  Heart(const std::vector<std::string> &h_node_names,
        const std::vector<std::vector<int>> &h_node_int_parameters,
        const std::vector<position> &h_node_positions,
        const std::vector<std::string> &h_path_names,
        const std::vector<std::vector<int>> &h_path_integer_parameters,
        const std::vector<std::vector<float>> &h_path_float_parameters);
  PathTable m_path_table;
  // It is important to initialize the NodeTable first because the PathTable
  // depends on it. The initialization occurs in the order they are declared in
  // the class, not the initialization list order.
  NodeTable m_node_table;
  static Heart *instance;

 public:
  static Heart *getInstance();
  void heart_automaton();
  // getter methods
  const NodeTable &getNodeTable() const { return m_node_table; }
  const PathTable &getPathTable() const { return m_path_table; }
  // setter methods
  NodeTable &setNodeTable() { return m_node_table; }
  PathTable &setPathTable() { return m_path_table; }
};

#endif  // _HEART_MODEL_HPP
