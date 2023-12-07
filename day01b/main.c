#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define TEST_FILE "testinput.txt"
#define INPUT_FILE "input.txt"

#define NUMBERS 9
#define MAX_LENGTH 5
char *numbers[NUMBERS][2] = {
    {"one", "1"},
    {"two", "2"},
    {"three", "3"},
    {"four", "4"},
    {"five", "5"},
    {"six", "6"},
    {"seven", "7"},
    {"eight", "8"},
    {"nine", "9"},
};

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

int is_in_nums(char *word, int length) {
    word[length] = '\0';
    for(size_t i = 0; i < NUMBERS; i++) {
        if(strcmp(numbers[i][0], word) == 0) {
            return atoi(numbers[i][1]);
        }
    }
    return 0;
}

int is_word_number(char *buffer, size_t buffer_index) {
    char *num_word = malloc(sizeof(char) * MAX_LENGTH);
    for(size_t i = 0; i < MAX_LENGTH; i++) {
        num_word[i] = buffer[buffer_index];
        buffer_index++; 
        int value = is_in_nums(num_word, i+1);
        if(value != 0) {
            return value;
        }
    }
    return 0;
}

typedef struct {
    char num;
    int is_initted;
} Num;

int main(void) {
    char *contents = read_from_file(INPUT_FILE);
    Num first = {
        .is_initted = 0,
    };
    Num last = {
        .is_initted = 0,
    };
    int total = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            char num[3];
            num[0] = first.num;
            num[1] = last.num;
            num[2] = '\0';
            total += atoi(num);
            first.is_initted = 0;
            last.is_initted = 0;
        }
        if(isalpha(contents[i])) {
            int value = is_word_number(contents, i);
            if(value != 0) {
                if(first.is_initted == 0) {
                    first.is_initted = 1;
                    first.num = value + '0';
                }
                last.num = value + '0';
            }
        }
        if(isdigit(contents[i])) {
            if(first.is_initted == 0) {
                first.is_initted = 1;
                first.num = contents[i];
            }
            last.num = contents[i];
        }
    }
    printf("%d\n", total);
}
