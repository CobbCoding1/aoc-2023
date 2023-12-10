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

typedef struct {
    size_t line_number;
    int actual_nums[32];
    int winning_nums[32];
    size_t winning_index;
    size_t actual_index;
} Line;

Line line_stack[7000 * 1000];
size_t line_stack_index = 0;

int main(void) {
    char *contents = read_from_file(INPUT);
    size_t line_number = 1;
    int before_colon = 1;
    int sum = 0;
    int cur_add = 1;
    int is_first = 1;
    int is_winning = 1;
    int line_index = 0;
    Line line = {0};
    for(size_t i = 0; i < strlen(contents); i++) {
        if(contents[i] == '\n') {
            for(size_t in = 0; in < line.winning_index; in++) {
            }
            line_index = 0;
            line.line_number = line_number;
            cur_add = 1;
            is_first = 1;
            is_winning = 1;
            before_colon = 1;
            line_stack[line_stack_index++] = line;
            line_number++;
            line.winning_index = 0;
            line.actual_index = 0;
        } else if(contents[i] == ':') {
            before_colon = 0;
        } else if(contents[i] == '|') {
            is_winning = 0;
        } else if(isdigit(contents[i]) && before_colon == 0) {
            char dig[3];
            size_t dig_in = 0;
            while(isdigit(contents[i])) {
                dig[dig_in++] = contents[i];
                i++;
            }
            i--;
            dig[dig_in] = '\0';
            if(is_winning) {
                line.winning_nums[line.winning_index++] = atoi(dig);
            } else {
                for(size_t in = 0; in < line.winning_index; in++) {
                    if(line.winning_nums[in] == atoi(dig)) {
                        line.actual_nums[line.actual_index++] = atoi(dig);
                    }
                } 
            } 
        }

        line_index++;
    }
    for(size_t i = 0; i < line_stack_index; i++) {
        //printf("----\n");
        for(size_t winnings = 0; winnings < line_stack[i].winning_index; winnings++) {
            //printf("%d\n", line_stack[i].winning_nums[winnings]);
        }
        int copy_number = 1;
        for(size_t actuals = 0; actuals < line_stack[i].actual_index; actuals++) {
            Line copy = line_stack[(line_stack[i].line_number-1) + copy_number]; 
            line_stack[line_stack_index++] = copy;
            //printf("id: %zu, index changed: %zu\n", line_stack[i].line_number, line_stack_index);
            copy_number++;
        }
    }
    printf("sum: %zu\n", line_stack_index);
    return 0;
}
