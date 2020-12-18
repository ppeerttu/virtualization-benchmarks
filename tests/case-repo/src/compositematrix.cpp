/**
 * @file compositematrix.cpp
 * A file implementing the class CompositeSquareMatrix.
 * @date 22.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see CompositeSquareMatrix
 */
#include "compositematrix.hpp"
#include <memory>
#include <sstream>

CompositeSquareMatrix::CompositeSquareMatrix(
  const SquareMatrix& sq1,
  const SquareMatrix& sq2,
  CSMFunction oprtr,
  char opnd
): oprnd1(sq1.clone()), oprnd2(sq2.clone()), oprtor(oprtr), op_sym(opnd), n(sq1.getDim()) {
  if (sq1.getDim() != sq2.getDim()) throw std::invalid_argument("Received invalid square matrixes - dimensions do not match.");
}

CompositeSquareMatrix::CompositeSquareMatrix(const CompositeSquareMatrix& m):
  oprnd1(m.oprnd1->clone()),
  oprnd2(m.oprnd2->clone()),
  oprtor(m.oprtor),
  op_sym(m.op_sym),
  n(m.getDim()) {}

CompositeSquareMatrix::CompositeSquareMatrix(CompositeSquareMatrix&& m):
  oprnd1(std::move(m.oprnd1)),
  oprnd2(std::move(m.oprnd2)),
  oprtor(std::move(m.oprtor)),
  op_sym(std::move(m.op_sym)),
  n(std::move(m.n)) {}

CompositeSquareMatrix* CompositeSquareMatrix::clone() const {
  return new CompositeSquareMatrix(*this);
}

void CompositeSquareMatrix::print(std::ostream& os) const {
  os << '(' << oprnd1->toString() << op_sym << oprnd2->toString() << ')';
}

std::string CompositeSquareMatrix::toString() const {
  std::stringstream ss;
  ss << *this;
  return ss.str();
}

ConcreteSquareMatrix CompositeSquareMatrix::evaluate(const Valuation& v) const {
  return oprtor(oprnd1->evaluate(v), oprnd2->evaluate(v));
}

CompositeSquareMatrix& CompositeSquareMatrix::operator=(const CompositeSquareMatrix& m) {
  if (this == &m) return *this;
  oprnd1 = uni_sq(m.oprnd1->clone());
  oprnd2 = uni_sq(m.oprnd2->clone());
  oprtor = m.oprtor;
  op_sym = m.op_sym;
  n = m.n;
  return *this;
}

CompositeSquareMatrix& CompositeSquareMatrix::operator=(CompositeSquareMatrix&& m) {
  oprnd1 = std::move(m.oprnd1);
  oprnd2 = std::move(m.oprnd2);
  oprtor = m.oprtor;
  op_sym = m.op_sym;
  n = m.n;
  return *this;
}

int CompositeSquareMatrix::getDim() const {
  return n;
}

CompositeSquareMatrix::~CompositeSquareMatrix() {}
