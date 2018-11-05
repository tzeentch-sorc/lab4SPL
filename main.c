#include <stdio.h>
#include <limits.h>
#include "solist.h"

#define FILENAME "saved"

//functions
T sqr(const T* arg) { return *arg * *arg;}
T cube(const T* arg) { return *arg * *arg * *arg;}
T absolute(const T* arg) { return *arg < 0 ? -*arg : *arg; }
T Num_x2(const T* arg) { return *arg * 2; }

void print_WSP(T *arg) { printf("%d ", *arg); }
void print_WNL(T *arg) { printf("%d\n", *arg); }

void sum(const T *number, T *acc) { *acc = *number + *acc; }
void min(const T *number, T *acc) { *acc= *acc < *number ? *acc : *number; }
void max(const T *number, T *acc) { *acc = *acc > *number ? *acc : *number; }

void list_print_msg(const struct list* list1, const char* out_msg, void(*function)(T *)){
    printf("%s\n", out_msg);
    list_foreach(list1, function);
    printf("\n");
}

int main() {
    struct list* solist = list_read(stdin, 0);
    list_print_msg(solist, "list with \' \': ", print_WSP);
    list_print_msg(solist, "list with \\n: ", print_WNL);

    struct list* solistSqr = list_map(solist, sqr);
    list_print_msg(solistSqr, "list squares:", print_WSP);
    list_free(solistSqr);

    struct list* solistCubes = list_map(solist, cube);
    list_print_msg(solistCubes, "list cubes:", print_WSP);
    list_free(solistCubes);

    printf("Sum of list with foldl: %d\n", list_foldl(0, sum, solist));
    printf("Min of list: %d\n", list_foldl(INT_MAX, min, solist));
    printf("Max of list: %d\n", list_foldl(INT_MIN, max, solist));

    list_map_mut(solist, absolute);
    list_print_msg(solist, "absolute values: ", print_WSP);

    struct list* iterated_list = list_iterate(1, 10, Num_x2);
    list_print_msg(iterated_list, "pows of 2: ", print_WSP);
    list_free(iterated_list);

    list_save(solist, FILENAME);
    list_free(solist);
    list_load(&solist,FILENAME);
    list_print_msg(solist, "saved list - and loaded: ", print_WSP);

    serialize(solist, FILENAME);
    list_free(solist);
    deserialize(&solist,FILENAME);
    list_print_msg(solist, "saved list - and loaded(binary): ", print_WSP);

    list_free(solist);
    return 0;
}