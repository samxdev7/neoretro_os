/* DYNAMIC.H - Another useful dynamic memory tool */
#ifndef DYNAMIC_H
#define DYNAMIC_H

#include <conio.h>
#include <graphics.h>
#include <stdlib.h>

void free_element(void *ptr);
void free_dynamic_array(void **ptr, short n_elements);
void alloc_deploy_options(char ***deploy_opt, short n_option);

void free_element(void *ptr) {
    if (ptr != NULL) 
        free(ptr);
}

void free_dynamic_array(void **ptr, short n_elements)
{
    unsigned short i; 

    if (ptr == NULL) 
        return;

    for (i = 0; i < n_elements; i++) {    
        if (ptr[i] == NULL) 
            return;

        free(ptr[i]);
        ptr[i] = NULL;
    }

    free(ptr);
    ptr = NULL;
}

/*
 * This function is used for create and show a deploy menu,
 * creating it with dynamic options that the developer
 * can insert.
 */
void alloc_deploy_options(char ***deploy_opt, short n_option)
{
    unsigned short i;

    *deploy_opt = (char **) malloc(n_option * sizeof(char *));

    for (i = 0; i < n_option; i++)
        (*deploy_opt)[i] = (char *) malloc(20 * sizeof(char));
}
#endif