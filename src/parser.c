#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

// 1차원 char 생성하기
char* create_char_mem() {
    char* temp = (char*)malloc(sizeof(char) * MAX_COMMAND_SIZE);
    if (!temp) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    return temp;
}

// 환경 변수 추가하기
void set_env_var(char* name, char* value, EnvVar** env_list){

    EnvVar* temp = *env_list;

    while (temp) {
        if (!strcmp(temp->name, name)) {
            free(temp->value);
            temp->value = create_char_mem();
            strcpy(temp->value, value);
            return;
        }
        temp = temp->next;
    }

    EnvVar* new_var = (EnvVar*)malloc(sizeof(EnvVar));
    new_var->name = create_char_mem();
    new_var->value = create_char_mem();
    strcpy(new_var->name, name);
    strcpy(new_var->value, value);
    new_var->next = *env_list;
    *env_list = new_var;
}

// 환경 변수를 배정하는 함수
// -1을 리턴하면 배정 X, 0을 리턴하면 배정 O
int assign_env_variable(char* input, EnvVar** env_list){

    char* point = strchr(input, '=');

    if(!point){
        return -1;
    }

    int name_len = point - input;
    char* name = create_char_mem(), *value = create_char_mem();
    for(int i = 0 ;i < name_len; i++){
        name[i] = input[i];
    }
    name[name_len] = '\0';

    const char* val_start = point + 1;
    for (int i = 0; val_start[i] != '\0'; i++) {
        value[i] = val_start[i];
    }
    value[strlen(val_start)] = '\0';

    set_env_var(name, value, env_list);

    free(name);
    free(value);
    return 0;
}

char* get_env_value(const char* name, EnvVar* env_list){

    EnvVar* temp = env_list;
    char* t = getenv(name);

    if (t){
        return t;
    }

    while (temp){
        if(!strcmp(temp->name, name)){
            return temp->value;
        }
        temp = temp->next;
    }

    return NULL;

}

char* replace_env_vars(const char* input, EnvVar* env_list){

    char* result = create_char_mem();
    int ri = 0, i = 0;

    while (input[i]){

        if (input[i] == '$'){
            i++;
            char* name_var = create_char_mem();
            char* value_var;
            int nvi = 0, vvi = 0;

            while(input[i] && (('A' <= input[i] && input[i] <= 'Z') || ('a' <= input[i] && input[i] <= 'z') || ('0' <= input[i] && input[i] <= '9'))){
                name_var[nvi++] = input[i++];
            }
            name_var[nvi] = '\0';

            value_var = get_env_value(name_var, env_list);

            if(value_var){
                for(vvi = 0; value_var[vvi]; vvi++){
                    result[ri++] = value_var[vvi];
                }
            }

            free(name_var);
        }
        else {
            result[ri++] = input[i++];
        }
    }
    result[ri] = '\0';
    return result;

}

