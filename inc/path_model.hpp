#ifndef _PATH_MODEL_HPP
#define _PATH_MODEL_HPP
#include <string>
#include <vector>
#include <unordered_map> 
#include "GLOBAL_ENUMS.hpp"
#include "data.hpp"
// #include "node_model.hpp"


class Path{
public:
    Path(
        const std::string &path_name,
        const std::vector<int> &path_integer_parameters,
        const std::vector<float> &path_float_parameters
    );
    // ~Path();
    // void path_automatron();

    // Getter methods
    const path_parameters& getParameters() const { return _path_para; }
    // Setter methods
    void setParameters(const path_parameters& parameters) { _path_para = parameters; }

private:
    path_parameters _path_para;
};


// root singleton class for the path table
class PathTable{
private:
    PathTable(
        std::vector<std::string> path_names,
        std::vector<std::vector<int>> path_integer_parameters,
        std::vector<std::vector<float>> path_float_parameters);
public:
    // ~PathTable();
    std::vector<std::vector<path_terminal_pair>> path_terminal_pairs_per_point_list;
    static PathTable *getInstance();
    std::vector<Path> path_table;       // TODO prevent direct access to this variable
    static PathTable* instance;
    // pathe_terminal_pairs_per_point getter
    // const std::vector<std::vector<path_terminal_pair>>& getPathTerminalPairsPerPoint() const { return path_terminal_pairs_per_point_list; }
};
#endif // _PATH_MODEL_HPP