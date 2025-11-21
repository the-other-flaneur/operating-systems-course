#include <stdbool.h>
#include "command.h"
#include <assert.h>
#include <stdlib.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "builtin.h"
#include "tests/syscall_mock.h"

static const char *builtInCommands[] =
{
    "cd",
    "help",
    "exit",
    NULL
};

//se piden solo 3 comandos internos cmd, help, exit
bool builtin_is_internal(scommand cmd)
{
    assert(cmd != NULL);
    bool isInternal = false;
    // si cmd es un comando interno devolver true y devolver false sea otro el caso
    unsigned int i = 0;
    while (builtInCommands[i] != NULL)
    {   
        if (strcmp(builtInCommands[i], scommand_front(cmd)) == 0) { isInternal = true; } i++;
    }
    return isInternal;
}

bool builtin_alone(pipeline p)
{
    assert(p != NULL);
    bool result = pipeline_length(p) == 1 && builtin_is_internal(pipeline_front(p));
    return result;
}

// implementa cd
static void builtin_cd(scommand cmd) {
    scommand_pop_front(cmd);

    const char *path = scommand_front(cmd);

    int result = chdir(path);

    if (result == -1)
    {
        fprintf(stderr, "Error cambiando de directorio.");
    }
}

// implementa cd
static void builtin_exit(scommand cmd) {
    exit(EXIT_SUCCESS);
}

// implementa cd
static void builtin_help(void) {
    char *title = "\n"
    " \033[1m\n"
    " *      __  __         ____            _     _            _ _             \n"
    " *     |  \\/  |       |  _ \\          | |   (_)          (_) |            \n"
    " *     | \\  / |_   _  | |_) | __ _ ___| |__  _ _ __   ___ _| |_ ___  _ __ \n"
    " *     | |\\/| | | | | |  _ < / _` / __| '_ \\| | '_ \\ / _ \\ | __/ _ \\| '__|\n"
    " *     | |  | | |_| | | |_) | (_| \\__ \\ | | | | | | |  __/ | || (_) | |   \n"
    " *     |_|  |_|\\__, | |____/ \\__,_|___/_| |_|_|_| |_|\\___|_|\\__\\___/|_|   \n"
    " *              __/ |                                                     \n"
    " *             |___/                                                      \n";


    // mensaje por salida estandar
    // nombre del shell
    fprintf(stdout, "%s.\n", title);

    // nombre de los autores 
    fprintf(stdout, "\033[33m\tAutores:\n\t\tFelipe Tomas Pagani\n\t\tMauro Elias Centurion\n \t\tBraian David Rodriguez\n");

    // comandos internos con descripcion de cada uno
    printf("\t\033[33mComandos Internos: \n\t\tcd: Cambia de directorio.\n\t\thelp: Imprime este mensaje.\n\t\texit: terminar el shell.\n\n");
}

void builtin_run(scommand cmd)
{
    assert(builtin_is_internal(cmd));

    char *cmd_string = scommand_front(cmd);

    if (strcmp(cmd_string, "cd") == 0) builtin_cd(cmd);
    else if (strcmp(cmd_string, "exit") == 0) builtin_exit(cmd);
    else if (strcmp(cmd_string, "help") == 0) builtin_help();
    else fprintf(stderr, "Comando builtin no encontrado.\n");
}