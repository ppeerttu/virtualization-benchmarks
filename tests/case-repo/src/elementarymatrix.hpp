/**
 * @file elementarymatrix.hpp
 * A file describing the API for class ElementaryMatrix.
 * @date 29.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see ElementaryMatrix
 */

#ifndef elementarymatrix_hpp
#define elementarymatrix_hpp

#include "element.hpp"
#include "squarematrix.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

template<class T>
class ElementaryMatrix : public SquareMatrix {

  unsigned int n;
  std::vector<std::vector<std::unique_ptr<T>>> elements;

  public:

    /**
     * An empty constructor (default constructor).
     */
    ElementaryMatrix() {};

    /**
     * A constructor for ElementaryMatrix that parses the parameter string and
     * throws an error if it is not valid square matrix. Otherwise the
     * numbers in given matrix are insterted into the ElementaryMatrix object.
     * @throws std::invalid_argument - Received invalid square matrix as input - dimensions do not match.
     * @param s - The input string representing a square matrix.
     */
    explicit ElementaryMatrix(const std::string& s);

    /**
     * A constructor for ElementaryMatrix that is formed from another instance of
     * ElementaryMatrix.
     * @param m - An instance of ElementaryMatrix to be copied.
     */
    ElementaryMatrix(const ElementaryMatrix& m);

    /**
     * Move constructor for ElementaryMatrix.
     * @param m - The ElementaryMatrix to be moved from.
     */
    ElementaryMatrix(ElementaryMatrix&& m): elements(std::move(m.elements)), n(m.n) { };

    /**
     * Destructor for ElementaryMatrix.
     */
    ~ElementaryMatrix() {};

    /**
     * An += operator for ElementaryMatrix, which will sum up the properties of
     * the caller and the parameter and then return a reference to the caller.
     * @throws std::out_of_range - Received invalid square matrix as input - dimensions do not match.
     * @param m The ElementaryMatrix instance to be added into the caller
     * {@code
     * ElementaryMatrix m1("[[1,2][3,4]]");
     * ElementaryMatrix m2("[[5,6][7,8]]");
     *
     * m1 += m2; // Doesn't modify m2, only m1
     * // Prints [[6, 8][10,12]]
     * std::cout << m1.toString() << std::endl;
     * }
     */
    ElementaryMatrix& operator+=(const ElementaryMatrix& m);

    /**
     * An -= operator for ElementaryMatrix, which will substract the properties of
     * the parameter from the caller and then return a reference to the caller.
     * @throws std::out_of_range - Received invalid square matrix as input - dimensions do not match.
     * @param m The ElementaryMatrix instance to be substracted from the caller
     * {@code
     * ElementaryMatrix m1("[[1,2][3,4]]");
     * ElementaryMatrix m2("[[5,6][7,8]]");
     *
     * m1 -= m2; // Doesn't modify m2, only m1
     * std::cout << m1.toString() << std::endl; // Prints [[-4,-4][-4,-4]]
     * }
     */
    ElementaryMatrix& operator-=(const ElementaryMatrix& m);

    /**
     * An *= operator for ElementaryMatrix, which is basically matrix multiplication.
     * See the code example for more details.
     * @throws std::out_of_range - Received invalid square matrix as input - dimensions do not match.
     * @param m The ElementaryMatrix instance to be multiplied with the caller
     * @return - A reference to this.
     * {@code
     * ElementaryMatrix m1("[[1,2][3,4]]");
     * ElementaryMatrix m2("[[5,6][7,8]]");
     *
     * m1 *= m2; // Doesn't modify m2, only m1
     * // Prints [[19,22][43,50]]
     * std::cout << m1.toString() << std::endl;
     * }
     */
    ElementaryMatrix& operator*=(const ElementaryMatrix& m);

    /**
     * Boolean operator for ElementaryMatrix; this uses
     * toString() -representation to determine the
     * equality of two ElementaryMatrix objects.
     * @param m - The object this is compared to.
     * @return - True if these matixes are equal to
     * each other, false otherwise.
     */
    bool operator==(const ElementaryMatrix& m) const {
      return (this->toString() == m.toString());
    };

    /**
     * Copy assignment operator for ElementaryMatrix. This uses plain
     * = -assignment operator to set the properties of parameter m
     * to the caller.
     * @param m - The object to be copied.
     * @return - A reference to the caller.
     */
    ElementaryMatrix& operator=(const ElementaryMatrix& m);

    /**
     * Move assignment operator for ElementaryMatrix. This operator
     * moves the properties of parameter to the caller, leaving
     * the parameter 'empty'.
     * @param m - The object to be moved.
     * @return - A reference to the caller.
     */
    ElementaryMatrix& operator=(ElementaryMatrix&& m);

    /**
     * Return a pointer to a new ElementaryMatrix
     * which is a copy of this.
     * @return - A reference to a new copy of this.
     */
    ElementaryMatrix* clone() const override {
      return new ElementaryMatrix(*this);
    };

    /**
     * Return a new ElementaryMatrix that is transposed version
     * from this ElementaryMatrix.
     * @return - The new ElementaryMatrix.
     * {@code
     * ElementaryMatrix m("[[1,2][3,4]]");
     * ElementaryMatrix m2 = m.transpose();
     * // Prints [[1,3][2,4]]
     * std::cout << m2.toString() << std::endl;
     * }
     */
    ElementaryMatrix transpose() const;

