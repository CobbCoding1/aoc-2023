#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#define TEST_FILE "testinput.txt"
#define INPUT_FILE "input.txt"

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
    char num;
    int is_initted;
} Num;

int main(void) {
    char *contents = read_from_file(INPUT_FILE);
    printf("%s\n", contents);
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
            printf("first: %c\n", first.num);
            printf("last: %c\n", last.num);
            first.is_initted = 0;
            last.is_initted = 0;
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
