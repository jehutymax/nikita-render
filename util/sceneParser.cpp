//
// Created by Rafael Campos on 3/22/16.
//

#include "sceneParser.h"

using nikita::sceneParser;
sceneParser::SceneTagMap sceneParser::sceneTags;

void nikita::sceneParser::loadFile(std::string filePath)
{
    OIIO::pugi::xml_document doc;
    OIIO::pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    Node scene = doc.child("nikita").child("scene");

    std::cout << "Reading file " << filePath << ": " << result.description() << std::endl;

    this->scene = std::make_shared<Scene>();

    for (OIIO::pugi::xml_node_iterator it = scene.begin(); it != scene.end(); ++it)
    {
        std::string currentNode(it->name());
        StringUtils::toLower(currentNode);

        switch(sceneTags.at(currentNode))
        {
            case SCENE:
                break;
            case SHAPE:
                processShape(*it);
                break;
            case SENSOR:
                processSensor(*it);
                break;
            default:
                break;
        }
    }
}

void sceneParser::processShape(const OIIO::pugi::xml_node &node)
{
    switch(sceneTags.at(getType(node)))
    {
        case SPHERE:
            createSphere(node);
            break;
        case SMF:
            createTriangleMesh(node);
            break;
        case OBJ:
            std::cout << "OBJ reader not implemented." << std::endl;
            return;
        default:
            std::cout << getType(node) << " not understood." << std::endl;
            return;
    }

}

void sceneParser::processSensor(const OIIO::pugi::xml_node &node)
{
    std::string sensorType = getType(node);
    int fieldOfView;
    if (getInteger(node, "fov", &fieldOfView))
        std::cout << "Field of view: " << fieldOfView << " degrees." << std::endl;

    // transform
    nikita::TransformPtr t = processTransform(node);

    // Film
    Node film = node.child("film");
    if (!film)
    {
        // no film has been specified
        // set default values here.
    }
    else
    {
        int w, h;
        std::string filename;
        getInteger(film, "width", &w);
        getInteger(film, "height", &h);
        getString(film, "filename", &filename);
        std::cout << "Width: " << w << "; Height: " << h << std::endl;
        std::cout << "Image to be written: " << filename << std::endl;

        this->film = std::make_shared<BWFilm>(w, h, filename);
    }

    // Sampler
    Node sampler = node.child("sampler");

    // Camera
    float screenDimensions[4] = {-1.f, 1.f, -1.f, 1.f};
    this->camera = std::make_shared<PerspectiveCamera>(*t, screenDimensions, fieldOfView, this->film);

}

nikita::TransformPtr sceneParser::processTransform(const Node &node)
{
    nikita::TransformPtr cumulative = std::make_shared<Transform>();

    Node transformNode;
    if (node.name() == "transform")
        transformNode = node;
    else if (hasTransform(node))
        transformNode = node.child("transform");
    else
        return cumulative;

    for (OIIO::pugi::xml_node_iterator ti = transformNode.begin(); ti != transformNode.end(); ti++)
    {
        Transform t;
        std::string tName(ti->name()); StringUtils::toLower(tName);
        switch(sceneTags.at(tName))
        {
            case ROTATE:
                t = processRotate(*ti);
                break;
            case TRANSLATE:
                t = processTranslate(*ti);
                break;
            case SCALE:
                t = processScale(*ti);
                break;
            case LOOKAT:
                t = processLookAt(*ti);
                break;
            default:
                break;
        }
        *cumulative = *cumulative * t;
    }

    return cumulative;
}

nikita::Transform sceneParser::processTranslate(const Node &node)
{
    Transform t;
    Attribute x = node.attribute("x");
    Attribute y = node.attribute("y");
    Attribute z = node.attribute("z");

    float xv = (x ? std::stof(x.value()) : 0.0f);
    float yv = (y ? std::stof(y.value()) : 0.0f);
    float zv = (z ? std::stof(z.value()) : 0.0f);
    Vector s(xv, yv, zv);
    t = Transform::getTranslation(s);
    return t;
}

nikita::Transform sceneParser::processRotate(const Node &node)
{
    nikita::Transform t;
    float angle = std::stof(node.attribute("angle").value());
    std::string x, y, z;
    x = node.attribute("x").value();
    y = node.attribute("y").value();
    z = node.attribute("z").value();

    if (x == "1")
        t = nikita::Transform::getRotationX(angle);
    else if (y == "1")
        t = nikita::Transform::getRotationY(angle);
    else if (z == "1")
        t = nikita::Transform::getRotationZ(angle);

    return t;
}

