/**
 * @file concretematrix_tests.cpp
 * This file contains unit tests for ConcreteSquareMatrix.
 * @date 22.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see ConcreteSquareMatrix
 * @see IntElement
 */

#include "elementarymatrix.hpp"
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <memory>

/**
 * Test the constructor to be functional.
 */
TEST_CASE( "ConcreteSquareMatrix.constructor", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should not throw an error with valid parameter" ) {
    CHECK_NOTHROW([&]() {
      ConcreteSquareMatrix sqMat;
    }());

    CHECK_NOTHROW([&]() {
      ConcreteSquareMatrix sqMat("[[1,2][3,4]]");
    }());

    CHECK_NOTHROW([&]() {
      ConcreteSquareMatrix sqMat1("[[1,2][3,4]]");
      ConcreteSquareMatrix sqMat2(sqMat1);
    }());

    CHECK_NOTHROW([&]() {
      ConcreteSquareMatrix sqMat1("[[1,2][3,4]]");
      ConcreteSquareMatrix sqMat2(std::move(sqMat1));
      CHECK( sqMat2.toString() == "[[1,2][3,4]]" );
    }());

  }

  SECTION( "Should throw an error with invalid parameter" ) {
    CHECK_THROWS_AS([&]() {
      ConcreteSquareMatrix sqMat("[ads]");
    }(), std::invalid_argument);
    CHECK_THROWS_AS([&]() {
      ConcreteSquareMatrix sqMat("[[392,-124,][2,1]]");
    }(), std::invalid_argument);
  }
}

/**
 * Testing transposing ConcreteSquareMatrix into a new ConcreteSquareMatrix
 */
TEST_CASE( "ConcreteSquareMatrix.transpose", "[ConcreteSquareMatrix]" ) {
  ConcreteSquareMatrix sq("[[1,2][3,4]]");
  ConcreteSquareMatrix sq2 = sq.transpose();
  CHECK( sq2.toString() == "[[1,3][2,4]]");
  ConcreteSquareMatrix sq3("[[1,2,3][4,5,6][7,8,9]]");
  ConcreteSquareMatrix sq4 = sq3.transpose();
  CHECK( sq4.toString() == "[[1,4,7][2,5,8][3,6,9]]");
}

/**
 * Testing the += -operator functionality.
 */
TEST_CASE( "ConcreteSquareMatrix.+= -operator", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should add all IntElements" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2][3,4]]");
    ConcreteSquareMatrix m3("[[1,-3][1,1]]");

    m1 += m2;

    CHECK(m1.toString() == "[[2,4][6,8]]");

    m1 += m3;

    CHECK(m1.toString() == "[[3,1][7,9]]");
  }

  SECTION( "Should throw an error with invalid parameter" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2,2][3,4,4][5,6,6]]");
    CHECK_THROWS_AS([&]() {
      m1 += m2;
    }(), std::out_of_range);
  }
}

/**
 * Testing the -= -operator functionality.
 */
TEST_CASE( "ConcreteSquareMatrix.-= -operator", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should substract all IntElements" ) {

    ConcreteSquareMatrix m2("[[1,2][3,4]]");
    ConcreteSquareMatrix m3("[[1,-3][1,1]]");

    m2 -= m3;

    CHECK(m2.toString() == "[[0,5][2,3]]");
  }


  SECTION( "Shoul throw an error with invalid parameter" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2,2][3,4,4][5,6,6]]");
    CHECK_THROWS_AS([&]() {
      m1 -= m2;
    }(), std::out_of_range);
  }
}

/**
 * Testing the *= -operator functionality.
 */
TEST_CASE( "ConcreteSquareMatrix.*= -operator", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should multiply all IntElements" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[5,6][7,8]]");
    // 1, 2   5, 7   5 + 14, 6 + 16     19, 22
    // 3, 4   6, 8   15 + 28, 18 + 32   43, 50
    m1 *= m2;
    CHECK(m1.toString() == "[[19,22][43,50]]");
  }

  SECTION( "Should throw an error with invalid parameter" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2,2][3,4,4][5,6,6]]");
    CHECK_THROWS_AS([&]() {
      m1 *= m2;
    }(), std::out_of_range);
  }
}

