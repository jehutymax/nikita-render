//
// Created by Rafael Campos on 3/27/16.
//

#include <cppunit/extensions/HelperMacros.h>
#include "../util/smfReader.h"

class TestReaders: public CppUnit::TestCase
{
public:
    CPPUNIT_TEST_SUITE(TestReaders);
    CPPUNIT_TEST(testReadersSmf);
    CPPUNIT_TEST_SUITE_END();

    void testReadersSmf();

};

CPPUNIT_TEST_SUITE_REGISTRATION(TestReaders);

void TestReaders::testReadersSmf()
{
    const char* testFile = "/Users/rafael/Projects/nikita-render/test/cube.smf";
    nikita::SmfReader::Vertices v;
    nikita::SmfReader::Indices idx;
    nikita::SmfReader::getGeometry(testFile, v, idx);

    CPPUNIT_ASSERT_EQUAL(8 * 3, (const int) v.size()); // number of vertices read from file
    CPPUNIT_ASSERT_EQUAL(12 * 3, (const int) idx.size()); // number of faces read from file
    int vertex_7 = (7 - 1) * 3;
    CPPUNIT_ASSERT(v[vertex_7] == 0.5); // 1st coord of vertex 7.
    CPPUNIT_ASSERT(v[vertex_7 + 1] == 0.5); // 2nd coord of vertex 7.
    CPPUNIT_ASSERT(v[vertex_7 + 2] == 0.5); // 3rd coord of vertex 7.
    CPPUNIT_ASSERT(v[vertex_7 + 3] == -0.5); // first coordinate of vertex 8.
}