#include <string.h>

char* passwordChecker(char* password) {
    if (strlen(password) < 8 || strlen(password) > 200) return "Password is either too much small or big\n";
    int countLower = 0;
    int countUpper = 0;
    int countDigits = 0;
    for (int i = 0; i < strlen(password); ++i) {
        if (*(password+i) >= '0' && *(password+i) <= '9') countDigits++;
        else if (*(password+i) >= 'A' && *(password+i) <= 'Z') countUpper++;
        else if (*(password+i) >= 'a' && *(password+i) <= 'z') countLower++;
    }
    if (countLower <= 1) return "password must have atleast two lower case!\n";
    else if (countUpper <= 1) return "password must have atleast two upper case!\n";
    else if (countDigits <= 1) return "password must have atleast two digits!\n";
    else return "\0";
}

void removeNewline(char* str) {
    int length = strlen(str);
    if (*(str+length-1) == '\n') *(str+length-1) = '\0';
}  
