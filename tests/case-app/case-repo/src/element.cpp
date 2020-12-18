/**
 * @file element.cpp
 * A file implementing the interface Element and classes
 * IntElement and VariableElement.
 * @date 22.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see Element
 */
#include "element.hpp"


template<>
std::string TElement<int>::toString() const {
  return std::to_string(val);
}

template<>
std::string TElement<char>::toString() const {
  std::string s;
  s.push_back(val);
  return s;
}


template<>
int TElement<int>::evaluate(const Valuation& v) const {
  return val;
}

template<>
int TElement<char>::evaluate(const Valuation& v) const {
  return v.at(val);
}

template<>
TElement<int>& TElement<int>::operator-=(const TElement<int>& elem) {
  val -= elem.val;
  return *this;
}

template<>
TElement<int>& TElement<int>::operator+=(const TElement<int>& elem) {
  val += elem.val;
  return *this;
}

template<>
TElement<int>& TElement<int>::operator*=(const TElement<int>& elem) {
  val *= elem.val;
  return *this;
}

template<>
TElement<int> operator*(const TElement<int>& elem1, const TElement<int>& elem2) {
  TElement<int> e(elem1.getVal() * elem2.getVal());
  return e;
}

template<>
TElement<int> operator+(const TElement<int>& elem1, const TElement<int>& elem2) {
  TElement<int> e(elem1.getVal() + elem2.getVal());
  return e;
}

template<>
TElement<int> operator-(const TElement<int>& elem1, const TElement<int>& elem2) {
  TElement<int> e(elem1.getVal() - elem2.getVal());
  return e;
}
