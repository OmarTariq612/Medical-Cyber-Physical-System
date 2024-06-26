#ifndef _PATH_MODEL_CPP
#define _PATH_MODEL_CPP
#include "../inc/path_model.hpp"

#include <assert.h>

#include "../inc/node_model.hpp"

// Constructor
Path::Path(const std::string &path_name_,
           const std::vector<int> &path_integer_parameters,
           const std::vector<float> &path_float_parameters)
    : _path_para{
          path_name_,
          static_cast<path_state_index_enum>(
              path_integer_parameters[0]),  // (1) Idle, (2) Antegrade
                                            // conduction, (3) Retrograde, (4)
                                            // Conflict, and (5) Double
          path_integer_parameters[1] - 1,   // entry_node_index=0 (originally 1)
          path_integer_parameters[2] - 1,   // exit_node_index=1 (originally 2)
          path_integer_parameters[3],       // amplitude_factor
          path_float_parameters[0],         // forward_speed
          path_float_parameters[1],         // backward_speed
          path_float_parameters[2],         // forward_timer_current
          path_float_parameters[3],         // forward_timer_default
          path_float_parameters[4],         // backward_timer_current
          path_float_parameters[5],         // backward_timer_default
          path_float_parameters[6],         // path_length
          path_float_parameters[7]          // path_slope
      } {}

PathTable::PathTable(
    const std::vector<std::string> &path_names_,
    const std::vector<std::vector<int>> &path_integer_parameters_,
    const std::vector<std::vector<float>> &path_float_parameters_) {
  assert(path_names_.size() == path_integer_parameters_.size());
  assert(path_names_.size() == path_float_parameters_.size());

  // the required size of path_table is already known
  // preallocating.
  path_table.reserve(path_names_.size());

  for (int i = 0; i < path_names_.size(); ++i) {
    path_table.emplace_back(path_names_[i], path_integer_parameters_[i],
                            path_float_parameters_[i]);
  }

  std::unordered_map<int, std::vector<path_terminal_pair>>
      node_idx_paths_terminals_umap;
  for (int path_idx = 0; path_idx < path_table.size(); ++path_idx) {
    int entry_idx = path_table[path_idx].getParameters().entry_node_index;
    int exit_idx = path_table[path_idx].getParameters().exit_node_index;
    node_idx_paths_terminals_umap[entry_idx].push_back(
        path_terminal_pair{path_idx, path_terminal_enum::entry});
    node_idx_paths_terminals_umap[exit_idx].push_back(
        path_terminal_pair{path_idx, path_terminal_enum::exit});
  }
  // used node_count instead of using the node_table.size() because the
  // node_table is not initialized yet and it will go into a cyclic dependency
  // problem
  int node_count = Data.node_names.size();

  // the required size of path_terminal_pairs_per_point_list is already known
  // preallocating.
  path_terminal_pairs_per_point_list.reserve(node_count);

  for (int node_idx = 0; node_idx < node_count; ++node_idx) {
    path_terminal_pairs_per_point_list.push_back(
        node_idx_paths_terminals_umap[node_idx]);
  }
}

/*
 * This function updates the status of a single path
 */
std::pair<bool, bool> Path::path_automaton(NodeTable &NT) {
  using enum path_state_index_enum;

  /*
  in the _path_para it changes:
      1. path_state_index         [2]
      2. forward_timer_current    [8]
      3. backward_timer_current   [10]
  in the NT it changes:
      1. activation               [9]
  */
  bool temp_node1_activation = false;
  bool temp_node2_activation = false;
  const int entry_node_index = _path_para.entry_node_index;
  const int exit_node_index = _path_para.exit_node_index;
  Node &entry_node = NT.node_table[entry_node_index];
  Node &exit_node = NT.node_table[exit_node_index];
  switch (_path_para.path_state_index) {
    case Idle:  // Idle
      // if activation coming from entry node
      if (entry_node.getParameters().activation == true) {
        // Antegrade conduction
        _path_para.path_state_index = Antegrade_conduction;
        // if activation coming from exit node
      } else if (exit_node.getParameters().activation == true) {
        // Retrograde conduction
        _path_para.path_state_index = Retrograde;
      }
      break;
    case Antegrade_conduction:  // Antegrade conduction
      // if activation coming from exit node
      if (exit_node.getParameters().activation == true) {  // double
        _path_para.path_state_index = Double;
      } else {
        // if timer running out
        if (_path_para.forward_timer_current == 0) {
          // reset timer
          _path_para.forward_timer_current = _path_para.forward_timer_default;
          // activate exit node
          temp_node2_activation = true;
          // go to conflict state
          _path_para.path_state_index = Conflict;
        } else {  // timer
          _path_para.forward_timer_current--;
        }
      }
      break;
    case Retrograde:  // Retro
      // if activation coming from entry node
      if (entry_node.getParameters().activation == true) {  // conflict
        // go to Double state
        _path_para.path_state_index = Double;
      } else {
        // if timer runs out
        if (_path_para.backward_timer_current == 0) {
          // reset timer
          _path_para.backward_timer_current = _path_para.backward_timer_default;
          // activate the entry node
          temp_node1_activation = true;
          // change state to conflict
          _path_para.path_state_index = Conflict;
        } else {  // timer
          _path_para.backward_timer_current--;
        }
      }
      break;
    case Conflict:  // Conflict
      // go to Idle state
      _path_para.path_state_index = Idle;
      break;
    case Double:  // double
      if (_path_para.backward_timer_current == 0) {
        // reset timer
        _path_para.backward_timer_current = _path_para.backward_timer_default;
        // activate the entry node
        temp_node1_activation = true;
        // change state to conflict
        _path_para.path_state_index = Conflict;
        return {temp_node1_activation, temp_node2_activation};
      }
      if (_path_para.forward_timer_current == 0) {
        // reset timer
        _path_para.forward_timer_current = _path_para.forward_timer_default;
        // activate exit node
        temp_node2_activation = true;
        // go to conflict state
        _path_para.path_state_index = Conflict;
        return {temp_node1_activation, temp_node2_activation};
      }
      if (std::abs(1 -
                   _path_para.forward_timer_current /
                       _path_para.forward_timer_default -
                   _path_para.backward_timer_current /
                       _path_para.backward_timer_default) <
          (0.9 / std::min(_path_para.forward_timer_default,
                          _path_para.backward_timer_default))) {
        _path_para.backward_timer_current = _path_para.backward_timer_default;
        _path_para.forward_timer_current = _path_para.forward_timer_default;
        _path_para.path_state_index = Conflict;
      } else {
        _path_para.forward_timer_current--;
        _path_para.backward_timer_current--;
      }
      break;
  }
  return {temp_node1_activation, temp_node2_activation};
}

#endif  // _PATH_MODEL_CPP
