#include <stdbool.h>
#include "command.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "strextra.h"

struct scommand_s {
    GSList* commands;
    char* redir_in;
    char* redir_out;
};

struct pipeline_s
{
    GSList* scommand;
    bool wait; // indica si el shell debe esperar la terminacion del comando antes de permitir ejecutar uno nuevo
};

scommand scommand_new(void) 
{   
    scommand result = malloc(sizeof(struct scommand_s));
    if (result == NULL) 
    {
        printf("No se pudo alojar memoria para new scommand.\n");
        exit(EXIT_FAILURE);
    }
    result->redir_in = NULL;
    result->redir_out = NULL;
    result->commands = NULL;
    assert(result != NULL && scommand_is_empty (result) && scommand_get_redir_in (result) == NULL && scommand_get_redir_out (result) == NULL);
    return result;
}

scommand scommand_destroy(scommand self)
{   
    assert(self != NULL);
    g_slist_free_full ((self->commands), g_free);
    free(self->redir_in);
    free(self->redir_out);
    self->commands = NULL;
    self->redir_in = NULL;
    self->redir_out = NULL;
    free(self);
    self = NULL;
    assert(self == NULL);
    return self;
}

void scommand_push_back(scommand self, char *argument) {
    assert(self!=NULL && argument!=NULL);
    self->commands = g_slist_append(self->commands, argument);
    assert(!scommand_is_empty(self));
}

void scommand_pop_front(scommand self) {
    assert(self != NULL && !scommand_is_empty(self));
    GSList *first = self->commands;
    char *elem = first->data;

    self->commands = g_slist_next(self->commands);
    
    g_slist_free_1(first);
    
    if (elem != NULL) free(elem);
}

void scommand_set_redir_in(scommand self, char * filename) {
    assert(self != NULL);
    if (self->redir_in != NULL) {
        free(self->redir_in);
    }
    self->redir_in = filename;
}

void scommand_set_redir_out(scommand self, char * filename) {
    assert(self != NULL);
    if (self->redir_out != NULL) {
        free(self->redir_out);
    }
    self->redir_out = filename;
}

bool scommand_is_empty(const scommand self) {
    assert(self!=NULL);
    bool is_empty = false;
    if(self->commands == NULL) is_empty = true;
    return is_empty;
}

unsigned int scommand_length(const scommand self) {
    assert(self!=NULL);
    unsigned int length = g_slist_length(self->commands);
    assert((length == 0 && scommand_is_empty(self)) || length > 0);
    return length;
}

char * scommand_front(const scommand self) {
    assert(self!=NULL && !scommand_is_empty(self));
    char *result = g_slist_nth_data(self->commands, 0); 
    assert(result != NULL);
    return result;
}

char * scommand_get_redir_in(const scommand self) {
    assert(self!=NULL);
    char *redir_in = self->redir_in;
    return redir_in;
}

char * scommand_get_redir_out(const scommand self) {
    assert(self!=NULL);
    char *redir_out = self->redir_out;
    return redir_out;
}

char * scommand_to_string(const scommand self) {
    assert(self!=NULL);
    char *result = strdup("");

    unsigned int length = scommand_length(self);

    char *temp;  // string actual

    for(unsigned int i = 0u; i < length; i++) {
        temp = result;  // string actual
        result = strmerge(result, g_slist_nth_data(self->commands, i));
        free(temp);  // free de la anterior
        temp = result; 
        result = strmerge(result, (i+1 == length ? "" : " "));
        free(temp);
    }

    if(scommand_get_redir_in(self)) {
        temp = result;
        result = strmerge(result, " < ");
        free(temp);
        temp = result;
        result = strmerge(result, scommand_get_redir_in(self));
        free(temp);
    }
    if(scommand_get_redir_out(self)) {
        temp = result;
        result = strmerge(result, " > ");
        free(temp);
        temp = result;
        result = strmerge(result, scommand_get_redir_out(self));
        free(temp);
    }

    return result;
}

/* Pipeline setup: */
pipeline pipeline_new(void) 
{
    pipeline result = malloc(sizeof(struct pipeline_s));
    if (result == NULL) 
    {
        printf("No se pudo alojar memoria para el pipeline.\n");
        exit(EXIT_FAILURE);
    }
    result->scommand = NULL;
    result->wait = true;
    assert(result != NULL && pipeline_is_empty(result) && pipeline_get_wait(result));
    return result;
}

pipeline pipeline_destroy(pipeline self)
{
    assert(self!=NULL);
    while (self->scommand != NULL){
        pipeline_pop_front(self);
    }
    free(self);
    self = NULL;
    return self;
}

void pipeline_push_back(pipeline self, scommand sc) 
{
    assert(self != NULL && sc != NULL);

    self->scommand = g_slist_append(self->scommand, sc);

    assert(!pipeline_is_empty(self));
}

void pipeline_pop_front(pipeline self)
{
    assert(self != NULL && !pipeline_is_empty(self));
    
    GSList *first = self->scommand;
    scommand elem = first->data;    
    elem = scommand_destroy(elem);    

    self->scommand = g_slist_delete_link(self->scommand, first);
}

void pipeline_set_wait(pipeline self, const bool w)
{
   assert(self!=NULL);
   self->wait = w;
}

bool pipeline_is_empty(const pipeline self)
{
    assert(self!=NULL);
    return self->scommand == NULL;
}


unsigned int pipeline_length(const pipeline self)
{
    assert(self!=NULL);
    unsigned int length = g_slist_length(self->scommand);
    assert((length == 0 && pipeline_is_empty(self)) || length > 0);
    return length;
}

scommand pipeline_front(const pipeline self)
{
    assert(self!=NULL && !pipeline_is_empty(self));
    scommand result = g_slist_nth_data(self->scommand, 0);
    assert(result != NULL);
    return result;
}

bool pipeline_get_wait(const pipeline self)
{
    assert(self!=NULL);
    return self->wait;
}

char * pipeline_to_string(const pipeline self)
{
    assert(self!=NULL);

    char * result = strdup("");

    unsigned int length = pipeline_length(self);

    char *temp;  // string actual

    for(unsigned int i = 0u; i < length; i++) {
        temp = result;  // string actual
        
        char *temp2 = scommand_to_string (g_slist_nth_data(self->scommand, i));

        result = strmerge(result, temp2);

        free(temp2);
    
        free(temp);  // free de la anterior
        
        temp = result; 
        result = strmerge(result, (i+1 == length ? "" : " | "));   
        free(temp);
    }

    if(!pipeline_get_wait(self)) {
        temp = result;  // string actual
        result = strmerge(result, " &");
        free(temp);  // free de la anterior
    }

    return result;
}


