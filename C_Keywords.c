#include <stdio.h>
#include <string.h>

int main() {
    // Array of all 32 C programming keywords (C99 standard)
    char *keywords[] = {
        "auto", "break", "case", "char", "const", "continue", "default", 
        "do", "double", "else", "enum", "extern", "float", "for", "goto", 
        "if", "int", "long", "register", "return", "short", "signed", 
        "sizeof", "static", "struct", "switch", "typedef", "union", 
        "unsigned", "void", "volatile", "while"
    };
    
    int total_keywords = sizeof(keywords) / sizeof(keywords[0]);
    
    printf("=== All C Programming Keywords ===\n");
    printf("Total keywords: %d\n\n", total_keywords);
    
    // Print keywords in columns
    for(int i = 0; i < total_keywords; i++) {
        printf("%-12s", keywords[i]);  // Left-aligned with 12 char width
        
        // Print 4 keywords per line
        if((i + 1) % 4 == 0) {
            printf("\n");
        }
    }
    
    printf("\n\n=== Keywords with Categories ===\n\n");
    
    // Print categorized keywords
    printf("Data Types:\n");
    char *data_types[] = {"char", "double", "float", "int", "long", "short", "signed", "unsigned", "void"};
    for(int i = 0; i < 9; i++) printf("%s ", data_types[i]);
    
    printf("\n\nControl Flow:\n");
    char *control[] = {"break", "case", "continue", "default", "do", "else", "for", "goto", "if", "return", "switch", "while"};
    for(int i = 0; i < 12; i++) printf("%s ", control[i]);
    
    printf("\n\nStorage Classes:\n");
    char *storage[] = {"auto", "extern", "register", "static"};
    for(int i = 0; i < 4; i++) printf("%s ", storage[i]);
    
    printf("\n\nOther Modifiers:\n");
    char *modifiers[] = {"const", "enum", "sizeof", "struct", "typedef", "union", "volatile"};
    for(int i = 0; i < 7; i++) printf("%s ", modifiers[i]);
    
    printf("\n\n");
    
    return 0;
}