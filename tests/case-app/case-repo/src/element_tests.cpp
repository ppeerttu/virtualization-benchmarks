/**
 * @file element_tests.cpp
 * Unit tests for interface Element by using Catch2
 * @date 22.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see Element
 * @see VariableElement
 * @see IntElement
 */

#include "element.hpp"
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <sstream>
#include <memory>

/* ===========================================================
    IntElement
  =========================================================== */

/**
 * Testing IntElement constructor to not throw an error
 */
TEST_CASE( "IntElement.constructor" ) {

  SECTION( "Should not throw an error" ) {

    REQUIRE_NOTHROW([&]() {
      IntElement elem;
    }());

    REQUIRE_NOTHROW([&]() {
      int testVal = 1;
      IntElement elem(testVal);
    }());
  }

}

/**
 * Testing IntElement member function getVal
 */
TEST_CASE( "IntElement.getVal" ) {

  SECTION( "Should return a copy of property val" ) {
    int value = 4;
    IntElement elem(value);
    CHECK(elem.getVal() == value);
    value = 5;
    CHECK(elem.getVal() != value);
  }
}

/**
 * Testing IntElement member function setVal
 */
TEST_CASE( "IntElement.setVal" ) {

  SECTION( "Should set the property val" ) {
    int value = 3;
    IntElement elem(value);
    elem.setVal(4);
    CHECK( elem.getVal() != value );
    CHECK( elem.getVal() == 4 );
  }
}

/**
 * Testing IntElement member function clone
 */
TEST_CASE( "IntElement.clone" ) {
  SECTION( "Should clone the element" ) {
    int value = 3;
    IntElement elem(value);
    std::unique_ptr<IntElement> ptr(elem.clone());
    CHECK( ptr->getVal() == value );
    ptr->setVal(7);
    CHECK( ptr->getVal() != value );
    CHECK( ptr->getVal() != elem.getVal() );
  }
}

/**
 * Testing IntElement member function overloaded operator +=
 */
TEST_CASE( "IntElement.+= -operator" ) {
  SECTION( "Should add val properties" ) {
    int val1 = 3;
    int val2 = 9;
    int sum = val1 + val2;

    IntElement elem1(val1);
    IntElement elem2(val2);
    elem1 += elem2;

    CHECK(elem1.getVal() == sum);
  }
}

/**
 * Testing IntElement member function overloaded operator -=
 */
TEST_CASE( "IntElement.-= -operator" ) {
  SECTION( "Should subtract val properties" ) {
    int val1 = 5;
    int val2 = 9;
    int subtraction = val1 - val2;

    IntElement elem1(val1);
    IntElement elem2(val2);
    elem1 -= elem2;

    CHECK(elem1.getVal() == subtraction);
  }
}

/**
 * Testing IntElement member function overloaded operator *=
 */
TEST_CASE( "IntElement.*= -operator" ) {
  SECTION( "Should multiply val properties" ) {
    int val1 = 5;
    int val2 = 9;
    int product = val1 * val2;

    IntElement elem1(val1);
    IntElement elem2(val2);
    elem1 *= elem2;

    CHECK(elem1.getVal() == product);
  }
}

/**
 * Testing overloaded operator << for IntElement
 */
TEST_CASE( "<< -operator for IntElement" ) {
  SECTION( "Should insert property val into stream" ) {
    int value = 41;
    std::stringstream ss;

    ss << value;
    IntElement elem(value);
    std::stringstream checkSs;
    checkSs << elem;

    CHECK(checkSs.str() == ss.str());
  }
}

/**
 * Testing overloaded operator << for pointer of IntElement
 */
TEST_CASE( "<< -operator for *IntElement" ) {
  SECTION( "Should insert property val into stream" ) {
    int value = 41;
    std::stringstream ss;

    ss << value;
    IntElement* elem = new IntElement(value);
    std::stringstream checkSs;
    checkSs << elem;

    CHECK(checkSs.str() == ss.str());
    delete elem;

  }
}

/**
 * Testing overloaded operator << for unique pointer of IntElement
 */
TEST_CASE( "<< -operator for unique_ptr<IntElement>" ) {
  SECTION( "Should insert property val into stream" ) {
    int value = 41;
    std::stringstream ss;

    ss << value;
    uni_int_elem elem(new IntElement(value));
    std::stringstream checkSs;
    checkSs << elem;

    CHECK(checkSs.str() == ss.str());

  }
}


/**
 * Testing overloaded operator * for IntElement
 */
TEST_CASE( "* -operator for IntElement" ) {
  SECTION( "Should return multiply of callers and parameters properties val as integer" ) {
    IntElement elem1(4);
    IntElement elem2(5);

    IntElement product = elem1 * elem2;
    CHECK(product.getVal() == 20);
  }
}

/**
 * Testing overloaded operator - for IntElement
 */
