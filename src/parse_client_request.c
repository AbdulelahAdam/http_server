#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int splitHeaders(char* header, char* method, char* path, char* http_version) {
    memset(method, 0, 10);
    memset(path, 0, 64);
    memset(http_version, 0, 16);

    char **tokens = malloc(4 * sizeof(char*));
    char* token = strtok(header, " ");
    int token_count = 0;
    int returnVal = -1;
  
  if(strcmp(token, "GET") != 0 && strcmp(token, "POST") != 0 && strcmp(token, "PUT") != 0 && strcmp(token, "PATCH") != 0 && strcmp(token, "DELETE") != 0)
  {
    strcpy(http_version, "HTTP/1.1");
    return 400;
  }
 
   while (token != NULL) {
        // Copy each token to a new string (with '\0' safely added)
        tokens[token_count] = malloc(strlen(token) + 1);  // +1 for '\0'
        if (tokens[token_count] == NULL) {
            printf("Memory allocation failed.\n");
            exit(1);
        }
       
        strcpy(tokens[token_count], token);
        token_count++;
        token = strtok(NULL, " ");
    }
        

    if (strlen(tokens[0]) > 8)
    {
        perror("HTTP Method is abnormally large\n");
        returnVal = 414;  
    }

    else 
        strcpy(method, tokens[0]);

    if((strlen(tokens[1]) == 1 && strcmp(tokens[1], "/") != 0) || strcmp(tokens[1], " ") == 0)
        returnVal = 400;
      
    else if (strlen(tokens[1]) <= 64)
    {
        //memset(path, '/0', sizeof(path));
        strncpy(path, tokens[1], strlen(tokens[1]));
    }

    else 
        returnVal = 414;     
      
    tokens[2][strlen(tokens[2]) - 1] = '\0';
   
    if(strcmp(tokens[2], "HTTP/1.0") != 0 && strcmp(tokens[2], "HTTP/1.1") != 0 && strcmp(tokens[2], "HTTP/2") != 0)
    {
      
      strcpy(http_version, tokens[2]);
      returnVal = 505;
    }
    
    else if(strlen(tokens[2]) > 10)
    {
        strcpy(http_version, "HTTP/1.1");
        returnVal = 414;
    }  
    else
        strcpy(http_version, tokens[2]);

    
    for (int i = 0; i < 3; i++) {
      free(tokens[i]);
    }
    free(tokens);  
    return returnVal;

   }
