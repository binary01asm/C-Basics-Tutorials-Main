#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

typedef enum {
    INPUT_MODE_DECIMAL,
    INPUT_MODE_BINARY,
    INPUT_MODE_HEXADECIMAL
} InputMode;

typedef struct {
    InputMode input_mode;
} BitwiseCalculator;

const char* get_input_mode_string(InputMode mode) {
    switch (mode) {
        case INPUT_MODE_DECIMAL: return "decimal";
        case INPUT_MODE_BINARY: return "binary";
        case INPUT_MODE_HEXADECIMAL: return "hexadecimal";
        default: return "unknown";
    }
}

void display_menu(InputMode mode) {
    printf("Bitwise Calculator\n");
    printf("==================\n");
    printf("1. AND\n");
    printf("2. OR\n");
    printf("3. XOR\n");
    printf("4. NOT\n");
    printf("5. Left Shift\n");
    printf("6. Right Shift\n");
    printf("7. Switch Input Mode (Current: %s)\n", get_input_mode_string(mode));
    printf("8. Exit\n");
    printf("==================\n");
    printf("Choose an option: ");
}

int is_valid_binary(const char* str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (*str != '0' && *str != '1') return 0;
        str++;
    }
    return 1;
}

int is_valid_hexadecimal(const char* str) {
    if (*str == '\0') return 0;
    while (*str) {
        if (!isxdigit(*str)) return 0;
        str++;
    }
    return 1;
}

long long convert_input(const char* value, InputMode mode, int* success) {
    char* endptr;
    long long result;
    
    *success = 1;
    
    switch (mode) {
        case INPUT_MODE_DECIMAL:
            result = strtoll(value, &endptr, 10);
            if (*endptr != '\0') {
                printf("Invalid decimal input format\n");
                *success = 0;
                return 0;
            }
            return result;
            
        case INPUT_MODE_BINARY:
            if (!is_valid_binary(value)) {
                printf("Invalid binary input format\n");
                *success = 0;
                return 0;
            }
            result = strtoll(value, &endptr, 2);
            return result;
            
        case INPUT_MODE_HEXADECIMAL:
            if (!is_valid_hexadecimal(value)) {
                printf("Invalid hexadecimal input format\n");
                *success = 0;
                return 0;
            }
            result = strtoll(value, &endptr, 16);
            return result;
            
        default:
            printf("Invalid input mode\n");
            *success = 0;
            return 0;
    }
}

void display_result(long long result, InputMode mode) {
    switch (mode) {
        case INPUT_MODE_DECIMAL:
            printf("Result: %lld\n", result);
            break;
            
        case INPUT_MODE_BINARY: {
            int started = 0;
            printf("Result: ");
            for (int i = 63; i >= 0; i--) {
                if ((result >> i) & 1) {
                    started = 1;
                    printf("1");
                } else if (started) {
                    printf("0");
                }
            }
            if (!started) printf("0");
            printf("\n");
            break;
        }
            
        case INPUT_MODE_HEXADECIMAL:
            printf("Result: %llX\n", result);
            break;
    }
}

void bitwise_operation(long long op1, long long op2, const char* op_name, InputMode mode) {
    long long result;
    
    if (strcmp(op_name, "AND") == 0) {
        result = op1 & op2;
    } else if (strcmp(op_name, "OR") == 0) {
        result = op1 | op2;
    } else if (strcmp(op_name, "XOR") == 0) {
        result = op1 ^ op2;
    } else if (strcmp(op_name, "NOT") == 0) {
        result = ~op1;
    } else if (strcmp(op_name, "LSHIFT") == 0) {
        result = op1 << op2;
    } else if (strcmp(op_name, "RSHIFT") == 0) {
        result = op1 >> op2;
    } else {
        printf("Invalid operation\n");
        return;
    }
    
    display_result(result, mode);
}

void trim_newline(char* str) {
    size_t len = strlen(str);
    if (len > 0 && str[len-1] == '\n') {
        str[len-1] = '\0';
    }
}

int main() {
    BitwiseCalculator calculator;
    calculator.input_mode = INPUT_MODE_DECIMAL;
    
    char choice[10];
    char op1_str[100];
    char op2_str[100];
    char mode_choice[10];
    
    while (1) {
        display_menu(calculator.input_mode);
        
        if (!fgets(choice, sizeof(choice), stdin)) break;
        trim_newline(choice);
        
        if (strcmp(choice, "1") == 0 || strcmp(choice, "2") == 0 || 
            strcmp(choice, "3") == 0 || strcmp(choice, "5") == 0 || 
            strcmp(choice, "6") == 0) {
            
            printf("Enter first operand: ");
            if (!fgets(op1_str, sizeof(op1_str), stdin)) continue;
            trim_newline(op1_str);
            
            printf("Enter second operand: ");
            if (!fgets(op2_str, sizeof(op2_str), stdin)) continue;
            trim_newline(op2_str);
            
            int success1, success2;
            long long op1 = convert_input(op1_str, calculator.input_mode, &success1);
            long long op2 = convert_input(op2_str, calculator.input_mode, &success2);
            
            if (success1 && success2) {
                if (strcmp(choice, "1") == 0) 
                    bitwise_operation(op1, op2, "AND", calculator.input_mode);
                else if (strcmp(choice, "2") == 0) 
                    bitwise_operation(op1, op2, "OR", calculator.input_mode);
                else if (strcmp(choice, "3") == 0) 
                    bitwise_operation(op1, op2, "XOR", calculator.input_mode);
                else if (strcmp(choice, "5") == 0) 
                    bitwise_operation(op1, op2, "LSHIFT", calculator.input_mode);
                else if (strcmp(choice, "6") == 0) 
                    bitwise_operation(op1, op2, "RSHIFT", calculator.input_mode);
            }
            
        } else if (strcmp(choice, "4") == 0) {
            printf("Enter operand: ");
            if (!fgets(op1_str, sizeof(op1_str), stdin)) continue;
            trim_newline(op1_str);
            
            int success;
            long long op1 = convert_input(op1_str, calculator.input_mode, &success);
            
            if (success) {
                bitwise_operation(op1, 0, "NOT", calculator.input_mode);
            }
            
        } else if (strcmp(choice, "7") == 0) {
            printf("Choose input mode:\n");
            printf("1. Decimal\n");
            printf("2. Binary\n");
            printf("3. Hexadecimal\n");
            printf("Enter choice: ");
            
            if (!fgets(mode_choice, sizeof(mode_choice), stdin)) continue;
            trim_newline(mode_choice);
            
            if (strcmp(mode_choice, "1") == 0) {
                calculator.input_mode = INPUT_MODE_DECIMAL;
            } else if (strcmp(mode_choice, "2") == 0) {
                calculator.input_mode = INPUT_MODE_BINARY;
            } else if (strcmp(mode_choice, "3") == 0) {
                calculator.input_mode = INPUT_MODE_HEXADECIMAL;
            } else {
                printf("Invalid input mode choice\n");
            }
            
        } else if (strcmp(choice, "8") == 0) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice\n");
        }
        
        printf("\n");
    }
    
    return 0;
}