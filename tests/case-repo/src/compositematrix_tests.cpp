/**
 * @file compositematrix_tests.cpp
 * This file contains unit tests for CompositeSquareMatrix.
 * @date 22.2.2018
 * @author Perttu Kärnä
 * @copyright © 2018 Perttu Kärnä. All rights reserved.
 * @see CompositeSquareMatrix
 */

#include "compositematrix.hpp"
#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"


TEST_CASE( "CompositeSquareMatrix.constructor" ) {

  SECTION( "Should construct the object with correct parameters" ) {
    SymbolicSquareMatrix sqMat1("[[a,b][c,d]]");
    ConcreteSquareMatrix sqMat2("[[1,3][7,1]]");
    ConcreteSquareMatrix sqMat3("[[1,3,5][7,1,-2][2,7,2]]");


    CHECK_NOTHROW([&]() {
      CompositeSquareMatrix csm(sqMat1, sqMat2, std::plus<ConcreteSquareMatrix>(), '+');
    }());

    CHECK_NOTHROW([&]() {
      CompositeSquareMatrix csm(sqMat1, sqMat2, std::plus<ConcreteSquareMatrix>(), '+');
      CompositeSquareMatrix csm2(csm);
      CompositeSquareMatrix csm3(csm, csm2, std::plus<ConcreteSquareMatrix>(), '+');
    }());

    CHECK_NOTHROW([&]() {
      CompositeSquareMatrix csm(sqMat1, sqMat2, std::plus<ConcreteSquareMatrix>(), '+');
      CompositeSquareMatrix csm2(std::move(csm));
    }());

    REQUIRE_THROWS_AS([&]() {
      CompositeSquareMatrix(sqMat1, sqMat3, std::plus<ConcreteSquareMatrix>(), '+');
    }(), std::invalid_argument);
  }
}

/**
 * Testing the CompositeSquareMatrix::print member function.
 */
TEST_CASE( "CompositeSquareMatrix.print -function", "[CompositeSquareMatrix]" ) {
  SECTION( "Should insert matrix as string into the given stream" ) {
    ConcreteSquareMatrix m1("[[-31,2][3,4214]]");
    SymbolicSquareMatrix m2("[[1,2][a,b]]");
    CompositeSquareMatrix cm(m1, m2, std::plus<ConcreteSquareMatrix>(), '+');

    std::stringstream ss;
    std::stringstream corrSs;
    corrSs << "([[-31,2][3,4214]]+[[1,2][a,b]])";

    cm.print(ss);
    CHECK(ss.str() == corrSs.str());

    CompositeSquareMatrix cm2(cm, m2, std::plus<ConcreteSquareMatrix>(), '+');
    std::stringstream ss2;
    cm2.print(ss2);
    CHECK(ss2.str() == "(([[-31,2][3,4214]]+[[1,2][a,b]])+[[1,2][a,b]])");
  }
}

/**
 * Testing the CompositeSquareMatrix::toString member function.
 */
TEST_CASE( "CompositeSquareMatrix.toString -function", "[CompositeSquareMatrix]" ) {
  SECTION( "Should return a new string with SuqreMatrix.print -style content" ) {
    ConcreteSquareMatrix m1("[[-31,2][3,4214]]");
    SymbolicSquareMatrix m2("[[1,2][a,b]]");
    CompositeSquareMatrix cm(m1, m2, std::plus<ConcreteSquareMatrix>(), '+');

    std::stringstream corrSs;
    corrSs << "([[-31,2][3,4214]]+[[1,2][a,b]])";

    CHECK(cm.toString() == corrSs.str());
  }
}

 /**
  * Testing the CompositeSquareMatrix::clone member function
  */
TEST_CASE( "CompositeSquareMatrix.clone -function", "[CompositeSquareMatrix]" ) {
  SECTION( "Should return a new object that is a clone of the caller" ) {
    ConcreteSquareMatrix m1("[[134,2][3,0]]");
    SymbolicSquareMatrix m2("[[1,b][2,5]]");
    CompositeSquareMatrix csm(m1, m2, std::plus<ConcreteSquareMatrix>(), '+');
    std::unique_ptr<CompositeSquareMatrix> csm2(csm.clone());

    CHECK(csm.toString() == csm2->toString());
  }
}

/**
 * Testing the CompositeSquareMatrix::evaluate member function
 */
