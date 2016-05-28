//
// Created by Rafael Campos on 3/22/16.
//

#ifndef NIKITA_RENDER_SCENEPARSER_H
#define NIKITA_RENDER_SCENEPARSER_H

#include "../core/nikita.h"
#include "../core/scene.h"
#include "../core/shapes/sphere.h"
#include "../core/shapes/triangleMesh.h"
#include "../core/sensor.h"
#include "../core/camera.h"
#include "../core/primitive.h"

#include "stringUtils.h"
#include "smfReader.h"

#include <OpenImageIO/pugixml.hpp>

using Node = OIIO::pugi::xml_node;
using Attribute = OIIO::pugi::xml_attribute;

namespace nikita {

    class sceneParser {
    public:
        sceneParser();
        void loadFile(std::string filePath);

        ScenePtr getScene();
        CameraPtr getCamera();

    private:
        bool useAccelerator;
        int acceleratorSize;
        ScenePtr scene;
        CameraPtr camera;
        SensorPtr film;
        SamplerPtr sampler;
        std::vector<GeoPrimPtr> objects;
        void processShape(const OIIO::pugi::xml_node &node);
        void processSensor(const OIIO::pugi::xml_node &node);
        MaterialPtr processMaterial(const OIIO::pugi::xml_node &node);
        void processLight(const Node &node);
        void processAccelerator(const Node &node);
        TransformPtr processTransform(const Node &node);
        Transform processRotate(const Node &node);
        Transform processTranslate(const Node &node);
        Transform processScale(const Node &node);
        Transform processLookAt(const Node &node);
        bool useAcceleration(const Node &scene);
        bool hasTransform(const Node& parentNode);
        bool hasMaterial(const Node& parentNode);
        bool hasColor(const Node& parentNode);
        void createSphere(const OIIO::pugi::xml_node &node);
        void createTriangleMesh(const OIIO::pugi::xml_node &node);
        bool getInteger(const Node& node, const std::string &property, int *result);
        bool getFloat(const Node& node, const std::string &property, float *result);
        bool getString(const Node& node, const std::string &property, std::string *result);
        Color getColor(const Node &node);
        Color getColor(const Attribute &attrib);
        const OIIO::pugi::char_t* getType(const Node &node);
        bool hasChildByName(const Node &parent, const std::string &name);


        enum SceneTag {
            NIKITA, SCENE, SHAPE, SPHERE, SMF, OBJ, SENSOR, MATERIAL, SAMPLER, FILM, INTEGER, FLOAT, BOOLEAN, TRANSLATE, ROTATE, LOOKAT,
            SCALE, POINT, VECTOR, TRANSFORM, MATTE, PHONG, REFLECTIVE, LIGHT, AMBIENT, ACCELERATOR, DEFAULT,
        };

        struct SceneTagMap : public std::map<std::string, SceneTag>
        {
            SceneTagMap()
            {
                this->operator[]("nikita") = NIKITA;
                this->operator[]("scene") = SCENE;
                this->operator[]("shape") = SHAPE;
                this->operator[]("sphere") = SPHERE;
                this->operator[]("smf") = SMF;
                this->operator[]("obj") = OBJ;
                this->operator[]("sensor") = SENSOR;
                this->operator[]("material") = MATERIAL;
                this->operator[]("sampler") = SAMPLER;
                this->operator[]("film") = FILM;
                this->operator[]("integer") = INTEGER;
                this->operator[]("float") = FLOAT;
                this->operator[]("boolean") = BOOLEAN;
                this->operator[]("translate") = TRANSLATE;
                this->operator[]("rotate") = ROTATE;
                this->operator[]("lookat") = LOOKAT;
                this->operator[]("scale") = SCALE;
                this->operator[]("point") = POINT;
                this->operator[]("vector") = VECTOR;
                this->operator[]("transform") = TRANSFORM;
                this->operator[]("matte") = MATTE;
                this->operator[]("phong") = PHONG;
                this->operator[]("reflective") = REFLECTIVE;
                this->operator[]("light") = LIGHT;
                this->operator[]("ambient") = AMBIENT;
                this->operator[]("accelerator") = ACCELERATOR;
                this->operator[]("default") = DEFAULT;

            };
            ~SceneTagMap(){}
        };
        static SceneTagMap sceneTags;
        void setBackground(const Node &scene);
    };
}

#endif //NIKITA_RENDER_SCENEPARSER_H
