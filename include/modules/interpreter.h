/**
    ZitroneLang Code Interpreter

    :: modules/interpreter.h

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
#ifndef MODULES_INTERPRETER_H
#define MODULES_INTERPRETER_H
#include <stdint.h>
#pragma once

#pragma region Includes
#include <stddef.h>
#include <stdbool.h>
#pragma endregion

#pragma region Preprocessors
#define VAR_MAX_NAME_LEN 256
#define VAR_MAX_VALUE_LEN 256
#define HASH_TABLE_SIZE 1024
#define VAR_MAX_MEM_LEN 8096
#pragma endregion

typedef enum {
    VAR_INT = 0,
    VAR_DOUBLE = 1,
    VAR_STRING = 2,
    VAR_BOOL = 3,
} vartype_t;

typedef struct ZITVariable {
    char name[VAR_MAX_NAME_LEN];
    vartype_t type;
    union {
        int i_val;
        double d_val;
        char s_val[VAR_MAX_VALUE_LEN];
        bool b_val;
    } value;
} zitvar_t;

struct Interpreter {
    zitvar_t* table[HASH_TABLE_SIZE];
    zitvar_t mem[VAR_MAX_MEM_LEN];
    size_t vars_size;
    bool active;
};

uint64_t ZIT__hash(const char *str);
double ZIT__get_num_value(const char* token);
int ZIT__process_line(char* input);
int ZIT__cli();
int ZIT__runner(const char* file_path);
int ZIT__init();

#endif