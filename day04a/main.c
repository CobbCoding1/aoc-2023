#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

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

int main(void) {
    char *contents = read_from_file(INPUT);
    int sum = 0;
    int cur_add = 1;
    int is_first = 1;
    int winning_nums[1024];
    size_t winning_index = 0;
    int is_winning = 1;
    int line_index = 0;
    for(size_t i = 0; i < strlen(contents); i++) {
        if(contents[i] == '\n') {
            for(size_t in = 0; in < winning_index; in++) {
                printf("%d\n", winning_nums[in]);
            }
            printf("----\n");
            line_index = 0;
            cur_add = 1;
            is_first = 1;
            is_winning = 1;
            winning_index = 0;
        } else if(contents[i] == '|') {
            is_winning = 0;
        } else if(isdigit(contents[i]) && line_index > 9) {
            char dig[3];
            size_t dig_in = 0;
            while(isdigit(contents[i])) {
                dig[dig_in++] = contents[i];
                i++;
            }
            i--;
            dig[dig_in] = '\0';
            if(is_winning) {
                winning_nums[winning_index++] = atoi(dig);
            } else {
                for(size_t in = 0; in < winning_index; in++) {
                    if(winning_nums[in] == atoi(dig)) {
                        sum += cur_add;
                        if(!is_first) {
                            cur_add *= 2;
                        } else {
                            is_first = 0;
                        }
                    }
                } 
            } 
        }

        line_index++;
    }
    printf("%d\n", sum);
    return 0;
}
