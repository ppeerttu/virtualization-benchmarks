/**
 * @file elementarymatrix.cpp
 * A file implementing the interface ElementaryMatrix.
 * @date 29.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see ElementaryMatrix
 */

#include "elementarymatrix.hpp"
#include <numeric>
#include <algorithm>
#include <cctype>
#include <sstream>


template<>
ElementaryMatrix<IntElement>::ElementaryMatrix(const std::string& s) {
  if (!isConcreteSquareMatrix(s)) throw std::invalid_argument("Received invalid square matrix - dimensions do not match.");
  int number = 0;
  std::string cp = s;
  std::stringstream ss;
  std::string tmp;
  ss << s;
  std::vector<uni_int_elem> v = {};
  for (std::string::iterator it = cp.begin(); it != cp.end(); ++it) {
    if (*it == '[' || *it == ']' || *it == ',') {
      if (tmp.length() == 0) continue;
      std::stringstream(tmp) >> number;
      uni_int_elem i(new IntElement(number));
      v.push_back(std::move(i));
      tmp = "";
      if (*it == ']') {
        elements.push_back(std::move(v));
        v.clear();
      }
    } else {
      tmp += *it;
    }
  }
  n = elements.size();
}

template<>
ElementaryMatrix<Element>::ElementaryMatrix(const std::string& s) {
  if (!isSymbolicSquareMatrix(s)) throw std::invalid_argument("Received invalid square matrix - dimensions do not match.");
  int number = 0;
  std::string cp = s;
  std::stringstream ss;
  std::string tmp;
  ss << s;
  std::vector<uni_elem> v = {};
  for (std::string::iterator it = cp.begin(); it != cp.end(); ++it) {
    if (*it == '[' || *it == ']' || *it == ',') {
      if (tmp.length() == 0) continue;
      if (std::stringstream(tmp) >> number) {
        uni_elem i(static_cast<IntElement*>(new IntElement(number)));
        v.push_back(std::move(i));
        tmp = "";
        if (*it == ']') {
          elements.push_back(std::move(v));
          v.clear();
        }
      } else {
        uni_elem i(static_cast<VariableElement*>(new VariableElement(tmp.at(0))));
        v.push_back(std::move(i));
        tmp = "";
        if (*it == ']') {
          elements.push_back(std::move(v));
          v.clear();
        }
      }

    } else {
      tmp += *it;
    }
  }
  n = elements.size();
}

template<>
ElementaryMatrix<IntElement>::ElementaryMatrix(const ElementaryMatrix<IntElement>& m){
  for (auto& ve : m.elements) {
    std::vector<uni_int_elem> newRow = {};
    for (auto& el : ve) {
      newRow.push_back(uni_int_elem(el->clone()));
    }
    elements.push_back(std::move(newRow));
  }
  n = m.n;
}

template<>
ElementaryMatrix<Element>::ElementaryMatrix(const ElementaryMatrix<Element>& m){
  for (auto& ve : m.elements) {
    std::vector<uni_elem> newRow = {};
    for (auto& el : ve) {
      newRow.push_back(uni_elem(el->clone()));
    }
    elements.push_back(std::move(newRow));
  }
  n = m.n;
}

template<>
ElementaryMatrix<IntElement> ElementaryMatrix<IntElement>::evaluate(const Valuation& val) const {
  return ElementaryMatrix<IntElement> (*this);
}

template<>
ElementaryMatrix<IntElement> ElementaryMatrix<Element>::evaluate(const Valuation& v) const {
  std::string str = "[";
  for (auto& ve : elements) {
    str += "[";
    for (auto& e : ve) {
      str += std::to_string(e->evaluate(v));
      str += ",";
    }
    if (str.length() > 0) str.pop_back();
    str += "]";
  }
  str += "]";
  return ElementaryMatrix<IntElement>(str);
}

template<>
ElementaryMatrix<Element>& ElementaryMatrix<Element>::operator=(const ElementaryMatrix<Element>& m) {
  if (this == &m) return *this;

  elements.clear();
  for (auto& ve : m.elements) {
    std::vector<uni_elem> newRow = {};
    for (auto& el : ve) {
      newRow.push_back(uni_elem(el->clone()));
    }
    elements.push_back(std::move(newRow));
  }
  n = m.n;
  return *this;
}

template<>
ElementaryMatrix<IntElement>& ElementaryMatrix<IntElement>::operator=(const ElementaryMatrix<IntElement>& m) {
  if (this == &m) return *this;

  elements.clear();
  for (auto& ve : m.elements) {
    std::vector<uni_int_elem> newRow = {};
    for (auto& el : ve) {
      newRow.push_back(uni_int_elem(el->clone()));
    }
    elements.push_back(std::move(newRow));
  }
  n = m.n;
  return *this;
}

template <>
ElementaryMatrix<IntElement>& ElementaryMatrix<IntElement>::operator=(ElementaryMatrix<IntElement>&& m) {
  elements = std::move(m.elements);
  n = m.n;
  m.n = 0;
  m.elements.clear();
  return *this;
}

