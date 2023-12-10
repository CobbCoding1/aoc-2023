#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TEST_INPUT "testinput.txt"
#define INPUT "input.txt"

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

typedef struct {
    int map[8][1024];
    size_t map_index[8];
} Map;


int main() {
    char *contents = read_from_file(TEST_INPUT);
    Map map = {0};
    int map_index = -1;
    int index = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == ':') {
            map_index++;
            index = 0;
        }
        if(isdigit(contents[i])) {
            char num[32];
            size_t num_index = 0;
            while(isdigit(contents[i])) {
                num[num_index++] = contents[i];
                i++;
            }
            i--;
            num[num_index] = '\0';
            printf("%s\n", num);
            map.map_index[map_index] = ++index;
            //printf("%zu\n", map.map_index[map_index]);;
            map.map[map_index][map.map_index[map_index]] = atoi(num);
        }
    }
    for(size_t i = 0; i < (size_t)map_index; i++) {
        printf("----\n");
        for(size_t j = 0; j < (size_t)map.map_index[j]; j++) {
            printf("%d\n", map.map[i][map.map_index[j]]);
        }
    }
    //printf("%s\n", contents);
}
