#include "core/nikita.h"

#include "util/sceneParser.h"
#include "util/smfReader.h"
#include "version.h"
#include "core/renderer.h"

int main(int argc, char* argv[]) {
    std::cout << "Nikita Renderer" << std::endl;
    std::cout << "Version " << nikita::version << std::endl;

    if (argc != 2)
    {
        std::cout <<
            "Usage: " << argv[0] << " [filename]\n" <<
            "Which: runs Nikita Render on the specified scene(s).\n" <<
            "Mandatory parameters:\n" <<
            "    [filename]       XML file containing the scene information.\n" << std::endl;

        return 0;
    }

    nikita::sceneParser parser;
    parser.loadFile(std::string(argv[1]));
    nikita::ScenePtr scene = parser.getScene();

    nikita::SimpleRenderer renderer(parser.getCamera());

    renderer.render(scene);

    return 0;
}