/**
 * Testing the ConcreteSquareMatrix::print member function.
 */
TEST_CASE( "ConcreteSquareMatrix.print -function", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should insert matrix as string into the given stream" ) {
    ConcreteSquareMatrix m1("[[-31,2][3,4214]]");

    std::stringstream ss;
    std::stringstream corrSs;
    corrSs << "[[-31,2][3,4214]]";

    m1.print(ss);
    CHECK(ss.str() == corrSs.str());
  }
}

/**
 * Testing the ConcreteSquareMatrix::toString member function.
 */
TEST_CASE( "ConcreteSquareMatrix.toString -function", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should return a new string with SuqreMatrix.print -style content" ) {

    ConcreteSquareMatrix m1("[[134,2][3,0]]");

    std::stringstream corrSs;
    corrSs << "[[134,2][3,0]]";

    CHECK(m1.toString() == corrSs.str());
  }
}

/**
 * Testing the ConcreteSquareMatrix::clone member function
 */
TEST_CASE( "ConcreteSquareMatrix.clone -function", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should return a new object that is a clone of the caller" ) {
    ConcreteSquareMatrix m1("[[134,2][3,0]]");
    std::unique_ptr<ConcreteSquareMatrix> m2(m1.clone());
    ConcreteSquareMatrix m3("[[3,2][0,22]]");

    CHECK(m1 == *m2);

    //m1 += m3;

    //CHECK(m1.toString() != m2->toString());
  }
}

/**
 * Testing the ConcreteSquareMatrix::evaluate member function
 */
TEST_CASE( "ConcreteSquareMatrix.evaluate -function", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should return a copy of itself" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    Valuation v {
      { 'x', 5 },
      { 'y', -122 }
    };
    ConcreteSquareMatrix m2 = m1.evaluate(v);
    CHECK( m1 == m2 );
  }
}

/**
 * Testing the ConcreteSquareMatrix::== -operator.
 */
TEST_CASE( "ConcreteSquareMatrix.== -operator", "[ConcreteSquareMatrix]") {
  SECTION( "Should return true when objects match" ) {
    ConcreteSquareMatrix m1("[[32,42,0][-32,23,11][99,120,-50]]");
    ConcreteSquareMatrix m2("[[32,42,0][-32,23,11][99,120,-50]]");
    CHECK(m1 == m2);
  }

  SECTION( "Should return false when objects do not match" ) {
    ConcreteSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
    ConcreteSquareMatrix m2("[[32,42,0][-32,23,11][99,120,-50]]");
    CHECK(!(m1 == m2));
  }
}

/**
 * Testing the ConcreteSquareMatrix::= copy operator.
 */
TEST_CASE( "ConcreteSquareMatrix.= -operator (copy)", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should copy the values of parameter to the caller" ) {
    ConcreteSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
    ConcreteSquareMatrix m2("[[32,42,0][-32,23,15][99,120,-50]]");
    m1 = m2;
    CHECK( m1.toString() == "[[32,42,0][-32,23,15][99,120,-50]]" );
    m1 = m1; // self assignment
    CHECK( m1.toString() == "[[32,42,0][-32,23,15][99,120,-50]]" );
  }
}

/**
 * Testing the SqueraMatrix::= move operator
 */
TEST_CASE( "ConcreteSquareMatrix.= -operator (move)", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should move the values of parameter to the caller" ) {
    ConcreteSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
    ConcreteSquareMatrix m2("[[32,42,0][-32,23,15][99,120,-50]]");
    m1 = std::move(m2);
    CHECK( m1.toString() == "[[32,42,0][-32,23,15][99,120,-50]]" );
  }
}

/**
 * Testing the ConcreteSquareMatrix::getDim member function
 */
TEST_CASE( "ConcreteSquareMatrix.getDim", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should return correct dimension" ) {
    ConcreteSquareMatrix m1("[[32,3,1][-32,23,11][99,120,7]]");
    ConcreteSquareMatrix m2("[[1]]");
    ConcreteSquareMatrix m3("[[32,-9][-32,23]]");
    CHECK( m1.getDim() == 3 );
    CHECK( m2.getDim() == 1 );
    CHECK( m3.getDim() == 2 );
  }
}

