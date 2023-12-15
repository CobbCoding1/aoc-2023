#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define HASHMAP_SIZE 1024
#define HASHMAP_IMPLEMENTATION
#include "hashmap.h"

#define TEST_INPUT "testinput.txt"
#define TEST_TEST_INPUT "testtestinput.txt"
#define INPUT "input.txt"

char *read_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    int64_t length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *contents = malloc(sizeof(char) * length + 1);
    fread(contents, 1, length, file);
    contents[length] = '\0';
    return contents;
}   

int rl_insts[1024] = {0};
size_t rl_len = 0;

typedef struct {
    char name[4];
    char left[4];
    char right[4];
} Element;

//Element map[1024];
size_t map_len = 0;

size_t find_in_map(char *str) {
    /*
    for(size_t i = 0; i < map_len; i++) {
        if(strcmp(str, map[i].name) == 0) {
            return i;
        }
    }
    */
    return 0;
}

int main() {
    Map *hashmap = malloc(sizeof(Map));
    init_map(hashmap);
    char *contents = read_from_file(INPUT);
    char starting_ident[4] = {0};
    int lr = 0;
    int starting = 1;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            lr = 1;
        }
        if(lr == 0) {
            if(contents[i] == 'R') {
                rl_insts[rl_len++] = 1;
            } else if(contents[i] == 'L') {
                rl_insts[rl_len++] = 0;
            }
        } else if(lr == 1) {
            if(isalpha(contents[i])) {
                char ident[4] = {0};
                size_t ident_len = 0;
                while(isalpha(contents[i])) {
                    ident[ident_len++] = contents[i];
                    i++;
                }
                i += 4;
                ident[ident_len] = '\0';
                char left[4] = {0};
                size_t left_len = 0;
                while(isalpha(contents[i])) {
                    left[left_len++] = contents[i];
                    i++;
                }
                i += 2;
                left[left_len] = '\0';
                
                char right[4] = {0};
                size_t right_len = 0;
                while(isalpha(contents[i])) {
                    right[right_len++] = contents[i];
                    i++;
                }
                right[right_len] = '\0';
                Element *elem = malloc(sizeof(Element));
                memcpy(elem->name, ident, 4);
                memcpy(elem->left, left, 4);
                memcpy(elem->right, right, 4);
                if(put_in_map(hashmap, ident, elem) != 0) {
                    assert(0);
                }
                if(starting) {
                    strcpy(starting_ident, ident);
                    starting = 0;
                }
                //map[map_len++] = *elem;
            }
        }
    }
    /*
    for(size_t i = 0; i < HASHMAP_SIZE; i++) {
        Hashmap_Node *node = hashmap->hashmap[i];
        if(node == NULL) {
            continue;
        }
        while(node != NULL){
            printf("%s: left->%s, right->%s -> ", node->key, ((Element*)node->value)->left, ((Element*)node->value)->right);
            node = node->next;
        }
        printf("NULL\n");
    }
    */

    Element *cur = get_from_map(hashmap, "AAA");
    if(cur == NULL) {
        assert(0);
    }
    printf("%s\n", cur->name);

    size_t inst_index = 0;
    //Element *cur = &map[0];
    size_t counter = 0;
    while(1) {
        if(strcmp(cur->name, "ZZZ") == 0) {
            printf("%zu\n", counter);
            break;
        }
        if(counter % 5000000 == 0) {
            printf("counter: %zu, name: %s\n", counter, cur->name);
        }
        int cur_inst = rl_insts[inst_index];
        if(inst_index == (rl_len)) {
            //printf("%s\n", cur->name);
            inst_index = 0;
        }
        //printf("%s\n", cur->name);

        if(cur_inst == 0) {
            cur = get_from_map(hashmap, cur->left);
            if(cur == NULL) {
                assert(0);
            }
        } else if(cur_inst == 1) {
            cur = get_from_map(hashmap, cur->right);
            if(cur == NULL) {
                assert(0);
            }
        }
        inst_index++;
        counter++;
    }
    delete_and_free_map(hashmap);
    return 0;
}
