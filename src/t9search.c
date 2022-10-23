#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#define BUFFER_SIZE 100
// 1. Handle all the inevitable errors 
//  - velke vstupni cislo - poresit
//  - vstup neni cislo
//  - vstup je ve spatnem formatu
//  - dlouhe radky v souboru
// 2. Profit


// Defining type to store our contacts for better readability
typedef struct contact_s {
    char name[BUFFER_SIZE];
    char number[BUFFER_SIZE];
} contact;

// Checks whether the input str[] contains filter[] the filter must appear
// in the same order but it can be spaced out with non-matching characters
// in between
bool str_contains(char str[], char filter[]){
    char key = filter[0];

    int f_len = strlen(filter);
    int s_len = strlen(str);

    if (f_len == 0){
        return true;
    }else{
        if (s_len > 0){
            for (int i = 0; i < s_len; ++i){
                if (str[i] == key){
                    if(str_contains(&str[i+1], &filter[1])){
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
contact load_contact(){
    char name[BUFFER_SIZE];
    char *res_a = fgets(name, BUFFER_SIZE, stdin);
    char num[BUFFER_SIZE];
    char *res_b = fgets(num, BUFFER_SIZE, stdin);

    if(res_a == NULL || res_b == NULL){
        exit(1);
    }

    contact c;
    strcpy(c.name, name);
    strcpy(c.number, num); 

    return c;
}

// Returns the number corresponding with the letter as specified
char alph_to_num(char alph){
    int num;
    alph = tolower(alph); 
    // Ugly but works, maybe find alternate solution
    switch(alph){
        case 'a':
        case 'b':
        case 'c':
            num = '2';
            break;
        case 'd':
        case 'e':
        case 'f':
            num = '3';
            break;
        case 'g':
        case 'h':
        case 'i':
            num = '4';
            break;
        case 'j':
        case 'k':
        case 'l':
            num = '5';
            break;
        case 'm':
        case 'n':
        case 'o':
            num = '6';
            break;
        case 'p':
        case 'q':
        case 'r':
        case 's':
            num = '7';
            break;
        case 't':
        case 'u':
        case 'v':
            num = '8';
            break;
        case 'w':
        case 'x':
        case 'y':
        case 'z':
            num = '9';
            break;
        case '+':
            num = '0';
            break;
        default:
            num = alph;
            break;
    }
    return num;
}

// Goes through the whole string and translates letters to numbers
void str_to_num(char *in){
    int len = strlen(in);
    for(int i = 0; i < len; ++i){
        in[i] = alph_to_num(in[i]);
    }
}

int main(int argc, char **argv){
    char filter[BUFFER_SIZE] = "";
    if(argc > 1){
        strcpy(filter, argv[1]);
    }

    // Load contact exits on EOF 
    while(true){
        contact c = load_contact();

        char num_name[BUFFER_SIZE];
        strcpy(num_name, c.name);
        str_to_num(num_name);
        if(str_contains(num_name, filter) || str_contains(c.number, filter)){
            //trim trailing newlines
            c.name[strcspn(c.name, "\n")] = 0;
            c.number[strcspn(c.number, "\n")] = 0;
            printf("%s, %s\n", c.name, c.number);
        }

    }
    return 0;
}


