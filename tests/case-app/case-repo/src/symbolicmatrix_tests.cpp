/**
 * @file symbolicmatrix_tests.cpp
 * This file contains unit tests for SymbolicSquareMatrix.
 * @date 22.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see SymbolicSquareMatrix
 * @see IntElement
 */

#include "elementarymatrix.hpp"
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <sstream>

/**
 * Test the constructor to be functional.
 */
TEST_CASE( "SymbolicSquareMatrix.constructor", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should not throw an error with valid parameter" ) {
    CHECK_NOTHROW([&]() {
      SymbolicSquareMatrix sqMat;
    }());

    CHECK_NOTHROW([&]() {
      SymbolicSquareMatrix sqMat("[[1,2][3,4]]");
    }());

    CHECK_NOTHROW([&]() {
      SymbolicSquareMatrix sqMat1("[[1,2][3,4]]");
      SymbolicSquareMatrix sqMat2(sqMat1);
    }());

    CHECK_NOTHROW([&]() {
      SymbolicSquareMatrix sqMat1("[[1,x][3,4]]");
      SymbolicSquareMatrix sqMat2(std::move(sqMat1));
      CHECK( sqMat1.toString() != sqMat2.toString() );
      CHECK( sqMat2.toString() == "[[1,x][3,4]]" );
    }());

  }

  SECTION( "Should throw an error with invalid parameter" ) {
    CHECK_THROWS_AS([&]() {
      SymbolicSquareMatrix sqMat("[ads]");
    }(), std::invalid_argument);
    CHECK_THROWS_AS([&]() {
      SymbolicSquareMatrix sqMat("[[392,-124,][2,1]]");
    }(), std::invalid_argument);
    CHECK_THROWS_AS([&]() {
      SymbolicSquareMatrix sqMat("[[4,2][xa,2]]");
    }(), std::invalid_argument);
  }
}

/**
 * Testing transposing SymbolicSquareMatrix into a new SymbolicSquareMatrix
 */
TEST_CASE( "SymbolicSquareMatrix.transpose", "[SymbolicSquareMatrix]" ) {
  SymbolicSquareMatrix sq("[[1,2][3,4]]");
  SymbolicSquareMatrix sq2 = sq.transpose();
  CHECK( sq2.toString() == "[[1,3][2,4]]");
  SymbolicSquareMatrix sq3("[[1,2,x][4,5,6][y,8,9]]");
  SymbolicSquareMatrix sq4 = sq3.transpose();
  CHECK( sq4.toString() == "[[1,4,y][2,5,8][x,6,9]]");
}


/**
 * Testing the SymbolicSquareMatrix::print member function.
 */
TEST_CASE( "SymbolicSquareMatrix.print -function", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should insert matrix as string into the given stream" ) {
    SymbolicSquareMatrix m1("[[-31,2][3,4214]]");

    std::stringstream ss;
    std::stringstream corrSs;
    corrSs << "[[-31,2][3,4214]]";

    m1.print(ss);
    CHECK(ss.str() == corrSs.str());
  }
}

/**
 * Testing the SymbolicSquareMatrix::toString member function.
 */
TEST_CASE( "SymbolicSquareMatrix.toString -function", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should return a new string with SuqreMatrix.print -style content" ) {

    SymbolicSquareMatrix m1("[[134,2][3,0]]");

    std::stringstream corrSs;
    corrSs << "[[134,2][3,0]]";

    CHECK(m1.toString() == corrSs.str());
  }
}

/**
 * Testing the SymbolicSquareMatrix::== -operator.
 */
TEST_CASE( "SymbolicSquareMatrix.== -operator", "[SymbolicSquareMatrix]") {
  SECTION( "Should return true when objects match" ) {
    SymbolicSquareMatrix m1("[[32,42,0][-32,23,11][99,120,-50]]");
    SymbolicSquareMatrix m2("[[32,42,0][-32,23,11][99,120,-50]]");
    CHECK(m1 == m2);
  }

  SECTION( "Should return false when objects do not match" ) {
    SymbolicSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
    SymbolicSquareMatrix m2("[[32,42,0][-32,23,11][99,120,-50]]");
    CHECK(!(m1 == m2));
  }
}

/**
 * Testing the SymbolicSquareMatrix::= copy operator.
 */
