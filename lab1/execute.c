#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>

#include "command.h"
#include "builtin.h"
#include "execute.h"

#include "tests/syscall_mock.h" // para usar mock syscalls

unsigned int num_done;

static void reap_child(int unused) {
    while(true) {
        pid_t pid = waitpid(-1, NULL, WNOHANG);
        if(pid <= 0) break;
        num_done++;
    }
}

static char ** get_commands(scommand cmd) {
    unsigned int length = scommand_length(cmd);
    char ** result = calloc(length + 1, sizeof(char*)); 

    for(unsigned int i = 0u; i < length; i++) {
        const char* front = scommand_front(cmd);
        result[i] = strdup(front); 
        scommand_pop_front(cmd);
    }

    result[length] = NULL; 

    return result;
}

static void execute_scommand(scommand cmd)
{   
    char **args = get_commands(cmd);

        // si el comando tiene redireccion de entrada
    if (scommand_get_redir_in(cmd))
    {   
    // abrir el archivo en lectura 
    int fd_in = open(scommand_get_redir_in(cmd), O_RDONLY, 0644);
    if (fd_in  < 0) {
        perror(scommand_get_redir_in(cmd));
        exit(EXIT_FAILURE);
    }

        // redireccionar stdin a archivo de entrada
        if (dup2(fd_in, STDIN_FILENO) == -1) {
            perror("dup2:");
            exit(EXIT_FAILURE);
        }

        close(fd_in);
    }

    // si el comando tiene redireccion de salida
    if (scommand_get_redir_out(cmd))
    {   
        int fd_out = open(scommand_get_redir_out(cmd), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        // abrir el archivo en escritura 
        if (fd_out  < 0) {
            perror(scommand_get_redir_out(cmd));
            exit(EXIT_FAILURE);
        }

        // redireccionar stdout a archivo de salida
        if (dup2(fd_out, STDOUT_FILENO) == -1) {
            perror("dup2:");
            exit(EXIT_FAILURE);
        }

        close(fd_out);
    }

    int result = execvp(args[0], args);
    if(result == -1) {perror(args[0]);exit(EXIT_FAILURE);}

    unsigned int i = 0u;

    while(args[i++]) free(args[i]);
    args = NULL;  
}

void execute_pipeline(pipeline apipe)
{
    assert(apipe != NULL);

    // si la pipeline no tiene commandos para ejecutar
    if (pipeline_is_empty(apipe)) {
        return;
    }

    num_done = 0;   // numero de procesos hijos que terminaron

    scommand cmd = pipeline_front(apipe); // primer scommand del pipeline

    // caso $ BUILTIN ... 
    if (builtin_is_internal(cmd)) builtin_run(cmd);

    // caso $ SCOMMAND | ... | SCOMMAND | ... 
    else if (!builtin_is_internal(cmd)) {
        unsigned int i, n;
        int prev_pipe, pfds[2];

        n = pipeline_length(apipe);
        prev_pipe = STDIN_FILENO;

        signal(SIGCHLD, reap_child);

        for (i = 0; i < n - 1; i++) {
            if (pipe(pfds) == -1) {
                perror("pipe");
                exit(EXIT_FAILURE);
            }

            int pid = fork();
            if (pid < 0) {
                perror("fork");
                exit(EXIT_FAILURE);
            } else if (pid == 0) {
                if (prev_pipe != STDIN_FILENO) {
                    if (dup2(prev_pipe, STDIN_FILENO) == -1) {
                        perror("dup2 stdin");
                        exit(EXIT_FAILURE);
                    }
                    close(prev_pipe);
                }
                if (dup2(pfds[1], STDOUT_FILENO) == -1) {
                    perror("dup2 stdout");
                    exit(EXIT_FAILURE);
                }
                close(pfds[0]);
                close(pfds[1]);

                execute_scommand(cmd);
                exit(EXIT_FAILURE);  // In case execute_scommand fails
            } else {
                // padre
                if (prev_pipe != STDIN_FILENO) {
                    close(prev_pipe);
                }
                close(pfds[1]);
                prev_pipe = pfds[0];

                // si opera en background imprimir numero de trabajo y pid del hijo corriendolo.
                if (!pipeline_get_wait(apipe) && i > 0) printf("[%d]  %d \n", i + 1, pid); 

                pipeline_pop_front(apipe);
                cmd = pipeline_front(apipe);
            }
        }   


        // caso ... | SCOMMAND /n
        int ult_pid = fork();
        if (ult_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (ult_pid == 0) {
            if (prev_pipe != STDIN_FILENO) {
                if (dup2(prev_pipe, STDIN_FILENO) == -1) {
                    perror("dup2 stdin (last child)");
                    exit(EXIT_FAILURE);
                }
                close(prev_pipe);
            }
            execute_scommand(cmd);
            exit(EXIT_FAILURE);  
        } else {
            // padre 
            if (prev_pipe != STDIN_FILENO) close(prev_pipe);            

            if (pipeline_get_wait(apipe))
                for (unsigned int i = 0; i < n; i++) wait(NULL);
            else 
                printf("[%d]  %d \n", n, ult_pid); 
        }
    }
}