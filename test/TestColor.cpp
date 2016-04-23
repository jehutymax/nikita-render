//
// Created by Rafael Campos on 4/16/16.
//

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "../core/color.h"


class TestColor : public CppUnit::TestCase
{
public:
    CPPUNIT_TEST_SUITE(TestColor);
    CPPUNIT_TEST(testOperations);
    CPPUNIT_TEST_SUITE_END();

    void testOperations();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TestColor);

void TestColor::testOperations()
{
    nikita::Color red = nikita::Color::red();
    nikita::Color blue = nikita::Color::blue();

    RGB redAndBlue(1, 0, 1);
    nikita::Color magenta = red + blue;
//    CPPUNIT_ASSERT_EQUAL(redAndBlue, redAndBlue);

}