TEST_CASE( "SymbolicSquareMatrix.= -operator (copy)", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should copy the values of parameter to the caller" ) {
    SymbolicSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
    SymbolicSquareMatrix m2("[[32,42,0][-32,23,15][99,120,-50]]");
    m1 = m2;
    CHECK( m1.toString() == "[[32,42,0][-32,23,15][99,120,-50]]" );
    m1 = m1; // self assignment
    CHECK( m1.toString() == "[[32,42,0][-32,23,15][99,120,-50]]" );
  }
}

/**
 * Testing the SqueraMatrix::= move operator
 */
 TEST_CASE( "SymbolicSquareMatrix.= -operator (move)", "[SymbolicSquareMatrix]" ) {
   SECTION( "Should move the values of parameter to the caller" ) {
     SymbolicSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
     SymbolicSquareMatrix m2("[[32,42,0][-32,23,15][99,120,-50]]");
     m1 = std::move(m2);
     CHECK( m1.toString() == "[[32,42,0][-32,23,15][99,120,-50]]" );
   }
 }

/**
 * Testing the SymbolicSquareMatrix::clone member function
 */
TEST_CASE( "SymbolicSquareMatrix.clone -function", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should return a new object that is a clone of the caller" ) {
    SymbolicSquareMatrix m1("[[x,2][3,y]]");
    std::unique_ptr<SymbolicSquareMatrix> m2(m1.clone());
    SymbolicSquareMatrix m3("[[3,2][0,22]]");

    CHECK(m1 == *m2);
  }
}

/**
 * Testing the SymbolicSquareMatrix::evaluate member function
 */
TEST_CASE( "SymbolicSquareMatrix.evaluate", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should return correct ConcreteSquareMatrix" ) {
    SymbolicSquareMatrix m1("[[32,x,1][-32,23,11][99,120,y]]");
    Valuation v {
      { 'x', 5 },
      { 'y', -122 }
    };
    ConcreteSquareMatrix c1 = m1.evaluate(v);
    CHECK( c1.toString() == "[[32,5,1][-32,23,11][99,120,-122]]" );
  }

  SECTION( "Should throw an error if receiving invalid valuation" ) {
    SymbolicSquareMatrix m1("[[32,x,1][-32,23,11][99,120,y]]");
    Valuation v {
      { 'x', 5 },
      { 'z', -122 }
    };

    CHECK_THROWS_AS([&]() {
      ConcreteSquareMatrix c1 = m1.evaluate(v);
    }(), std::out_of_range);

  }
}

/**
 * Testing the SymbolicSquareMatrix::getDim member function
 */
TEST_CASE( "SymbolicSquareMatrix.getDim", "[SymbolicSquareMatrix]" ) {
  SECTION( "Should return correct dimension" ) {
    SymbolicSquareMatrix m1("[[32,3,1][-32,23,11][99,120,7]]");
    SymbolicSquareMatrix m2("[[1]]");
    SymbolicSquareMatrix m3("[[32,-9][-32,23]]");
    CHECK( m1.getDim() == 3 );
    CHECK( m2.getDim() == 1 );
    CHECK( m3.getDim() == 2 );
  }
}


/**
 * Testing the helper methods isSymbolicSquareMatrix
 * and validateSymbolicRow for SymbolicSquareMatrix.
 */
TEST_CASE( "Valid matrixes (symbolic)", "[symbolicMatrix]" ) {
	std::string validStrings[] = {
		"[[4,2,5][3,7,9][200,49,2]]",
		"[[900,343,6,75,8][3,532,6,3,787][-3,-53,-8,856,1000][230,429,2,321,-32442][9238,392,23,5231,3]]",
		"[[24903]]",
		"[[2,3][6,2]]",
		"[[3213,32132,323][32,423,3][942,4123,29300]]",
    "[[a,5][b,2]]"
	};

	SECTION( "Valid [0] should result to true" ) {
    for (int i = 0; i < 6; i++) {
  		REQUIRE(isSymbolicSquareMatrix(validStrings[i]));
    }
	}
}

/**
 * Testing the helper methods isSymbolicSquareMatrix
 * and validateSymbolicRow for SymbolicSquareMatrix.
 */
TEST_CASE( "Invalid matrixes (symbolic)", "[symbolicMatrix]" ) {
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
		"[[24,42,][2,10]]",
    "[[2,4][sa,4]]",
    "[[5,-2][x,|]]",
    "[[5,6][,,2]]"
	};

	SECTION( "Invalid [0] should result to false" ) {
    for (int i = 0; i < 16; i++) {
  		REQUIRE(!isSymbolicSquareMatrix(invalidStrings[i]));
    }
	}

}
