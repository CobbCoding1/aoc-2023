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

int64_t times = 0;
int64_t distances = 0;
size_t distances_len = 0;

int main() {
    char *contents = read_from_file(INPUT);
    int64_t distance = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            distance = 1;
        } else if(isdigit(contents[i])) {
            char num[64] = {0};
            size_t num_len = 0;
            while(isdigit(contents[i])) {
                num[num_len++] = contents[i];
                i++;
            }
            i--;
            num[num_len] = '\0';
            if(distance == 0) {
                times = atol(num);
            } else {
                distances = atol(num);
            }
        }
    }
    printf("time: %ld, distance: %ld\n", times, distances);
    int64_t product = 1;
    int64_t sum = 0;
    for(size_t speed = 0; (int)speed <= times; speed++) {
        int time_left = times - speed;
        int64_t total = time_left * speed;
        if(total > distances) {
            sum++;
        }
    }
    product *= sum;
    printf("final: %ld\n", product);
    return 0;
}
