#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
// How much characters are allowed in search and on each line of the input from
// stdin (+ the \n and \0)
#define BUFFER_SIZE 100+2

// We need to differentiate between error and end of file when loading contacts
// so we use this enum instead of regular boolean
enum load_status {ok, end, error};

// Struct to store our contact information
typedef struct contact_s {
    char name[BUFFER_SIZE];
    char number[BUFFER_SIZE];
} contact;

// Checks whether the str contains the filter, if spaced is true the characters
// dont need to be next to each other and will still return true as long as they
// are in the same order as in the filter
bool str_contains(char str[], char filter[], bool spaced){
    int key_idx = 0;

    int f_len = strlen(filter);
    int s_len = strlen(str);

    for(int i = 0; i < s_len; ++i){
        if(str[i] != filter[key_idx] && !spaced){
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

// Reads two lines from stdin and puts them
// in the contact struct
enum load_status load_contact(contact *c){
    char name[BUFFER_SIZE] = "";
    char num[BUFFER_SIZE] = "";

    if(fgets(name, BUFFER_SIZE, stdin) == NULL ||
        fgets(num, BUFFER_SIZE, stdin) == NULL){
        return end;
    }

    // Trim trailing newlines if they exist otherwise return error - too much
    // characters on a line
    int name_newline_idx = strcspn(name, "\n");
    int num_newline_idx = strcspn(num, "\n");
    if(name_newline_idx >= BUFFER_SIZE-1 || num_newline_idx >= BUFFER_SIZE-1){
        return error;
    }
    name[name_newline_idx] = '\0';
    num[num_newline_idx] = '\0';

    strcpy(c->name, name);
    strcpy(c->number, num); 

    return ok;
}

// Returns the number corresponding with the letter as specified in the
// assignment
char alph_to_num(char alph){

    alph = tolower(alph); 
    int y = 10;
    // Index of the string is the number to which the character will be converted
    // if it's a part of the string
    char *conversion_array[10] = {
        "+",       // 0 
        "1",       // 1
        "abc",     // 2
        "def",     // 3
        "ghi",     // 4
        "jkl",     // 5
        "mno",     // 6
        "pqrs",    // 7
        "tuv",     // 8
        "wxyz"     // 9
    };

    for(int i = 0; i < y; ++i){
        int j = 0;
        while(conversion_array[i][j] != '\0'){
            if(conversion_array[i][j] == alph){
                return '0'+i;
            }
            ++j;
        }
    }
    return alph;

}

// Goes through the whole string and translates letters to numbers
void str_to_num(char *in){
    int len = strlen(in);
    for(int i = 0; i < len; ++i){
        if(isdigit(in[i])){
            continue;
        }
        in[i] = alph_to_num(in[i]);
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

// Translates all variables in the contact struct using str_to_num()
// Makes a new copy since we need the original for printing
contact contact_to_num(contact in_cont){
    contact num_cont;
    strcpy(num_cont.name, in_cont.name);
    strcpy(num_cont.number, in_cont.number);
    str_to_num(num_cont.name);
    str_to_num(num_cont.number);

    return num_cont;
} 

int main(int argc, char **argv){
    char filter[BUFFER_SIZE] = "";
    // Toggled by the "-s" flag
    bool spaced = false;

    // If we have more than 2 arguments we check for flags
    if(argc > 2){
        // Iterate through all arguments that could contain a flag
        // 0 is filename, last is reserved for the search so we exclude those
        for(int i = 1; i < argc-1; ++i){
            if(argv[i][0] != '-'){
                continue;
            }

            // Go through the whole argument and find valid flag, or print error
            // and end end with 1 on invalid flag
            int arglen = strlen(argv[i]);
            for(int j = 1; j < arglen; ++j){
                switch(argv[i][j]){
                    case 's':
                        spaced = true;
                        break;
                    default:
                        fprintf(stderr, "Invalid flag: %c\n", argv[i][j]);
                        return 1;
                }
            }
        }
    }
    if(argc > 1){
        strcpy(filter, argv[argc-1]);
        if(!str_is_onlynum(filter)){
            fprintf(stderr, "Wrong argument format: %s\n", filter);
            fprintf(stderr, "Search can only contain numbers \n");
            return 1;
        }
    }

    bool no_results = true;
    contact org_cont;
    contact num_cont;
    enum load_status status;

    // While load_contact doesn't reach EOF or line that is too big filter the
    // entries and 
    while((status = load_contact(&org_cont)) == ok){
        num_cont = contact_to_num(org_cont);

        if(str_contains(num_cont.name, filter, spaced) ||
            str_contains(num_cont.number, filter, spaced) ||
            strcmp(filter, "") == 0){
            // We can now say that there are at least some results
            no_results = false;
            printf("%s, %s\n", org_cont.name, org_cont.number);
        }

    }
    if(status == error){
        fprintf(stderr, "Input line too long\n");
        return 1;
    }
    if(no_results){
        printf("Not found\n");
    }
    return 0;
}
