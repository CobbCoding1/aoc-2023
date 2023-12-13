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
    int held;
    int distance_traveled;
    int time_left;
} Speeds;

Speeds speed_stack[128] = {0};
size_t speed_len = 0;

int times[64] = {0};
size_t times_len = 0;
int distances[64] = {0};
size_t distances_len = 0;

int main() {
    char *contents = read_from_file(INPUT);
    int distance = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            distance = 1;
        } else if(isdigit(contents[i])) {
            char num[10] = {0};
            size_t num_len = 0;
            while(isdigit(contents[i])) {
                num[num_len++] = contents[i];
                i++;
            }
            i--;
            num[num_len] = '\0';
            if(distance == 0) {
                times[times_len++] = atoi(num);
            } else {
                distances[distances_len++] = atoi(num);
            }
        }
    }
    int product = 1;
    for(size_t i = 0; i < times_len; i++) {
        int sum = 0;
        for(size_t speed = 0; (int)speed <= times[i]; speed++) {
            Speeds possibility = {
                .held = speed,
                .time_left = times[i] - speed,
            };
            int total = possibility.time_left * speed;
            if(total > distances[i]) {
                sum++;
            }
        }
        product *= sum;
    }
    printf("final: %d\n", product);
    return 0;
}
