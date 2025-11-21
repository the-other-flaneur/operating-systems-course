#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pwd.h>

#include "command.h"
#include "execute.h"
#include "parser.h"
#include "parsing.h"
#include "builtin.h"

#include "obfuscated.h"

static void show_prompt(void) {
    char path_dir[256];
    char hostname[256];
    char *user;
    struct passwd *pw;

    pw = getpwuid(getuid());
    if (pw) user = pw->pw_name;
    else user = "unknown";

    gethostname(hostname, sizeof(hostname));

    getcwd(path_dir, sizeof(path_dir));

    printf("\033[1;32m%s@\033[34m%s\033[0m:\033[33m%s\033[0m/$ ", user, hostname, path_dir);

    fflush(stdout);
}

int main(int argc, char *argv[]) {
    pipeline pipe;
    Parser input;
    bool quit = false;

    input = parser_new(stdin);
    while (!quit) {
        // ping_pong_loop(NULL);
        show_prompt();
        pipe = parse_pipeline(input);

        quit = parser_at_eof(input); /* Hay que salir luego de ejecutar? */

        if (pipe && !pipeline_is_empty(pipe))
        {   
            // quit = quit || builtin_is_exit(pipe);
            execute_pipeline(pipe);
            pipeline_destroy(pipe);
        }
    }
    parser_destroy(input); input = NULL;
    return EXIT_SUCCESS;
}