TEST_CASE( "- -operator for IntElement" ) {
  SECTION( "Should return multiply of callers and parameters properties val as integer" ) {
    IntElement elem1(4);
    IntElement elem2(5);

    IntElement result = elem1 - elem2;
    CHECK(result.getVal() == -1);
  }
}

/**
 * Testing overloaded operator + for IntElement
 */
TEST_CASE( "+ -operator for IntElement" ) {
  SECTION( "Should return multiply of callers and parameters properties val as integer" ) {
    IntElement elem1(4);
    IntElement elem2(5);

    IntElement result = elem1 + elem2;
    CHECK(result.getVal() == 9);
  }
}

/**
 * Testing the overloaded operator == for IntElement.
 */
TEST_CASE( "== -operator for IntElement" ) {
  SECTION( "Should return true if values equal" ) {
    IntElement e1(4);
    IntElement e2(4);
    IntElement e3(16);
    CHECK( e1 == e2 );
    e1 *= e2;
    CHECK( e1 == e3);
  }

  SECTION( "Should return false if values do not equal" ) {
    IntElement e1(4);
    IntElement e2(4);
    IntElement e3(18);
    CHECK( e1 == e2 );
    e1 *= e2;
    CHECK( !(e1 == e3) );
  }
}

TEST_CASE( "IntElement.toString" ) {
  SECTION( "Should return the value as string" ) {
    IntElement e(4);
    CHECK( e.toString() == "4" );
  }
}

TEST_CASE( "IntElement.evaluate" ) {
  SECTION( "Should return the value of the Element" ) {
    IntElement e(4);
    Valuation v;
    CHECK( e.evaluate(v) == 4 );
  }
}

/* ===========================================================
    VariableElement
  =========================================================== */

/**
 * Testing VariableElement constructor to not throw an error
 */
TEST_CASE( "VariableElement.constructor" ) {

  SECTION( "Should not throw an error" ) {

    REQUIRE_NOTHROW([&]() {
      VariableElement elem;
    }());

    REQUIRE_NOTHROW([&]() {
      char testVal = 'x';
      VariableElement elem(testVal);
    }());
  }

}

/**
 * Testing VariableElement member function getVal
 */
TEST_CASE( "VariableElement.getVal" ) {

  SECTION( "Should return a copy of property val" ) {
    char value = 'x';
    VariableElement elem(value);
    CHECK(elem.getVal() == value);
    value = '3';
    CHECK(elem.getVal() != value);
  }
}

/**
 * Testing VariableElement member function setVal
 */
TEST_CASE( "VariableElement.setVal" ) {

  SECTION( "Should set the property val" ) {
    char value = '2';
    VariableElement elem(value);
    elem.setVal('4');
    CHECK( elem.getVal() != value );
  }
}

/**
 * Testing VariableElement member function clone
 */
TEST_CASE( "VariableElement.clone" ) {
  SECTION( "Should clone the element" ) {
    char value = '3';
    VariableElement elem(value);
    std::unique_ptr<VariableElement> ptr(elem.clone());
    CHECK( ptr->getVal() == value );
    ptr->setVal('7');
    CHECK( ptr->getVal() != value );
    CHECK( ptr->getVal() != elem.getVal() );
  }
}

/**
 * Testing overloaded operator << for VariableElement
 */
TEST_CASE( "<< -operator for VariableElement" ) {
  SECTION( "Should insert property val into stream" ) {
    char value = '4';
    std::stringstream ss;

    ss << value;
    VariableElement elem(value);
    std::stringstream checkSs;
    checkSs << elem;

    CHECK(checkSs.str() == ss.str());
  }
}



/**
 * Testing the overloaded operator == for VariableElement.
 */
TEST_CASE( "== -operator for VariableElement" ) {
  SECTION( "Should return true if values equal" ) {
    VariableElement e1('4');
    VariableElement e2('4');
    VariableElement e3(16);
    CHECK( e1 == e2 );
  }

  SECTION( "Should return false if values do not equal" ) {
    VariableElement e1('4');
    VariableElement e3('2');
    CHECK( !(e1 == e3) );
  }
}

TEST_CASE( "VariableElement.toString" ) {
  SECTION( "Should return the value as string" ) {
    VariableElement e('4');
    CHECK( e.toString() == "4" );
  }
}

TEST_CASE( "VariableElement.evaluate" ) {
  SECTION( "Should return the value of the Element if found" ) {
    VariableElement e('x');
    Valuation v {
      { 'x', 6 },
      { 'a', 7 }
    };
    CHECK( e.evaluate(v) == 6 );
    e.setVal('a');
    CHECK( e.evaluate(v) == 7 );
  }

  SECTION( "Should throw an exception if mappings not found" ) {
    VariableElement e('y');
    Valuation v {{ 'x', 6 },{ 'a', 7 }};
    CHECK_THROWS_AS([&]() {
      e.evaluate(v);
    }(), std::out_of_range);
  }
}
