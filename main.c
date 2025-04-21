#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Function to get element symbol from atomic number
char* getElementSymbol(int num) {
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

// Load dictionary from file
char** loadDictionary(const char* filename, int* size) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open dictionary file.\n");
        exit(EXIT_FAILURE);
    }

    char** dictionary = malloc(100000 * sizeof(char*));
    char word[100];
    *size = 0;

    while (fgets(word, sizeof(word), file)) {
        word[strcspn(word, "\n")] = '\0'; // Remove newline
        dictionary[*size] = malloc(strlen(word) + 1);
        strcpy(dictionary[*size], word);
        (*size)++;
    }

    fclose(file);
    return dictionary;
}

// Convert word to lowercase
void toLowerCase(char* dest, const char* src) {
    int i = 0;
    while (src[i]) {
        dest[i] = tolower(src[i]);
        i++;
    }
    dest[i] = '\0';
}

// Check if word is in dictionary
int isWordInDictionary(const char* word, char** dictionary, int size) {
    char lowerWord[100];
    toLowerCase(lowerWord, word);

    for (int i = 0; i < size; i++) {
        char lowerDictWord[100];
        toLowerCase(lowerDictWord, dictionary[i]);
        if (strcmp(lowerWord, lowerDictWord) == 0) {
            return 1;
        }
    }
    return 0;
}

// Levenshtein distance (edit distance)
int levenshteinDistance(const char *s1, const char *s2) {
    int len1 = strlen(s1), len2 = strlen(s2);
    int *v0 = calloc(len2 + 1, sizeof(int));
    int *v1 = calloc(len2 + 1, sizeof(int));

    for (int i = 0; i <= len2; i++) v0[i] = i;

    for (int i = 0; i < len1; i++) {
        v1[0] = i + 1;
        for (int j = 0; j < len2; j++) {
            int cost = (tolower(s1[i]) == tolower(s2[j])) ? 0 : 1;
            v1[j + 1] = fmin(fmin(v1[j] + 1, v0[j + 1] + 1), v0[j] + cost);
        }
        int *tmp = v0; v0 = v1; v1 = tmp;
    }

    int distance = v0[len2];
    free(v0);
    free(v1);
    return distance;
}

// Sugg
void suggestClosestWord(const char* phrase, char** dictionary, int size) {
    int minDistance = 999;
    char* closestWord = NULL;

    for (int i = 0; i < size; i++) {
        int dist = levenshteinDistance(phrase, dictionary[i]);
        if (dist < minDistance) {
            minDistance = dist;
            closestWord = dictionary[i];
        }
    }

    if (closestWord) {
        printf("Closest match: %s (distance = %d)\n", closestWord, minDistance);
    }
}

// Recursive phrase generator
void generatePhrases(char* str, int index, char* result, char** dictionary, int dictSize, int* foundAny) {
    if (index == strlen(str)) {
        if (isWordInDictionary(result, dictionary, dictSize)) {
            printf("Found: %s\n", result);
            *foundAny = 1;
        } else {
            printf("Not Found: %s\n", result);
            suggestClosestWord(result, dictionary, dictSize);
        }
        return;
    }

    // Try 1 digit
    char digitStr[2];
    digitStr[0] = str[index];
    digitStr[1] = '\0';
    int num = atoi(digitStr);
    char* symbol = getElementSymbol(num);
    if (symbol) {
        char newResult[100];
        strcpy(newResult, result);
        strcat(newResult, symbol);
        generatePhrases(str, index + 1, newResult, dictionary, dictSize, foundAny);
    }

    // Try 2 digits
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
            generatePhrases(str, index + 2, newResult, dictionary, dictSize, foundAny);
        }
    }
}

int main() {
    int dictSize = 0;
    char** dictionary = loadDictionary("words.txt", &dictSize);

    char number[100];
    printf("Enter the number code: ");
    scanf("%s", number);

    char result[100] = "";
    int foundAny = 0;
    printf("\nPossible element phrases:\n");
    generatePhrases(number, 0, result, dictionary, dictSize, &foundAny);

    if (!foundAny) {
        printf("\nNo exact match found, but suggestions were provided above.\n");
    }

    for (int i = 0; i < dictSize; i++) {
        free(dictionary[i]);
    }
    free(dictionary);

    return 0;
}
