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

#define STRENGTHS_LEN 13
char strengths[STRENGTHS_LEN] = "J23456789TQKA";
char hands[1024][6] = {0};
size_t hands_len = 0;
int bid[1024] = {0};
size_t bid_len = 0;

int comp (const void * a, const void * b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}

typedef enum {
    HIGH_CARD,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
} Types;

typedef struct {
    char cur_char;
    char max_chars[STRENGTHS_LEN];
    int card_values[5];
    int collisions;
    int j_collisions;
    int rank;
    size_t id;
    Types type;
    int bid;
} Matches;

typedef struct {
    Matches matches[STRENGTHS_LEN];
    size_t matches_len;
} Matches_Collection;

Matches_Collection matches_stack[1024] = {0};
size_t matches_len = 0;

Matches rankings[1024] = {0};
size_t rankings_len = 0;

int main() {
    char *contents = read_from_file(INPUT);
    int is_bid = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        char word[6] = {0};
        size_t word_len = 0;
        while(!isspace(contents[i])) {
            word[word_len++] = contents[i++];
        }
        word[word_len] = '\0';
        if(is_bid) {
            bid[bid_len++] = atoi(word);
        } else {
            strcpy(hands[hands_len++], word);
        }
        is_bid = !is_bid;
    }
    
    for(size_t i = 0; i < hands_len; i++) {
        Matches_Collection collection = {0};
        int max_collisions = 0;
        int second_max_collisions = 0;
        char max_char = '2';
        char second_max_char = '2';
        
        for(size_t s = 0; s < STRENGTHS_LEN; s++) {
            int collisions = 0;
            int j_collisions = 0;
            char cur_char = strengths[s];
            Matches match = {0};
            for(size_t k = 0; hands[i][k] != '\0'; k++) {
                if(hands[i][k] == cur_char) {
                    collisions += 1;
                }
                if(hands[i][k] == 'J') {
                    j_collisions += 1;
                }
            }
            match.collisions = collisions;
            match.j_collisions = j_collisions;
            //match.max_chars[i] = collisions;
            match.id = i+1;
            match.bid = bid[i];
            if(collisions > 0) {
                match.cur_char = cur_char;
                //printf("collection.matches_len: %zu\n", collection.matches_len);
                if(collection.matches_len > 13) {
                    collection.matches_len = 0;
                }
                collection.matches[collection.matches_len++] = match;
                //printf("%s, %c, %d\n", hands[i], cur_char, collisions);
            }
        }
        
        for(size_t collect = 0; collect < collection.matches_len; collect++) {
            for(size_t k = 0; hands[i][k] != '\0'; k++) {
                char str[2];
                str[0] = hands[i][k];
                str[1] = '\0';
                collection.matches[collect].card_values[k] = strcspn(strengths, str);
                //printf("hands[i] = %s, value of %c is %d, k = %zu\n", hands[i], hands[i][k], collection.matches[collect].card_values[k], k);
            }
        }
        
            //printf("i: %zu\n", matches_len);
        matches_stack[matches_len++] = collection;
    }
    
    for(size_t i = 0; i < matches_len; i++) {
        size_t type_index = matches_stack[i].matches_len - 1;
        int match_collisions = 0;
        Types current_type = HIGH_CARD;
        for(size_t j = 0; j < matches_stack[i].matches_len; j++) {
            //matches_stack[i].matches[j].collisions += matches_stack[i].matches[j].j_collisions;
            Matches current = matches_stack[i].matches[j];
            int is_j = 0;
            if(current.j_collisions > 0) {
                is_j = 1;
            }
            printf("id: %zu, col: %d, j_col: %d, matches_len: %zu, current_char: %c\n", 
                   current.id, current.collisions, current.j_collisions, matches_stack[i].matches_len, current.cur_char);

            if(current.j_collisions < 5 && current.cur_char != 'J') {
                current.collisions += current.j_collisions;
            }
            if(current.collisions == 5) {
                current_type = FIVE_OF_A_KIND;
            } else if(current.collisions == 4) {
                current_type = FOUR_OF_A_KIND;
            } else if(current.collisions == 3) {
                if((matches_stack[i].matches_len - is_j) == 2) {
                    current_type = FULL_HOUSE;
                } else {
                    current_type = THREE_OF_A_KIND;
                }
            } else if(current.collisions == 2) {
                if(((matches_stack[i].matches_len - is_j) == 3)) {
                    current_type = TWO_PAIR;
                } else if((matches_stack[i].matches_len - is_j) == 4) {
                    current_type = ONE_PAIR;
                }
            }
            matches_stack[i].matches[j].type = current_type;
            matches_stack[i].matches[j].rank += current_type;
            //printf("cur: %c, type: %d, rank: %d\n", matches_stack[i].matches[j].cur_char, matches_stack[i].matches[j].type, matches_stack[i].matches[j].rank);
        }

        Types greatest = 0;
        int g_index = 0;
        for(size_t j = 0; j < type_index+1; j++) {
            if(matches_stack[i].matches[j].type > greatest) {
                greatest = matches_stack[i].matches[j].type;
                g_index = j;
            } 
        }
        if(matches_stack[i].matches[g_index].type == 1) {
            Matches current = matches_stack[i].matches[g_index];
            //printf("id: %zu, TWO PAIR j_collisions: %d\n", current.id, current.j_collisions);
        }
        printf("current_id: %zu, CURRENT TYPE: %d, greatest: %d\n", matches_stack[i].matches[g_index].id, matches_stack[i].matches[g_index].type, greatest);
        rankings[rankings_len++] = matches_stack[i].matches[g_index];
    }

    int sorted = 0;
    while(!sorted) {
        sorted = 1;
        for(size_t i = 0; i < rankings_len; i++) {
            if(i + 1 < rankings_len) {
                if(rankings[i].rank == rankings[i+1].rank) {
                    int value = 0;
                    
                    while(rankings[i].card_values[value] == rankings[i+1].card_values[value] && value < STRENGTHS_LEN) {
                        value++;
                    }
                    //printf("%zu, value is: %d, card values1: %d card values 2: %d\n", 
                    //       rankings[i].id, value, rankings[i].card_values[value], rankings[i+1].card_values[value]);
                    if(rankings[i].card_values[value] > rankings[i+1].card_values[value]) {
                        sorted = 0;
                        Matches temp = rankings[i];
                        rankings[i] = rankings[i + 1];
                        rankings[i + 1] = temp;
                        
                    } else {
                        continue;
                    }
                } else if(rankings[i].rank > rankings[i+1].rank) {
                    sorted = 0;
                    Matches temp = rankings[i];
                    rankings[i] = rankings[i + 1];
                    rankings[i + 1] = temp;
                }
            }
        }
    }


    int64_t total = 0;
    for(size_t i = 0; i < rankings_len; i++) {
        total += (rankings[i].bid * (i + 1));
       /* 
        printf("id: %zu, bid: %d, max_char: %c, second_max_char: %c, current_char: %c, occurances: %d, type: %d, rank: %d\n", 
               rankings[i].id, rankings[i].bid, 48, 48, rankings[i].cur_char, 
               rankings[i].collisions, rankings[i].type, 
               rankings[i].rank);
       */ 
    }
    printf("total: %ld\n", total);
    printf("%zu\n", rankings_len);
    return 0;
}