template <>
ElementaryMatrix<Element>& ElementaryMatrix<Element>::operator=(ElementaryMatrix<Element>&& m) {
  elements = std::move(m.elements);
  n = m.n;
  m.n = 0;
  m.elements.clear();
  return *this;
}

template<>
ElementaryMatrix<IntElement> ElementaryMatrix<IntElement>::transpose() const {
  ElementaryMatrix<IntElement> newSqMat;
  std::vector<uni_int_elem> row = {};
  for (int i = 0; i < elements.size(); i++) {
    for (auto& ve : elements) {
      row.push_back(uni_int_elem(ve[i]->clone()));
    }
    newSqMat.elements.push_back(std::move(row));
    row.clear();
  }
  return newSqMat;
}

template<>
ElementaryMatrix<Element> ElementaryMatrix<Element>::transpose() const {
  ElementaryMatrix<Element> newSqMat;
  std::vector<uni_elem> row = {};
  for (int i = 0; i < elements.size(); i++) {
    for (auto& ve : elements) {
      row.push_back(uni_elem(ve[i]->clone()));
    }
    newSqMat.elements.push_back(std::move(row));
    row.clear();
  }
  return newSqMat;
}

template<>
ElementaryMatrix<IntElement>& ElementaryMatrix<IntElement>::operator+=(const ElementaryMatrix<IntElement>& m) {
  if (n != m.n) throw std::out_of_range("Dimensions do not match with given ConcreteSquareMatrix objects!");
  for (int i = 0; i < elements.size(); i++) {
    for (int j = 0; j < elements[i].size(); j++) {
      *elements[i][j] += *m.elements[i][j];
    }
  }
  return *this;
}

template<>
ElementaryMatrix<IntElement>& ElementaryMatrix<IntElement>::operator-=(const ElementaryMatrix<IntElement>& m) {
  if (n != m.n) throw std::out_of_range("Dimensions do not match with given ConcreteSquareMatrix objects!");
  for (int i = 0; i < elements.size(); i++) {
    for (int j = 0; j < elements[i].size(); j++) {
      *elements[i][j] -= *m.elements[i][j];
    }
  }
  return *this;
}

template<>
ElementaryMatrix<IntElement>& ElementaryMatrix<IntElement>::operator*=(const ElementaryMatrix<IntElement>& m) {
  if (n != m.n) throw std::out_of_range("Dimensions do not match with given ConcreteSquareMatrix objects!");

  ElementaryMatrix<IntElement> trans = m.transpose();
  std::vector<std::vector<uni_int_elem>> newElements = {};
  for (auto& vi : elements) {
    std::vector<uni_int_elem> newRow = {};
    for (auto& vi2 : trans.elements) {
      std::vector<uni_int_elem> temp = {};
      temp.resize(vi.size());
      std::transform(vi.begin(), vi.end(), vi2.begin(),
        temp.begin(), [](uni_int_elem& el1, uni_int_elem& el2) {
          uni_int_elem ptr(new IntElement(el1->getVal() * el2->getVal()));
          return ptr;
        });
      auto elem = std::accumulate(temp.begin(), temp.end(),
        uni_int_elem(new IntElement(0)),
        [](uni_int_elem& el1, uni_int_elem& el2) {
          uni_int_elem ptr(new IntElement(el1->getVal() + el2->getVal()));
          return ptr;
        });
      newRow.push_back(std::move(elem));
    }
    newElements.push_back(std::move(newRow));
  }
  elements = std::move(newElements);
  return *this;
}

template<>
ElementaryMatrix<IntElement> operator+(const ElementaryMatrix<IntElement>& first, const ElementaryMatrix<IntElement>& second) {
  ElementaryMatrix<IntElement> sqMat(first);
  sqMat += second;
  return sqMat;
}

template<>
ElementaryMatrix<IntElement> operator-(const ElementaryMatrix<IntElement>& first, const ElementaryMatrix<IntElement>& second) {
  ElementaryMatrix<IntElement> sqMat(first);
  sqMat -= second;
  return sqMat;
}

template<>
ElementaryMatrix<IntElement> operator*(const ElementaryMatrix<IntElement>& first, const ElementaryMatrix<IntElement>& second) {
  ElementaryMatrix<IntElement> sqMat(first);
  sqMat *= second;
  return sqMat;
}


/**
 * Validate given string to be valid row for matrix.
 * @param row - The row of matrix to be validated (should be i.e "32,40,7")
 * @param count - The number of columns in the matrix
 * @return - Boolean indicating whether given row is valid for matrix
 */
bool validateSymbolicRow(std::string row, int count) {
	if (row.length() > 0 && (row.at(0) == ',' || row.at(row.length() - 1) == ',')) return false;
	std::stringstream ss;
	ss << row;
	int commas = 0;
	std::string temp;
	int number;
	while (std::getline(ss, temp, ',')) {
		if (std::stringstream(temp) >> number) {
			commas++;
		} else if (std::stringstream(temp).str().size() == 1) {
			if (isalpha(std::stringstream(temp).str().at(0))) {
        commas++;
        continue;
      }
      return false;
		} else {
      return false;
    }
	}
	return commas == count;
}

