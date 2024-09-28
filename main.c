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

// Recursive function to generate all possible phrases using pointers
void generatePhrases(char* str, int index, char* result) {
    if (index == strlen(str)) {
        printf("%s\n", result);
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
        generatePhrases(str, index + 1, newResult);
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
            generatePhrases(str, index + 2, newResult);
        }
    }
}

int main() {
    char number[100];
    printf("Enter a number: ");
    scanf("%s", number);

    char result[100] = "";
    printf("Possible element phrases:\n");
    generatePhrases(number, 0, result);

    return 0;
}
