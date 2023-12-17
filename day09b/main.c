#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define TEST_INPUT "testinput.txt"
#define TEST_TEST_INPUT "testtestinput.txt"
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

int equal(int64_t *arr, size_t arr_size, int value) {
    for(size_t i = 0; i < arr_size; i++) {
        if(arr[i] != value) {
            return 0;
        }
    }
    return 1;
}

int64_t data[1024][128] = {0};
size_t data_len = 0;
size_t sub_data_len[1024] = {0};

int main() {
    char *contents = read_from_file(INPUT);
    size_t sub_len = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(contents[i] == '\n') {
            data_len++;
            sub_len = 0;
        }
        if(isdigit(contents[i]) || contents[i] == '-') {
            char num[512] = {0};
            size_t num_len = 0;
            while(isdigit(contents[i]) || contents[i] == '-') {
                num[num_len++] = contents[i];
                i++;
            }
            i--;
            num[num_len] = '\0';
            data[data_len][sub_len++] = atol(num);
            sub_data_len[data_len] = sub_len;
        }
    }
    int64_t sum = 0;
    for(size_t i = 0; i < data_len; i++) {
            printf("---\n");
        int64_t differences[1024][128] = {0};
        size_t differences_len[1024] = {0};
        size_t differences_index = 0;
        memcpy(differences[differences_index], data[i], sizeof(int64_t) * sub_data_len[i]);
        differences_len[differences_index] = sub_data_len[i];
        size_t zero_index = 0;
        while(1) {
            //printf("-------\n");
            for(size_t j = 0; j < differences_len[differences_index] - 1; j++) {
                int64_t dif = differences[differences_index][j+1] - differences[differences_index][j];
               /* 
                printf("dif[dif_index][j]: %ld, differences[dif_index][j + 1]: %ld, dif: %ld, dif_index: %zu, sub_data_len: %zu, i: %zu, j: %zu dif_len: %zu\n", 
                       differences[differences_index][j], differences[differences_index][j+1], 
                       dif, differences_index, sub_data_len[i], i, j, differences_len[differences_index]);
               */ 
                differences[differences_index+1][differences_len[differences_index+1]++] = dif;
            }
            differences_index++;
            if(equal(differences[differences_index-1], differences_len[differences_index-1], 0)) {
                break;
            }
            zero_index++;
        }
        int64_t cur_add = 0;
        for(int k = zero_index; k >= 0; k--) {
            printf("%d\n", k);
            printf("k[k+1]: %ld\n", differences[k][0]);
            cur_add = differences[k+1][0];
            //printf("cur add: %ld\n", cur_add);
            differences[k][0] -= cur_add;
            printf("differences[k][0]: %ld\n", differences[k][0]);
        }
        cur_add = differences[0][0];
        printf("cur add: %ld\n", cur_add);
        sum += (cur_add);
        printf("sum: %ld\n", sum);
        //printf("%zu\n", differences[0][differences_len[1]]);
    }
    printf("%ld\n", sum);
}
