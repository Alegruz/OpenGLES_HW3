#version 300 es

uniform mat4 worldMat, viewMat, projMat;
uniform vec3 eyePos, lightPosL, lightPosR;
uniform float lightAttL[3], lightAttR[3];

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

out vec2 v_texCoord;
out vec3 v_view, v_lightL, v_lightR;
out float v_attL, v_attR;
out vec3 v_diffL, v_diffR;
out vec3 v_reflL, v_reflR;

//uniform mat4 u_mvp;
//uniform mat4 u_lightMVP;
//uniform mat4 u_world;
//
//uniform vec3 u_lightPos;
//
//out vec4 v_shadowCoord;
//out float v_diffuse;

void main()
{
//    v_shadowCoord = u_lightMVP * vec4(position, 1.0f);
//    gl_Position = u_mvp * vec4(position, 1.0f);
//    //gl_Position = u_lightMVP * a_position;
//
//    //light
//    vec4 worldPos = u_world * vec4(position, 1.0f);
//    vec3 lightDir = normalize(worldPos.xyz - u_lightPos.xyz);
//    vec3 worldNormal = normalize(mat3(u_world) * normal);
//    v_diffuse = dot(-lightDir, worldNormal);

// -----------------------------------------------------------

//     world-space position
    vec4 worldPos = worldMat * vec4( position, 1.0 );

    // light vectors
    v_lightL = normalize( worldPos.xyz - lightPosL );
    v_lightR = normalize( worldPos.xyz - lightPosR );

    // view vector
    v_view = normalize( worldPos.xyz - eyePos );

    // world-space vertex normal
    vec3 worldNormal = normalize( mat3(transpose(inverse(worldMat))) * normal );

    v_diffL = vec3( dot( -v_lightL, worldNormal ) );
    v_diffR = vec3( dot( -v_lightR, worldNormal ) );
//    v_reflL = reflect(v_lightL, worldNormal);
//    v_reflR = reflect(v_lightR, worldNormal);
    v_reflL = 2.0 * worldNormal * dot( worldNormal, v_lightL ) - v_lightL;
    v_reflR = 2.0 * worldNormal * dot( worldNormal, v_lightR ) - v_lightR;

    // attenuations
    float distL = length(lightPosL - worldPos.xyz);
    float distR = length(lightPosR - worldPos.xyz);
    v_attL = (1.0 / (lightAttL[0] + lightAttL[1] * distL + lightAttL[2] * distL * distL));
    v_attR =  (1.0 / (lightAttR[0] + lightAttR[1] * distR + lightAttR[2] * distR * distR));

    // texture coordinates
    v_texCoord = texCoord;

    // clip-space position
    gl_Position = projMat * viewMat * worldPos;
}