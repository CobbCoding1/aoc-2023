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

typedef struct {
    int64_t value;
    int64_t is_converted;
} Seeds;

typedef struct {
    Seeds seeds[256];
    size_t seeds_len;
    Seeds soils[256];
    size_t soils_len;
    Seeds fertilizer[256];
    size_t fertilizer_len;
    Seeds water[256];
    size_t water_len;
    Seeds light[256];
    size_t light_len;
    Seeds temperature[256];
    size_t temperature_len;
    Seeds humidity[256];
    size_t humidity_len;
    Seeds location[256];
    size_t location_len;
} Map;

typedef struct {
    int64_t range[3][16];
    size_t range_len[3];
} Table;

typedef struct {
    int64_t original;
    int64_t converted;
    int64_t is_converted;
} Value;

int64_t is_in_stack(int64_t value, Value arr[128]) {
    for(size_t i = 0; i < 128; i++) {
        if(arr[i].original == value) {
            return 1;
        }
    }
    return 0;
}


int main() {
    char *contents = read_from_file(INPUT);
    Map map = {0};
    int64_t seeds = 0;
    for(size_t i = 0; contents[i] != '\0'; i++) {
        if(isdigit(contents[i])) {
            char num[128] = {0};
            size_t num_len = 0;
            if(seeds == 1) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.seeds[map.seeds_len++].value = atol(num);
            } else if(seeds == 2) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.soils[map.soils_len++].value = atol(num);
            } else if(seeds == 3) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.fertilizer[map.fertilizer_len++].value = atol(num);
            } else if(seeds == 4) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.water[map.water_len++].value = atol(num);
            } else if(seeds == 5) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.light[map.light_len++].value = atol(num);
            } else if(seeds == 6) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.temperature[map.temperature_len++].value = atol(num);
            } else if(seeds == 7) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.humidity[map.humidity_len++].value = atol(num);
            } else if(seeds == 8) {
                while(isdigit(contents[i])) {
                    num[num_len++] = contents[i];
                    i++;
                }
                i--;
                map.location[map.location_len++].value = atol(num);
            }
        }
        if(contents[i] == ':') {
            seeds += 1;
        }
    }
    //Table table = {0};
    int64_t cur = 0;
    int64_t dest = 0;
    int64_t source = 0;
    int64_t range = 0;
    Value soil_stack[128] = {0};
    size_t soil_stack_size = 0;
    for(size_t i = 0; i < map.soils_len; i++) {
        switch (cur) {
            case 0:
                dest = map.soils[i].value;
                break;
            case 1:
                source = map.soils[i].value;
                break;
            case 2:
                range = map.soils[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t seed = 0; seed < map.seeds_len; seed++) {
                if(map.seeds[seed].value >= source && map.seeds[seed].value <= source + range) {
                    int64_t soil = map.seeds[seed].value - dif; 
                    soil_stack[soil_stack_size].original = map.seeds[seed].value;
                    soil_stack[soil_stack_size++].converted = soil;
                    map.seeds[seed].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < map.seeds_len; i++) {
        if(map.seeds[i].is_converted == 0) {
            soil_stack[soil_stack_size].converted = map.seeds[i].value;
            soil_stack[soil_stack_size++].original = map.seeds[i].value;
        }
    }


    cur = 0;
    dest = 0;
    source = 0;
    range = 0;
    Value fertilizer_stack[128] = {0};
    size_t fertilizer_stack_size = 0;
    for(size_t i = 0; i < map.fertilizer_len; i++) {
        switch (cur) {
            case 0:
                dest = map.fertilizer[i].value;
                break;
            case 1:
                source = map.fertilizer[i].value;
                break;
            case 2:
                range = map.fertilizer[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t soil = 0; soil < soil_stack_size; soil++) {
                if(soil_stack[soil].converted >= source && soil_stack[soil].converted < source + range) {
                    int64_t fertilizer = soil_stack[soil].converted - dif; 
                    fertilizer_stack[fertilizer_stack_size].original = soil_stack[soil].converted;
                    fertilizer_stack[fertilizer_stack_size++].converted = fertilizer;
                    soil_stack[soil].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < soil_stack_size; i++) {
        if(soil_stack[i].is_converted == 0) {
            fertilizer_stack[fertilizer_stack_size].converted = soil_stack[i].converted;
            fertilizer_stack[fertilizer_stack_size++].original = soil_stack[i].converted;
        }
    }

    cur = 0;
    dest = 0;
    source = 0;
    range = 0;
    Value water_stack[128] = {0};
    size_t water_stack_size = 0;
    for(size_t i = 0; i < map.water_len; i++) {
        switch (cur) {
            case 0:
                dest = map.water[i].value;
                break;
            case 1:
                source = map.water[i].value;
                break;
            case 2:
                range = map.water[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t fertilizer = 0; fertilizer < fertilizer_stack_size; fertilizer++) {
                if(fertilizer_stack[fertilizer].converted >= source && fertilizer_stack[fertilizer].converted < source + range) {
                    int64_t water = fertilizer_stack[fertilizer].converted - dif; 
                    water_stack[water_stack_size].original = fertilizer_stack[fertilizer].converted;
                    water_stack[water_stack_size++].converted = water;
                    fertilizer_stack[fertilizer].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < fertilizer_stack_size; i++) {
        if(fertilizer_stack[i].is_converted == 0) {
            water_stack[water_stack_size].converted = fertilizer_stack[i].converted;
            water_stack[water_stack_size++].original = fertilizer_stack[i].converted;
        }
    }


    cur = 0;
    dest = 0;
    source = 0;
    range = 0;
    Value light_stack[128] = {0};
    size_t light_stack_size = 0;
    for(size_t i = 0; i < map.light_len; i++) {
        switch (cur) {
            case 0:
                dest = map.light[i].value;
                break;
            case 1:
                source = map.light[i].value;
                break;
            case 2:
                range = map.light[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t water = 0; water < water_stack_size; water++) {
                if(water_stack[water].converted >= source && water_stack[water].converted < source + range) {
                    int64_t light = water_stack[water].converted - dif; 
                    light_stack[light_stack_size].original = water_stack[water].converted;
                    light_stack[light_stack_size++].converted = light;
                    water_stack[water].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < water_stack_size; i++) {
        if(water_stack[i].is_converted == 0) {
            light_stack[light_stack_size].converted = water_stack[i].converted;
            light_stack[light_stack_size++].original = water_stack[i].converted;
        }
    }


    cur = 0;
    dest = 0;
    source = 0;
    range = 0;
    Value temperature_stack[128] = {0};
    size_t temperature_stack_size = 0;
    for(size_t i = 0; i < map.temperature_len; i++) {
        switch (cur) {
            case 0:
                dest = map.temperature[i].value;
                break;
            case 1:
                source = map.temperature[i].value;
                break;
            case 2:
                range = map.temperature[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t light = 0; light < light_stack_size; light++) {
                if(light_stack[light].converted >= source && light_stack[light].converted < source + range) {
                    int64_t temperature = light_stack[light].converted - dif; 
                    temperature_stack[temperature_stack_size].original = light_stack[light].converted;
                    temperature_stack[temperature_stack_size++].converted = temperature;
                    light_stack[light].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < light_stack_size; i++) {
        if(light_stack[i].is_converted == 0) {
            temperature_stack[temperature_stack_size].converted = light_stack[i].converted;
            temperature_stack[temperature_stack_size++].original = light_stack[i].converted;
        }
    }

    cur = 0;
    dest = 0;
    source = 0;
    range = 0;
    Value humidity_stack[128] = {0};
    size_t humidity_stack_size = 0;
    for(size_t i = 0; i < map.humidity_len; i++) {
        switch (cur) {
            case 0:
                dest = map.humidity[i].value;
                break;
            case 1:
                source = map.humidity[i].value;
                break;
            case 2:
                range = map.humidity[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t temperature = 0; temperature < temperature_stack_size; temperature++) {
                if(temperature_stack[temperature].converted >= source && temperature_stack[temperature].converted < source + range) {
                    int64_t humidity = temperature_stack[temperature].converted - dif; 
                    humidity_stack[humidity_stack_size].original = temperature_stack[temperature].converted;
                    humidity_stack[humidity_stack_size++].converted = humidity;
                    temperature_stack[temperature].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < temperature_stack_size; i++) {
        if(temperature_stack[i].is_converted == 0) {
            humidity_stack[humidity_stack_size].converted = temperature_stack[i].converted;
            humidity_stack[humidity_stack_size++].original = temperature_stack[i].converted;
        }
    }


    cur = 0;
    dest = 0;
    source = 0;
    range = 0;
    Value location_stack[128] = {0};
    size_t location_stack_size = 0;
    for(size_t i = 0; i < map.location_len; i++) {
        switch (cur) {
            case 0:
                dest = map.location[i].value;
                break;
            case 1:
                source = map.location[i].value;
                break;
            case 2:
                range = map.location[i].value;
                break;
        }
        cur++;
        if(cur == 3) {
            int64_t dif = source - dest;
            for(size_t humidity = 0; humidity < humidity_stack_size; humidity++) {
                if(humidity_stack[humidity].converted >= source && humidity_stack[humidity].converted < source + range) {
                    int64_t location = humidity_stack[humidity].converted - dif; 
                    location_stack[location_stack_size].original = humidity_stack[humidity].converted;
                    location_stack[location_stack_size++].converted = location;
                    humidity_stack[humidity].is_converted = 1;
                }
            }
            cur = 0;    
        }
    }

    for(size_t i = 0; i < humidity_stack_size; i++) {
        if(humidity_stack[i].is_converted == 0) {
            location_stack[location_stack_size].converted = humidity_stack[i].converted;
            location_stack[location_stack_size++].original = humidity_stack[i].converted;
        }
    }

    int64_t lowest = location_stack[0].converted;
    for(size_t i = 0; i < location_stack_size; i++) {
        printf("%ld\n", location_stack[i].converted);
        if(location_stack[i].converted < lowest) {
            lowest = location_stack[i].converted;
        }
    }
    printf("lowest is: %ld\n", lowest);
}
