/**
 * @file compositematrix.hpp
 * A file describing the API for class CompositeSquareMatrix.
 * @date 19.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see CompositeSquareMatrix
 */

#ifndef compositematrix_hpp
#define compositematrix_hpp

#include "element.hpp"
#include "squarematrix.hpp"
#include "elementarymatrix.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <functional>

using CSMFunction =
std::function<ConcreteSquareMatrix(
  const ConcreteSquareMatrix&,
  const ConcreteSquareMatrix&
)>;

class CompositeSquareMatrix : public SquareMatrix {

  uni_sq oprnd1;
  uni_sq oprnd2;

  CSMFunction oprtor;

  char op_sym;

  int n;

  public:

    /**
     * A constructor for CompositeSquareMatrix.
     * @param op1 - A SquareMatrix object.
     * @param op2 - A SquareMatrix object.
     * @param opr - A function for processing the SquareMatrixes.
     * @param ops - The symbol of operator at opr.
     * @throws std::invalid_argument - Dimensions do not match.
     */
     CompositeSquareMatrix(
      const SquareMatrix& op1,
      const SquareMatrix& op2,
      CSMFunction opr,
      char ops
     );

    /**
     * A constructor for CompositeSquareMatrix that is formed from another instance of
     * CompositeSquareMatrix.
     * @param m - An instance of CompositeSquareMatrix to be copied.
     */
    CompositeSquareMatrix(const CompositeSquareMatrix& m);

    /**
     * Move constructor for CompositeSquareMatrix.
     * @param m - The CompositeSquareMatrix to be moved from.
     */
    CompositeSquareMatrix(CompositeSquareMatrix&& m);

    /**
     * Destructor for CompositeSquareMatrix.
     */
    virtual ~CompositeSquareMatrix();

    /**
     * Copy assignment operator for CompositeSquareMatrix.
     * @param m - The object to be copied.
     * @return - A reference to the caller.
     */
    CompositeSquareMatrix& operator=(const CompositeSquareMatrix& m);

    /**
     * Move assignment operator for CompositeSquareMatrix. This operator
     * moves the properties of parameter to the caller, leaving
     * the parameter 'empty'.
     * @param m - The object to be moved.
     * @return - A reference to the caller.
     */
    CompositeSquareMatrix& operator=(CompositeSquareMatrix&& m);

    /**
     * Return a pointer to a new ConcreteSquareMatrix
     * which is a copy of this.
     * @return - A reference to a new copy of this.
     */
    CompositeSquareMatrix* clone() const override;


    /**
     * A function that prints the CompositeSquareMatrix into the given stream.
     * @param os - The stream in which the print is inserted to.
     * {@code
     * ConcreteSquareMatrix csq1("[[1]]");
     * ConcreteSquareMatrix csq2("[[2]]");
     * auto func = [&](const ConcreteSquareMatrix& s1, const ConcreteSquareMatrix s2) {
     *   return s1 + s2;
     * };
     * CompositeSquareMatrix matrix(csq2, csq1, func, '+');
     * std::stringstream ss;
     * matrix.print(ss);
     * std::cout << ss; // Prints '([[1]]+[[2]])'
     * }
     */
    void print(std::ostream& os) const override;

    /**
     * A function that returns a string which includes a same expression form
     * of CompositeSquareMatrix as described in CompositeSquareMatrix::print.
     * @return A new string containing the representation of this CompositeSquareMatrix.
     */
    std::string toString() const override;

    /**
     * Return an evaluated version of this CompositeSquareMatrix.
     * During the process the operands of this will be evaluated
     * and the results will be insetered into operator of this as
     * a parameter. The result of that will be returned.
     * @param val - Valuation for evaluation
     * @return - A new ConcreteSquareMatrix.
     */
    ConcreteSquareMatrix evaluate(const Valuation& val) const override;

    /**
     * Return the dimension of CompositeSquareMatrix.
     * @return - The dimension of the CompositeSquareMatrix.
     */
    int getDim() const override;

};

#endif /* compositematrix_hpp */