/**
 * Function to determine if the given string is valid square matrix.
 * @param matrix - The string to be validated.
 * @return - Boolean indicating if the given string is valid square matrix.
 * {@code
 * bool validSymbolicSquareMatrix = isSymbolicSquareMatrix("[[2,3][40,-2]]"); // true
 * bool notValidSymbolicSquareMatrix = isSymbolicSquareMatrix("[[3][900,32]]"); // false
 * }
 */
bool isSymbolicSquareMatrix(const std::string& matrix) {
	std::string tmp = matrix;
	bool lastIsOpen = false;
	bool firstOpenUsed = false;
	bool lastCloseUsed = false;
	char lastChar;
	std::string::const_iterator iterator;
	int i = 0;
	int arrIndex = 0;
	int arr[matrix.length()];
	int dimension = 0;
	for (iterator = matrix.begin(); iterator != matrix.end(); ++iterator) {
		if (lastCloseUsed) return false;
		if (i == 0) {
			if (*iterator != '[') return false;
			firstOpenUsed = true;
		} else if (*iterator == '[') {
			if (!lastIsOpen && firstOpenUsed) {
				lastIsOpen = true;
				dimension++;
				arr[arrIndex] = i;
				arrIndex++;
			}
			else return false;
		} else if (*iterator == ']') {
			if (lastIsOpen) {
				lastIsOpen = false;
				arr[arrIndex] = i;
				arrIndex++;
			} else {
				if (lastChar != ']') return false;
				lastCloseUsed = true;
			}
		}
		lastChar = *iterator;
		i++;
	}
	if (!lastCloseUsed) return false;

	bool result = true;
	for (int j = 1; j < arrIndex; j += 2) {
		if ((j - 2) > 0 && (arr[j - 1] - arr[j - 2]) != 1) return false;
		std::string subStr = tmp.substr(arr[j - 1] + 1, arr[j] - arr[j - 1] - 1);
		result = result && validateSymbolicRow(subStr, dimension);
	}
	return result;
}

/**
 * Validate given string to be valid row for matrix.
 * @param row - The row of matrix to be validated (should be i.e "32,40,7")
 * @param count - The number of columns in the matrix
 * @return - Boolean indicating whether given row is valid for matrix
 */
bool validateConcreteRow(std::string row, int count) {
	if (row.length() > 0 && (row.at(0) == ',' || row.at(row.length() - 1) == ',')) return false;
	std::stringstream ss;
	ss << row;
	int commas = 0;
	std::string temp;
	int number;
	while (std::getline(ss, temp, ',')) {
		if (std::stringstream(temp) >> number) {
			commas++;
		} else {
			return false;
		}
	}
	return commas == count;
}

/**
 * Function to determine if the given string is valid square matrix.
 * @param matrix - The string to be validated.
 * @return - Boolean indicating if the given string is valid square matrix.
 * {@code
 * bool validElementaryMatrix<IntElement> = isConcreteSquareMatrix("[[2,3][40,-2]]"); // true
 * bool notValidElementaryMatrix<IntElement> = isConcreteSquareMatrix("[[3][900,32]]"); // false
 * }
 */
bool isConcreteSquareMatrix(const std::string& matrix) {
	std::string tmp = matrix;
	bool lastIsOpen = false;
	bool firstOpenUsed = false;
	bool lastCloseUsed = false;
	char lastChar;
	std::string::const_iterator iterator;
	int i = 0;
	int arrIndex = 0;
	int arr[matrix.length()];
	int dimension = 0;
	for (iterator = matrix.begin(); iterator != matrix.end(); ++iterator) {
		if (lastCloseUsed) return false;
		if (i == 0) {
			if (*iterator != '[') return false;
			firstOpenUsed = true;
		} else if (*iterator == '[') {
			if (!lastIsOpen && firstOpenUsed) {
				lastIsOpen = true;
				dimension++;
				arr[arrIndex] = i;
				arrIndex++;
			}
			else return false;
		} else if (*iterator == ']') {
			if (lastIsOpen) {
				lastIsOpen = false;
				arr[arrIndex] = i;
				arrIndex++;
			} else {
				if (lastChar != ']') return false;
				lastCloseUsed = true;
			}
		}
		lastChar = *iterator;
		i++;
	}
	if (!lastCloseUsed) return false;

	bool result = true;
	for (int j = 1; j < arrIndex; j += 2) {
		if ((j - 2) > 0 && (arr[j - 1] - arr[j - 2]) != 1) return false;
		std::string subStr = tmp.substr(arr[j - 1] + 1, arr[j] - arr[j - 1] - 1);
		result = result && validateConcreteRow(subStr, dimension);
	}
	return result;
}
