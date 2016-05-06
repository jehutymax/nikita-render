if (NOT APPLE)
    cmake_minimum_required(VERSION 2.0)
else(NOT APPLE)
    cmake_minimum_required(VERSION 3.4)
endif(NOT APPLE)

project(nikita_render)

if (NOT APPLE)
    include_directories(
            ../3rd_party
            ../3rd_party/openimageio/dist/linux64/include
            ../3rd_party/tbb/tbb/include)

    link_directories(
            ../3rd_party/openimageio/dist/linux64/lib)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fpermissive")
else (NOT APPLE) #i.e., it is Apple
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (NOT APPLE)


set(SOURCE_FILES
        main.cpp
        version.h
        util/sceneParser.cpp
        util/sceneParser.h
        core/transform.h
        core/shape.h
        core/nikita.h
        core/triangleMesh.cpp
        core/triangleMesh.h
        util/smfReader.cpp
        util/smfReader.h
        util/stringUtils.cpp
        util/stringUtils.h
        core/accelerator/BoundingBox.cpp
        core/accelerator/BoundingBox.h
        util/mathUtils.h
        core/sphere.cpp
        core/sphere.h
        core/ray.cpp
        core/ray.h
        numerical/quadratic.h
        core/scene.cpp
        core/scene.h
        core/renderer.cpp
        core/renderer.h
        core/camera.cpp
        core/camera.h
        core/sensor.cpp
        core/sensor.h
        core/sampler.cpp
        core/sampler.h
        core/transform.cpp
        util/cpuTimer.h
        core/color.cpp
        core/color.h
        core/light.cpp
        core/light.h
        core/bxdf.cpp
        core/bxdf.h
        core/intersection.cpp
        core/intersection.h
        core/primitive.cpp
        core/primitive.h
        core/surfaceShader.cpp
        core/surfaceShader.h
        core/material.cpp
        core/material.h
        core/accelerator/bvh.cpp core/accelerator/bvh.h)

set(TEST_SOURCE_FILES
        test/main.cpp
        test/TestShapes.cpp
        test/TestReaders.cpp
        util/stringUtils.cpp
        util/stringUtils.h
        core/accelerator/BoundingBox.cpp
        core/accelerator/BoundingBox.h
        core/ray.cpp
        core/ray.h
        test/TestNumerical.cpp
        core/triangleMesh.cpp
        core/color.cpp
        test/TestColor.cpp)

add_executable(nikita_render ${SOURCE_FILES})
add_executable(nikita_render_tests ${TEST_SOURCE_FILES})

target_link_libraries(nikita_render OpenImageIO)
target_link_libraries(nikita_render_tests cppunit)
