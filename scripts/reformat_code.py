#!/usr/bin/env python3

# a list of all file extensions to actually check
EXTENSIONS_TO_CHECK = [
    '.c',
    '.cpp',
    '.cxx',
    '.h',
    '.hpp',
    '.hxx',
    '.m',
    '.mm',
    '.json',
    '.cmake',
]


def reformat_source_file(file):
    from subprocess import run, PIPE

    # reformat the file in-place
    clang_result = run(['clang-format', '--style=file', '-i', '--sort-includes', file], stdout=PIPE)

def check_file_format(file_list):
    from sys import exit
    from os.path import splitext

    # loop through all files we want to format
    for current_file in file_list:
        # reformat the current file
        reformat_source_file(current_file)

        # tell the user the results for the current file
        print(f"Formatted {current_file}...")

def get_source_files():
    from os import walk
    from os.path import join, splitext
    return_list = []
    all_files = [join(dp, f) for dp, dn, filenames in walk('source/') for f in filenames if splitext(f)[1] in EXTENSIONS_TO_CHECK]
    for current_file in all_files:
        if 'vendor' not in current_file:
            return_list.append(current_file)
    return return_list

if __name__ == '__main__':
    files = get_source_files()
    check_file_format(files)
