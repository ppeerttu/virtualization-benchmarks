/**
 * @file main.cpp
 * This is the main program containing square matrix calculator interface.
 * @date 7.3.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 */

#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include <string>
#include <stack>
#include <cctype>
#include <cstdlib>
#include "squarematrix.hpp"
#include "elementarymatrix.hpp"
#include "compositematrix.hpp"

void listenForUserInput();
bool isAddToValuation(const std::string str);
void addToValuation(std::string str, Valuation& val);
bool isReadyForComposition(const std::stack<uni_sq>& m);
void printStackInfo(const std::stack<uni_sq>& m);
void printHelp();
bool isInteger(const std::string& s);
void createPlusCompositeMatrix(std::stack<uni_sq>& matrixes);
void createMinusCompositeMatrix(std::stack<uni_sq>& matrixes);
void createMultiplyCompositeMatrix(std::stack<uni_sq>& matrixes);


int main( int argc, char* argv[] ) {

  // Run unit tests
  int result = Catch::Session().run( argc, argv );

  // Start the calculator
  listenForUserInput();
  return 0;
}

/**
 * Listen for user input until the user types 'quit'.
 * Gather all given matrixes into the stack and composite
 * calculation based on user input on them.
 */
void listenForUserInput() {
  std::stack<uni_sq> matrixes;
  Valuation val {};
  std::map<std::string, int> cmdMapper = {
    { "quit", 1 },
    { "+", 2 },
    { "-", 3 },
    { "*", 4 },
    { "=", 5 },
    { "help", 6 },
    { "stack", 7 }
  };
  std::string cmd;
  int cmdValue;
  bool loop = true;

  while (loop) {
    std::cout << "Give input (type 'help' for possible commands): ";
    std::cin >> cmd;

    try {
      cmdValue = cmdMapper.at(cmd);
    } catch (const std::exception& e) {
      cmdValue = 0;
    }

    switch (cmdValue) {
      case 1:
        loop = false;
        std::cout << "Exiting...\n";
        break;
      case 2:
        createPlusCompositeMatrix(matrixes);
        break;
      case 3:
        createMinusCompositeMatrix(matrixes);
        break;
      case 4:
        createMultiplyCompositeMatrix(matrixes);
        break;
      case 5:
        if (matrixes.size() < 1) {
          std::cout << "Cant't evaluate: the stack is empty!\n\n";
          break;
        }
        try {
          std::cout << matrixes.top()->evaluate(val) << std::endl;
        } catch (std::exception& e) {
          std::cout << "Can't evaluate: undefined variables in matrixes!\n\n";
          break;
        }
        break;
      case 6:
        printHelp();
        break;
      case 7:
        printStackInfo(matrixes);
        break;
      default:
        if (isAddToValuation(cmd)) {
          addToValuation(cmd, val);
          break;
        }
        try {
          uni_sq ptr(new SymbolicSquareMatrix(cmd));
          matrixes.push(std::move(ptr));
          std::cout << "SquareMatrix " << matrixes.top() << " created\n";
        } catch (std::exception& e) {
          std::cout << "Received invalid matrix!\n\n";
        }
        break;
    }
  }
}

/**
 * Take two matrixes from the stack and generate a multiply
 * composite matrix out of them and put it back on top of the stack.
 * @param matrixes - The stack holding the matrixes.
 */
void createMultiplyCompositeMatrix(std::stack<uni_sq>& matrixes) {
  if (!isReadyForComposition(matrixes)) {
    std::cout << "Can't composite a calculation: only 1 matrix at stack!\n\n";
    return;
  }
  uni_sq sq1 = std::move(matrixes.top());
  matrixes.pop();
  uni_sq sq2 = std::move(matrixes.top());
  matrixes.pop();
  try {
    uni_sq ptr(new CompositeSquareMatrix(*sq2, *sq1, std::multiplies<ConcreteSquareMatrix>(), '*'));
    matrixes.push(std::move(ptr));
    std::cout << matrixes.top() << std::endl;
  } catch (std::exception& e) {
    std::cout << "Dimensions do not match!\n\n";
    matrixes.push(std::move(sq2));
    matrixes.push(std::move(sq1));
  }
}

/**
 * Take two matrixes from the stack and generate a minus
 * composite matrix out of them and put it back on top of the stack.
 * @param matrixes - The stack holding the matrixes.
 */
