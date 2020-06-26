#version 300 es

precision mediump float;

uniform sampler2D textureDiff;

uniform vec3 matSpec, matAmbi, matEmit;
uniform float matSh;
uniform vec3 srcDiffL, srcSpecL, srcAmbiL;
uniform vec3 srcDiffR, srcSpecR, srcAmbiR;

//in vec3 v_normal;
in vec2 v_texCoord;
in vec3 v_view, v_lightL, v_lightR;
in float v_attL, v_attR;
in vec3 v_diffL, v_diffR;
in vec3 v_reflL, v_reflR;

layout(location = 0) out vec4 fragColor;

//uniform sampler2D s_shadow;
//uniform vec4 u_lightColor;
//
//in vec4 v_shadowCoord;
//in float v_diffuse;

void main()
{
//    vec3 rgb = vec3(v_diffuse, v_diffuse, v_diffuse);
//    rgb = clamp(rgb, 0.0, 1.0) * u_lightColor.xyz;
//
//    vec4 shadowCoord = v_shadowCoord / v_shadowCoord.w;
//    shadowCoord = shadowCoord * 0.5 + 0.5;
//    float currentDepth = shadowCoord.z;
//
//    vec2 uv = shadowCoord.xy;
//    //uv.y = -uv.y;
//    //uv = uv * 0.5 + 0.5;
//
//    // Used to lower moiré pattern and self-shadowing
//    currentDepth += 0.0005;
//
//    float shadowDepth = texture(s_shadow, uv).z;
//
//    float shadow = 1.0;
//    // .w로 나눠진 값을 사용할경우 (-)/(-)가 발생할수 있어서 원본을 그냥 쓴다
//    if (v_shadowCoord.w > 0.0) {
//        shadow = shadowDepth < currentDepth ? 0.5 : 1.0;
//    }
//
////    gl_FragColor = vec4(rgb * shadow, 1.0);
//    fragColor = vec4(rgb * shadow, 1.0);
//    //gl_FragColor = vec4(v, v, v, 1.0);

// ----------------------------------------------------------------------------

    // re-normalize unit vectors (normal, view, and light vectors)
    vec3 view = normalize(v_view);
    vec3 lightL = normalize(v_lightL);
    vec3 lightR = normalize(v_lightR);
    vec3 reflL = normalize(v_reflL);
    vec3 reflR = normalize(v_reflR);

    // diffuse term
    vec4 albedo = texture(textureDiff, v_texCoord);
//    vec3 diffL = ceil(5.0f * (max(v_diffL, 0.0) * srcDiffL)) / 5.0f;
//    vec3 diffR = ceil(5.0f * (max(v_diffR, 0.0) * srcDiffR)) / 5.0f;
    vec3 diffL = max(v_diffL, 0.0) * srcDiffL;
    vec3 diffR = max(v_diffR, 0.0) * srcDiffR;
//    vec3 diff = albedo.rgb * (ceil(5.0f * (diffL + diffR)) / 5.0f);
    vec3 diff = albedo.rgb * (diffL + diffR);

    // specular term
    vec3 specL = pow(max(dot(reflL, -view), 0.0), matSh) * srcSpecL;
    vec3 specR = pow(max(dot(reflR, -view), 0.0), matSh) * srcSpecR;
//    vec3 spec = albedo.xyz * ceil(5.0f * (specL + specR) * matSpec) / 5.0f;
    vec3 spec = albedo.rgb * (specL + specR) * matSpec;

    // ambient term
    vec3 ambiL = srcAmbiL;
    vec3 ambiR = srcAmbiR;
    vec3 ambi = albedo.rgb * (ambiL + ambiR) * matAmbi;

    float alpha = 1.0f;

    // final output color with alpha
    fragColor = vec4(ceil(5.0f * (diff + spec + ambi)) / 5.0f, alpha);
//    fragColor = vec4(albedo.rgb, alpha);
}