//
// Created by Rafael Campos on 3/22/16.
//

#ifndef NIKITA_RENDER_SCENEPARSER_H
#define NIKITA_RENDER_SCENEPARSER_H

#include "../core/nikita.h"
#include "../core/scene.h"
#include "../core/sphere.h"
#include "../core/triangleMesh.h"
#include "../core/sensor.h"
#include "../core/camera.h"

#include "stringUtils.h"
#include "smfReader.h"

#include <OpenImageIO/pugixml.hpp>

using Node = OIIO::pugi::xml_node;
using Attribute = OIIO::pugi::xml_attribute;

namespace nikita {

    class sceneParser {
    public:
        void loadFile(std::string filePath);

        ScenePtr getScene();
        CameraPtr getCamera();

    private:
        std::shared_ptr<Scene> scene;
        std::shared_ptr<Camera> camera;
        std::shared_ptr<Sensor> film;
        std::shared_ptr<Sampler> sampler;
        void processShape(const OIIO::pugi::xml_node &node);
        void processSensor(const OIIO::pugi::xml_node &node);
        TransformPtr processTransform(const Node &node);
        Transform processRotate(const Node &node);
        Transform processTranslate(const Node &node);
        Transform processScale(const Node &node);
        Transform processLookAt(const Node &node);
        bool hasTransform(const Node& parentNode);
        void createSphere(const OIIO::pugi::xml_node &node);
        void createTriangleMesh(const OIIO::pugi::xml_node &node);
        bool getInteger(const Node& node, const std::string &property, int *result);
        bool getFloat(const Node& node, const std::string &property, float *result);
        bool getString(const Node& node, const std::string &property, std::string *result);
        const OIIO::pugi::char_t* getType(const Node &node);


        enum SceneTag {
            NIKITA, SCENE, SHAPE, SPHERE, SMF, OBJ, SENSOR, SAMPLER, FILM, INTEGER, FLOAT, BOOLEAN, TRANSLATE, ROTATE, LOOKAT,
            SCALE, POINT, VECTOR, TRANSFORM, DEFAULT,
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
                this->operator[]("default") = DEFAULT;

            };
            ~SceneTagMap(){}
        };
        static SceneTagMap sceneTags;
    };
}

#endif //NIKITA_RENDER_SCENEPARSER_H
