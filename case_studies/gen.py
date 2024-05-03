import scipy.io
import argparse
from pathlib import Path
from contextlib import contextmanager
from typing import Iterable, Any
from math import isinf
from glob import glob


class Cursor:
    def __init__(
        self, *, ident_level=0, print_fn=print, use_spaces=True, num_of_spaces=4
    ):
        self.indent_level = ident_level
        self.print_fn = print_fn
        self.tab = " " * num_of_spaces if use_spaces else "\t"

    def indent(self):
        self.indent_level += 1

    def outdent(self):
        if self.indent_level == 0:
            return
        self.indent_level -= 1

    def print(self, *args, **kwargs):
        for _ in range(0, self.indent_level):
            self.print_fn(self.tab, end="")
        self.print_fn(*args, **kwargs)

    def newline(self):
        self.print_fn()

    def __enter__(self) -> "Cursor":
        self.indent()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.outdent()

    @contextmanager
    def vector(self, _type: str, name: str, const=True):
        self.print(f"{'const ' if const else ''}std::vector<{_type}> {name} = {{")
        self.indent()
        try:
            yield
        finally:
            self.outdent()
            self.print("};")

    def comment(self, string):
        return self.print(f"/* {string} */")

    @contextmanager
    def guard(self, name: str):
        self.print(f"#ifndef {name}")
        self.print(f"#define {name}")
        self.newline()
        self.comment("DONT EDIT THIS FILE. It is generated.")
        self.newline()
        self.print("#include <string>")
        self.print("#include <vector>")
        self.print('#include "GLOBAL_ENUMS.hpp"')
        self.newline()
        try:
            yield
        finally:
            self.print(f"#endif  // {name}")

    def initializer_list(self, iterable: Iterable[Any], tailing_comma=True):
        self.print(f"{{{', '.join(map(str, iterable))}}}{',' if tailing_comma else ''}")

    @contextmanager
    def klass(self, name: str):
        self.print("class {\npublic:")
        self.indent()
        try:
            yield
        finally:
            self.outdent()
            self.print(f"}} {name};\n")


def generate_node_table(cursor: Cursor, node_table):
    node_names = []
    node_int_parameters = []

    for node in node_table:
        name = node[0][0]
        arrays = node[1:]
        int_parameters = [
            elem for arr in (map(lambda arr: arr.flatten(), arrays)) for elem in arr
        ]
        node_names.append(f'"{name}"')
        node_int_parameters.append(int_parameters)

    cursor.comment("Nodes Data")
    cursor.comment("matlab name: node_table")

    with cursor.vector("std::string", "node_names", const=True):
        for i in range(0, len(node_names), 9):
            node_names_iteration = node_names[i : i + 9]
            cursor.print(f"{', '.join(node_names_iteration)},")

    cursor.newline()

    with cursor.vector("std::vector<int>", "node_int_parameters", const=True):
        for node_int_parameter in node_int_parameters:
            cursor.initializer_list(node_int_parameter)
    cursor.newline()


def generate_node_positions(cursor: Cursor, node_positions):
    positions = [position for position in node_positions]

    cursor.comment("matlab name: node_pos")
    with cursor.vector("position", "node_positions", const=True):
        for position in positions:
            cursor.initializer_list(position)
    cursor.newline()


def generate_path_table(cursor: Cursor, path_table):
    path_names = []
    path_int_parameters = []
    path_float_parameters = []

    for path in path_table:
        name = path[0][0]
        arrays = path[1:]
        parameters = [
            elem for arr in (map(lambda arr: arr.flatten(), arrays)) for elem in arr
        ]

        if any(isinf(elem) for elem in parameters):
            continue

        path_names.append(f'"{name}"')
        path_int_parameters.append(parameters[:4])
        path_float_parameters.append(parameters[4:])

    cursor.comment("Path Data")
    cursor.comment("matlab name: path_table")

    with cursor.vector("std::string", "path_names", const=True):
        for i in range(0, len(path_names), 9):
            path_names_iteration = path_names[i : i + 9]
            cursor.print(f"{', '.join(path_names_iteration)},")
    cursor.newline()

    with cursor.vector("std::vector<int>", "path_int_parameters", const=True):
        for path_int_parameter in path_int_parameters:
            cursor.initializer_list(path_int_parameter)
    cursor.newline()

    with cursor.vector("std::vector<float>", "path_float_parameters", const=True):
        for path_float_parameter in path_float_parameters:
            cursor.initializer_list(path_float_parameter)
    cursor.newline()


