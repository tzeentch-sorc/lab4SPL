#ifndef _SOLIST_H
#define _SOLIST_H

#include <stdio.h>
#include <stdbool.h>

typedef int T;

typedef struct node{
    struct node* nextElem;
    T* elemValue;
} node;

typedef struct list{
    node* firstElem;
};



struct list* list_create();

node* node_create(T);

void delete_node(node*);

void list_add_front(T, struct list*);

void list_add_back(T, struct list*);

T list_get(size_t, const struct list*);

void list_free(struct list*);

size_t list_length(const struct list*);

struct list* list_node_at(size_t, const struct list*);

T list_sum(const struct list*);

void list_foreach(const struct list*, void (*function)(T*));

struct list* list_read(FILE*, int);

struct list* list_map(const struct list*, T (*function)(const T*));

T list_foldl(T, void (*function)(const T*, T*), struct list*);

void list_map_mut(const struct list*, T (*function)(const T*));

struct list* list_iterate(T, size_t, T (*function)(const T *));

bool list_save(struct list* list1, const char* filename);

bool list_load(struct list** list1, const char* filename);

bool serialize(struct list* lst, const char* filename);

bool deserialize(struct list** lst, const char* filename);

#endif