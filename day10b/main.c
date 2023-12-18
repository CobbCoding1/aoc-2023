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

int check_if_enclosed(Tile tiles[11][9], size_t width, size_t height, size_t x, size_t y) {
    int is_enclosed = 0;
    if(x + 1 >= width || x == 0 || y + 1 >= height || y == 0) {
        return 0;
    } 
    if (tiles[x+1][y].connecting) {
        is_enclosed++;
    } else if(tiles[x+1][y].type == GROUND) {
        is_enclosed += check_if_enclosed(tiles, width, height, x+1, y);
    } 
    if(tiles[x-1][y].connecting) {
        is_enclosed++;
    } else if(tiles[x-1][y].type == GROUND) {
        is_enclosed += check_if_enclosed(tiles, width, height, x-1, y);
    } 
    if(tiles[x][y+1].connecting) {
        is_enclosed++;
    } else if(tiles[x][y+1].type == GROUND) {
        is_enclosed += check_if_enclosed(tiles, width, height, x, y+1);
    } 
    if(tiles[x][y-1].connecting) {
        is_enclosed++;
    } else if(tiles[x][y-1].type == GROUND) {
        is_enclosed += check_if_enclosed(tiles, width, height, x, y-1);
    }

    //printf("is enclosed: %d, %zu, %zu\n", is_enclosed, x, y);
    if(is_enclosed >= 4) {
        return 1;
    }
    return 0;
}

int main() {
    char *contents = read_from_file(TEST_INPUT);
    size_t width;
    for(width = 0; contents[width] != '\n'; width++) continue;
    size_t height = width;
    height = 9;

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
    size_t count = 0;
    current = tiles[starting.x][starting.y];
    current.type = STARTING;
    //tiles[starting.x][starting.y].connecting = 1;
    printf("x: %zu, y: %zu\n", current_loc.x, current_loc.y);
    while(1) {
        printf("current_loc: x: %zu, y: %zu\n", current_loc.x, current_loc.y);
        printf("current: %d, right: %d, connecting: %d, ", current.type, tiles[current_loc.y][current_loc.x+1].type, 
               tiles[current_loc.y][current_loc.x+1].connecting);
        printf("down: %d, connecting: %d, ", tiles[current_loc.y+1][current_loc.x].type, 
               tiles[current_loc.y+1][current_loc.x].connecting);
        printf("left: %d, connecting: %d, ", tiles[current_loc.y][current_loc.x-1].type,
               tiles[current_loc.y][current_loc.x-1].connecting);
        printf("up: %d, connecting: %d ", tiles[current_loc.y-1][current_loc.x].type, 
               tiles[current_loc.y-1][current_loc.x].connecting);

        printf("\n");

        switch(current.type) {
            case VERTICAL:
                if((tiles[current_loc.y+1][current_loc.x].type == NW ||
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
                    goto end_loop;
                    printf("current: %d\n", current.type);
                    break;
                }
                break;
            case HORIZONTAL:
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
                } else {
                    goto end_loop;
                    printf("current: %d\n", current.type);
                    return 0;
                    break;
                }
                break;
            case NE:
                if((tiles[current_loc.y][current_loc.x+1].type == SW ||
                   tiles[current_loc.y][current_loc.x+1].type == NW ||
                   tiles[current_loc.y][current_loc.x+1].type == HORIZONTAL)
                   && tiles[current_loc.y][current_loc.x+1].connecting == 0) {
                    current_loc.x = current_loc.x + 1;
                    current_loc.y = current_loc.y;
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
                    goto end_loop;
                    return 0;
                    printf("current: %d\n", current.type);
                    break;
                }
                break;
            case NW:
                if((tiles[current_loc.y][current_loc.x-1].type == SE ||
                   tiles[current_loc.y][current_loc.x-1].type == NE ||
                   tiles[current_loc.y][current_loc.x-1].type == HORIZONTAL)
                   && tiles[current_loc.y][current_loc.x-1].connecting == 0) {
                    current_loc.x = current_loc.x - 1;
                    current_loc.y = current_loc.y;
                    tiles[current_loc.y][current_loc.x].connecting = 1;
                    current = tiles[current_loc.y][current_loc.x];
                    break;
                } else if((tiles[current_loc.y-1][current_loc.x].type == SW ||
                   tiles[current_loc.y-1][current_loc.x].type == SE ||
                   tiles[current_loc.y-1][current_loc.x].type == VERTICAL)
                   && tiles[current_loc.y-1][current_loc.x].connecting == 0) {
                    printf("TRUE\n");
                    current_loc.x = current_loc.x;
                    current_loc.y = current_loc.y - 1;
                    tiles[current_loc.y][current_loc.x].connecting = 1;
                    current = tiles[current_loc.y][current_loc.x];
                    break;
                } else {
                    printf("current: %d, y-1: %d, %d\n", current.type, tiles[current_loc.y-1][current_loc.x].connecting, SE);
                    goto end_loop;
                    return 0;
                    break;
                }
                break;
            case SW:
                if((tiles[current_loc.y][current_loc.x-1].type == NE ||
                   tiles[current_loc.y][current_loc.x-1].type == SE ||
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
                } else {
                    printf("current: %d, y+1: %d, %d\n", current.type, tiles[current_loc.y+1][current_loc.x].connecting, NW);
                    goto end_loop;
                    return 0;
                    break;
                }
                break;
            case SE:
                if((tiles[current_loc.y][current_loc.x+1].type == SW ||
                   tiles[current_loc.y][current_loc.x+1].type == NW ||
                   tiles[current_loc.y][current_loc.x+1].type == HORIZONTAL)
                   && tiles[current_loc.y][current_loc.x+1].connecting == 0) {
                    printf("SW\n");
                    current_loc.x = current_loc.x + 1;
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
                } else {
                    printf("current: %d\n", current.type);
                    goto end_loop;
                    return 0;
                    break;
                }
                break;
            case GROUND:
                return 0;
                break;
            case STARTING:
                /*if((tiles[current_loc.y][current_loc.x+1].type == SW ||
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
                */
                if((tiles[current_loc.y+1][current_loc.x].type == NW ||
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
                    goto end_loop;
                    return 0;
                    printf("current: %d\n", current.type);
                    break;
                }
                break;
        }
    }
end_loop:

    for(size_t i = 0; i < width; i++) {
        int iter = 0;
        for(size_t j = 0; j < height; j++) {
            if(tiles[i][j].connecting || tiles[i][j].type == STARTING) {
                printf("%d", tiles[i][j].type);
            } else if(check_if_enclosed(tiles, width, height, i, j)) {
                count++;
                printf("I");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("count: %zu\n", count);

    return 0;
}
