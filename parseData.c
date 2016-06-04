#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>

static const char dir_path[] = "data/2015/"; 

struct month {
    int hours[8];
    int vel_med[8];
};

int splitString(char *line, char **str_data) {
    int i, j, wordCount = 0, charCount = 0;
    for (i = 0; line[i] != '\0'; i++);
    int len = i;
    int max_len_word = 5;
    char word[5] = "";

    for (i = 0; i < len; i++) {
        if(line[i] != ',') {
            word[charCount++] = line[i];
            continue;
        }

        if (word[0] == '\0') { // empty string
            continue;
        }

        str_data[wordCount] = (char *) malloc(sizeof(char *) * max_len_word);
        for (j = 0; j < charCount; j++) {
            str_data[wordCount][j] = word[j];
        }
        word[0] = '\0';
        wordCount++;
        charCount = 0;
    }
/*
    if (word[0] != '\0') { // empty string
        str_data[wordCount] = (char *) malloc(sizeof(char *) * max_len_word);
        for (j = 0; j < charCount; j++) {
            if (word[j] != '\n') {
                str_data[wordCount][j] = word[j];
            }
        }
        wordCount++;
    }    
*/
    return wordCount;
}

void getDataFromFile(FILE *p_file) {
    char line[1024];
    int i;

    for (i = 0; i < 5; i++) fgets(line, 1024, p_file); //skip 5 lines.

    fgets(line, 1024, p_file); 
    char **str_data; 
    int len;

    while (line[1] != ',') {
        for (i = 0; line[i] != '\0'; i++);
        len = i;
        //revisar asignacion memoria...
        str_data = (char**) malloc(sizeof(char *) * len);

        len = splitString(line, str_data);
        for (i = 0; i < len; i++) {
            printf("%s ,", str_data[i]);
        }
        printf("\n----------------------------\n");
        fgets(line, 1024, p_file);
    }
}    

int main(int argc, char **argv) {
    DIR *directory = opendir(dir_path);
    if (directory == NULL) {
        fprintf(stderr, "Error opening directory - %s\n", strerror(errno));
        return 1;
    }

    struct dirent *in_file;
    FILE *entry_file;
    char f_path[100];
    strcpy(f_path, dir_path);

    while ((in_file = readdir(directory))) { //get all file from directory
        if (!strcmp(in_file->d_name, ".") || !strcmp(in_file->d_name, "..")) {
            continue;
        }   


        entry_file = fopen(strcat(f_path, in_file->d_name), "r");
        printf("File name: %s \n", in_file->d_name);
        getDataFromFile(entry_file);
        if (entry_file != NULL) {
            fclose(entry_file);
        }

        break;
        f_path[0] = '\0'; //clean the string of the path.
        strcpy(f_path, dir_path);
    }

    return 0;
}
