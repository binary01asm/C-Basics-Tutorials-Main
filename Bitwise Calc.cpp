#include <iostream>
#include <string>
#include <bitset>
#include <sstream>
#include <iomanip>

enum class InputMode {
    Decimal,
    Binary,
    Hexadecimal
};

class BitwiseCalculator {
private:
    InputMode input_mode = InputMode::Decimal;

    void display_menu() const {
        std::cout << "Bitwise Calculator\n";
        std::cout << "==================\n";
        std::cout << "1. AND\n";
        std::cout << "2. OR\n";
        std::cout << "3. XOR\n";
        std::cout << "4. NOT\n";
        std::cout << "5. Left Shift\n";
        std::cout << "6. Right Shift\n";
        std::cout << "7. Switch Input Mode (Current: " << get_input_mode_string() << ")\n";
        std::cout << "8. Exit\n";
        std::cout << "==================\n";
        std::cout << "Choose an option: ";
    }

    std::string get_input_mode_string() const {
        switch (input_mode) {
            case InputMode::Decimal: return "decimal";
            case InputMode::Binary: return "binary";
            case InputMode::Hexadecimal: return "hexadecimal";
            default: return "unknown";
        }
    }

    long long convert_input(const std::string& value) const {
        try {
            switch (input_mode) {
                case InputMode::Decimal:
                    return std::stoll(value);
                case InputMode::Binary:
                    return std::stoll(value, nullptr, 2);
                case InputMode::Hexadecimal:
                    return std::stoll(value, nullptr, 16);
                default:
                    throw std::runtime_error("Invalid input mode");
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid input format\n";
            throw;
        }
    }

    void display_result(long long result) const {
        switch (input_mode) {
            case InputMode::Decimal:
                std::cout << "Result: " << result << "\n";
                break;
            case InputMode::Binary:
                std::cout << "Result: " << std::bitset<64>(result).to_string().substr(
                    std::bitset<64>(result).to_string().find('1') != std::string::npos ? 
                    std::bitset<64>(result).to_string().find('1') : 63) << "\n";
                break;
            case InputMode::Hexadecimal:
                std::cout << "Result: " << std::hex << std::uppercase << result << std::dec << "\n";
                break;
        }
    }

    void bitwise_operation(long long op1, long long op2, const std::string& op_name) {
        long long result;
        
        if (op_name == "AND") {
            result = op1 & op2;
        } else if (op_name == "OR") {
            result = op1 | op2;
        } else if (op_name == "XOR") {
            result = op1 ^ op2;
        } else if (op_name == "NOT") {
            result = ~op1;
        } else if (op_name == "LSHIFT") {
            result = op1 << op2;
        } else if (op_name == "RSHIFT") {
            result = op1 >> op2;
        } else {
            std::cerr << "Invalid operation\n";
            return;
        }

        display_result(result);
    }

public:
    void run() {
        std::string choice;
        
        while (true) {
            display_menu();
            
            std::getline(std::cin, choice);
            
            if (choice == "1" || choice == "2" || choice == "3" || 
                choice == "5" || choice == "6") {
                std::string op1_str, op2_str;
                
                std::cout << "Enter first operand: ";
                std::getline(std::cin, op1_str);
                
                std::cout << "Enter second operand: ";
                std::getline(std::cin, op2_str);
                
                try {
                    long long op1 = convert_input(op1_str);
                    long long op2 = convert_input(op2_str);
                    
                    if (choice == "1") bitwise_operation(op1, op2, "AND");
                    else if (choice == "2") bitwise_operation(op1, op2, "OR");
                    else if (choice == "3") bitwise_operation(op1, op2, "XOR");
                    else if (choice == "5") bitwise_operation(op1, op2, "LSHIFT");
                    else if (choice == "6") bitwise_operation(op1, op2, "RSHIFT");
                } catch (const std::exception&) {
                    // Error already displayed in convert_input
                }
            } else if (choice == "4") {
                std::string op1_str;
                
                std::cout << "Enter operand: ";
                std::getline(std::cin, op1_str);
                
                try {
                    long long op1 = convert_input(op1_str);
                    bitwise_operation(op1, 0, "NOT");
                } catch (const std::exception&) {
                    // Error already displayed in convert_input
                }
            } else if (choice == "7") {
                std::cout << "Choose input mode:\n";
                std::cout << "1. Decimal\n";
                std::cout << "2. Binary\n";
                std::cout << "3. Hexadecimal\n";
                std::cout << "Enter choice: ";
                
                std::string mode_choice;
                std::getline(std::cin, mode_choice);
                
                if (mode_choice == "1") {
                    input_mode = InputMode::Decimal;
                } else if (mode_choice == "2") {
                    input_mode = InputMode::Binary;
                } else if (mode_choice == "3") {
                    input_mode = InputMode::Hexadecimal;
                } else {
                    std::cout << "Invalid input mode choice\n";
                }
            } else if (choice == "8") {
                std::cout << "Exiting...\n";
                break;
            } else {
                std::cout << "Invalid choice\n";
            }
            
            std::cout << "\n";
        }
    }
};

int main() {
    BitwiseCalculator calculator;
    calculator.run();
    return 0;
}