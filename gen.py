import scipy.io
import argparse
from pathlib import Path
from contextlib import contextmanager
from typing import Iterable, Any

original_print = print


def print_bound_to_file(file):
    global print

    def bound_print(*args, **kwargs):
        return original_print(*args, **kwargs, file=file)

    print = bound_print


def initializer_list(iterable: Iterable[Any], tailing_comma=True):
    print(f"{{{', '.join(map(str, iterable))}}}{',' if tailing_comma else ''}")


@contextmanager
def vector(_type: str, name: str, const=True):
    print(f"{'const ' if const else ''}std::vector<{_type}> {name} = {{")
    try:
        yield
    finally:
        print("};")


@contextmanager
def guard(name: str):
    print(f"#ifndef {name}")
    print(f"#define {name}\n")
    comment("DONT EDIT THIS FILE. It is generated.")
    print()
    try:
        yield
    finally:
        print(f"#endif  // {name}")


def tab(use_spaces=True, num_of_spaces=4):
    print(" " * num_of_spaces if use_spaces else "\t", end="")


def comment(string):
    print(f"/* {string} */")


def generate_hpp_file_from_mat(matfile_path: str, hppfile_path: str):
    matfile = scipy.io.loadmat(matfile_path)

    with open(hppfile_path, "w", encoding="utf-8") as hppfile:
        print_bound_to_file(hppfile)
        define_name = "_" + Path(hppfile_path).name.replace(".", "_").upper()
        with guard(define_name):
            print("class {\npublic:")

            # =========================== node_table ==================================

            node_table = matfile["node_table"]
            node_names = []
            node_int_parameters = []

            for node in node_table:
                name = node[0][0]
                arrays = node[1:]
                int_parameters = [
                    elem
                    for arr in (map(lambda arr: arr.flatten(), arrays))
                    for elem in arr
                ]
                node_names.append(f'"{name}"')
                node_int_parameters.append(int_parameters)

            tab()
            comment("Nodes Data")
            tab()
            comment("matlab name: node_table")
            tab()
            with vector("std::string", "node_names", const=True):
                for i in range(0, len(node_names), 9):
                    node_names_iteration = node_names[i : i + 9]
                    tab()
                    tab()
                    print(f"{', '.join(node_names_iteration)},")
                tab()
            print()

            tab()
            with vector("std::vector<int>", "node_int_parameters", const=True):
                for node_int_parameter in node_int_parameters:
                    tab()
                    tab()
                    initializer_list(node_int_parameter)
                tab()
            print()

            # =========================== node_pos ==================================

            node_pos = matfile["node_pos"]
            positions = [position for position in node_pos]

            tab()
            comment("matlab name: node_pos")
            tab()
            with vector("node_position", "node_positions", const=True):
                for position in positions:
                    tab()
                    tab()
                    initializer_list(position)
                tab()
            print()

            # =========================== path_table ==================================

            path_table = matfile["path_table"]

            path_names = []
            path_int_parameters = []
            path_float_parameters = []

            for path in path_table:
                name = path[0][0]
                arrays = path[1:]
                path_names.append(f'"{name}"')
                parameters = [
                    elem
                    for arr in (map(lambda arr: arr.flatten(), arrays))
                    for elem in arr
                ]
                path_int_parameters.append(parameters[:4])
                path_float_parameters.append(parameters[4:])

            tab()
            comment("Path Data")
            tab()
            comment("matlab name: path_table")
            tab()
            with vector("std::string", "path_names", const=True):
                for i in range(0, len(path_names), 9):
                    path_names_iteration = path_names[i : i + 9]
                    tab()
                    tab()
                    print(f"{', '.join(path_names_iteration)},")
                tab()
            print()

            tab()
            with vector("std::vector<int>", "path_int_parameters", const=True):
                for path_int_parameter in path_int_parameters:
                    tab()
                    tab()
                    initializer_list(path_int_parameter)
                tab()
            print()

            tab()
            with vector("std::vector<float>", "path_float_parameters", const=True):
                for path_float_parameter in path_float_parameters:
                    tab()
                    tab()
                    initializer_list(path_float_parameter)
                tab()

            print("} Data;\n")


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("matfile_path", type=str, help="mat file path (input)")
    parser.add_argument("hppfile_path", type=str, help="hpp file path (output)")
    args = parser.parse_args()

    matfile_path = args.matfile_path
    hppfile_path = args.hppfile_path

    generate_hpp_file_from_mat(matfile_path=matfile_path, hppfile_path=hppfile_path)


if __name__ == "__main__":
    main()
