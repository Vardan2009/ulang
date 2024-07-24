#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_VARS 100
#define MAX_LABELS 100
#define MAX_INSTRUCTIONS 100
#define VAR_NAME_LENGTH 50
typedef enum { SET, MOV, INC, DEC, ADD, ENLEQ, ENLNEQ, ENLGT, ENLLT, JMP, LBL, OUT, INVALID } Opcode;
typedef struct { Opcode opcode; char arg1[VAR_NAME_LENGTH]; char arg2[VAR_NAME_LENGTH]; } Instruction;
typedef struct { char name[VAR_NAME_LENGTH]; int value; } Variable;
Variable variables[MAX_VARS];
int var_count = 0;
Instruction code[MAX_INSTRUCTIONS];
Instruction* labels[MAX_LABELS];
Instruction *code_start;
Instruction *code_end;
Instruction *ptr;
Opcode get_opcode(const char *op) {
    static const struct { const char *name; Opcode opcode; } opcodes[] = {
        {"SET", SET}, {"MOV", MOV}, {"INC", INC}, {"DEC", DEC},
        {"ADD", ADD}, {"ENLEQ", ENLEQ}, {"ENLNEQ", ENLNEQ}, {"ENLGT", ENLGT},
        {"ENLLT", ENLLT}, {"JMP", JMP}, {"LBL", LBL}, {"OUT", OUT}
    };
    for (int i = 0; i < sizeof(opcodes) / sizeof(opcodes[0]); i++)
        if (strcmp(op, opcodes[i].name) == 0) return opcodes[i].opcode;
    return INVALID;
}
int get_variable_value(const char *name) {
    for (int i = 0; i < var_count; i++) if (strcmp(variables[i].name, name) == 0) return variables[i].value;
    fprintf(stderr, "ulang: variable %s not found\n", name);
    exit(1);
}
void set_variable_value(const char *name, int value) {
    for (int i = 0; i < var_count; i++)
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            return;
        }
    if (var_count >= MAX_VARS) {
        fprintf(stderr, "ulang: too many variables\n"); exit(1);
    }
    strncpy(variables[var_count].name, name, VAR_NAME_LENGTH);
    variables[var_count].value = value;
    var_count++;
}
Instruction parse_instruction(char* line) {
    char* op_str = strtok(line, " \n");
    Opcode op = get_opcode(op_str);
    if (op == INVALID) {
        fprintf(stderr, "ulang: invalid opcode %s\n", op_str);
        exit(1);
    }
    Instruction instr = {op, "", ""};
    char* arg1_str = strtok(NULL, " \n");
    char* arg2_str = (op != INC && op != DEC && op != LBL && op != JMP) ? strtok(NULL, " \n") : NULL;
    if (arg1_str) strncpy(instr.arg1, arg1_str, VAR_NAME_LENGTH);
    if (arg2_str) strncpy(instr.arg2, arg2_str, VAR_NAME_LENGTH);
    return instr;
}
void handle_comparison(Opcode op, int arg1_value, int arg2_value) {
    int condition_met = 0;
    switch (op) {
        case ENLEQ: condition_met = arg1_value != arg2_value; break;
        case ENLNEQ: condition_met = arg1_value == arg2_value; break;
        case ENLLT: condition_met = arg1_value >= arg2_value; break;
        case ENLGT: condition_met = arg1_value <= arg2_value; break;
        default: return; // No comparison action needed
    }
    if (condition_met) ptr++;
}
void execute_instruction(Instruction i) {
    int arg1_value, arg2_value;
    switch (i.opcode) {
        case SET:
            arg2_value = isdigit(i.arg2[0]) ? atoi(i.arg2) : get_variable_value(i.arg2);
            set_variable_value(i.arg1, arg2_value);
            break;
        case MOV:
            arg1_value = get_variable_value(i.arg2);
            set_variable_value(i.arg1, arg1_value);
            break;
        case INC:
            arg1_value = get_variable_value(i.arg1);
            set_variable_value(i.arg1, arg1_value + 1);
            break;
        case DEC:
            arg1_value = get_variable_value(i.arg1);
            set_variable_value(i.arg1, arg1_value - 1);
            break;
        case ADD:
            arg1_value = get_variable_value(i.arg1);
            arg2_value = isdigit(i.arg2[0]) ? atoi(i.arg2) : get_variable_value(i.arg2);
            set_variable_value(i.arg1, arg1_value + arg2_value);
            break;
        case LBL:
            labels[atoi(i.arg1)] = ptr;
            break;
        case JMP:
            ptr = labels[atoi(i.arg1)];
            break;
        case OUT:
            if (strcmp(i.arg1, "C") == 0) 
                printf("%c", get_variable_value(i.arg2));
            else if (strcmp(i.arg1, "D") == 0) 
                printf("%d", get_variable_value(i.arg2));
            else {
                fprintf(stderr, "ulang: invalid format specifier %s\n", i.arg1);
                exit(1);
            }
            break;
        case ENLEQ: case ENLNEQ: case ENLLT: case ENLGT:
            arg1_value = get_variable_value(i.arg1);
            arg2_value = get_variable_value(i.arg2);
            handle_comparison(i.opcode, arg1_value, arg2_value);
            break;
        default:
            fprintf(stderr, "ulang: unknown opcode %d\n", i.opcode);
            exit(1);
    }
}
void load_code(const char* filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) { fprintf(stderr, "ulang: failed to read file %s\n", filename); exit(1);}
    char* line = NULL;
    size_t len = 0, read;
    int instr_count = 0;
    while ((read = getline(&line, &len, f)) != -1) if (line[0] != '~') code[instr_count++] = parse_instruction(line);
    free(line);
    fclose(f);
    code_start = &code[0];
    code_end = &code[instr_count - 1];
    ptr = code_start;
}
int main(int argc, char *argv[]) {
    if (argc != 2) { fprintf(stderr, "ulang: usage: ulang <path>.ul\n"); return 1; }
    load_code(argv[1]);
    while (ptr >= code_start && ptr <= code_end) { execute_instruction(*ptr); ptr++; }
    return 0;
}