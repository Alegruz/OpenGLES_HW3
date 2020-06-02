#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/player.h"

#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Texture* Scene::diffuse = nullptr;
Material* Scene::material = nullptr;
Object* Scene::lineDraw = nullptr;
Texture* Scene::lineColor = nullptr;
Material* Scene::lineMaterial = nullptr;

void Scene::setup(AAssetManager* aAssetManager) {
    Asset::setManager(aAssetManager);

    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 0.0f, 80.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);

    //player
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));


    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
    Scene::lineDraw->worldMat = scale(vec3(1.0f / 3.0f));
}

void Scene::screen(int width, int height) {
    Scene::camera->aspect = (float) width/height;
}

void Scene::update(float deltaTime) {
    Scene::program->use();

    Scene::camera->update();

    const float playTime = 5.0;
    static float time = 0.0f;
    time += deltaTime;
    if (time >= playTime) time -= playTime;

    int prevFrame = (int)floor(time / playTime * motions.size());
    int nextFrame = (int)ceil(time / playTime * motions.size());
    if(nextFrame >= motions.size()) nextFrame = 0;
    float ratio = time / playTime * motions.size() - (float)prevFrame;

    // from bone space to world space
    vector<mat4x4> bone2world = {mat4x4(1.0f)};
    // from world space to bone space
    vector<mat4x4> world2bone = {mat4x4(1.0f)};

    for(int jointIdx = 1; jointIdx < jNames.size(); jointIdx ++) {
        //Motion interpolation using an Euler angle or quaternion.
        // Please use prevFrame, nextFrame, ratio, R, bone2world, and world2bone.
        //float x = ;
        //float y = ;
        //float z = ;

        //mat4x4 R = ;

        // With motion
        //bone2world;
        //world2bone;
    }

    //set new vertex
    vector<Vertex> newVertex;
    //for(int vertexIdx = 0; vertexIdx <= playerVertices.size(); vertexIdx++){
    //    vec3 result;
    //    for(int boneIter = 0; boneIter < 4; boneIter ++) {

    //    }
    //}
    //Scene::player->load(newVertex, playerIndices);
    Scene::player->draw();

    //If you want to see the skeleton as a line..
    //    glLineWidth(20);
    //    Scene::lineDraw->load(?, ?);
    //    Scene::lineDraw->draw();
}