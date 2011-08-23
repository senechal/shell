#include <stdio.h>
#include <string.h>

int main () {
    char mat[100][100];
    char word[100];
    char *tokenPtr;
    int i = 0;

    while (1) {
	printf ("shell>");
	scanf ("%[^\n]", word);
	setbuf (stdin, "NULL");
	if (strcmp(word, "exit") == 0) break;
    	if (strlen(word) > 100) {
	    printf("Comando invalido!\n");
	    continue;
	}
	else {
	    tokenPtr = strtok(word, " ");
	    do {
		strcpy(mat[i], tokenPtr);
		printf ("mat[i] = %s\n", mat[i]);
		i++;
		tokenPtr = strtok(NULL, " ");
		printf ("tokenPtr = %s\n", tokenPtr);
		if ((tokenPtr == "|") || (tokenPtr == "<") || (tokenPtr == ">") || strcmp(tokenPtr, ">>") == 0) {
		    printf ("tokenPtr if = %s", tokenPtr);
		    strcpy(mat[i], "NULL");
		    i++;
		} 
	    } while (tokenPtr != NULL);
	    strcpy(mat[i], "NULL");
	}
    }
    i = 0;
    while (strcmp(mat[i], "NULL") != 0) {
	printf ("%s ", mat[i]);
	i++;
    }
    printf ("\n");
    return 0;
}
