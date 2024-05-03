#ifndef _GLOBAL_ENUMS_HPP
#define _GLOBAL_ENUMS_HPP
#include <string>
#include <vector>

/* Node */
struct position {
  float x, y;
};

enum class node_state_index_enum { rest = 1, ERP = 2, RRP = 3 };

// The terminal of the path connected to the node
enum class path_terminal_enum { entry = 1, exit = 2 };

// a struct that has path_idx, terminal pair for each path connected to the node
struct path_terminal_pair {
  int path_idx;
  path_terminal_enum terminal;
};

// Is the node an Atrioventricular node related or not as they have different
// resopnse pattern
enum AVness_enum { AV = 1, non_AV = 2 };

struct node_parameters {
  std::string node_name;
  node_state_index_enum node_state_index =
      node_state_index_enum::rest;  // 1 for rest, 2 for ERP, 3 for RRP
  // The total refractory period is comprised of the (1) Rset, (2) Conduction,
  // (3) ERP, (4) RRP, and (5) Trest periods. Starting or Ending with a Rest
  // period.
  int TERP_current;
  int TERP_default;  // The ERP (Effective Refractory Period) refers to the
                     // period during which cardiac cells are unable to respond
                     // to a new electrical stimulus, regardless of its
                     // strength.
  int TRRP_current;
  int TRRP_default;  // The RRP (Relative Refractory Period) follows the ERP and
                     // represents a phase during which cardiac cells have
                     // partially recovered their excitability. While the cells
                     // are still in a refractory state, they are more
                     // responsive to strong stimuli compared to during the
                     // ERP.RRP is shorter than that of the ERP.
  int Trest_current;
  int Trest_default;  // The Trest (Rest) period is the period during which the
                      // cell is not excitable and is not responsive to any
                      // stimulus.
  bool activation;    // 1 for activated, 0 for not activated
  int Terp_min;       // The minimum effective refractory period
  int Terp_max;       // The maximum effective refractory period
  int index_of_path_activate_the_node;  // The index of the path that activates
                                        // the node, used to exclude the path
                                        // from the list of paths that the node
                                        // affects
  AVness_enum AVness;  // Is the node an Atrioventricular node related or not as
                       // they have
  std::vector<path_terminal_pair>
      connected_paths;  // The paths that are connected to the node
  position node_pos;    // The position of the node in the grid
};

/* Path */
enum class path_state_index_enum {
  Idle = 1,
  Antegrade_conduction = 2,
  Retrograde = 3,
  Conflict = 4,
  Double = 5
};

struct path_parameters {
  std::string path_name;  // The name of the path
  path_state_index_enum
      path_state_index;  // (1) Idle, (2) Antegrade conduction, (3) Retrograde,
                         // (4) Conflict, and (5) Double
  int entry_node_index;  // The index of the node that the path starts from
  int exit_node_index;   // The index of the node that the path ends at
  int amplitude_factor;  // The amplitude factor of the path, i.e. the Gain
  float forward_speed;   // The speed of the wavefront in the forward direction
  float backward_speed;  // The speed of the wavefront in the backward direction
  float forward_timer_current;   // The current timer for the forward direction
  float forward_timer_default;   // The default timer for the forward direction
  float backward_timer_current;  // The current timer for the backward direction
  float backward_timer_default;  // The default timer for the backward direction
  float path_length;             // The length of the path
  float path_slope;              // The slope of the path
};

#endif  // _GLOBAL_ENUMS_HPP