void remove_env_var(EnvVar** head, const char* key){
    EnvVar* current = *head;
    EnvVar* prev = NULL;

    if(current == NULL){
        printf("현재 current는 NULL입니다\n");
    }

    while (current != NULL) {
        if (strcmp(current->name, key) == 0) {
            if (prev == NULL) {
                *head = current->next;
            } 
            else {
                prev->next = current->next;
                *head = prev;
            }
            printf("%s\n",(current)->name);
            free(current->name);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

void free_env_vars(EnvVar* env_list){

    EnvVar* prev = env_list;
    EnvVar* curr = env_list->next;

    while(curr){
        free(prev);
        prev = curr;
        curr = curr->next;
    }

    free(prev);
    free(curr);

}

// 사용자 입력 파싱하기
Command parse_single_command(char *input, EnvVar* env_list) {

    input = replace_env_vars(input, env_list);
    int capacity = 2;

    Command cmd;
    cmd.name = create_char_mem();
    cmd.args = (char**)malloc(sizeof(char*) * capacity);
    cmd.argc = 0;
    cmd.input_file = NULL;
    cmd.output_file = NULL;
    cmd.is_background = false;
    cmd.redirect_type = NO_REDIRECT;

    int len = strlen(input);
    int i = 0;
    while (i < len) {
        while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;
        if (i >= len) break;

        // 리디렉션 기호 확인
        if (input[i] == '<' || input[i] == '>') {
            int redirect_type = 0;
            if (input[i] == '<') {
                redirect_type = REDIRECT_INPUT;
                i++;
            } else if (input[i] == '>') {
                i++;
                if (input[i] == '>') {
                    redirect_type = REDIRECT_APPEND;
                    i++;
                } else {
                    redirect_type = REDIRECT_OUTPUT;
                }
            }

            while (i < len && (input[i] == ' ' || input[i] == '\t')) i++;

            char* filename = create_char_mem();
            int file_index = 0;
            while (i < len && input[i] != ' ' && input[i] != '\t') {
                filename[file_index++] = input[i++];
            }
            filename[file_index] = '\0';

            if (redirect_type == REDIRECT_INPUT) {
                cmd.input_file = filename;
            } else {
                cmd.output_file = filename;
            }
            cmd.redirect_type = redirect_type;
            continue;
        }

        // 일반 인자 처리
        char* arg = create_char_mem();
        int arg_index = 0;
        char quote = 0;

        if (input[i] == '"' || input[i] == '\'') {
            quote = input[i++];
            while (i < len && input[i] != quote) {
                arg[arg_index++] = input[i++];
            }
            if (i < len && input[i] == quote) {
                i++;
            }
        } else {
            while (i < len && input[i] != ' ' && input[i] != '\t' && input[i] != '>' && input[i] != '<') {
                arg[arg_index++] = input[i++];
            }
        }

        arg[arg_index] = '\0';

        if (cmd.argc == capacity) {
            capacity *= 2;
            cmd.args = realloc(cmd.args, sizeof(char*) * capacity);
        }

        cmd.args[cmd.argc] = arg;
        if (cmd.argc == 0) strcpy(cmd.name, arg);
        cmd.argc++;
    }

    if (strcmp(cmd.args[cmd.argc - 1], "&") == 0) {
        cmd.is_background = true;
        cmd.argc -= 1;
    }

    cmd.args = realloc(cmd.args, sizeof(char*) * (cmd.argc + 1));
    cmd.args[cmd.argc] = NULL;

    return cmd;
}



Command* parse_input(char *input, int* num_cmds, EnvVar* env_list) {
    int capacity = 2;
    *num_cmds = 0;

    Command* cmds = malloc(sizeof(Command) * capacity);
    if (!cmds) {
        fprintf(stderr, "Command 배열 메모리 할당 실패\n");
        exit(EXIT_FAILURE);
    }

    char* saveptr;
    char* token = strtok_r(input, "|", &saveptr);
    while (token != NULL) {
        // 앞뒤 공백 제거
        while (*token == ' ') token++;
        char* end = token + strlen(token) - 1;
        while (end >= token && (*end == ' ' || *end == '\n')) *end-- = '\0';

        // 토큰이 완전히 비어 있는 경우 무시
        if (strlen(token) == 0) {
            token = strtok_r(NULL, "|", &saveptr);
            continue;
        }

        if (*num_cmds == capacity) {
            capacity *= 2;
            cmds = realloc(cmds, sizeof(Command) * capacity);
            if (!cmds) {
                fprintf(stderr, "Command 배열 메모리 재할당 실패\n");
                exit(EXIT_FAILURE);
            }
        }

        cmds[*num_cmds] = parse_single_command(token, env_list);
        (*num_cmds)++;

        token = strtok_r(NULL, "|", &saveptr);
    }

    return cmds;
}

// Command 메모리 해제 함수
void free_command(Command* cmd) {
    free(cmd->name);
    for (int i = 0; i < cmd->argc; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd->input_file);
    free(cmd->output_file);
}

void free_commands(Command* cmds, int num_cmds) {
    for (int i = 0; i < num_cmds; i++) {
        free_command(&cmds[i]);
    }
    free(cmds);
}