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
#include <stdint.h>
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
    uint64_t hash = ZIT__hash(name);
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        uint64_t idx = (hash + i) % HASH_TABLE_SIZE;
        if (zit.table[idx] == NULL) {
            return NULL;
        }
        if (strcmp(zit.table[idx]->name, name) == 0) {
            return zit.table[idx];
        }
    }
    return NULL;
}



uint64_t ZIT__hash(const char *str) {
    uint64_t hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_TABLE_SIZE;
}

double ZIT__get_num_value(const char* token) {
    zitvar_t* var = find_var(token);
    if (var != NULL) {
        if (var->type == VAR_DOUBLE) {
            return var->value.d_val;
        }
        if (var->type == VAR_INT) {
            return (double)var->value.i_val;
        }
        if(var->type == VAR_STRING) {
        	return atof(var->value.s_val);
        }
    }
    return atof(token);
}

int ZIT__process_line(char* input) {
    if (zit.vars_size >= VAR_MAX_MEM_LEN) {
        fprintf(stderr, "ERROR: Memory is full");
        return 1;
    }
    
    if (input == NULL || input[0] == '\0' || input[0] == '#') {
        return 0;
    }

    char t_input[512];
    strncpy(t_input,input,sizeof(t_input));
    char* c_cmd = strtok(t_input, " ");

    if(!zit.active_logic && c_cmd != NULL ) {
    	if(strcmp(c_cmd,"ende") != 0 && strcmp(c_cmd,"ansonsten" )!= 0) {
    		return 0;
    	}
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
                uint64_t h = ZIT__hash(name);
                for (int i = 0; i < HASH_TABLE_SIZE; i++) {
                    uint64_t idx = (h + i) % HASH_TABLE_SIZE;
                    if (zit.table[idx] == NULL) {
                        zit.table[idx] = var;
                        break;
                    }
                }
            }
            if (val[0] >= '0' && val[0] <= '9') {
                var->type = VAR_DOUBLE;
                var->value.d_val = atof(val);
            } else {
                var->type = VAR_STRING;
                strncpy(var->value.s_val, val, VAR_MAX_VALUE_LEN);
            }
            printf("k(%s)<%p> = v(%s)<%p>\n", var->name, var->name, var->value.s_val, var->value.s_val);
        } else {
            fprintf(stderr, "name & value is null\n");
        }
    }
    // CMD: berech <a_value> <operation> <b_value>
    else if (strcmp(cmd, "berech") == 0) {
        char* a_val = strtok(NULL, " ");
        char* op_val = strtok(NULL, " ");
        char* b_val = strtok(NULL, " ");
        zitvar_t* var = NULL;
        if (a_val != NULL && op_val != NULL && b_val != NULL) {
            double a_n_val = ZIT__get_num_value(a_val);
            double b_n_val = ZIT__get_num_value(b_val);
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
                    break;
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
                zit.table[ZIT__hash("res")] = var;
            }
            var->type = VAR_DOUBLE;
            var->value.d_val = result;
            printf("result<%p>(%s) = %f\n", var, var->name, var->value.d_val);
        }
    }
    //CMD: eingabe <var> | eingabe x 
    else if(strcmp(cmd, "eingabe") == 0) {
    	char *inp_s = strtok(NULL, " ");
    	if(inp_s != NULL) {
    	    zitvar_t *inp = find_var(inp_s);
    	    if(inp != NULL) {
	    		printf("inp %s: ",inp_s);
	    		fflush(stdout);
	    	    inp->type = VAR_STRING;
	    	    fgets(inp->value.s_val,VAR_MAX_VALUE_LEN,stdin);
                inp->value.s_val[strcspn(inp->value.s_val,"\n")]='\0';
	        }
    	}
    }
    // CMD: wenn <var> <arg> <var2> | wenn empire == petyx
    else if(strcmp(cmd,"wenn") == 0) {
        char *a_val = strtok(NULL, " ");
        char *op_val = strtok(NULL, " ");
        char *b_val = strtok(NULL, " ");
        if(a_val != NULL && op_val != NULL && b_val != NULL ) {
	        bool res = false;
            zitvar_t* a_var = find_var(a_val);
            zitvar_t* b_var = find_var(b_val);

            const char* a_str = a_var ? a_var->value.s_val : a_val;
            const char* b_str = b_var ? b_var->value.s_val : b_val;

            double rw = ZIT__get_num_value(a_val);
            double lw = ZIT__get_num_value(b_val);

            if(rw == 0 && lw == 0 && strcmp(a_str, "0") != 0 && strcmp(b_str, "0") != 0) {
            	if(strcmp(op_val,"==") == 0) {
            		res = (strcmp(a_str,b_str)==0);
            	}
            	else if(strcmp(op_val,"!=")==0) {
            	    res = (strcmp(a_str,b_str) != 0);
            	}
            }
            else {
		        if(strcmp(op_val, "==") == 0) {
		        	res = (rw == lw);
		        }
		        else if(strcmp(op_val, "!=") == 0) {
		        	res = (rw != lw);
		        }
		        else if(strcmp(op_val, "<=") == 0) {
		        	res = (rw <= lw);
		        }
		        else if(strcmp(op_val, ">=") == 0) {
		        	res = (rw >= lw);
		        }
		    }
	        zit.active_logic = res;
	        zit.met = res;
	    }
    }
    else if (strcmp(cmd, "ende") == 0) {
    	zit.active_logic = true;
    }
    else if (strcmp(cmd, "ansonsten") == 0) {
    	if(zit.met == false) {
    		zit.active_logic = true;
    	}else {
    	    zit.active_logic = false;
    	}
    }
    // CMD: druck <arg1> <arg2> ...
    else if (strcmp(cmd, "druck") == 0) {
        if(!zit.active_logic) {
        	return 0;
        }
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
    zit.active_logic = true;
    zit.met = false;
    return 0;
}
#pragma endregion