def generate_egm_table(cursor: Cursor, egm_table):
    egm_names = []
    egm_int_parameters = []
    egm_types = []

    for egm_elem in egm_table:
        name = egm_elem[0][0]
        arrays = egm_elem[1]
        parameters = [
            elem for arr in (map(lambda arr: arr.flatten(), arrays)) for elem in arr
        ]
        egm_type = [
            elem
            for arr in (map(lambda arr: arr.flatten(), egm_elem[2]))
            for elem in arr
        ][0]

        egm_names.append(f'"{name}"')
        egm_int_parameters.append(parameters)
        egm_types.append(egm_type)

    egm_single_names = []
    egm_single_int_parameters = []
    egm_double_names = []
    egm_double_int_parameters = []

    for elem in zip(egm_names, egm_int_parameters, egm_types):
        if elem[2] == "s":
            egm_single_names.append(elem[0])
            egm_single_int_parameters.append(elem[1])
        else:
            egm_double_names.append(elem[0])
            egm_double_int_parameters.append(elem[1])

    cursor.comment("matlab name: egm_table")

    with cursor.vector("std::string", "egm_single_names", const=True):
        for i in range(0, len(egm_single_names), 9):
            egm_single_names_iteration = egm_single_names[i : i + 9]
            cursor.print(f"{', '.join(egm_single_names_iteration)},")
    cursor.newline()

    with cursor.vector("int", "egm_single_int_parameters", const=True):
        for egm_single_int_parameter in egm_single_int_parameters:
            cursor.print(f"{egm_single_int_parameter[0]},")
    cursor.newline()

    with cursor.vector("std::string", "egm_double_names", const=True):
        for i in range(0, len(egm_double_names), 9):
            egm_double_names_iteration = egm_double_names[i : i + 9]
            cursor.print(f"{', '.join(egm_double_names_iteration)},")
    cursor.newline()

    with cursor.vector("std::pair<int, int>", "egm_double_int_parameters", const=True):
        for egm_double_int_parameter in egm_double_int_parameters:
            cursor.initializer_list(egm_double_int_parameter)
    cursor.newline()


def generate_probe_table(cursor: Cursor, probe_table):
    probe_names = []
    probe_int_parameters = []

    for probe_elem in probe_table:
        name = probe_elem[0][0]
        arrays = probe_elem[1]
        parameters = [
            elem for arr in (map(lambda arr: arr.flatten(), arrays)) for elem in arr
        ]

        probe_names.append(f'"{name}"')
        probe_int_parameters.append(parameters)

    cursor.comment("matlab name: probe_table")

    with cursor.vector("std::string", "probe_names", const=True):
        for i in range(0, len(probe_names), 9):
            probe_names_iteration = probe_names[i : i + 9]
            cursor.print(f"{', '.join(probe_names_iteration)},")
    cursor.newline()

    with cursor.vector("std::vector<int>", "probe_int_parameters", const=True):
        for probe_int_parameter in probe_int_parameters:
            cursor.initializer_list(probe_int_parameter)
    cursor.newline()


def generate_probe_positions(cursor: Cursor, probe_positions):
    positions = [position for position in probe_positions]

    cursor.comment("matlab name: probe_pos")
    with cursor.vector("position", "probe_positions", const=True):
        for position in positions:
            cursor.initializer_list(position)
    cursor.newline()


def generate_pace_para(cursor: Cursor, pace_para):
    pace_names = []
    pace_int_parameters = []

    for pace_elem in pace_para:
        name = pace_elem[0][0]
        arrays = pace_elem[1:]
        int_parameters = [
            elem for arr in (map(lambda arr: arr.flatten(), arrays)) for elem in arr
        ]
        pace_names.append(f'"{name}"')
        pace_int_parameters.append(int_parameters)

    cursor.comment("matlab name: pace_para")

    with cursor.vector("std::string", "component_names", const=True):
        for i in range(0, len(pace_names), 9):
            pace_names_iteration = pace_names[i : i + 9]
            cursor.print(f"{', '.join(pace_names_iteration)},")

    cursor.newline()

    with cursor.vector("std::vector<int>", "pace_int_parameters", const=True):
        for pace_int_parameter in pace_int_parameters:
            cursor.initializer_list(pace_int_parameter)
    cursor.newline()


KEY_FUNCION_MAPPING = [
    ("node_table", generate_node_table),
    ("node_pos", generate_node_positions),
    ("path_table", generate_path_table),
    ("egm_table", generate_egm_table),
    ("probe_table", generate_probe_table),
    ("probe_pos", generate_probe_positions),
    ("pace_para", generate_pace_para),
]


def generate_hpp_file_from_mat(matfile_path: str, hppfile_path: str, identifier: str):
    matfile = scipy.io.loadmat(matfile_path)

    with open(hppfile_path, "w", encoding="utf-8") as hppfile:

        def print_to_hppfile(*args, **kwargs):
            return print(*args, **kwargs, file=hppfile)

        cursor = Cursor(print_fn=print_to_hppfile)
        define_name = "_" + Path(hppfile_path).name.replace(".", "_").upper()
        with cursor.guard(define_name):
            with cursor.klass(f"{identifier.capitalize()}Data"):
                for key, generate_fn in KEY_FUNCION_MAPPING:
                    generate_fn(cursor, matfile[key])


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("outdir", type=str, help="output directory")
    args = parser.parse_args()

    outdir = args.outdir

    for matfile_path in glob("./case_studies/*.mat"):
        identifier = Path(matfile_path).name.lower()[:-4]
        hppfile_path = Path(outdir) / f"{identifier}_data_gen.hpp"
        generate_hpp_file_from_mat(
            matfile_path=matfile_path, hppfile_path=hppfile_path, identifier=identifier
        )


if __name__ == "__main__":
    main()
