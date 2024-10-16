#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void splitHeaders(char* header, char* method, char* path, char* http_version) {
    memset(method, '\0', sizeof(method));
    memset(path, '\0', sizeof(path));
    memset(http_version, '\0', sizeof(http_version));
    char **tokens = malloc(3 * sizeof(char*));
    char* token = strtok(header, " ");
    int token_count = 0;
    
   while (token != NULL) {
        // Copy each token to a new string (with '\0' safely added)
        tokens[token_count] = malloc(strlen(token) + 1);  // +1 for '\0'
        if (tokens[token_count] == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
        strcpy(tokens[token_count], token);  // Copy token to allocated space
        token_count++;
        token = strtok(NULL, " ");
    }


    if (strlen(tokens[0]) < 10)
        strcpy(method, tokens[0]);

    else 
    {
        perror("HTTP Method is abnormally large\n");
        exit(1);
    }

    if (strlen(tokens[1]) < 64)
      strcpy(path, tokens[1]);

    else 
    {
        perror("Parsed resource path is abnormally large\n");
        exit(1);
    }

    if (sizeof(tokens[2]) < 16)
        strncpy(http_version, tokens[2], sizeof(tokens[2]));

    else 
    {
        perror("Parsed HTTP version is abnormally large\n");
        exit(1);
    }
    free(tokens);
   }
