/**
    ZitroneLang Code Interpreter

    :: main.c

    Copyright (c) 2026 ruscmi, voncov

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#pragma region Includes
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <modules/interpreter.h>
#pragma endregion

#pragma region CLI
struct CLIOptions {
    const char* source_file_path;
};

static void CLI__print_help(const char* p_name) {
    printf("ZitroneLang Code Interpreter v.0.1.1\n\n");
    printf("Usage: %s [OPTIONS] --file <FILE>\n\nOptions:\n", p_name);
    printf("\t--file <PATH_TO_SOURCE>\n\t\tPath to source file\n");
}

static struct CLIOptions CLI__parse_args(int argc, char** argv) {
    struct CLIOptions cli_opt = {0};
    for (int i = 1; i < argc; ++i) {
        const char* arg = argv[i];
        if (strcmp(arg, "--file") == 0) {
            cli_opt.source_file_path = argv[++i];
        }
        else if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            CLI__print_help(argv[0]);
        }
        else {
            printf("Unknown argument: %s\n", arg);
            exit(1);
        }
    }

    return cli_opt;
}
#pragma endregion

int main(int argc, char** argv) {
    ZIT__init();
    if (argc == 1) {
        return ZIT__cli();
    }
    struct CLIOptions opt = CLI__parse_args(argc, argv);
    if (opt.source_file_path != NULL || opt.source_file_path[0] != '\0') {
        const char* source_file_path = opt.source_file_path;
        printf("[main] Source file path: %s\n", source_file_path);
        return ZIT__runner(source_file_path);
    }
    CLI__print_help(argv[0]);
    return 0;
}