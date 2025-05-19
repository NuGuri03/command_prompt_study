#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "redirect.h"

#define MAX_COMMAND_SIZE 1024

typedef struct EnvVar{
    char* name;
    char* value;
    struct EnvVar* next;
}EnvVar;

typedef struct {
    char* name;
    char** args;
    int argc;

    char* input_file;
    char* output_file;
    bool is_background;
    RedirectType redirect_type;
} Command;

// 사용자 입력 파싱하기
Command parse_single_command(char *input, EnvVar* env_list);

// 메모리 공간 생성
char* create_char_mem();

Command* parse_input(char *input, int* num_cmds, EnvVar* env_list);

void set_env_var(char* name, char* value, EnvVar** env_list);
int assign_env_variable(char* input, EnvVar** env_list);
char* replace_env_vars(const char* input, EnvVar* env_list);
char* get_env_value(const char* name, EnvVar* env_list);
void remove_env_var(EnvVar** head, const char* key);
void free_env_vars(EnvVar* env_list);

// cmd free
void free_command(Command* cmd);
void free_commands(Command* cmds, int num_cmds);
#endif