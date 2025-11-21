#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "parsing.h"
#include "parser.h"
#include "command.h"

static scommand parse_scommand(Parser p) {
    /* Devuelve NULL cuando hay un error de parseo */

    // crea instancia de simple-command para almacenar el resultado.
    scommand cmd = NULL;
    cmd = scommand_new();

    // tomar la primera salida del parser.
    arg_kind_t type;
    char *arg = parser_next_argument(p, &type);

    bool found_redir_in = false, found_redir_out = false;

    if(arg == NULL) return NULL;
    
    while(arg != NULL) {
        found_redir_in = type == ARG_INPUT;
        found_redir_out = type == ARG_OUTPUT;

        if(type == ARG_NORMAL) {
            scommand_push_back(cmd, arg);
        } else if(type == ARG_INPUT && found_redir_in) {
            scommand_set_redir_in(cmd, arg);
        } else if(type == ARG_OUTPUT && found_redir_out) {
            scommand_set_redir_out(cmd, arg);
        }
        arg = parser_next_argument(p, &type);
    }

    if(found_redir_in && !scommand_get_redir_in(cmd)) {
        printf("syntax error near unexpected token `newline'\n");
        cmd = scommand_destroy(cmd);
    }
    if(found_redir_out && !scommand_get_redir_out(cmd)) {
        printf("syntax error near unexpected token `newline'\n");
        cmd = scommand_destroy(cmd);
    }

    return cmd;
}

pipeline parse_pipeline(Parser p) {
    assert(p != NULL && !parser_at_eof(p));
    scommand cmd = NULL;
    bool backgr = false;    // indica si el pipeline contiene '&' como operador para ejecutar en segundo plano
    bool error = false, another_pipe=true;

    pipeline result = NULL;

    cmd = parse_scommand(p);
    error = (cmd==NULL); 
    
    if(!error) result = pipeline_new();

    while (another_pipe && !error) {
        pipeline_push_back(result, cmd); // agrega el comando cmd al final del pipeline.
        parser_op_background(p, &backgr); // indica si se encontro el operador y lo consume sin modificar otros simbolos de la entrada, si lo encuentra backgr = true.
        parser_op_pipe(p, &another_pipe);
        cmd = parse_scommand(p);
        error = (cmd==NULL); 

        if (another_pipe && (error || backgr))
        {
            printf("syntax error near unexpected token | \n");
            result = NULL;
        }
    }
    
    // pipeline por default tiene wait en true.
    if(backgr && !another_pipe) pipeline_set_wait(result, !backgr); 

    bool garbage;
    parser_garbage(p, &garbage);
	
	if (garbage){
		char *garbage_str = parser_last_garbage(p);
		if (garbage_str != NULL){
			fprintf(stderr,"Warning: Garbage found: %s\n", garbage_str);
	    }
    }

    return result; 
}

