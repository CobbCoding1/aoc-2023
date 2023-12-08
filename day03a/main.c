#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TEST_INPUT "testinput.txt"
#define INPUT "input.txt"

typedef enum {
    DIGIT = 0,
    DOT,
    SYMBOL,
} Ident;

typedef struct {
    Ident ident;
    char value;
} Element;

typedef struct {
    char num[4];
    int is_valid;
} Value;

char *read_from_file(char *filename) {
    FILE *file = fopen(filename, "r");
    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *contents = malloc(sizeof(char) * length + 1);
    fread(contents, 1, length, file);
    contents[length] = '\0';
    return contents;
}   

int main() {
    char *contents = read_from_file(INPUT);
    size_t width = 0;
    size_t height = 0;
    for(int i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            break;
        } else {
            width++;
        }
    }
    height = strlen(contents) / (width + 1);
    Element table[width][height];
    size_t width_index = 0;
    size_t height_index = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            width_index = 0;
            height_index++;
        } else if(isdigit(contents[i])) {
            Element elem = {.ident = DIGIT, .value = contents[i]};
            table[width_index][height_index] = elem;
            width_index++;
        } else if(contents[i] == '.') {
            Element elem = {.ident = DOT};
            table[width_index][height_index] = elem;
            width_index++;
        } else {
            Element elem = {.ident = SYMBOL};
            table[width_index][height_index] = elem;
            width_index++;
        }
    }
    int is_valid = 0;
    Value value_stack[1024];
    size_t stack_index = 0;
    int sum = 0;

    for(size_t h = 0; h < height; h++) {
        Value val = {0};
        for(size_t w = 0; w < width; w++) {
            if(table[w][h].ident == DIGIT) {
                size_t num_index = 0;
                char num[4];
                num[num_index++] = table[w][h].value;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        if((j + (int)w) < 0 || (j + w) > width || (i + (int)h) < 0 || (h + i) > height) {
                            continue;
                        }
                        if(i == (int)h && j == (int)w) {
                            continue;
                        }
                        if(table[w+j][h+i].ident == SYMBOL) {
                            is_valid = 1;
                        }
                    }
                }
                while(w+1 < width && table[w+1][h].ident == DIGIT) {
                    w++;
                    if(!is_valid) {
                        for(int i = -1; i < 2; i++) {
                            for(int j = -1; j < 2; j++) {
                                if(i == (int)h && j == (int)w) {
                                    continue;
                                }
                                if((j + (int)w) < 0 || (j + w) > width || (i + (int)h) < 0 || (h + i) > height) {
                                    continue;
                                }
                                if(table[w+j][h+i].ident == SYMBOL) {
                                    is_valid = 1;
                                    break;
                                }
                            }
                        }
                    }
                    num[num_index++] = table[w][h].value;
                }
                val.is_valid = is_valid;
                num[num_index] = '\0';
                strcpy(val.num, num);
                if(is_valid) {
                    printf("%s\n", num);
                    sum += atoi(num);
                    value_stack[stack_index++] = val;
                }
                is_valid = 0;
            }
        }



        printf("\n");
    }
    printf("sum: %d\n", sum);
    return 0;
}
