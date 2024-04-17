#include "../inc/heart_model.hpp"

Heart::Heart(const std::vector<std::string> &h_node_names,
             const std::vector<std::vector<int>> &h_node_int_parameters,
             const std::vector<node_position> &h_node_positions,
             const std::vector<std::string> &h_path_names,
             const std::vector<std::vector<int>> &h_path_integer_parameters,
             const std::vector<std::vector<float>> &h_path_float_parameters)
    : m_path_table(h_path_names, h_path_integer_parameters,
                   h_path_float_parameters),
      m_node_table(h_node_names, h_node_int_parameters, h_node_positions,
                   m_path_table) {}

// initialize the instance to nullptr
Heart *Heart::instance = nullptr;

Heart *Heart::getInstance() {
  if (instance == nullptr) {
    instance = new Heart(Data.node_names, Data.node_int_parameters,
                         Data.node_positions, Data.path_names,
                         Data.path_int_parameters, Data.path_float_parameters);
  }
  return instance;
}

/*
 * A function updates the parameters for nodes and paths in one time stamp
 */
void Heart::heart_automaton() {
  NodeTable temp_node = m_node_table;
  PathTable temp_path = m_path_table;
  PathTable path_table = m_path_table;
  NodeTable node_table = m_node_table;
  PathTable temp_path_node = m_path_table;  // a temp path table that can be
                                            // updated by node automaton
  std::vector<bool> temp_act(m_node_table.node_table.size());

  for (int i = 0; i < m_node_table.node_table.size(); ++i) {
    Node &temp_node_elem = temp_node.node_table[i];
    temp_node_elem.setIndex_of_path_activate_the_node(-1);
    temp_node_elem.node_automaton(temp_path_node);
    temp_act[i] = temp_node_elem.getParameters().activation;
  }

  // update parameters for each path
  for (int i = 0; i < m_path_table.path_table.size(); ++i) {
    auto [node_act_1, node_act_2] =
        temp_path.path_table[i].path_automaton(node_table);
    const Path &path = path_table.path_table[i];
    const int entry_index = path.getParameters().entry_node_index;
    if (node_table.node_table[entry_index].getParameters().node_state_index !=
        ERP) {
      temp_act[entry_index] = temp_act[entry_index] || node_act_1;
      if (node_act_1 == true)
        temp_node.node_table[entry_index].setIndex_of_path_activate_the_node(i);
    } else {
      temp_act[entry_index] = false;
      node_table.node_table[entry_index].setTERP_current(
          node_table.node_table[entry_index].getParameters().TERP_default);
    }
    const int exit_index = path.getParameters().exit_node_index;
    if (node_table.node_table[exit_index].getParameters().node_state_index !=
        ERP) {
      temp_act[exit_index] = temp_act[exit_index] || node_act_2;
      if (node_act_2 == true)
        temp_node.node_table[exit_index].setIndex_of_path_activate_the_node(i);
    } else {
      temp_act[exit_index] = false;
      node_table.node_table[exit_index].setTERP_current(
          node_table.node_table[exit_index].getParameters().TERP_default);
    }
  }
  // update the parameters to the member variable m_node_table
  for (int i = 0; i < m_node_table.node_table.size(); ++i)
    temp_node.node_table[i].setActivation(temp_act[i]);
  m_node_table = temp_node;

  // update the parameters to the member variable m_path_table
  for (int i = 0; i < temp_path.path_table.size(); ++i) {
    if (temp_path_node.path_table[i].getParameters().forward_timer_default !=
        temp_path.path_table[i].getParameters().forward_timer_default) {
      std::cout << "tmp_path doesnt equal tmp_path_node 1" << std::endl;
      temp_path.path_table[i].setForwardTimerDefault(
          temp_path_node.path_table[i].getParameters().forward_timer_default);
      if (temp_path_node.path_table[i].getParameters().path_state_index ==
          Idle) {
        temp_path.path_table[i].setForwardTimerCurrent(
            temp_path.path_table[i].getParameters().forward_timer_default);
      }
    }
    if (temp_path.path_table[i].getParameters().backward_timer_default !=
        m_path_table.path_table[i].getParameters().backward_timer_default) {
      std::cout << "tmp_path doesnt equal tmp_path_node 2" << std::endl;
      m_path_table.path_table[i].setBackwardTimerDefault(
          temp_path.path_table[i].getParameters().backward_timer_default);
      if (temp_path.path_table[i].getParameters().path_state_index == Idle) {
        m_path_table.path_table[i].setBackwardTimerCurrent(
            m_path_table.path_table[i].getParameters().backward_timer_default);
      }
    }
  }

  m_path_table = temp_path;
}