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

static void list_insert_last_node(ioopm_list_t *list, ioopm_list_element_t *element){
    list->last->tail = element;
    list->last = element;
    list->size ++;
}

static void list_insert_first_node(ioopm_list_t *list, ioopm_list_element_t *element){
    element->tail = list->first;
    list->first = element;
    list->size ++;
}

static ioopm_list_element_t *find_previous_node(ioopm_list_t *list, int index){
    ioopm_list_element_t *previous = list->first;
    for(int i = 0; i < index - 1 ; i ++){
        previous = previous->tail;
    }
    return previous;
}



void ioopm_list_append(ioopm_list_t *list, int value){
    ioopm_list_element_t *new = calloc(sizeof(ioopm_list_element_t), 1);
    new->value = value;

    if(list->size == 0){
        list_first_node_create(list, new);
        return;
    }else{
        list_insert_last_node(list, new);
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
    int size = list->size;
    if(index > size || index < 0){
        return;
    }
    ioopm_list_element_t *new = calloc(sizeof(ioopm_list_element_t), 1);
    new->value = value;

    if(index == 0) {
        list_insert_first_node(list, new);
        return;
    }
    else if(index == size) {
        list_insert_last_node(list, new);
        return;
    }

    ioopm_list_element_t *current = find_previous_node(list, index);
    new->tail = current->tail;
    current->tail = new;
    list->size ++;  
}

bool ioopm_list_remove(ioopm_list_t *list, int index, int *result){
    int size = list->size;
    if(index < 0 || index >= size){
        return false;
    }

    ioopm_list_element_t *current = list->first;
    if(index == 0){
        *result = current->value;
        list->first = current->tail;
        free(current);
        list->size --;
        if(list->size == 0){
            list->last = NULL;
        }
        return true;
    }
    
    current = find_previous_node(list, index);
    ioopm_list_element_t *to_remove = current->tail;
    if(index == size - 1){
        list->last = current;
        *result = to_remove->value; 
        free(to_remove);
        current->tail = NULL;
        list->size --;
        return true;
    }

    current->tail = to_remove->tail;
    *result = to_remove->value;
    free(to_remove);
    list->size --;
    return true;
}

bool ioopm_list_get(ioopm_list_t *list, int index, int *result){
    if(index < 0 || index >= list->size){
        return false;
    }
    else if(index == 0){
        *result = list->first->value;
        return true;
    }

    ioopm_list_element_t *element = find_previous_node(list, index);
    *result = element->tail->value;
    return true;
}



