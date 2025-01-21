/*
 *
 * periodic decoder version:1.2
 * use dictionary for detecting variable compositions
 * Mohammad Mahdi Khaligh
 * github: gatemans
 *
 */

//------------------------------ver1.2-------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to map atomic numbers to element symbols using pointers
char* getElementSymbol(int num) {
    // List of element symbols up to atomic number 118 (Oganesson)
    static char* elements[] = {
            "H",  "He", "Li", "Be", "B",  "C",  "N",  "O",  "F",  "Ne",
            "Na", "Mg", "Al", "Si", "P",  "S",  "Cl", "Ar", "K",  "Ca",
            "Sc", "Ti", "V",  "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn",
            "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr", "Y",  "Zr",
            "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn",
            "Sb", "Te", "I",  "Xe", "Cs", "Ba", "La", "Ce", "Pr", "Nd",
            "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb",
            "Lu", "Hf", "Ta", "W",  "Re", "Os", "Ir", "Pt", "Au", "Hg",
            "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th",
            "Pa", "U",  "Np", "Pu", "Am", "Cm", "Bk", "Cf", "Es", "Fm",
            "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds",
            "Rg", "Cn", "Nh", "Fl", "Mc", "Lv", "Ts", "Og"
    };

    if (num >= 1 && num <= 118) {
        return elements[num - 1];
    }
    return NULL;
}

// Function to load the dictionary file into memory
char** loadDictionary(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open dictionary file.\n");
        exit(EXIT_FAILURE);
    }

    char** dictionary = malloc(1000000 * sizeof(char*));
    char result[100000] ;
    char newResult[100000];

    char word[100];
    *size = 0;

    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0'; // Remove newline character
        dictionary[*size] = malloc(strlen(word) + 1);
        strcpy(dictionary[*size], word);
        (*size)++;
    }

    fclose(file);
    return dictionary;
}

// Function to check if a phrase exists in the dictionary
int isWordInDictionary(const char* word, char** dictionary, int size) {
    char lowerWord[100];
    for (int i = 0; word[i]; i++) {
        lowerWord[i] = tolower(word[i]);
    }
    lowerWord[strlen(word)] = '\0';

    for (int i = 0; i < size; i++) {
        char lowerDictWord[100];
        for (int j = 0; dictionary[i][j]; j++) {
            lowerDictWord[j] = tolower(dictionary[i][j]);
        }
        lowerDictWord[strlen(dictionary[i])] = '\0';

        if (strcmp(lowerWord, lowerDictWord) == 0) {
            return 1; // Word found
        }
    }
    return 0; // Word not found
}

// Recursive function to generate all possible phrases
void generatePhrases(char* str, int index, char* result, char** dictionary, int dictSize) {
    if (index == strlen(str)) {
        if (isWordInDictionary(result, dictionary, dictSize)) {
            printf("Found: %s\n", result);
        } else {
            printf("Not Found: %s\n", result);
        }
        return;
    }

    // Try to take 1 digit
    char digitStr[2];
    digitStr[0] = str[index];
    digitStr[1] = '\0';
    int num = atoi(digitStr);
    char* symbol = getElementSymbol(num);
    if (symbol) {
        char newResult[100];
        strcpy(newResult, result);
        strcat(newResult, symbol);
        generatePhrases(str, index + 1, newResult, dictionary, dictSize);
    }

    // Try to take 2 digits
    if (index + 1 < strlen(str)) {
        char twoDigitStr[3];
        twoDigitStr[0] = str[index];
        twoDigitStr[1] = str[index + 1];
        twoDigitStr[2] = '\0';
        num = atoi(twoDigitStr);
        symbol = getElementSymbol(num);
        if (symbol) {
            char newResult[100];
            strcpy(newResult, result);
            strcat(newResult, symbol);
            generatePhrases(str, index + 2, newResult, dictionary, dictSize);
        }
    }
}

int main() {
    // Load dictionary file
    int dictSize = 0;
    char** dictionary = loadDictionary("words.txt", &dictSize);

    // Input number sequence
    char number[100];
    printf("Enter the code:");
    scanf("%s", number);

    char result[100] = "";
    printf("Possible element phrases:\n");
    generatePhrases(number, 0, result, dictionary, dictSize);

    // Free allocated memory for dictionary
    for (int i = 0; i < dictSize; i++) {
        free(dictionary[i]);
    }
    free(dictionary);

    return 0;
}
