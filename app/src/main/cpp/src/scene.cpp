#include "scene.h"
#include "binary/animation.h"
#include "binary/skeleton.h"
#include "binary/plain.h"
#include "binary/player.h"

#include <cmath>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

//Shader* Scene::shadowVertexShader = nullptr;
//Shader* Scene::shadowFragmentShader = nullptr;
//Program* Scene::shadowProgram = nullptr;
//GLuint Scene::fbo = 0;
//GLuint Scene::fbo_depth_tex = 0;
//int Scene::shadow_map_width = 1024;
//int Scene::shadow_map_height = 1024;

Shader* Scene::vertexShader = nullptr;
Shader* Scene::fragmentShader = nullptr;
Program* Scene::program = nullptr;
Camera* Scene::camera = nullptr;
Object* Scene::player = nullptr;
Object* Scene::plain = nullptr;
Texture *Scene::diffuse = nullptr;
Material *Scene::material = nullptr;
Object *Scene::lineDraw = nullptr;
Texture *Scene::lineColor = nullptr;
Material *Scene::lineMaterial = nullptr;
LeftLight* Scene::lightL = nullptr;
RightLight* Scene::lightR = nullptr;
vector<mat4x4> Scene::sBoneToWorld;
vector<mat4x4> Scene::sWorldToBone;

int Scene::width = 0;
int Scene::height = 0;