TEST_CASE( "CompositeSquareMatrix.evaluate -function", "[CompositeSquareMatrix]" ) {
  SECTION( "Should calculate plus operand" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    SymbolicSquareMatrix m2("[[1,m][3,x]]");
    CompositeSquareMatrix csm(m1, m2, std::plus<ConcreteSquareMatrix>(), '+');

    Valuation v {
      { 'm', 2 },
      { 'x', 4 }
    };

    ConcreteSquareMatrix result = csm.evaluate(v);

    CHECK(result.toString() == "[[2,4][6,8]]");
  }

  SECTION( "Should calculate minus operand" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    SymbolicSquareMatrix m2("[[1,m][x,1]]");
    CompositeSquareMatrix csm(m1, m2, std::minus<ConcreteSquareMatrix>(), '-');

    Valuation v {
      { 'm', -3 },
      { 'x', 1 }
    };

    ConcreteSquareMatrix result = csm.evaluate(v);

    CHECK(result.toString() == "[[0,5][2,3]]");
  }

  SECTION( "Should calculate multiply operand" ) {
    ConcreteSquareMatrix m1("[[1,2][3,4]]");
    SymbolicSquareMatrix m2("[[m,x][7,8]]");
    // 1, 2   5, 7   5 + 14, 6 + 16     19, 22
    // 3, 4   6, 8   15 + 28, 18 + 32   43, 50
    CompositeSquareMatrix csm(m1, m2, std::multiplies<ConcreteSquareMatrix>(), '*');

    Valuation v {
      { 'm', 5 },
      { 'x', 6 }
    };

    ConcreteSquareMatrix result = csm.evaluate(v);

    CHECK(result.toString() == "[[19,22][43,50]]");
  }
}


/**
 * Testing the CompositeSquareMatrix::= copy operator.
 */
TEST_CASE( "CompositeSquareMatrix.= -operator (copy)", "[CompositeSquareMatrix]" ) {
  SECTION( "Should copy the values of parameter to the caller" ) {
    ConcreteSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
    ConcreteSquareMatrix m2("[[32,42,0][-32,23,15][99,120,-50]]");
    ConcreteSquareMatrix m3("[[323,42,1][-32,23,11][99,120,-50]]");
    ConcreteSquareMatrix m4("[[321,42,0][-32,23,15][99,120,-50]]");
    CompositeSquareMatrix csm(m1, m2, std::plus<ConcreteSquareMatrix>(), '+');
    CompositeSquareMatrix csm2(m3, m4, std::plus<ConcreteSquareMatrix>(), '+');

    csm = csm2;
    CHECK( csm.toString() == "([[323,42,1][-32,23,11][99,120,-50]]+[[321,42,0][-32,23,15][99,120,-50]])" );
    CHECK( csm2.toString() == csm.toString() );

    csm = csm; // self assignment
    CHECK( csm.toString() == "([[323,42,1][-32,23,11][99,120,-50]]+[[321,42,0][-32,23,15][99,120,-50]])" );
  }
}

/**
 * Testing the CompositeSquareMatrix::= move operator
 */
 TEST_CASE( "CompositeSquareMatrix.= -operator (move)", "[CompositeSquareMatrix]" ) {
   SECTION( "Should move the values of parameter to the caller" ) {
     ConcreteSquareMatrix m1("[[32,42,1][-32,23,11][99,120,-50]]");
     ConcreteSquareMatrix m2("[[32,42,0][-32,23,15][99,120,-50]]");
     ConcreteSquareMatrix m3("[[323,42,1][-32,23,11][99,120,-50]]");
     ConcreteSquareMatrix m4("[[321,42,0][-32,23,15][99,120,-50]]");
     CompositeSquareMatrix csm(m1, m2, std::plus<ConcreteSquareMatrix>(), '+');
     CompositeSquareMatrix csm2(m3, m4, std::plus<ConcreteSquareMatrix>(), '+');

     csm = std::move(csm2);
     CHECK( csm.toString() == "([[323,42,1][-32,23,11][99,120,-50]]+[[321,42,0][-32,23,15][99,120,-50]])" );
   }
 }

 /**
  * Testing the CompositeSquareMatrix::getDim member function
  */
 TEST_CASE( "CompositeSquareMatrix.getDim", "[CompositeSquareMatrix]" ) {
   SECTION( "Should return correct dimension" ) {
     ConcreteSquareMatrix m11("[[32,3,1][-32,23,11][99,120,7]]");
     ConcreteSquareMatrix m12("[[32,3,2][-32,23,11][99,120,7]]");
     ConcreteSquareMatrix m21("[[1]]");
     ConcreteSquareMatrix m22("[[3]]");
     ConcreteSquareMatrix m31("[[32,-9][-32,23]]");
     ConcreteSquareMatrix m32("[[32,100][-32,23]]");

     CompositeSquareMatrix m1(m11, m12, std::plus<ConcreteSquareMatrix>(), '+');
     CompositeSquareMatrix m2(m21, m22, std::plus<ConcreteSquareMatrix>(), '+');
     CompositeSquareMatrix m3(m31, m32, std::plus<ConcreteSquareMatrix>(), '+');

     CHECK( m1.getDim() == 3 );
     CHECK( m2.getDim() == 1 );
     CHECK( m3.getDim() == 2 );
   }
 }