void createMinusCompositeMatrix(std::stack<uni_sq>& matrixes) {
  if (!isReadyForComposition(matrixes)) {
    std::cout << "Can't composite a calculation: only 1 matrix at stack!\n\n";
    return;
  }
  uni_sq sq1 = std::move(matrixes.top());
  matrixes.pop();
  uni_sq sq2 = std::move(matrixes.top());
  matrixes.pop();
  try {
    uni_sq ptr(new CompositeSquareMatrix(*sq2, *sq1, std::minus<ConcreteSquareMatrix>(), '-'));
    matrixes.push(std::move(ptr));
    std::cout << matrixes.top() << std::endl;
  } catch (std::exception& e) {
    std::cout << "Dimensions do not match!\n\n";
    matrixes.push(std::move(sq2));
    matrixes.push(std::move(sq1));
  }
}

/**
 * Take two matrixes from the stack and generate a plus
 * composite matrix out of them and put it back on top of the stack.
 * @param matrixes - The stack holding the matrixes.
 */
void createPlusCompositeMatrix(std::stack<uni_sq>& matrixes) {
  if (!isReadyForComposition(matrixes)) {
    std::cout << "Can't composite a calculation: only 1 matrix at stack!\n\n";
    return;
  }
  uni_sq sq1 = std::move(matrixes.top());
  matrixes.pop();
  uni_sq sq2 = std::move(matrixes.top());
  matrixes.pop();
  try {
    uni_sq ptr(new CompositeSquareMatrix(*sq2, *sq1, std::plus<ConcreteSquareMatrix>(), '+'));
    matrixes.push(std::move(ptr));
    std::cout << matrixes.top() << std::endl;
  } catch (std::exception& e) {
    std::cout << "Dimensions do not match!\n\n";
    matrixes.push(std::move(sq2));
    matrixes.push(std::move(sq1));
  }
}

/**
 * Check if the given input can be generally regarded
 * as a variable initialization for valuation.
 * @input str - The given user input
 * @return - True if possible variable initialization, false otherwise
 */
bool isAddToValuation(const std::string str) {
  if (
    str.length() < 3
    || !isalpha(str.at(0))
    || str.at(1) != '='
  ) return false;
  return true;
}

/**
 * Insert a new variable into the given valuation.
 * @param str - The given variable initialization input
 * @param val - The valuation for the variable
 */
void addToValuation(std::string str, Valuation& val) {
  char key = str.at(0);
  int value;
  str.erase(0, 2);
  if (isInteger(str)) {
    try {
      value = std::stoi(str);
      val[key] = value;
      std::cout << key << " is " << value << std::endl;
    } catch (std::exception& e) {
      std::cout << "Invalid value for key " << key << " (it should be number)\n";
    }
    return;
  }
  if (str.length() != 1) {
    std::cout << "A variable key can contain only one character\n";
    return;
  }
  try {
    int value = val.at(str.at(0));
    val[key] = value;
    std::cout << key << " is " << value << std::endl;
  } catch (std::exception& e) {
    std::cout << "Undefined variable '" << str.at(0) << "'\n";
  }

}

/**
 * Check if the given stack has at least one SquareMatrix in it
 * @return - False if empty, true otherwise
 */
bool isReadyForComposition(const std::stack<uni_sq>& s) {
  return (s.size() > 1);
}

/**
 * Check if given string can be formally be regarded as an
 * integer (ignoring the size).
 * @return - True if the string can be regarded as an integer, false otherwise
 */
bool isInteger(const std::string& s) {
   if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;
   char *p;
   // Iterate s character by character and if found any character that is not
   // possible to be part of an integer, assign it into p
   strtol(s.c_str(), &p, 10) ;
   return (*p == 0) ;
}

/**
 * Print the size of the stack and the top element of the stack.
 */
void printStackInfo(const std::stack<uni_sq>& m) {
  if (m.size() == 0) {
    std::cout << "The stack is empty\n";
    return;
  }
  std::cout << "The stack size: " << m.size() << std::endl;
  std::cout << "The square matrix on top: " << m.top() << std::endl;
}

/**
 * Print all possible commands and short descriptions for them.
 */
void printHelp() {
  std::cout << "Possible commands:\n\n";
  std::cout << "'quit'          - Exit process\n";
  std::cout << "'[[2,3][2,x]]'  - Create a square matrix, values can be either an integer or variables (see storing a variable below)\n";
  std::cout << "'+'             - Composite a plus calculation with two latest matrixes from the stack\n";
  std::cout << "'-'             - Composite a minus calculation with two latest matrixes from the stack\n";
  std::cout << "'*'             - Composite a multiply calculation with two latest matrixes from the stack\n";
  std::cout << "'='             - Evaluate and print the latest matrix from the stack\n";
  std::cout << "'<char>=<int>'  - Store an integer into an variable (character, a-zA-Z)\n";
  std::cout << "'<char>=<char>' - Store an integer into an variable from another variable (character, a-zA-Z)\n";
  std::cout << "'help'          - Print possible commands\n";
  std::cout << "'stack'         - Print the square matrix on top of the stack and the size of the stack\n\n";
}
