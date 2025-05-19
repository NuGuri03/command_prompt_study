#include "parser.h"

int handle_internal_command(Command cmd, EnvVar* env_list);
int execute_commands(Command* cmds, int num_cmds, EnvVar* env_list);