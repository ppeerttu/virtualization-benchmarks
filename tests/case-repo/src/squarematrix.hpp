/**
 * @file squarematrix.hpp
 * A file describing the API for interface SquareMatrix.
 * @date 18.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see SquareMatrix
 */

#ifndef squarematrix_hpp
#define squarematrix_hpp

#include "element.hpp"
#include <string>
#include <iostream>
#include <memory>

// Defined at elementarymatrix.hpp
template <typename T>
class ElementaryMatrix;

class SquareMatrix {
  public:
    /**
     * Destructor for SquareMatrix.
     */
    virtual ~SquareMatrix() {};

    /**
     * A function that prints the SquareMatrix into the given stream.
     * The form of print is similar to the parameter for SquareMatrix
     * constructor SquareMatrix(const std::string& str).
     * @param os - The stream in which the print is inserted to.
     * {@code
     * SquareMatrix matrix("[[1,2][3,4]]");
     * std::stringstream ss;
     * matrix.print(ss);
     * std::cout << ss; // Prints '[[1,2][3,4]]'
     * }
     */
    virtual void print(std::ostream& os) const = 0;

    /**
     * A function that returns a string which includes a same expression form
     * of SquareMatrix as described in SquareMatrix::print.
     * @return A new string containing the representation of this SquareMatrix.
     */
    virtual std::string toString() const = 0;

    /**
     * Return a pointer to a new SquareMatrix
     * which is a copy of this.
     * @return - A reference to a new copy of this.
     */
    virtual SquareMatrix* clone() const = 0;

    /**
     * Returns a evaluated version of caller SquareMatrix.
     * @param val - Valuation for evaluation
     * @return - A new ConcreteSquareMatrix which is based on
     * caller SquareMatrix object and param val object.
     */
    virtual ElementaryMatrix<IntElement> evaluate(const Valuation& val) const = 0;

    /**
     * Return the dimension of SquareMatrix.
     * @return - The dimension of the SquareMatrix.
     */
    virtual int getDim() const = 0;

    /**
     * Operator overloading for ostream interface. Use the SquareMatrix::print
     * vitual member function to print the SquareMatrix object into the stream.
     * @param os - The stream object
     * @param sq - The SquareMatrix object
     * @return - The stream object with sq printed in it.
     */
    friend std::ostream& operator<<(std::ostream& os, const SquareMatrix& sq) {
      sq.print(os);
      return os;
    }
};

template<class T>
std::ostream& operator<<(std::ostream& os, const std::unique_ptr<T>& ptr) {
  os << *ptr;
  return os;
}

/**
 * Unique pointer type declaration for SquareMatrix
 */
typedef std::unique_ptr<SquareMatrix> uni_sq;

#endif /* squarematrix_hpp */