nikita::Transform sceneParser::processScale(const Node &node)
{
    Transform scale;
    // if 'value' is specified, it's a uniform scale
    Attribute value = node.attribute("value");
    if (value)
    {
        float scaleValue = std::stof(value.value());
        scale = Transform::getUniformScale(scaleValue);
    }
    else
    {
        Attribute x = node.attribute("x");
        Attribute y = node.attribute("y");
        Attribute z = node.attribute("z");

        float xv = (x ? std::stof(x.value()) : 1.0f);
        float yv = (y ? std::stof(y.value()) : 1.0f);
        float zv = (z ? std::stof(z.value()) : 1.0f);
        Vector s(xv, yv, zv);
        scale = Transform::getScale(s);
    }

    return scale;
}

Transform sceneParser::processLookAt(const Node &node)
{
    Attribute origin = node.attribute("origin");
    Attribute target = node.attribute("target");
    Attribute up = node.attribute("up");

    Point pOrigin, pTarget, pUp;
    if (origin)
    {
        std::vector<std::string> originTokens = StringUtils::tokenize(origin.value(), ",");
        pOrigin = Point(std::stof(originTokens[0]), std::stof(originTokens[1]), std::stof(originTokens[2]));
    }

    if (target)
    {
        std::vector<std::string> targetTokens = StringUtils::tokenize(target.value(), ",");
        pTarget = Point(std::stof(targetTokens[0]), std::stof(targetTokens[1]), std::stof(targetTokens[2]));
    }

    if (up)
    {
        std::vector<std::string> upTokens = StringUtils::tokenize(up.value(), ",");
        pUp = Point(std::stof(upTokens[0]), std::stof(upTokens[1]), std::stof(upTokens[2]));
    }

    return Transform::getLookAt(pOrigin, pTarget, pUp);
}

bool sceneParser::hasTransform(const Node &parentNode)
{
    Node transf = parentNode.child("transform");
    if (transf)
        return true;
    else
        return false;
}

void sceneParser::createSphere(const OIIO::pugi::xml_node &node)
{
    OIIO::pugi::xml_node radiusNode = node.find_child_by_attribute("name", "radius");
    float radius, phi, zmin, zmax;
    if (radiusNode)
        radius = std::stof(radiusNode.attribute("value").value());
    else
        radius = 2.0;

    if (!getFloat(node, "phi", &phi))
        phi = 360.0;

    if (!getFloat(node, "zmin", &zmin))
        zmin = -radius;

    if (!getFloat(node, "zmax", &zmax))
        zmax = radius;

    // Get a transform for the sphere
    TransformPtr pt = processTransform(node);
    TransformPtr ptInv = std::make_shared<Transform>(pt->getInv());
    this->scene->objects.push_back(std::make_shared<Sphere>(pt, ptInv, radius, zmin, zmax, phi));
}

void sceneParser::createTriangleMesh(const OIIO::pugi::xml_node &node)
{
    std::string file;
    getString(node, "filename", &file);

    std::vector<float> trianglePos;
    std::vector<int> faces;

    SmfReader::getGeometry(file.c_str(), trianglePos, faces);

    int numTriangles = faces.size() / 3;
    TransformPtr pt = processTransform(node);
    TransformPtr ptInv = std::make_shared<Transform>(pt->getInv());
    this->scene->objects.push_back(std::make_shared<TriangleMesh>(pt, ptInv, numTriangles, trianglePos.size(), trianglePos, faces));
    std::cout << "SMF file was loaded with " << numTriangles << " triangles." << std::endl;
}

const OIIO::pugi::char_t* sceneParser::getType(const Node &node)
{
    return node.attribute("type").value();
}

bool sceneParser::getInteger(const Node &node, const std::string &property, int *result)
{
    Node n = node.find_child_by_attribute("name", property.c_str());
    if (n)
    {
        *result = std::stoi(n.attribute("value").value());
        return true;
    }
    else return false;
}

bool sceneParser::getFloat(const Node &node, const std::string &property, float *result)
{
    Node n = node.find_child_by_attribute("name", property.c_str());
    if (n)
    {
        *result = std::stof(n.attribute("value").value());
        return true;
    }
    else return false;
}

bool sceneParser::getString(const Node &node, const std::string &property, std::string *result)
{
    Node n = node.find_child_by_attribute("name", property.c_str());
    if (n)
    {
        *result = std::string(n.attribute("value").value());
        return true;
    }
    else return false;
}

nikita::ScenePtr sceneParser::getScene()
{
    return scene;
}

nikita::CameraPtr sceneParser::getCamera()
{
    return camera;
}