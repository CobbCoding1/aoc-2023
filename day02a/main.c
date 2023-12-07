#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define TEST_INPUT "testinput.txt"
#define INPUT "input.txt"
#define MAX_SIZE 200 

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

size_t split_by_newline(char *str, char *results[MAX_SIZE]) {
    int newlines = 0;
    int max_size = 0;
    int cur_size = 0;
    for(size_t i = 0; str[i] != '\0'; i++) {
        cur_size++;
        if(str[i] == '\n') {
            if(cur_size > max_size) {
                max_size = cur_size;
            }
            newlines++;
        }
    }
    char result[newlines][MAX_SIZE];
    char cur[max_size];
    size_t cur_index = 0;
    size_t result_index = 0;
    for(size_t i = 0; str[i] != '\0'; i++) {
        if(str[i] == '\n') {
            cur[cur_index] = '\0';
            strcpy(result[result_index], cur);
            result_index++;
            cur_index = 0;
        }
        cur[cur_index] = str[i];
        cur_index++;
    }
    for(size_t i = 0; i < (size_t)newlines; i++) {
        results[i] = malloc(sizeof(char) * MAX_SIZE);
        strcpy(results[i], result[i]);
    }
    return newlines;
}

char *generate_color(char *buffer, size_t buffer_index) {
    char *num_word = malloc(sizeof(char) * 5);
    size_t word_index = 0;
    while(isalpha(buffer[buffer_index])) {
        num_word[word_index++] = buffer[buffer_index++];
    }
    return num_word;
}

int main() {    
    char *contents = read_from_file(INPUT);
    char *results[MAX_SIZE];
    size_t lines = split_by_newline(contents, results);
    int total = 0;
    for(size_t i = 0; i < lines; i++) {
        int id = 0;
        int rgb[3] = {0, 0, 0};
        for(size_t j = 0; results[i][j] != '\0'; j++) {
            if(isdigit(results[i][j])) {
                if(id == 0) {
                    char id_str[4];
                    int id_index = 0;
                    while(isdigit(results[i][j])) {
                        id_str[id_index] = results[i][j];
                        j++;
                        id_index++;
                    }
                    id_str[id_index] = '\0';
                    id = atoi(id_str);
                    printf("id: %d\n", id);
                } else {
                    char value[3];
                    int value_index = 0;
                    while(isdigit(results[i][j])) {
                        value[value_index] = results[i][j];
                        value_index++;
                        j++;
                    }
                    value[value_index] = '\0';
                    j++;
                    char *word = generate_color(results[i], j);
                    j -= 2;
                    if(strcmp(word, "red") == 0) {
                        if(atoi(value) > rgb[0]) {
                            rgb[0] = atoi(value); 
                        }
                    } else if(strcmp(word, "green") == 0) {
                        if(atoi(value) > rgb[1]) {
                            rgb[1] = atoi(value); 
                        }
                    } else if(strcmp(word, "blue") == 0) {
                        if(atoi(value) > rgb[2]) {
                            rgb[2] = atoi(value); 
                        }
                    } else {
                        assert(0);
                    }
                    //printf("%s: %s\n", word, value);
                }
            }
        }
        int is_valid = rgb[0] <= 12 && rgb[1] <= 13 && rgb[2] <= 14;
        if(is_valid) {
            total += id;
        }
        printf("is_valid = %d, r: %d, g: %d, b: %d\n", is_valid, rgb[0], rgb[1], rgb[2]);
    }
    printf("total: %d\n", total);
}
