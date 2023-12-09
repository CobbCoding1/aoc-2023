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
    GEAR,
} Ident;

typedef struct {
    Ident ident;
    char value;
} Element;

typedef struct {
    char num[4];
    char num2[4];
    int is_valid;
} Value;

char *trim(char *str, size_t len) {
    char *result = malloc(sizeof(char) * 4);
    size_t result_in = 0;
    for(size_t i = 0; i < len; i++) {
        if(isdigit(str[i])) {
            result[result_in++] = str[i];
        }
    }
    result[result_in] = '\0';
    return result;
}

// stolen from https://stackoverflow.com/questions/8534274/is-the-strrev-function-not-available-in-linux
char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

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
    char *contents = read_from_file(TEST_INPUT);
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
        } else if(contents[i] == '*') {
            Element elem = {.ident = GEAR};
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
        //Value val = {0};
        for(size_t w = 0; w < width; w++) {
            if(table[w][h].ident == GEAR) {
                printf("GEAR\n");
                char num[5];
                char num2[5];
                size_t num_index = 0;
                size_t num2_index = 0;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        if((j + (int)w) < 0 || (j + w) > width || (i + (int)h) < 0 || (h + i) > height) {
                            continue;
                        }
                        if(i == (int)h && j == (int)w) {
                            continue;
                        }
                        if(table[w+j][h+i].ident == DIGIT) {
                            is_valid += 1;
                            num_index = 2;
                            num2_index = 2;
                            if(is_valid == 1) {
                                num[num_index] = table[w+j][h+i].value;
                                int right = 1;
                                while((w+right+j < width && table[w+right+j][h+i].ident == DIGIT)) {
                                    num[num_index+right] = table[w+right+j][h+i].value;
                                    right++;
                                }

                                int left = 1;
                                while(((int)w-left+j >= 0 && table[w-left+j][h+i].ident == DIGIT)) {
                                    num[num_index-left] = table[w-left+j][h+i].value;
                                    left++;
                                }
                            } else {
                                num2[num2_index] = table[w+j][h+i].value;
                                int right = 1;
                                while((w+right+j < width && table[w+right+j][h+i].ident == DIGIT)) {
                                    num2[num2_index+right] = table[w+right+j][h+i].value;
                                    right++;
                                }

                                int left = 1;
                                while(((int)w-left+j >= 0 && table[w-left+j][h+i].ident == DIGIT)) {
                                    num2[num2_index-left] = table[w-left+j][h+i].value;
                                    left++;
                                }
                            }
                        }
                    }
                }
                char *trimmed1 = trim(num, 5);
                char *trimmed2 = trim(num2, 5);
                printf("%s %s\n", trimmed1, trimmed2);
                free(trimmed1);
                free(trimmed2);
                is_valid = 0;
            }
            /*
            if(table[w][h].ident == DIGIT) {
                size_t num_index = 0;
                size_t num2_index = 0;
                char num[4];
                char num2[4];
                num[num_index++] = table[w][h].value;
                for(int i = -1; i < 2; i++) {
                    for(int j = -1; j < 2; j++) {
                        if((j + (int)w) < 0 || (j + w) > width || (i + (int)h) < 0 || (h + i) > height) {
                            continue;
                        }
                        if(i == (int)h && j == (int)w) {
                            continue;
                        }
                        if(table[w+j][h+i].ident == GEAR) {
                            is_valid = 1;
                        }
                    }
                }
                if(is_valid) {
                    is_valid = 0;
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
                                    if(table[w+j][h+i].ident == GEAR) {
                                        for(int k = -1; k < 2; k++) {
                                            for(int p = -1; p < 2; p++) {
                                                if(k == (int)h && p == (int)w) {
                                                    continue;
                                                }
                                                if((p + (int)w) < 0 || (p + w) > width || (k + (int)h) < 0 || (h + k) > height) {
                                                    continue;
                                                }
                                                if(table[w+p][h+k].ident == DIGIT) {
                                                    is_valid = 1;
                                                    while(w+p+1 < width && table[w+p+1][h+k].ident == DIGIT) {
                                                        w++;
                                                        num2[num2_index++] = table[w+p][h+k].value;
                                                    }
                                                    if(num2_index == 1) {
                                                        while((int)w+p-1 >= 0 && table[w+p-1][h+k].ident == DIGIT) {
                                                            w--;
                                                            num2[num2_index++] = table[w+p][h+k].value;
                                                        }
                                                        strrev(num2);
                                                        w += num2_index - 1;
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        num[num_index++] = table[w][h].value;
                    }

                    printf("num index = %zu is_valid = %d\n", num_index, is_valid);
                    if(num_index == 1) {
                            while((int)w-1 >= 0 && table[w-1][h].ident == DIGIT) {
                                w--;
                                    if(!is_valid) {
                                        for(int i = -1; i < 2; i++) {
                                            for(int j = -1; j < 2; j++) {
                                                if(i == (int)h && j == (int)w) {
                                                    continue;
                                                }
                                                if((j + (int)w) < 0 || (j + w) > width || (i + (int)h) < 0 || (h + i) > height) {
                                                    continue;
                                                }
                                                if(table[w+j][h+i].ident == GEAR) {
                                                    for(int k = -1; k < 2; k++) {
                                                        for(int p = -1; p < 2; p++) {
                                                            if(k == (int)h && p == (int)w) {
                                                                continue;
                                                            }
                                                            if((p + (int)w) < 0 || (p + w) > width || (k + (int)h) < 0 || (h + k) > height) {
                                                                continue;
                                                            }
                                                            if(table[w+p][h+k].ident == DIGIT) {
                                                                is_valid = 1;
                                                                while(w+p+1 < width && table[w+p+1][h+k].ident == DIGIT) {
                                                                    w++;
                                                                    num2[num2_index++] = table[w+p][h+k].value;
                                                                }
                                                                if(num2_index == 1) {
                                                                    while((int)w+p-1 >= 0 && table[w+p-1][h+k].ident == DIGIT) {
                                                                        w--;
                                                                        num2[num2_index++] = table[w+p][h+k].value;
                                                                    }
                                                                    w += num2_index - 1;
                                                                }
                                                                break;
                                                            }
                                                        }
                                                    }
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                num[num_index++] = table[w][h].value;
                            }
                        strrev(num);
                        w += num_index - 1;
                    }

                    val.is_valid = is_valid;
                    num[num_index] = '\0';
                    num2[num2_index] = '\0';
                    strcpy(val.num, num);
                    strcpy(val.num2, num2);
                    printf("%s 2: %s\n", num, num2);
                    if(is_valid) {
                        //printf("%s 2: %s\n", num, num2);
                        sum += (atoi(num) * atoi(num2));
                        value_stack[stack_index++] = val;
                    }
                    is_valid = 0;
                }
            }
            */
        }
    }
    printf("sum: %d\n", sum);
    return 0;
}
