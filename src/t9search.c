#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
// How much characters are allowed in search and on each line of the input from
// stdin + the \n and \0
#define BUFFER_SIZE 100+2
// 1. Profit


// Defining type to store our contacts for better readability
typedef struct contact_s {
    char name[BUFFER_SIZE];
    char number[BUFFER_SIZE];
} contact;

bool str_contains(char str[], char filter[]){
    int key_idx = 0;

    int f_len = strlen(filter);
    int s_len = strlen(str);

    for(int i = 0; i < s_len; ++i){
        if(str[i] != filter[key_idx]){
            key_idx = 0; 
        }
        if(str[i] == filter[key_idx]){
            key_idx += 1;
            if(key_idx == f_len){
                return true;
            }
        }
    }

    return false;
}

// Checks whether the input str[] contains filter[] the filter must appear
// in the same order but it can be spaced out with non-matching characters
// in between
bool str_contains_gap(char str[], char filter[]){
    char key = filter[0];

    int f_len = strlen(filter);
    int s_len = strlen(str);

    if(f_len == 0){
        return true;
    }else{
        if(s_len > 0){
            for(int i = 0; i < s_len; ++i){
                if(str[i] == key){
                    if(str_contains_gap(&str[i+1], &filter[1])){
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

// Reads two lines from stdin and puts them
// in the contact struct
bool load_contact(contact *c){
    char name[BUFFER_SIZE];
    char num[BUFFER_SIZE];

    if(fgets(name, BUFFER_SIZE, stdin) == NULL ||
        fgets(num, BUFFER_SIZE, stdin) == NULL){
        return false;
    }

    strcpy(c->name, name);
    strcpy(c->number, num); 

    return true;
}

// Returns the number corresponding with the letter as specified
char alph_to_num(char alph){

    alph = tolower(alph); 
    int y = 10;
    int x = 4;
    char conversion_table[10][4] = {
        {'+', '0', '0', '0'},   // 0 
        {'1', '1', '1', '1'},   // 1
        {'a', 'b', 'c', '2'},   // 2
        {'d', 'e', 'f', '3'},   // 3
        {'g', 'h', 'i', '4'},   // 4
        {'j', 'k', 'l', '5'},   // 5
        {'m', 'n', 'o', '6'},   // 6
        {'p', 'q', 'r', 's'},   // 7
        {'t', 'u', 'v', '8'},   // 8
        {'w', 'x', 'y', 'z'}    // 9
    };

    for(int i = 0; i < y; ++i){
        for(int j = 0; j < x; ++j){
            if(conversion_table[i][j] == alph){
                return '0'+i;
            }
        }
    }
    return alph;

}

// Goes through the whole string and translates letters to numbers
void str_to_num(char *in){
    int len = strlen(in);
    for(int i = 0; i < len; ++i){
        if(!isdigit(in[i])){
            in[i] = alph_to_num(in[i]);
        }
    }
}

// Check whether the string contains purely numbers
bool str_is_onlynum(char *str){
    int s_len = strlen(str);
    for(int i = 0; i < s_len; ++i){
        if(!isdigit(str[i])){
            return false;
        }
    }
    return true;
}

int main(int argc, char **argv){
    char filter[BUFFER_SIZE] = "";
    if(argc > 1){
        strcpy(filter, argv[1]);
        if(!str_is_onlynum(filter)){
            fprintf(stderr, "Wrong argument format: %s\n", filter);
            fprintf(stderr, "Argument can only contain numbers \n");
            return 1;
        }
    }

    bool no_results = true;
    contact c;

    while(load_contact(&c)){
        char parsed_name[BUFFER_SIZE];
        char parsed_num[BUFFER_SIZE];

        strcpy(parsed_name, c.name);
        strcpy(parsed_num, c.number);

        str_to_num(parsed_name);
        str_to_num(parsed_num);

        if(str_contains(parsed_name, filter) ||
            str_contains(parsed_num, filter) ||
            strcmp(filter, "") == 0){
            // We can now say that there are at least some results
            no_results = false;
            // Trim trailing newlines if the string contains them
            int name_newline_idx = strcspn(c.name, "\n");
            int num_newline_idx = strcspn(c.number, "\n");
            c.name[name_newline_idx] = 0;
            c.number[num_newline_idx] = 0;
            printf("%s, %s\n", c.name, c.number);
        }

    }
    if(no_results){
        printf("Not found\n");
    }
    return 0;
}


