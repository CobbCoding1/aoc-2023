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

#define STRENGTHS_LEN 13
char strengths[STRENGTHS_LEN] = "23456789TJQKA";
char hands[1024][6] = {0};
size_t hands_len = 0;
int bid[1024] = {0};
size_t bid_len = 0;

typedef enum {
    ONE_PAIR,
    TWO_PAIR,
    FULL_HOUSE,
    THREE_OF_A_KIND,
    FOUR_OF_A_KIND,
    FIVE_OF_A_KIND,
    HIGH_CARD,
} Types;

typedef struct {
    char cur_char;
    int collisions;
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

int main() {
    char *contents = read_from_file(TEST_INPUT);
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
        for(size_t s = 0; s < STRENGTHS_LEN; s++) {
            int collisions = 0;
            char cur_char = strengths[s];
            Matches match = {0};
            for(size_t k = 0; hands[i][k] != '\0'; k++) {
                if(hands[i][k] == cur_char) {
                    collisions += 1;
                }
            }
            match.cur_char = cur_char;
            match.collisions = collisions;
            match.id = i+1;
            match.bid = bid[i];
            collection.matches[collection.matches_len++] = match;
            if(collisions > 0) {
                //printf("%s, %c, %d\n", hands[i], cur_char, collisions);
            }
        }
        matches_stack[matches_len++] = collection;
    }
    for(size_t i = 0; i < matches_len; i++) {
        int match_collisions = 0;
        for(size_t j = 0; j < STRENGTHS_LEN; j++) {
            Matches current = matches_stack[i].matches[j];
            if(current.collisions > 0) {
                match_collisions++;
            }
            if(current.collisions >= 3) {
                current.type = current.collisions;
                printf("id: %zu, bid: %d, current_char: %c, collisions: %d\n", 
                       matches_stack[i].matches[j].id, current.bid, matches_stack[i].matches[j].cur_char, 
                       matches_stack[i].matches[j].collisions);
            }
        }
    }
    return 0;
}