    /**
     * A function that prints the ElementaryMatrix into the given stream.
     * The form of print is similar to the parameter for ElementaryMatrix
     * constructor ElementaryMatrix(const std::string& str).
     * @param os - The stream in which the print is inserted to.
     * {@code
     * ElementaryMatrix matrix("[[1,2][3,4]]");
     * std::stringstream ss;
     * matrix.print(ss);
     * std::cout << ss; // Prints '[[1,2][3,4]]'
     * }
     */
    void print(std::ostream& os) const override {
      std::string str = "[";
      for (auto& ve : elements) {
        str += "[";
        for (auto& e : ve) {
          str += e->toString();
          str += ",";
        }
        if (str.length() > 0) str.pop_back();
        str += "]";
      }
      str += "]";
      os << str;
    };

    /**
     * A function that returns a string which includes a same expression form
     * of ElementaryMatrix as described in ElementaryMatrix::print.
     * @return A new string containing the representation of this ElementaryMatrix.
     */
    std::string toString() const override {
      std::stringstream ss;
      ss << *this;
      return ss.str();
    };

    /**
     * Returns a copy of this.
     * @param val - Valuation for evaluation
     * @return - A new ElementaryMatrix which is a copy of this.
     */
    ElementaryMatrix<IntElement> evaluate(const Valuation& val) const override;

    /**
     * Return the dimension of ElementaryMatrix.
     * @return - The dimension of the ElementaryMatrix.
     */
    int getDim() const override {
      return n;
    };
};

/**
 * Operator + for ElementaryMatrix. The same as ElementaryMatrix::operator+= but instead of
 * returning the reference to the caller this operator creates a new instance
 * of ElementaryMatrix and sets the sum of two parameter objects into the new
 * ElementaryMatrix.
 * @param elem1 - The ElementaryMatrix to be summed up with elem2
 * @param elem2 - The ElementaryMatrix to be summed up elem1
 * @return  A new ElementaryMatrix
 * {@code
 * ElementaryMatrix m1("[[1,2][3,4]]");
 * ElementaryMatrix m2("[[5,6][7,8]]");
 *
 * ElementaryMatrix m3 = m1 + m2; // Doesn't modify m2 or m1
 * // Prints [[6, 8][10,12]]
 * std::cout << m3.toString() << std::endl;
 * }
 */
template <typename T>
ElementaryMatrix<T> operator+(const ElementaryMatrix<T>& first, const ElementaryMatrix<T>& second);

/**
 * Operator - for ElementaryMatrix. The same as ElementaryMatrix::operator-= but instead of
 * returning the reference to the caller this operator creates a new instance
 * of ElementaryMatrix and sets the substraction of two parameter objects into the new
 * ElementaryMatrix.
 * @param elem1 - The ElementaryMatrix to be substracted by elem2
 * @param elem2 - The ElementaryMatrix to be substracted from elem1
 * {@code
 * ElementaryMatrix m1("[[1,2][3,4]]");
 * ElementaryMatrix m2("[[5,6][7,8]]");
 *
 * ElementaryMatrix m3 = m1 - m2; // Doesn't modify m2 or m1
 * std::cout << m3.toString() << std::endl; // Prints [[-4,-4][-4,-4]]
 * }
 */
template <typename T>
ElementaryMatrix<T> operator-(const ElementaryMatrix<T>& first, const ElementaryMatrix<T>& second);

/**
 * Operator * for ElementaryMatrix. The same as ElementaryMatrix::operator*= but instead of
 * returning the reference to the caller this operator creates a new instance
 * of ElementaryMatrix and sets the product of two parameter objects into the new
 * ElementaryMatrix.
 * @param elem1 - The ElementaryMatrix to be multiplied by elem2
 * @param elem2 - The ElementaryMatrix to be multiplied by elem1
 * @return - A new ElementaryMatrix
 * {@code
 * ElementaryMatrix m1("[[1,2][3,4]]");
 * ElementaryMatrix m2("[[5,6][7,8]]");
 *
 * ElementaryMatrix m3 = m1 * m2; // Doesn't modify m2 or m1
 * // Prints [[19,22][43,50]]
 * std::cout << m3.toString() << std::endl;
 * }
 */
template <typename T>
ElementaryMatrix<T> operator*(const ElementaryMatrix<T>& first, const ElementaryMatrix<T>& second);


/**
 * Function to determine if the given string is valid square matrix.
 * @param matrix - The string to be validated.
 * @return - Boolean indicating if the given string is valid square matrix.
 * {@code
 * bool validConcreteSquareMatrix = isConcreteSquareMatrix("[[2,3][40,-2]]"); // true
 * bool notValidConcreteSquareMatrix = isConcreteSquareMatrix("[[3][900,32]]"); // false
 * }
 */
bool isConcreteSquareMatrix(const std::string&);

/**
 * Function to determine if the given string is valid square matrix.
 * @param matrix - The string to be validated.
 * @return - Boolean indicating if the given string is valid square matrix.
 * {@code
 * bool validSymbolicSquareMatrix = isSymbolicSquareMatrix("[[2,3][40,-2]]"); // true
 * bool notValidSymbolicSquareMatrix = isSymbolicSquareMatrix("[[3][900,32]]"); // false
 * }
 */
bool isSymbolicSquareMatrix(const std::string& matrix);

using ConcreteSquareMatrix = ElementaryMatrix<IntElement>;
using SymbolicSquareMatrix = ElementaryMatrix<Element>;

#endif /* elementarymatrix_hpp */
