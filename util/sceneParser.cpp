//
// Created by Rafael Campos on 3/22/16.
//

#include "sceneParser.h"

using nikita::sceneParser;
sceneParser::SceneTagMap sceneParser::sceneTags;

sceneParser::sceneParser()
    : scene(std::make_shared<Scene>())
{ }

void nikita::sceneParser::loadFile(std::string filePath)
{
    OIIO::pugi::xml_document doc;
    OIIO::pugi::xml_parse_result result = doc.load_file(filePath.c_str());
    Node scene = doc.child("nikita").child("scene");

    std::cout << "Reading file " << filePath << ": " << result.description() << std::endl;

    setBackground(scene);

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
            case LIGHT:
                processLight(*it);
                break;
            default:
                break;
        }
    }
}
void sceneParser::setBackground(const Node &scene)
{
    Color background;
    if (scene.attribute("background"))
        background = getColor(scene.attribute("background"));
    else
        background = Color(0.86, 0.98, 0.98);
    sceneParser::scene->backgroundColor = background;
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

        this->film = std::make_shared<RGBFilm>(w, h, filename);
    }

    // Sampler
    Node sampler = node.child("sampler");

    // Camera
    float screenDimensions[4] = {-1.f, 1.f, -1.f, 1.f};
    this->camera = std::make_shared<PerspectiveCamera>(*t, screenDimensions, fieldOfView, this->film);

}

nikita::MaterialPtr sceneParser::processMaterial(const OIIO::pugi::xml_node &node)
{
    Node materialNode;
    if (node.name() == "material")
        materialNode = node;
    else if (hasMaterial(node))
        materialNode = node.child("material");
    else
    {
        // make default
        MattePtr matte = std::make_shared<Matte>();
        matte->setKa(0.2);
        matte->setKd(0.8);
        matte->setCd(Color::green());
        return MaterialPtr(matte);
    }

    switch(sceneTags.at(getType(materialNode))) {
        case MATTE: {
            float ka;
            if (!getFloat(materialNode, "ka", &ka))
                ka = 0;
            float kd;
            if (!getFloat(materialNode, "kd", &kd))
                kd = 0;
            Color cd = getColor(materialNode);
            MattePtr matte = std::make_shared<Matte>();
            matte->setKa(ka);
            matte->setKd(kd);
            matte->setCd(cd);
            return MaterialPtr(matte);
        }
        case PHONG: {
            float ka;
            if (!getFloat(materialNode, "ka", &ka))
                ka = 0;
            float kd;
            if (!getFloat(materialNode, "kd", &kd))
                kd = 0;
            float ks;
            if (!getFloat(materialNode, "ks", &ks))
                ks = 0;
            float exp;
            if (!getFloat(materialNode, "exp", &exp))
                exp = 6;
            Color cd = getColor(materialNode);
            PhongPtr phong = std::make_shared<Phong>();
            phong->setKa(ka);
            phong->setKd(kd);
            phong->setKs(ks);
            phong->setExp(exp);
            phong->setCd(cd);
            return MaterialPtr(phong);
        }
        default:
            std::cout << getType(node) << " not understood." << std::endl;
            return nullptr;
    }

}

void sceneParser::processLight(const Node &node)
{
    switch(sceneTags.at(getType(node)))
    {
        case POINT: {
            Color c = getColor(node);
            float intensity = (getFloat(node, "intensity", &intensity) ? intensity : 0.0f);
            TransformPtr lightTransform = processTransform(node);
            PointLightPtr pp = std::make_shared<PointLight>(lightTransform, 1);
            pp->setIntensity(intensity);
            pp->setColor(c);
            this->scene->lights.push_back(LightPtr(pp));
            break;
        }
        case AMBIENT: {
            Color c = getColor(node);
            float intensity = (getFloat(node, "intensity", &intensity) ? intensity : 0.0f);
            TransformPtr lightTransform = processTransform(node);
            AmbientLightPtr ap = std::make_shared<AmbientLight>(lightTransform, 1);
            ap->setIntensity(intensity);
            ap->setColor(c);
            this->scene->ambientLight = ap;
            break;
        }
        default:
            std::cout << getType(node) << " not understood." << std::endl;
            return;
    }
}

nikita::Color sceneParser::getColor(const Node &node)
{
    Node colorNode;
    if (node.name() == "color")
        colorNode = node;
    else if (hasColor(node))
        colorNode = node.child("color");
    else
        return Color::black();

    Attribute c = colorNode.attribute("value");
    return getColor(c);

}

nikita::Color sceneParser::getColor(const Attribute &c)
{
    std::vector<std::string> colorTokens = StringUtils::tokenize(c.value(), ",");

    if(colorTokens.size() == 3)
    {
        float r = std::stof(colorTokens[0]);
        float g = std::stof(colorTokens[1]);
        float b = std::stof(colorTokens[2]);
        return Color(r,g,b);
    }
    else{
        std::cout << "Color information couldn't be read. Falling back to Black." << std::endl;
        return Color::black();
    }
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
    return hasChildByName(parentNode, "transform");
}

bool sceneParser::hasMaterial(const Node &parent)
{
    return hasChildByName(parent, "material");
}

bool sceneParser::hasColor(const Node &parent)
{
    return hasChildByName(parent, "color");
}

bool sceneParser::hasChildByName(const Node &parent, const std::string &name)
{
    Node n = parent.child(name.c_str());
    if (n)
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
    ShapePtr spherePtr = std::make_shared<Sphere>(pt, ptInv, radius, zmin, zmax, phi);
    MaterialPtr materialPtr = processMaterial(node);

    this->scene->objects.push_back(std::make_shared<GeoPrim>(spherePtr, materialPtr));
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
    ShapePtr trianglePtr = std::make_shared<TriangleMesh>(pt, ptInv, numTriangles, trianglePos.size(), trianglePos, faces);
    MaterialPtr materialPtr = processMaterial(node);
    this->scene->objects.push_back(std::make_shared<GeoPrim>(trianglePtr, materialPtr));
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