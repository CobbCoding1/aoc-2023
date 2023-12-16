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

size_t maximum(size_t nums[6]) {
    size_t max = 0;
    for(size_t i = 0; i < 6; i++) {
        if(nums[i] > max) {
            max = nums[i];
        }
    }
    return max;
}

size_t lcm(size_t nums[6]) {
    size_t step = 0;
    size_t max = 0;

    step = maximum(nums);
    max = step;

    while(1) {
        size_t mod = 6;
        for(size_t i = 0; i < 6; i++) {
            if(max % nums[i] == 0) {
                mod--; 
            }
        }
        if(mod == 0) {
            break;
        }
        if(max % 100000000 == 0) {
            printf("%zu\n", max);
        }
        max += step;
    }
    
    return max;
}

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
    char starting_idents[6][4] = {0};
    size_t starting_len = 0;
    int lr = 0;
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
            if(isalpha(contents[i]) || isdigit(contents[i])) {
                char ident[4] = {0};
                size_t ident_len = 0;
                while(isalpha(contents[i]) || isdigit(contents[i])) {
                    ident[ident_len++] = contents[i];
                    i++;
                }
                i += 4;
                ident[ident_len] = '\0';
                char left[4] = {0};
                size_t left_len = 0;
                while(isalpha(contents[i]) || isdigit(contents[i])) {
                    left[left_len++] = contents[i];
                    i++;
                }
                i += 2;
                left[left_len] = '\0';
                
                char right[4] = {0};
                size_t right_len = 0;
                while(isalpha(contents[i]) || isdigit(contents[i])) {
                    right[right_len++] = contents[i];
                    i++;
                }
                right[right_len] = '\0';
                Element *elem = malloc(sizeof(Element));
                memcpy(elem->name, ident, 4);
                memcpy(elem->left, left, 4);
                memcpy(elem->right, right, 4);
                //printf("name: %s, right: %s, left: %s\n", ident, right, left);
                if(put_in_map(hashmap, ident, elem) != 0) {
                    assert(0);
                }
                if(ident[2] == 'A') {
                    strcpy(starting_idents[starting_len++], ident);
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

    Element *curs[6] = {0};
    size_t curs_size = 0;
    for(size_t i = 0; i < starting_len; i++) {
        curs[curs_size++] = get_from_map(hashmap, starting_idents[i]);
        if(curs[curs_size - 1] == NULL) {
            assert(0);
        }
        printf("%s\n", curs[curs_size - 1]->name);
    }

    size_t inst_index = 0;
    //Element *cur = &map[0];
    size_t counter = 0;
    size_t counters[6] = {0};
    while(1) {
        int is_end = 0;
        for(size_t i = 0; i < curs_size; i++) {
            if(curs[i]->name[2] == 'Z' && counters[i] == 0) {
                counters[i] = counter;
                //printf("name: %s ", curs[i]->name);
                is_end++;
            }
            if(is_end > 3) {
                return 0;
            }
        }
        int stuff = 0;
        for(size_t i = 0; i < 6; i++) {
            if(counters[i] > 0) {
                stuff++;
            }
        }
        if(stuff == 6) {
            for(size_t i = 0; i < 6; i++) {
                if(counters[i] > 0) {
                    printf("%zu\n", counters[i]);
                }
            }
            break;
        }
        if((size_t)is_end == (curs_size)) {
            for(size_t i = 0; i < curs_size; i++) {
                printf("%s\n", curs[i]->name);
            }
            printf("%zu\n", counter);
            break;
        }

        if(counter % 5000000 == 0) {
            for(size_t i = 0; i < curs_size; i++) {
                //printf("%s ", curs[i]->name);
            }
            //printf("%zu\n", counter);
        }
        int cur_inst = rl_insts[inst_index];
        if(inst_index == (rl_len)) {
            inst_index = 0;
        }

        if(cur_inst == 0) {
            for(size_t i = 0; i < curs_size; i++) {
                curs[i] = get_from_map(hashmap, curs[i]->left);
                if(curs[i] == NULL) {
                    assert(0);
                }
            }
        } else if(cur_inst == 1) {
            for(size_t i = 0; i < curs_size; i++) {
                curs[i] = get_from_map(hashmap, curs[i]->right);
                if(curs[i] == NULL) {
                    assert(0);
                }
            }
        }
        inst_index++;
        counter++;
    }
    size_t answer = lcm(counters);
    printf("%zu\n", answer);
    delete_and_free_map(hashmap);
    return 0;
}