/**
 * Testing the + -operator functionality.
 */
TEST_CASE( "+ -operator for ConcreteSquareMatrix", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should add all IntElements" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2][3,4]]");
    ConcreteSquareMatrix m3("[[1,-3][1,1]]");
    ConcreteSquareMatrix tester;

    tester = m1 += m2;

    CHECK(tester.toString() == "[[2,4][6,8]]");

    tester = tester + m3;

    CHECK(tester.toString() == "[[3,1][7,9]]");
  }

  SECTION( "Should throw an error with invalid parameter" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2,2][3,4,4][5,6,6]]");
    CHECK_THROWS_AS([&]() {
      ConcreteSquareMatrix t = m1 + m2;
    }(), std::out_of_range);
  }
}

/**
 * Testing the - -operator functionality.
 */
TEST_CASE( "- operator for ConcreteSquareMatrix", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should substract all IntElements" ) {

    ConcreteSquareMatrix m2("[[1,2][3,4]]");
    ConcreteSquareMatrix m3("[[1,-3][1,1]]");
    ConcreteSquareMatrix tester;

    tester = m2 - m3;

    CHECK(tester.toString() == "[[0,5][2,3]]");
  }


  SECTION( "Shoul throw an error with invalid parameter" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2,2][3,4,4][5,6,6]]");
    CHECK_THROWS_AS([&]() {
      ConcreteSquareMatrix t = m1 - m2;
    }(), std::out_of_range);
  }
}

/**
 * Testing the * -operator functionality.
 */
TEST_CASE( "* -operator for ConcreteSquareMatrix", "[ConcreteSquareMatrix]" ) {
  SECTION( "Should multiply all IntElements" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[5,6][7,8]]");
    ConcreteSquareMatrix tester;
    // 1, 2   5, 7   5 + 14, 6 + 16     19, 22
    // 3, 4   6, 8   15 + 28, 18 + 32   43, 50
    tester = m1 * m2;
    CHECK(tester.toString() == "[[19,22][43,50]]");
  }

  SECTION( "Should throw an error with invalid parameter" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    ConcreteSquareMatrix m2("[[1,2,2][3,4,4][5,6,6]]");
    CHECK_THROWS_AS([&]() {
      ConcreteSquareMatrix t = m1 * m2;
    }(), std::out_of_range);
  }
}

/**
 * Testing the helper methods isConcreteSquareMatrix
 * and validateRow for ConcreteSquareMatrix.
 */
TEST_CASE( "Valid matrixes (concrete)", "[concreteMatrix]" ) {
	std::string validStrings[] = {
		"[[4,2,5][3,7,9][200,49,2]]",
		"[[900,343,6,75,8][3,532,6,3,787][-3,-53,-8,856,1000][230,429,2,321,-32442][9238,392,23,5231,3]]",
		"[[24903]]",
		"[[2,3][6,2]]",
		"[[3213,32132,323][32,423,3][942,4123,29300]]"
	};

	SECTION( "All valid values should return true" ) {
    for (int i = 0; i < 5; i++) {
  		REQUIRE( isConcreteSquareMatrix(validStrings[i]) );
    }
	}
}

/**
 * Testing the helper methods isConcreteSquareMatrix
 * and validateRow for ConcreteSquareMatrix.
 */
TEST_CASE( "Invalid matrixes (concrete)", "[concreteMatrix]" ) {
	std::string invalidStrings[] = {
		"x[[2,3][2,4]]",
		"[[24,2][42,5]]x",
		"[[s32,4][3s,4]]",
		"[[32]",
		"[232,424,23][32,233,32][66,53,7]",
		"[43,1][7,99]]",
		"[[23,41,4][70,2,4]s[2,64,77]]",
		"[[42,52,1][32,5,2]521,452,5]]",
		"[[32,421,2][8,76,5][9,2,593]a]",
		"[[5,6][[3,2]]",
		"[[4,2,5][27,7[,4][99,200,42]]",
		"[[4,2]]86,0]]",
		"[[24,42,][2,10]]"
	};

	SECTION( "All invalid values should return false" ) {
    for (int i = 0; i < 13; i++) {
  		REQUIRE( !isConcreteSquareMatrix(invalidStrings[i]) );
    }
	}
}
