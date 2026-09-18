#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include "linked_list.h"

struct ioopm_list_element{
    int value;
    ioopm_list_element_t *tail;
};

struct list{
    ioopm_list_element_t *first;
    ioopm_list_element_t *last;
    int size;
};

ioopm_list_t *ioopm_list_create(void){
    return calloc(sizeof(ioopm_list_t), 1);
}

void ioopm_list_destroy(ioopm_list_t *list){
    ioopm_list_element_t *element = list->first;
    if(element == NULL){
        free(list);
        return;
    }
    list->first = element->tail;
    free(element);
    ioopm_list_destroy(list);
}

static void list_first_node_create(ioopm_list_t *list, ioopm_list_element_t *element){
    list->first = element;
    list->last = element;
    list->size ++;
}

void ioopm_list_append(ioopm_list_t *list, int value){
    ioopm_list_element_t *new = calloc(sizeof(ioopm_list_element_t), 1);
    new->value = value;

    if(list->size == 0){
        list_first_node_create(list, new);
        return;
    }else{
        ioopm_list_element_t *last = list->last;
        last->tail = new;
        list->last = new;
        list->size ++;
    }
}

void ioopm_list_prepend(ioopm_list_t *list, int value){
    ioopm_list_element_t *new = calloc(sizeof(ioopm_list_element_t), 1);
    new->value = value;

    if(list->size == 0){
        list_first_node_create(list, new);
        return;
    }
    else{
        ioopm_list_element_t *first = list->first;
        new->tail = first;
        list->first = new;
        list->size ++;
    }
}

int ioopm_list_head(ioopm_list_t *list){
    return list->first->value;
}

int ioopm_list_last(ioopm_list_t *list){
    return list->last->value;
}

void ioopm_list_insert(ioopm_list_t *list, int index, int value){
    if(index > list->size || index < 0){
        return;
    }
    ioopm_list_element_t *new = calloc(sizeof(ioopm_list_element_t), 1);
    

}
