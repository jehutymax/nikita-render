//
// Created by Rafael Campos on 3/26/16.
//

#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>


static void dump(CppUnit::Test *test)
{
    if (test == NULL)
    {
        std::cerr << "Error: no tests found." << std::endl;
        return;
    }

    std::cout << test->getName() << std::endl;
    for (int i = 0; i < test->getChildTestCount(); i++)
    {
        dump(test->getChildTestAt(i));
    }
}

int run(int argc, char* argv[])
{
    const char* programName = argv[0];

    std::vector<std::string> tests;

    if (tests.empty()) tests.push_back(""); // run everything

    try {
        CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();

        CppUnit::TestRunner runner;
        runner.addTest(registry.makeTest());

        CppUnit::TestResult controller;

        CppUnit::TestResultCollector result;
        controller.addListener(&result);

        CppUnit::BriefTestProgressListener progress;
        controller.addListener(&progress);

        for (std::size_t i = 0; i < tests.size(); i++)
        {
            runner.run(controller, tests[i]);
        }

        CppUnit::CompilerOutputter output(&result, std::cerr);
        output.write();

        return result.wasSuccessful() ? EXIT_SUCCESS : EXIT_FAILURE;

    } catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

}

int main(int argc, char* argv[])
{
    return run(argc, argv);
}