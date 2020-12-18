/**
 * @file element.hpp
 * A file describing the API for interface Element.
 * @date 29.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see Element
 */

#ifndef element_hpp
#define element_hpp

#include <string>
#include <iostream>
#include <map>
#include <memory>

/**
 * Valuation mapper for VariableElement. Each key represents
 * variable and has an integer value that. Used to convert
 * VariableElements to IntElements, thus transforming
 * SymbolicSquareMatrix into ConcreteSquareMatrix.
 */
using Valuation = std::map<char, int>;

class Element {
  public:
    virtual ~Element() {};
    virtual std::string toString() const = 0;
    virtual Element* clone() const = 0;
    virtual int evaluate(const Valuation& val) const = 0;

    /**
     * Operator << for Element. This operator takes a stream as first argument
     * and inserts the property val of second argument into the stream.
     * @param os - The stream where the property val should be inserted to
     * @param elem - The TElement to be set into the stream
     * @return Reference to the stream given as parameter
     * {@code
     * TElement<int> elem(73);
     * std::cout << elem; // prints '73'
     * }
     */
    friend std::ostream& operator<<(std::ostream& os, const Element& elem) {
      os << elem.toString();
      return os;
    };

    /**
     * Operator << for *Element, see operator << for Element.
     * @param os - The stream where the property val should be inserted to
     * @param elem - The pointer of Element to be set into the stream
     * @return Reference to the stream given as parameter
     */
    friend std::ostream& operator<<(std::ostream& os, const Element *ptr) {
      os << ptr->toString();
      return os;
    };

};

typedef std::unique_ptr<Element> uni_elem;

template <typename T>
class TElement : public Element {

  T val;

  public:

    /**
     * An empty TElement constructor (default constructor).
     */
    TElement() {};

    /**
     * TElement constructor with setting the parameter as property var.
     * @param v An integer to be set for this TElement as a property var
     */
    TElement(T v): val(v) {};

    /**
     * Destructor for TElement.
     */
    ~TElement() {};

    /**
     * Get a copy of the var property of this.
     * @return Property var of this TElement
     */
    T getVal() const {
      return val;
    };

    /**
     * Set the property var of TElement.
     * @param v - The value to be set for this TElement as property var
     */
    void setVal(T v) {
      val = v;
    };

    /**
     * Return a string representation of the value of the Element.
     * @return - A string containing the value of the Element.
     */
    std::string toString() const;

    /**
     * Return the mapped value of param v with key var if found.
     * @reutrn - The mapped value of the param v if found.
     * @throws std::out_of_range if the Valuation does not contain
     * mappings for property var of the Element.
     */
    int evaluate(const Valuation& val) const;

    /**
     * Create a clone of the TElement. That is a totally new
     * copy of the caller of this function.
     * @return - A pointer referring to the new element.
     */
    TElement* clone() const {
      return new TElement(val);
    };

    /**
     * Operator -= for TElement. Substract the property val of parameter TElement
     * from the property val of caller.
     * @param elem - The TElement to be substracted from the caller TElement
     * @return A reference to the caller
     * {@code
     * TElement<int> elem(3);  // elem.val = 3
     * TElement<int>  elem2(4); // elem2.val = 4
     * elem -= elem2;       // elem2.val = 4, elem.val = -1
     * }
     */
    TElement& operator-=(const TElement&);

    /**
     * Operator += for TElement. Add the property val of parameter TElement
     * into the property val of caller.
     * @param elem - The TElement to be added to the caller TElement
     * @return A reference to the caller
     * {@code
     * TElement<int>  elem(3);  // elem.val = 3
     * TElement<int>  elem2(4); // elem2.val = 4
     * elem += elem2;       // elem2.val = 4, elem.val = 7
     * }
     */
    TElement& operator+=(const TElement&);

    /**
     * Operator *= for TElement. Set the product of property val of parameter
     * TElement and the property val of the caller.
     * @param elem - The TElement to be multiplied with the caller TElement
     * @return A reference to the caller
     * {@code
     * TElement<int> elem(3);  // elem.val = 3
     * TElement<int>  elem2(4); // elem2.val = 4
     * elem *= elem2;       // elem2.val = 4, elem.val = 12
     * }
     */
    TElement& operator*=(const TElement&);

    /**
     * Equality operator for TElement. Compare given element's values
     * and return the result.
     * @param elem - The TElement to be compared to.
     * @return - The value of comparison.
     */
    bool operator==(const TElement& elem) const {
      return val == elem.val;
    };

};

/**
 * Operator << for unique_ptr<TElement>, see operator << for TElement.
 * @param os - The stream where the property val should be inserted to
 * @param elem - The pointer of TElement to be set into the stream
 * @return Reference to the stream given as parameter
 * {@code
 * TElement<int>* = new TElement<int>(73);
 * std::cout << elem; // prints '73'
 * }
 */
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::unique_ptr<TElement<T>>& ptr) {
  os << ptr->toString();
  return os;
};


/**
 * Operator * for TElement. The same as TElement::operator*= but instead of
 * returning the reference to the caller this operator creates a new instance
 * of TElement and sets the product of two parameter objects into the new
 * TElement.
 * @param elem1 - The TElement to be multiplied by elem2
 * @param elem2 - The TElement to be multiplied by elem1
 * @return A new TElement
 * {@code
 * TElement elem(3);                      // elem.val = 3
 * TElement elem2(4);                     // elem2.val = 4
 * TElement newElem = elem * elem2;       // elem2.val = 4, elem.val = 3, newElem.val = 12
 * }
 */
template <typename T>
TElement<T> operator*(const TElement<T>& elem1, const TElement<T>& elem2);

/**
 * Operator + for TElement. The same as TElement::operator+= but instead of
 * returning the reference to the caller this operator creates a new instance
 * of TElement and sets the sum of two parameter objects into the new
 * TElement.
 * @param elem1 - The TElement to be summed up with elem2
 * @param elem2 - The TElement to be summed up elem1
 * @return  A new TElement
 * {@code
 * TElement elem(3);                      // elem.val = 3
 * TElement elem2(4);                     // elem2.val = 4
 * TElement newElem = elem + elem2;       // elem2.val = 4, elem.val = 3, newElem.val = 7
 * }
 */
template <typename T>
TElement<T> operator+(const TElement<T>& elem1, const TElement<T>& elem2);

/**
 * Operator - for TElement. The same as TElement::operator-= but instead of
 * returning the reference to the caller this operator creates a new instance
 * of TElement and sets the substraction of two parameter objects into the new
 * TElement.
 * @param elem1 - The TElement to be substracted by elem2
 * @param elem2 - The TElement to be substracted from elem1
 * @return  A new TElement
 * {@code
 * TElement elem(3);                      // elem.val = 3
 * TElement elem2(4);                     // elem2.val = 4
 * TElement newElem = elem - elem2;       // elem2.val = 4, elem.val = 3, newElem.val = -1
 * }
 */
template <typename T>
TElement<T> operator-(const TElement<T>& elem1, const TElement<T>& elem2);

using IntElement = TElement<int>;
using VariableElement = TElement<char>;


/**
 * Typedefinition for unique_ptr that has a reference to IntElement.
 */
typedef std::unique_ptr<IntElement> uni_int_elem;

/**
 * Typedefinition for unique_ptr that has a reference to VariableElement.
 */
typedef std::unique_ptr<VariableElement> uni_var_elem;


#endif /* element_hpp */
