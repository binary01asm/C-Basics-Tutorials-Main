# ============================================================================
# SIMPLE CALCULATOR PROGRAM
# This program performs basic arithmetic operations on two numbers
# ============================================================================

# ----------------------------------------------------------------------------
# ARITHMETIC FUNCTION DEFINITIONS
# Each function takes two parameters and returns a single result
# ----------------------------------------------------------------------------

def add(a, b):
    """Return the sum of two numbers."""
    return a + b  # Perform addition


def sub(a, b):
    """Return the difference between two numbers (a - b)."""
    return a - b  # Perform subtraction


def mul(a, b):
    """Return the product of two numbers."""
    return a * b  # Perform multiplication


def division(a, b):
    """Return the quotient of a divided by b (a / b)."""
    return a / b  # Perform division (floating-point result)


# ----------------------------------------------------------------------------
# PROGRAM HEADER DISPLAY
# Creates a visual separator for better user interface
# ----------------------------------------------------------------------------
print("~"*10 + "="*50 + "~"*10)  # Print decorative border


# ----------------------------------------------------------------------------
# USER INPUT SECTION
# Collects numbers and operation choice from the user
# ----------------------------------------------------------------------------
num1 = float(input("Enter First Number: "))   # Convert first input to float for decimal support
num2 = float(input("Enter Second Number: "))  # Convert second input to float for decimal support
op = input("Enter The Operation (+, -, *, /): ")  # Get the arithmetic operator


# ----------------------------------------------------------------------------
# OPERATION SELECTOR FUNCTION
# Maps operator symbols to corresponding arithmetic functions
# ----------------------------------------------------------------------------
def operator(ops):
    """
    Select and execute the appropriate arithmetic operation.
    
    Parameters:
    ops (str): The operator symbol (+, -, *, /)
    
    Returns:
    float or str: The result of the operation or an error message
    """
    if ops == '+':  # Check for addition
        return add(num1, num2)  # Call add function
    elif ops == '-':  # Check for subtraction
        return sub(num1, num2)  # Call sub function
    elif ops == '*':  # Check for multiplication
        return mul(num1, num2)  # Call mul function
    elif ops == '/':  # Check for division
        if num2 != 0:  # Prevent division by zero
            return division(num1, num2)  # Call division function
        else:
            return "Error: Division by zero!"  # Handle division by zero error
    else:
        return "Error: Invalid operation!"  # Handle invalid operator input


# ----------------------------------------------------------------------------
# EXECUTE OPERATION AND DISPLAY RESULT
# ----------------------------------------------------------------------------
result = operator(op)  # Call operator function with user's chosen operation
print(f"Result: {result}")  # Display the result using f-string formatting