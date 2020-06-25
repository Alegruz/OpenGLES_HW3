#ifndef HW3_SCENE_H
#define HW3_SCENE_H

#include "global.h"
#include "program.h"
#include "camera.h"
#include "object.h"
#include "texture.h"
#include "material.h"

class Scene {
public:
    static void setup(AAssetManager* aAssetManager);
    static void screen(int width, int height);
    static void update(float deltaTime);
    static bool initShadowFBO();
    static void draw();
    static void drawGround(Program* program);

private:
    static Shader* shadowVertexShader;
    static Shader* shadowFragmentShader;
    static Program* shadowProgram;
    static Shader* vertexShader;
    static Shader* fragmentShader;
    static Program* program;
    static Camera* camera;
    static Texture* diffuse;
    static Material* material;
    static Object* player;
    static Object* plain;
    static LeftLight* lightL;
    static RightLight* lightR;
    static vector<mat4x4> sBoneToWorld;
    static vector<mat4x4> sWorldToBone;

    static Object* lineDraw;
    static Texture* lineColor;
    static Material* lineMaterial;

    static GLuint fbo;
    static GLuint fbo_depth_tex;
    static int shadow_map_width;
    static int shadow_map_height;

    static int width;
    static int height;

    static void lightMovement(float deltaTime);
};

#endif //HW3_SCENE_H
