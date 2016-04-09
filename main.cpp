#include "core/nikita.h"

#include "util/sceneParser.h"
#include "util/smfReader.h"
#include "version.h"
#include "core/renderer.h"

int main() {
    std::cout << "Nikita Renderer" << std::endl;
    std::cout << "Version " << nikita::version << std::endl;

    nikita::sceneParser parser;
    parser.loadFile("/Users/rafael/Projects/nikita-render/test/nikita_scene.xml");
    nikita::Scene scene = *parser.getScene();

    nikita::SimpleRenderer renderer(parser.getCamera());

    renderer.render(scene);

    std::cout << ((scene.objects[0].get())->isIntersectable() ? "true" : "false") << std::endl;
    return 0;
}