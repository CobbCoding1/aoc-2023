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

typedef enum {
    VERTICAL,
    HORIZONTAL,
    NE,
    NW,
    SW,
    SE,
    GROUND,
    STARTING,
} Pipes;

typedef struct {
    int connecting;
    Pipes type;
} Tile;

typedef struct {
    size_t x;
    size_t y;
} Vector2;


int main() {
    char *contents = read_from_file(TEST_INPUT);
    size_t width;
    for(width = 0; contents[width] != '\n'; width++) continue;
    size_t height = width;

    Tile tiles[height][width];

    Vector2 starting = {0};
    
    size_t w_index = 0;
    size_t h_index = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        Pipes current_pipe = GROUND;
        switch(contents[i]) {
            case '.':
                current_pipe = GROUND;
                break;
            case '|':
                current_pipe = VERTICAL;
                break;
            case '-':
                current_pipe = HORIZONTAL;
                break;
            case 'L':
                current_pipe = NE;
                break;
            case 'J':
                current_pipe = NW;
                break;
            case '7':
                current_pipe = SW;
                break;
            case 'F':
                current_pipe = SE;
                break;
            case 'S':
                current_pipe = STARTING;
                starting.x = w_index;
                starting.y = h_index;
                break;
            case '\n':
                w_index = 0;
                h_index++;
                continue;
                break;
        }
        tiles[h_index][w_index].connecting = 0;
        tiles[h_index][w_index++].type = current_pipe;
    }


    Vector2 current_loc = {
        .x = starting.x,
        .y = starting.y,
    };
    Tile current;
    tiles[current_loc.x][current_loc.y].connecting = 1;
    while(1) {
        printf("%d: %d, ", tiles[current_loc.y][current_loc.x+1].type, tiles[current_loc.y][current_loc.x+1].connecting);
        printf("%d: %d, ", tiles[current_loc.y+1][current_loc.x].type, tiles[current_loc.y+1][current_loc.x].connecting);
        printf("%d: %d, ", tiles[current_loc.y][current_loc.x-1].type, tiles[current_loc.y][current_loc.x-1].connecting);
        printf("%d: %d ", tiles[current_loc.y-1][current_loc.x].type, tiles[current_loc.y-1][current_loc.x].connecting);
        printf("\n");
        if((tiles[current_loc.y][current_loc.x+1].type == SW ||
           tiles[current_loc.y][current_loc.x+1].type == NW ||
           tiles[current_loc.y][current_loc.x+1].type == HORIZONTAL)
           && tiles[current_loc.y][current_loc.x+1].connecting == 0) {
            current_loc.x = current_loc.x + 1;
            current_loc.y = current_loc.y;
            tiles[current_loc.y][current_loc.x].connecting = 1;
            current = tiles[current_loc.y][current_loc.x];
        } else if((tiles[current_loc.y][current_loc.x-1].type == SE ||
           tiles[current_loc.y][current_loc.x-1].type == NE ||
           tiles[current_loc.y][current_loc.x-1].type == HORIZONTAL)
           && tiles[current_loc.y][current_loc.x-1].connecting == 0) {
            current_loc.x = current_loc.x - 1;
            current_loc.y = current_loc.y;
            tiles[current_loc.y][current_loc.x].connecting = 1;
            current = tiles[current_loc.y][current_loc.x];
        } else if((tiles[current_loc.y+1][current_loc.x].type == NW ||
           tiles[current_loc.y+1][current_loc.x].type == NE ||
           tiles[current_loc.y+1][current_loc.x].type == VERTICAL)
           && tiles[current_loc.y+1][current_loc.x].connecting == 0) {
            current_loc.x = current_loc.x;
            current_loc.y = current_loc.y + 1;
            tiles[current_loc.y][current_loc.x].connecting = 1;
            current = tiles[current_loc.y][current_loc.x];
        } else if((tiles[current_loc.y-1][current_loc.x].type == SW ||
           tiles[current_loc.y-1][current_loc.x].type == SE ||
           tiles[current_loc.y-1][current_loc.x].type == VERTICAL)
           && tiles[current_loc.y-1][current_loc.x].connecting == 0) {
            current_loc.x = current_loc.x;
            current_loc.y = current_loc.y - 1;
            tiles[current_loc.y][current_loc.x].connecting = 1;
            current = tiles[current_loc.y][current_loc.x];
        } else {
            printf("current: %d\n", current.type);
            break;
        }
    }


    for(size_t i = 0; i < width; i++) {
        for(size_t j = 0; j < height; j++) {
            if(tiles[i][j].connecting) {
                printf("%d", tiles[i][j].type);
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    return 0;
}
