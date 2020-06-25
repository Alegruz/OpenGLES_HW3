#ifndef HW3_CAMERA_H
#define HW3_CAMERA_H

#include "global.h"
#include "program.h"

class Camera {
public:
    vec3 eye, at, up;
    float fovy, aspect, zNear, zFar;

    Camera(Program* program);

    virtual void update() const;

private:
    Program* program;

    virtual void create(Program* program);
};

class Light {
public:
    vec3 position;
    float attenuation[3];
    vec3 diffuse, specular, ambient;

    Light(Program* program);
    virtual void update() const;

protected:
    Program* program;
    virtual void create(Program* program);
};

class LeftLight : public Light {
public:
    LeftLight(Program *program);
    virtual void update() const override;

protected:
    virtual void create(Program* program) override;
};

class RightLight : public Light {
public:
    RightLight(Program *program);
    virtual void update() const override;

protected:
    virtual void create(Program* program) override;
};

#endif //HW3_CAMERA_H
