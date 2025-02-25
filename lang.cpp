#include <iostream> // Input/Output Streams
#include <cmath> // Mathematical Functions (e.g., pow, sqrt, sin, cos, tan)
#include <string> // String Class
#include <vector> // Vector Class
#include <stdexcept> // Standard Exception Classes (e.g., invalid_argument, runtime_error)
#include <cctype> // Character Classification Functions (e.g., isdigit, isalpha)
#include <cstdlib> // General Purpose Functions (e.g., atof, atoi)
#include <cfloat> // Floating Point Constants (e.g., DBL_MAX, DBL_MIN)
#include <climits> // Integer Constants (e.g., INT_MAX, INT_MIN)
#include <algorithm>

using namespace std;

struct T {
    string typ;
    string bol;
    double num;
};
// r is the number of register, mem is the stack memory
// r[0] is the program pointer
const int r_size = 16;
const int m_size = 256;
vector<double> r(r_size);
vector<double> mem(m_size);

double parseFactor(string expression, size_t& pos);
double parseTerm(string expression, size_t& pos);
double parseExpression(string expression, size_t& pos);
double compute(string expression);
double talk( string expression);

string sym_expr = "=+-<>&|";
string sym_term = "_*/^";
string sym_fact = ".()#![]~{}";

bool isskip = false; bool isinside = false;
bool check(string format, char c){
    return format.find(c) != string::npos;
}

// Recursive descent parser for factors in mathematical expressions
double parseFactor(string expression, size_t& pos) {

    if (expression[pos] == '~') {
        pos++;
        double result = parseFactor(expression, pos);
        //if(result > r_size - 1){ throw runtime_error("Register overflow.");}
        isskip = true;
        return result;

    } else if (expression[pos] == '#') {
        pos++;
        double result = parseFactor(expression, pos);
        //if(result > r_size - 1){ throw runtime_error("Register overflow.");}
        r[0] = result;
        //isskip = true;
        return mem[result];

    } else if (expression[pos] == '{') {
        pos++;
        isinside = true;
        double result = parseExpression(expression, pos);
        isinside = false;
        if (expression[pos] != '}') { throw runtime_error("Unbalanced parentheses"); }
        pos++;

        return result;
    } else if (expression[pos] == '[') {
        pos++;
        isinside = true;
        double result = parseExpression(expression, pos);
        isinside = false;
        if (expression[pos] != ']') { throw runtime_error("Unbalanced parentheses"); }
        if (result > m_size - 1 || result < 0)    { throw runtime_error("Register overflow.");}
        pos++;

        return mem[result];
    } else if (expression[pos] == '(') {
        pos++;
        isinside = true;
        double result = parseExpression(expression, pos);
        isinside = false;
        if (expression[pos] != ')') { throw runtime_error("Unbalanced parentheses"); }
        pos++;

        return result;
    } else if (expression[pos] == '!') {
        pos++;
        double result = parseFactor(expression, pos);
        double factorial = (result <0) ? -1:1;
        for (int i = 2; i <= abs((int)result); i++) {
            factorial *= i;
        }
        return factorial;
    } else if (expression[pos] == '-' || expression[pos] == '+') {
        double sign = (expression[pos] == '-') ? -1:1;
        pos++;
        double result = parseFactor(expression, pos);
        return sign*result;
    } else {
        size_t start = pos;
        while (pos < expression.size() && (isdigit(expression[pos]) || expression[pos] == '.')) {
            pos++;
        }
        return atof(expression.substr(start, pos - start).c_str());
    }
}
// Recursive descent parser for terms in mathematical expressions
double parseTerm(string expression, size_t& pos) {
    double result = parseFactor(expression, pos);
    while (pos < expression.size() && check(sym_term,expression[pos]))
    {
        char op = expression[pos++];
        double factor = parseFactor(expression, pos);
        if(op == '_'){
            int sign = (factor>result)? 1:-1;
            int i = (int) result;
            while(i!= (int) factor){
                talk(to_string(i));
                i+=sign;
            }
            result = factor;
        }
        else if (op == '*') {   result *= factor;} 
        else if (op == '/') {
            if (factor == 0)    throw runtime_error("Division by zero");
                                result /= factor;
        } else if (op == '^') { result = pow(result, factor); }
    }
    return result;
}

// Recursive descent parser for mathematical expressions
double parseExpression(string expression, size_t& pos) {
    double result = parseTerm(expression, pos);
    while (pos < expression.size() && check(sym_expr,expression[pos]))
    {
        char op = expression[pos++];
        double term = parseTerm(expression, pos);
        if (op == '=')      { return (result == term) ? 1 : -1;}
        else if (op == '+') { return result + term;} 
        else if (op == '-') { return result - term; } 
        else if (op == '&') { return ((result>0) && (term >0)) ? 1 : -1; } 
        else if (op == '|') { return ((result>0) || (term >0)) ? 1 : -1; } 
        else if (op == '>') { return (result > term) ? 1 : -1; }
        else if (op == '<') { return (result < term) ? 1 : -1; }
    }
}

// Function to calculate the result of a mathematical expression
double compute(string expression) {
    // Remove any whitespace from the expression
    
    expression.erase(remove(expression.begin(), expression.end(), ' '), expression.end());
    cout << "expr = " << expression << endl;
    // Check if the expression contains any invalid characters
    for (int i = 0; i < expression.size(); i++) {
        char c = expression[i];
        if (!isdigit(c) && !check(sym_expr+sym_fact+sym_term,c)) 
        {
            throw invalid_argument("Invalid character in expression "+expression+".");
        }
    }
    // Evaluate the expression using a simple recursive descent parser
    size_t pos = 0;
    return parseExpression(expression, pos);
}

double talk( string expression){
    double result;
    try {
        result = compute(expression);
        cout <<"[" <<r[0]<<"]";

        if(!isskip){
            mem[r[0]] = result;
            cout << ":" << result << endl;
        }
    } catch (const exception& e) {
        cout << " 'Error: " << e.what() <<"'" << endl;
        r[0]--;
    } 

    if(isskip){
        isskip = false;
        cout << "~"; 
        r[0] = result;
    }
    else{
        r[0] ++;
    } 
    cout <<"[" <<r[0]<<"]? ";
}

int main() {
    mem[0] = 255; // maximum size of memory
    cout << "Type '~[ptr::INT]' to start: ";
    while (true) {  
        double result; string expression;
        // only move the stack pointer without changing the memory
        cin >> expression;
        talk(expression);
    }
    return 0;
}
// 