void Scene::setup(AAssetManager *aAssetManager) {
    Asset::setManager(aAssetManager);

//    Scene::shadowVertexShader = new Shader(GL_VERTEX_SHADER, "shadow_vertex.glsl");
//    Scene::shadowFragmentShader = new Shader(GL_FRAGMENT_SHADER, "shadow_fragment.glsl");
    Scene::vertexShader = new Shader(GL_VERTEX_SHADER, "vertex.glsl");
    Scene::fragmentShader = new Shader(GL_FRAGMENT_SHADER, "fragment.glsl");

//    Scene::shadowProgram = new Program(Scene::vertexShader, Scene::fragmentShader);
    Scene::program = new Program(Scene::vertexShader, Scene::fragmentShader);

//    initShadowFBO();
    Scene::camera = new Camera(Scene::program);
    Scene::camera->eye = vec3(0.0f, 40.0f, 50.0f);

    Scene::lightL = new LeftLight(program);
    Scene::lightR = new RightLight(program);
    lightL->position = vec3(-100.0f, 60.0f, 100.0f);
    lightR->position = vec3(150.0f, 60.0f, -150.0f);

    Scene::diffuse = new Texture(Scene::program, 0, "textureDiff", playerTexels, playerSize);
    Scene::material = new Material(Scene::program, diffuse);

    //player
    Scene::player = new Object(program, material, playerVertices, playerIndices);
    player->worldMat = scale(vec3(1.0f / 3.0f));

    Scene::plain = new Object(program, material, plainVertices, plainIndices);

    Scene::lineColor = new Texture(Scene::program, 0, "textureDiff", {{0xFF, 0x00, 0x00}}, 1);
    Scene::lineMaterial = new Material(Scene::program, lineColor);
    Scene::lineDraw = new Object(program, lineMaterial, {{}}, {{}}, GL_LINES);
    Scene::lineDraw->worldMat = scale(vec3(1.0f / 3.0f));

    sBoneToWorld.reserve(jNames.size());
    sWorldToBone.reserve(jNames.size());
    mat4x4 translation = translate(mat4x4(), vec3(jOffset[0][0]
            , jOffset[0][1]
            , jOffset[0][2])) * mat4x4(1.0f);
    sBoneToWorld.push_back(translation * mat4x4(1.0f));
    sWorldToBone.push_back(inverse(translation) * mat4x4(1.0f));

    // default pose
    for (int jointIdx = 1; jointIdx < jNames.size(); ++jointIdx) {
        mat4x4 R = translate(mat4x4(), vec3(jOffset[jointIdx][0], jOffset[jointIdx][1], jOffset[jointIdx][2]));

        sBoneToWorld.push_back(mat4x4(1.0f));
        sWorldToBone.push_back(inverse(R) * sWorldToBone[jParents[jointIdx]]);
    }
}
//
//bool Scene::initShadowFBO() {
//    int shadow_map_w = shadow_map_width;
//    int shadow_map_h = shadow_map_height;
//    glGenTextures(1, &fbo_depth_tex);
//    glBindTexture(GL_TEXTURE_2D, fbo_depth_tex);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, shadow_map_w, shadow_map_h, 0, GL_DEPTH_COMPONENT, GL_INT, NULL);
//    glBindTexture(GL_TEXTURE_2D, 0);
//    glGenFramebuffers(1, &fbo);
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fbo_depth_tex, 0);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//    return true;
//}
//
//void Scene::draw() {
//    LOG_PRINT_DEBUG("DRAW");
//    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
//    {
//        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
//
//        auto prog = shadowProgram;
//        glUseProgram(prog->get());
//
//        //glViewport(0, 0, width(), height());
//        glViewport(0, 0, shadow_map_width, shadow_map_height);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glEnable(GL_CULL_FACE);
//        glEnable(GL_DEPTH_TEST);
//        glCullFace(GL_FRONT);
//
//        glEnableVertexAttribArray(0);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
//                              sizeof(Vertex), (const void *) offsetof(Vertex, pos));
////        auto pos_loc = glGetUniformLocation(prog->get(), "a_position");
//        auto mvp_loc = glGetUniformLocation(prog->get(), "u_mvp");
////        glEnableVertexAttribArray(pos_loc);
//
//
//        float aspect = Scene::camera->aspect;
//        //glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);
//        glm::mat4 proj_mat = glm::perspective(45.0f, Scene::camera->aspect,10.0f,40000.0f);
//        glm::mat4 view_mat = glm::lookAt(Scene::lightL->position, vec3(0, 0, 0), glm::vec3(0,1,0));
//
//
//        glm::mat4 world_mat;
//        glm::mat4 mvp_mat;
//
//        mvp_mat = proj_mat * view_mat * world_mat;
//        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));
//        drawGround(prog);
//
//        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//    }
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//    if(program != nullptr) {
//        LOG_PRINT_DEBUG("PROGRAM IS NOT NULLPTR!!");
//        auto prog = program;
//        glUseProgram(prog->get());
//
//        glViewport(0, 0, Scene::width, Scene::height);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        glEnable(GL_CULL_FACE);
//        glEnable(GL_DEPTH_TEST);
//        glCullFace(GL_BACK);
//
//        glEnableVertexAttribArray(0);
//        glEnableVertexAttribArray(1);
//        glEnableVertexAttribArray(2);
//        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
//                              sizeof(Vertex), (const void *) offsetof(Vertex, pos));
//        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
//                              sizeof(Vertex), (const void *) offsetof(Vertex, nor));
//        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
//                              sizeof(Vertex), (const void *) offsetof(Vertex, tex));
////        auto pos_loc = glGetAttribLocation(prog->get(), "a_position");
////        auto normal_loc = glGetAttribLocation(prog->get(), "a_normal");
//        auto mvp_loc = glGetUniformLocation(prog->get(), "u_mvp");
//        auto world_loc = glGetUniformLocation(prog->get(), "u_world");
//        auto light_mvp_loc = glGetUniformLocation(prog->get(), "u_lightMVP");
//        auto shadow_loc = glGetUniformLocation(prog->get(), "s_shadow");
//
//        auto light_color_loc = glGetUniformLocation(prog->get(), "u_lightColor");
//        auto light_pos_loc = glGetUniformLocation(prog->get(), "u_lightPos");
//
////        glEnableVertexAttribArray(pos_loc);
////        glEnableVertexAttribArray(normal_loc);
//
//        float aspect = Scene::camera->aspect;
//        //glm::mat4 proj_mat = glm::perspective(60.0f, aspect, 0.1f, 100.0f);
//        glm::mat4 proj_mat = glm::perspective(45.0f, Scene::camera->aspect,10.0f,40000.0f);
//        glm::mat4 light_view_mat = glm::lookAt(Scene::lightL->position, vec3(0, 0, 0), glm::vec3(0,1,0));
//        glm::mat4 cam_view_mat = glm::lookAt(Scene::camera->eye, Scene::camera->at, glm::vec3(0,1,0));
//
//        glm::mat4 world_mat;
//        glm::mat4 light_mvp_mat;
//
//        glm::mat4 light_view_proj_mat = proj_mat * light_view_mat;
//
//        glm::mat4 cam_mvp_mat;
//
//        glBindTexture(GL_TEXTURE_2D, fbo_depth_tex);
//        glActiveTexture(GL_TEXTURE0);
//        glUniform1i(shadow_loc, 0);
//
//        glUniform3fv(light_pos_loc, 1, glm::value_ptr(Scene::lightL->position));
//
//        //glm::vec4 ground_color(0.3f,0.3f,0.3f,1);
//        glm::vec4 ground_color(0.6f,0.6f,0.6f,1);
//        glUniform4fv(light_color_loc, 1, glm::value_ptr(ground_color));
//
//        light_mvp_mat = light_view_proj_mat * world_mat;
//        cam_mvp_mat = proj_mat * cam_view_mat * world_mat;
//        glUniformMatrix4fv(mvp_loc, 1, GL_FALSE, glm::value_ptr(cam_mvp_mat));
//        glUniformMatrix4fv(light_mvp_loc, 1, GL_FALSE, glm::value_ptr(light_mvp_mat));
//        glUniformMatrix4fv(world_loc, 1, GL_FALSE, glm::value_ptr(world_mat));
//        drawGround(prog);
//    }
//}
//
//void Scene::drawGround(Program* program)
//{
//    LOG_PRINT_DEBUG("DRAWGROUND");
//    glEnableVertexAttribArray(0);
//    glEnableVertexAttribArray(1);
////    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
////                          sizeof(Vertex), (const void *) offsetof(Vertex, pos));
////    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
////                          sizeof(Vertex), (const void *) offsetof(Vertex, nor));
////    auto pos_loc = glGetAttribLocation(program->get(), "a_position");
////    auto normal_loc = glGetAttribLocation(program->get(), "a_normal");
//
//    // Ground
//    float vertex_data[] = {
//            -35, 2, -35,
//            -35, 2, 15,
//            15, 2, 15,
//            15, 2, -35
//    };
//    float normal_data[] = {
//            0, 1, 0,
//            0, 1, 0,
//            0, 1, 0,
//            0, 1, 0
//    };
//
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertex_data);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, normal_data);
//    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
//}
//
//void Scene::screen(int width, int height) {
//    Scene::width = width;
//    Scene::height = height;
//    Scene::camera->aspect = (float) width / height;
//}

