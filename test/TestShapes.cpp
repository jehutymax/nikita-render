//
// Created by Rafael Campos on 3/27/16.
//

#include <cppunit/extensions/HelperMacros.h>
#include "../util/smfReader.h"
#include "../core/shapes/triangleMesh.h"
#include "../core/accelerator/BoundingBox.h"

class TestShapes: public CppUnit::TestCase
{
public:
        CPPUNIT_TEST_SUITE(TestShapes);
        CPPUNIT_TEST(testTriangleMesh);
        CPPUNIT_TEST(testBoundingBox);
        CPPUNIT_TEST_SUITE_END();

        void testTriangleMesh();
        void testBoundingBox();

};

    CPPUNIT_TEST_SUITE_REGISTRATION(TestShapes);

    void TestShapes::testTriangleMesh()
    {
        const char* testFile = "/Users/rafael/Projects/nikita-render/test/cube.smf";
        nikita::SmfReader::Vertices v;
        nikita::SmfReader::Indices idx;
        nikita::SmfReader::getGeometry(testFile, v, idx);
        nikita::TriangleMesh mesh(nullptr, nullptr, idx.size(), v.size(), v, idx);

        CPPUNIT_ASSERT(true);
    }

    void TestShapes::testBoundingBox()
    {
        Point a(1,4,1);
        Point b(5,1,3);
        Point in(2,2,2);
        Point out(5,5,5);

        // expected values:
        Point min(1,1,1);
        Point max(5,4,3);
        float area = 52;
        float volume = 24;

        nikita::BoundingBox bbox(a, b);

        CPPUNIT_ASSERT_EQUAL(min, bbox.pMin());
        CPPUNIT_ASSERT_EQUAL(max, bbox.pMax());
        CPPUNIT_ASSERT_EQUAL(area, bbox.surfaceArea());
        CPPUNIT_ASSERT_EQUAL(volume, bbox.volume());
        CPPUNIT_ASSERT(bbox.inside(in));
        CPPUNIT_ASSERT(!bbox.inside(out));

        // expand the box and check if the properties follow.
        bbox.expand(0.5);
        area = 94;
        volume = 60.0;

        CPPUNIT_ASSERT_EQUAL(area, bbox.surfaceArea());
        CPPUNIT_ASSERT_EQUAL(volume, bbox.volume());
    }