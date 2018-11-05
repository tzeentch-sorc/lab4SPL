#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include "solist.h"



node* node_create(T number){
    node* n = malloc(sizeof(node*)); //initialize structure
    n->nextElem = NULL; //set a pointer to next node as null
    n->elemValue = malloc(sizeof(T*)); //allocate memory for value
    *n->elemValue = number; //set the value
    return n;
}

void delete_node(node* n){
    if(!n) return;
    free(n->elemValue);
    free(n);
}

struct list* list_create(){
    struct list* listEnt = malloc(sizeof(struct list*));
    listEnt->firstElem = NULL;
    return listEnt;
}

void list_add_front(T number, struct list* list1){
    if(!list1) return;
    node* n = node_create(number);
    if (list1->firstElem)
        n->nextElem = list1->firstElem;
    list1->firstElem = n;
}

void list_add_back(T number, struct list* list1){
    if(!list1) return;
    node* n = node_create(number);
    if(!list1->firstElem){ //if list is empty just add elem as 1st
        list1->firstElem = n;
        return;
    }
    node* current = list1->firstElem;
    while(current->nextElem)
        current = current->nextElem;
    current->nextElem = n;
}

size_t list_length(const struct list* list1){
    if(!list1) return 0;
    size_t counter = 0;
    node* current = list1->firstElem;
    if(!current) return counter;
    ++counter;
    while(current->nextElem) {
        current = current->nextElem;
        ++counter;
    }
    return counter;
}

T list_get(size_t pos, const struct list* list1){
    if((!list1)||(pos > list_length(list1) - 1))
        return 0;
    node* n = list1->firstElem;
    for(size_t i = 0; i<pos; ++i){
        n = n->nextElem;
    }
    return *n->elemValue;
}

void list_free(struct list* list1){
    if(!list1) return;
    node* n = list1->firstElem;
    if(n) {
        while (n) {
            node* temp = n; //save current node
            n = n->nextElem; //switch to next node
            delete_node(temp); //delete current node
        }
        delete_node(n);
    }
    free(list1);
}

struct list* list_node_at(size_t pos, const struct list* list1){
    if((!list1)||(pos > list_length(list1) - 1))
        return NULL;
    struct list* res = malloc(sizeof(struct list*));
    if(!pos){
        memcpy(res, list1, sizeof(struct list)); //list1 is const list* and we don`t need to change it. Certainly do not.
        return res;
    }
    node* n = list1->firstElem;
    for (size_t i = 0; i < pos; ++i) {
        n = n->nextElem;
    }
    memcpy(res, list1, sizeof(struct list));
    res->firstElem = n;
    return res;

}

T list_sum(const struct list* list1){
    T sum = 0;
    if(!list1||!list1->firstElem) return sum;
    node* current = list1->firstElem;
    sum+=*current->elemValue; //add 1st elem to the sum
    while(current->nextElem) {
        current = current->nextElem;
        sum+=*current->elemValue;
    }
    return sum;
}

void list_foreach(const struct list *list1, void (*function)(T*)){
    if(!list1 || !function || !list1->firstElem) return; //check if smth is null
    node* n = list1->firstElem;
    while (n->nextElem){
        function(n->elemValue);
        n = n->nextElem;
    }
    (*function)(n->elemValue); //invoke func for the last elem
}

struct list* list_read(FILE* filename, int addFront){
    struct list* list1 = list_create();
    T number = 0;
    int symb = 'a';
    do{
        fscanf(filename, "%d", &number);
        if(addFront){
            list_add_front(number, list1);
        } else list_add_back(number, list1);
        symb =fgetc(filename);
    } while(symb != '\n' && symb != EOF && symb !='\r');
    return list1;
}

struct list* list_map(const struct list* list1, T (*function)(const T*)){
    if(!list1 || !function || !list1->firstElem) return NULL; //check if smth is null
    struct list* changed_list = list_create();
    node* n = list1->firstElem;
    while(n->nextElem) {
        list_add_back(function(n->elemValue), changed_list);
        n = n->nextElem;
    }
        list_add_back(function(n->elemValue), changed_list);
    return changed_list;
}

T list_foldl(T acc_start, void (*function)(const T*, T*), struct list* list1){
    T accum = acc_start;
    if(!list1 || !function || !list1->firstElem) return accum;
    node* n = list1->firstElem;
    while(n->nextElem) {
        function(n->elemValue, &accum);
        n = n->nextElem;
    }
    function(n->elemValue, &accum);
    return accum;
}

void list_map_mut(const struct list* list1, T (*function)(const T*)){
    if(!list1 || !function || !list1->firstElem) return; //check if smth is null
    node* n = list1->firstElem;
    while(n->nextElem) {
        *n->elemValue = function(n->elemValue);
        n = n->nextElem;
    }
    *n->elemValue = function(n->elemValue);
}

struct list* list_iterate(T startVal, size_t size, T (*function)(const T *)){
    struct list* result = list_create();
    T temp = startVal;
    for(size_t i = 0; i < size; ++i){
        list_add_back(temp,result);
        temp = function(&temp);
    }
    return result;
}

bool list_save(struct list* list1, const char* filename){
    if (!list1 || !list1->firstElem)
        return false;
    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return false;
    node* n = list1->firstElem;
    while (n->nextElem){
        fprintf(file, "%d ", *n->elemValue);
        n = n->nextElem;
    }
    fprintf(file, "%d", *n->elemValue);
    fclose(file);
    if(ferror(file))
        return false;
    return true;
}

bool serialize(struct list* list1, const char* filename){
    if (!list1 || !list1->firstElem)
        return false;
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
        return false;
    node* n = list1->firstElem;
    while (n->nextElem){
        fprintf(file, "%d ", *n->elemValue);
        n = n->nextElem;
    }
    fprintf(file, "%d", *n->elemValue);
    fclose(file);
    if(ferror(file))
        return false;
    return true;
}

bool list_load(struct list** list1, const char* filename){
    FILE *file = fopen(filename, "r");
    if (file == NULL)
        return false;
    *list1 = list_read(file, 0);
    fclose(file);
    if(ferror(file))
        return false;
    return true;
}

bool deserialize(struct list** list1, const char* filename){
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return false;
    *list1 = list_read(file, 0);
    fclose(file);
    if(ferror(file))
        return false;
    return true;
}