void Scene::update(float deltaTime) {
    Scene::program->use();
    quat cameraRotation = quat(vec3(0, radians(0.0f), 0));
    Scene::camera->eye = vec3(toMat4(cameraRotation) * vec4(Scene::camera->eye, 1.0f));
    Scene::camera->update();
    Scene::lightL->update();
    Scene::lightR->update();

    const float playTime = 5.0;
    static float time = 0.0f;
    time += deltaTime;
    if (time >= playTime) time -= playTime;

    int prevFrame = (int) floor(time / playTime * motions.size());
    int nextFrame = (int) ceil(time / playTime * motions.size());
    if (nextFrame >= motions.size()) nextFrame = 0;
    float ratio = time / playTime * motions.size() - (float) prevFrame;


    static vector<mat4x4> animation;
    animation.reserve(jNames.size());
    quat rootQuaternionPrevRotationY = quat(vec3(0
            , radians(motions[prevFrame][4])
            , 0));
    quat rootQuaternionNextRotationY = quat(vec3(0
            , radians(motions[nextFrame][4])
            , 0));
    quat rootQuaternionPrevRotationX = quat(vec3(radians(motions[prevFrame][3])
            , 0
            , 0));
    quat rootQuaternionNextRotationX = quat(vec3(radians(motions[nextFrame][3])
            , 0
            , 0));
    quat rootQuaternionPrevRotationZ = quat(vec3(0
            , 0
            , radians(motions[prevFrame][5])));
    quat rootQuaternionNextRotationZ = quat(vec3(0
            , 0
            , radians(motions[nextFrame][5])));
    animation[0] = toMat4(mix(rootQuaternionPrevRotationZ, rootQuaternionNextRotationZ, ratio))
            * toMat4(mix(rootQuaternionPrevRotationX, rootQuaternionNextRotationX, ratio))
            * toMat4(mix(rootQuaternionPrevRotationY, rootQuaternionNextRotationY, ratio))
            * translate(mat4x4(), vec3(motions[prevFrame][0] * (1.0f - ratio) + motions[nextFrame][0] * ratio
            , motions[prevFrame][1] * (1.0f - ratio) + motions[nextFrame][1] * ratio
            , motions[prevFrame][2] * (1.0f - ratio) + motions[nextFrame][2] * ratio));
    sBoneToWorld[0] = animation[0] * sBoneToWorld[0];

    // animated pose
    for (int jointIdx = 1; jointIdx < jNames.size(); ++jointIdx) {
        // interpolate key data
        quat quaternionPrevRotationY = quat(vec3(0
                , radians(motions[prevFrame][6 + (jointIdx - 1) * 3 + 1])
                , 0));
        quat quaternionNextRotationY = quat(vec3(0
                , radians(motions[nextFrame][6 + (jointIdx - 1) * 3 + 1])
                , 0));
        quat quaternionPrevRotationX = quat(vec3(radians(motions[prevFrame][6 + (jointIdx - 1) * 3 + 0])
                , 0
                , 0));
        quat quaternionNextRotationX = quat(vec3(radians(motions[nextFrame][6 + (jointIdx - 1) * 3 + 0])
                , 0
                , 0));
        quat quaternionPrevRotationZ = quat(vec3(0
                , 0
                , radians(motions[prevFrame][6 + (jointIdx - 1) * 3 + 2])));
        quat quaternionNextRotationZ = quat(vec3(0
                , 0
                , radians(motions[nextFrame][6 + (jointIdx - 1) * 3 + 2])));

        mat4x4 R = translate(mat4x4(), vec3(jOffset[jointIdx][0]
                , jOffset[jointIdx][1]
                , jOffset[jointIdx][2]));

        // compute mia = mi-1a * mip * mil
        // sBoneToWorld[i] = sBoneToWorld[parent] * R * rotation
        animation[jointIdx] = R
                              * toMat4(mix(quaternionPrevRotationZ, quaternionNextRotationZ, ratio))
                              * toMat4(mix(quaternionPrevRotationX, quaternionNextRotationX, ratio))
                              * toMat4(mix(quaternionPrevRotationY, quaternionNextRotationY, ratio));
        sBoneToWorld[jointIdx] = sBoneToWorld[jParents[jointIdx]] * animation[jointIdx];
    }
    // invoke vertex shader for skinning


    //set new vertex
    vector<Vertex> newVertex;
    for(int vertexIdx = 0; vertexIdx < playerVertices.size(); ++vertexIdx){
        vec3 result = vec3(0.0f);
        vec3 resultNormal = vec3(0.0f);
        for(int boneIter = 0; boneIter < 4; ++boneIter) {
            int bone = playerVertices[vertexIdx].bone[boneIter];
            if (bone > -1) {
                result += vec3(playerVertices[vertexIdx].weight[boneIter]
                        * sBoneToWorld[bone]
                        * sWorldToBone[bone]
                        * vec4(playerVertices[vertexIdx].pos, 1));
                resultNormal += vec3(playerVertices[vertexIdx].weight[boneIter]
                        * transpose(inverse(sBoneToWorld[bone] * sWorldToBone[bone]))
                        * vec4(playerVertices[vertexIdx].nor, 1));
            }
        }
        newVertex.push_back({result, resultNormal, playerVertices[vertexIdx].tex});
//        newVertex.push_back({result, playerVertices[vertexIdx].nor, playerVertices[vertexIdx].tex});
    }
    Scene::player->load(newVertex, playerIndices);

//    draw();
    Scene::player->draw();

//    If you want to see the skeleton as a line..
//    vector<Vertex> skeletonPos = {{jOffset[0]}};
//    vector<Index> skeletonIndex;
//    for (int jointIdx = 1; jointIdx < jNames.size(); ++jointIdx)
//    {
//        int parentIdx = jParents[jointIdx];
//
//        skeletonPos.push_back({vec3(column(sBoneToWorld[jointIdx], 3))});
//
//        skeletonIndex.push_back(jointIdx);
//        skeletonIndex.push_back(parentIdx);
//    }
//    glLineWidth(20);
//
//    Scene::lineDraw->load(skeletonPos, skeletonIndex);
//    Scene::lineDraw->draw();
//
    Scene::plain->draw();
}