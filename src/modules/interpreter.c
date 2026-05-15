/**
    ZitroneLang Code Interpreter

    :: modules/interpreter.c

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
#include <stdlib.h>
#include <stdio.h>
#include <modules/interpreter.h>
#include <string.h>
#pragma endregion

#pragma region Interpreter
static bool initialized = false;
static struct Interpreter zit = {0};

static zitvar_t* find_var(const char* name) {
    for (size_t i = 0; i < zit.vars_size; i++) {
        if (strcmp(zit.mem[i].name, name) == 0) {
            return &zit.mem[i];
        }
    }
    return NULL;
}

int ZIT__process_line(char* input) {
    if (input == NULL || input[0] == '\0' || input[0] == '#') {
        return 0;
    }
    char* cmd = strtok(input, " ");
    if (cmd == NULL) {
        return 0;
    }
    // CMD: exit
    if (strcmp(cmd, "exit") == 0) {
        printf("Zitrone: Bye bye! 🍋\n");
        zit.active = false;
        return 0;
    }
    // CMD: erstellen <name> <value>
    else if (strcmp(cmd, "erstellen") == 0) {
        char* name = strtok(NULL, " ");
        char* val = strtok(NULL, " ");
        zitvar_t* var = NULL;
        if (name != NULL && val != NULL) {
            var = find_var(name);
            if (var == NULL) {
                var = &zit.mem[zit.vars_size++];
                strncpy(var->name, name, VAR_MAX_NAME_LEN);
            }
            var->type = VAR_STRING;
            strncpy(var->value.s_val, val, VAR_MAX_VALUE_LEN);
        } else {
            fprintf(stderr, "name & value is null\n");
        }
        printf("k(%s)<%p> = v(%s)<%p>\n", var->name, var->name, var->value.s_val, var->value.s_val);
    }
    // CMD: berech <a_value> <operation> <b_value>
    else if (strcmp(cmd, "berech") == 0) {
        char* a_val = strtok(NULL, " ");
        char* op_val = strtok(NULL, " ");
        char* b_val = strtok(NULL, " ");
        zitvar_t* var = NULL;
        if (a_val != NULL && op_val != NULL && b_val != NULL) {
            double a_n_val = atof(a_val);
            double b_n_val = atof(b_val);
            double result = 0;
            char n_op_val = op_val[0];

            switch (n_op_val) {
                case '+': {
                    result = a_n_val + b_n_val;
                    break;
                }
                case '-': {
                    result = a_n_val - b_n_val;
                    break;
                }
                case '*': {
                    result = a_n_val * b_n_val;
                    break;
                }
                case '/': {
                    if (b_n_val == 0) {
                        fprintf(stderr, "ERROR: Cannot divide by zero\n");
                        break;
                    }
                    result = a_n_val / b_n_val;
                }
                default: {
                    fprintf(stderr, "Invalid math operation\n");
                    break;
                }
            }
            var = find_var("res");
            if (var == NULL) {
                var = &zit.mem[zit.vars_size++];
                strcpy(var->name, "res");
            }
            var->type = VAR_DOUBLE;
            var->value.d_val = result;
            printf("result<%p>(%s) = %f\n", var, var->name, var->value.d_val);
        }
    }
    // CMD: druck <arg1> <arg2> ...
    else if (strcmp(cmd, "druck") == 0) {
        char* arg = strtok(NULL, " ");
        zitvar_t* var = NULL;
        while (arg) {
            var = find_var(arg);
            if (var != NULL) {
                if (var->type == VAR_DOUBLE) {
                    printf("%f ", var->value.d_val);
                }
                else if (var->type == VAR_STRING) {
                    printf("%s ", var->value.s_val);
                }
                else if (var->type == VAR_INT) {
                    printf("%d ", var->value.i_val);
                }
                else if (var->type == VAR_BOOL) {
                    bool value = var->value.b_val;
                    if (value == true) {
                        printf("TRUE ");
                    }
                    if (value == false) {
                        printf("FALSE ");
                    }
                }
            }
            else {
                printf("%s ", arg);
            }
            arg = strtok(NULL, " ");
        }
        printf("\n");
    }
    else {
        fprintf(stderr, "Zitrone ERROR: Unknown command: \"%s\"\n", cmd);
    }

    return 0;
}

int ZIT__cli() {
    char buf[1024];
    printf("ZitroneLang REPL v0.1.1 (Lemon Logic)\n");
    while (zit.active) {
        printf("zit> ");
        if (fgets(buf, sizeof(buf), stdin) == false) {
            break;
        }
        buf[strcspn(buf, "\n")] = '\0';
        ZIT__process_line(buf);
    }
    return 0;
}

int ZIT__runner(const char* file_path) {
    FILE* f = fopen(file_path, "r");
    if (f == NULL) {
        fprintf(stderr, "ERROR: Could not open file %s\n", file_path);
        return 1;
    }
    char line[1024];
    while (fgets(line, sizeof(line), f) && zit.active) {
        line[strcspn(line, "\n")] = '\0';
        ZIT__process_line(line);
    }
    fclose(f);
    return 0;
}

int ZIT__init() {
    zit.vars_size = 0;
    zit.active = true;
    return 0;
}
#pragma endregion