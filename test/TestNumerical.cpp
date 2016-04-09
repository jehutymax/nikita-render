//
// Created by Rafael Campos on 4/2/16.
//

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "../numerical/quadratic.h"


class TestNumerical: public CppUnit::TestCase
{
public:
    CPPUNIT_TEST_SUITE(TestNumerical);
    CPPUNIT_TEST(testQuadratic);
    CPPUNIT_TEST_SUITE_END();

    void testQuadratic();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestNumerical);

void TestNumerical::testQuadratic()
{
    // zero real-valued roots
    float a = 1, b = 1, c = 1;
    CPPUNIT_ASSERT(!numerical::quadratic::solve(a, b, c, nullptr, nullptr));

    // one root
    float x0 = 0, x1 = 0;
    b = 2;
    CPPUNIT_ASSERT(numerical::quadratic::solve(a, b, c, &x0, &x1));
    CPPUNIT_ASSERT(x0 == x1);
    CPPUNIT_ASSERT_EQUAL(-1.0f, x0);

    // two roots
    a = 1; b = -1; c = -2;
    CPPUNIT_ASSERT(numerical::quadratic::solve(a, b, c, &x0, &x1));
    CPPUNIT_ASSERT(x0 < x1);
    CPPUNIT_ASSERT_EQUAL(-1.0f, x0);
    CPPUNIT_ASSERT_EQUAL(2.0f, x1);
}