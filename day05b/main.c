#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TEST_INPUT "testinput.txt"
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

typedef struct {
    int64_t value;
    int64_t is_converted;
} Seeds;

typedef struct {
    int64_t original;
    int64_t converted;
    int64_t is_converted;
} Value;

typedef struct {
    Value conversions[8][256];
    size_t conversions_len[8];
    Value global_stacks[8][128];
    size_t global_stack_size[8];
} Map;

typedef struct {
    int64_t range[3][16];
    size_t range_len[3];
} Table;

int64_t is_in_stack(int64_t value, Value arr[128]) {
    for(size_t i = 0; i < 128; i++) {
        if(arr[i].original == value) {
            return 1;
        }
    }
    return 0;
}

void calculate(Value *source_stack, size_t source_len, Value dest_stack, Value *stack) {
    int cur = 0;
    int64_t dest = 0;
    int64_t source = 0;
    int64_t range = 0;
    for(size_t i = 0; i < source_len; i++) {
        switch (cur) {
            case 0:
                dest = source_stack[i].original;
                break;
            case 1:
                source = source_stack[i].original;
                break;
            case 2:
                range = source_stack[i].original;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            if(dest_stack.converted >= source && dest_stack.converted < source + range) {
                int64_t calc = dest_stack.converted - dif; 
                stack->original = dest_stack.converted;
                stack->converted = calc;
                dest_stack.is_converted = 1;
            }
            cur = 0;    
        }
    }

    if(dest_stack.is_converted == 0) {
        stack->converted = dest_stack.converted;
        stack->original = dest_stack.converted;
    }
}


int main() {
    char *contents = read_from_file(INPUT);
    Map map = {0};
    int64_t seeds = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(isdigit(contents[i])) {
            char num[128] = {0};
            size_t num_len = 0;
            if(seeds >= 1) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.conversions[seeds - 1][map.conversions_len[seeds-1]].original = atol(num);
                map.conversions[seeds - 1][map.conversions_len[seeds-1]].converted = atol(num);
                map.conversions_len[seeds-1]++;
            }
        }
        if(contents[i] == ':') {
            seeds += 1;
        }
    }

    Value value = {0};
    int64_t low = 9223372036854775807;
    for(size_t i = 0; i < map.conversions_len[0]; i += 2) {
        printf("current: %ld\n", map.conversions[0][i].converted);
        for(int64_t seed = map.conversions[0][i].converted; seed < map.conversions[0][i].converted + map.conversions[0][i+1].converted; seed++) {
            Value seed_val = {.converted = seed, .original = seed};
            calculate(map.conversions[1], map.conversions_len[1], seed_val, &value);
            for(size_t i = 2; i < 8; i++) {
                Value new_value = {0};
                calculate(map.conversions[i], map.conversions_len[i], value, 
                          &new_value);
                value = new_value;
            }
            if(value.converted < low) {
                printf("low: %ld\n", low);
                low = value.converted;
            }
        }
    }
    printf("final low: %ld\n", low);
}
