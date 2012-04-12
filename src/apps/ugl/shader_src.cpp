#include "shader_src.h"
#include <assert.h>

const char *untyped_shadowmap_vs = 
"vec3 get_point(vec4 wp)\n"
"{\n"
"return vec3(wp.xyz);\n"
"}\n"
"vec3 transform_point(mat4 m[2], vec3 p)\n"
"{\n"
"return (m[0] * vec4(p.xyz, 1.0)).xyz;\n"
"}\n"
"vec4 project(mat4 m[2], vec3 p)\n"
"{\n"
"return m[0] * vec4(p.xyz, 1.0);\n"
"}\n"
"attribute vec3 local_point;\n"
"attribute vec3 local_normal;\n"
"uniform mat4 mXf[2];\n"
"uniform mat4 lXf[2];\n"
"uniform mat4 lpXf[2];\n"
"void main()\n"
"{\n"
"vec3 s_pos = local_point;\n"
"vec3 w_pos = transform_point(mXf,s_pos);\n"
"gl_Position = project(lpXf,transform_point(lXf,w_pos));\n"
"}\n";


const char *untyped_shadowmap_fs = 
"void main ()\n"
"{\n"
"gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);\n"
"}\n";

const char *untyped_image_vs = 
"vec3 get_point(vec4 wp)\n"
"{\n"
"return vec3(wp.xyz);\n"
"}\n"
"vec3 transform_point(mat4 m[2], vec3 p)\n"
"{\n"
"return (m[0] * vec4(p.xyz, 1.0)).xyz;\n"
"}\n"
"vec3 transform_direction(mat4 m[2], vec3 p)\n"
"{\n"
"return (m[0] * vec4(p.xyz, 0.0)).xyz;\n"
"}\n"
"vec4 project(mat4 m[2], vec3 p)\n"
"{\n"
"return m[0] * vec4(p.xyz, 1.0);\n"
"}\n"
"attribute vec3 local_point;\n"
"attribute vec3 local_normal;\n"
"varying vec3 norm;\n"
"varying vec3 position;\n"
"varying vec4 shadow_coord0;\n"
"varying vec4 shadow_coord1;\n"
"varying vec4 shadow_coord2;\n"
"varying vec4 shadow_coord3;\n"
"varying vec4 shadow_coord4;\n"
"varying vec4 shadow_coord5;\n"
"uniform mat4 mXf[2];\n"
"uniform mat4 vXf[2];\n"
"uniform mat4 pXf[2];\n"
"uniform mat4 lXf0[2];\n"
"uniform mat4 lXf1[2];\n"
"uniform mat4 lXf2[2];\n"
"uniform mat4 lXf3[2];\n"
"uniform mat4 lXf4[2];\n"
"uniform mat4 lXf5[2];\n"
"uniform mat4 lpXf[2];\n"
"void main()\n"
"{\n"
"vec3 s_pos = local_point;\n"
"vec3 w_pos = transform_point(mXf, s_pos);\n"
"vec3 v_pos = transform_point(vXf, w_pos);\n"
"norm = transform_direction(mXf, local_normal);\n"
"position = w_pos;\n"
"shadow_coord0 = project(lpXf, transform_point(lXf0, w_pos));\n"
"shadow_coord1 = project(lpXf, transform_point(lXf1, w_pos));\n"
"shadow_coord2 = project(lpXf, transform_point(lXf2, w_pos));\n"
"shadow_coord3 = project(lpXf, transform_point(lXf3, w_pos));\n"
"shadow_coord4 = project(lpXf, transform_point(lXf4, w_pos));\n"
"shadow_coord5 = project(lpXf, transform_point(lXf5, w_pos));\n"
"gl_Position = project(pXf, v_pos);\n"
"}\n";

const char *untyped_pointlight_fs = 
"vec3 homogenize(vec4 hp)\n"
"{\n"
"return vec3(hp.xyz) / hp.w;\n"
"}\n"
"float vector_lengthSqr(vec3 v)\n"
"{\n"
"return dot(v,v);\n"
"}\n"
"float poscos(vec3 n, vec3 w)\n"
"{\n"
"return max(dot(n,w),0.0);\n"
"}\n"
"vec4 attach_alpha(vec3 c,float a)\n"
"{\n"
"return vec4(c.xyz, a);\n"
"}\n"
"varying vec3 norm;\n"
"varying vec3 position;\n"
"varying vec4 shadow_coord0;\n"
"varying vec4 shadow_coord1;\n"
"varying vec4 shadow_coord2;\n"
"varying vec4 shadow_coord3;\n"
"varying vec4 shadow_coord4;\n"
"varying vec4 shadow_coord5;\n"
"uniform vec3 light_pos;\n"
"uniform vec3 eye_pos;\n"
"uniform vec3 intensity;\n"
"uniform vec3 rhod;\n"
"uniform vec3 rhos;\n"
"uniform float n;\n"
"uniform vec3 em;\n"
"uniform float invsamples;\n"
"uniform sampler2D shadow_map0;\n"
"uniform sampler2D shadow_map1;\n"
"uniform sampler2D shadow_map2;\n"
"uniform sampler2D shadow_map3;\n"
"uniform sampler2D shadow_map4;\n"
"uniform sampler2D shadow_map5;\n"
"float shadow_test(sampler2D shadow_map, vec4 shadow_coord, float old_shadow)\n"
"{\n"
"float shadow = old_shadow;\n"
"vec3 scoord = homogenize(shadow_coord) - vec3(-1.0,-1.0,-1.0);\n"
"if(scoord.x < 0.0)\n"
"return shadow;\n"
"if(scoord.x > 2.0)\n"
"return shadow;\n"
"if(scoord.y < 0.0)\n"
"return shadow;\n"
"if(scoord.y > 2.0)\n"
"return shadow;\n"
"if(shadow_coord.w > 0.0)\n"
"{\n"
"float epsilon = (0.01);\n"
"if(texture2D(shadow_map, vec2(scoord.x * 0.5, scoord.y * 0.5)).x < (scoord.z * 0.5) - epsilon)\n"
"shadow = float(0.0) ;\n"
"}\n"
"return shadow;\n"
"}\n"
"void main()\n"
"{\n"
"vec3 lv = light_pos - position;\n"
"vec3 L = normalize(lv);\n"
"vec3 N = norm;\n"
"vec3 V = normalize(eye_pos-position);\n"
"vec3 R = reflect(-L, N);\n"
"float ss_pdf = float(1.0);\n"
"float ls_pdf = float(1.0);\n"
"float shadow = 1.0;\n"
"shadow = shadow_test(shadow_map0, shadow_coord0, shadow);\n"
"shadow = shadow_test(shadow_map1, shadow_coord1, shadow);\n"
"shadow = shadow_test(shadow_map2, shadow_coord2, shadow);\n"
"shadow = shadow_test(shadow_map3, shadow_coord3, shadow);\n"
"shadow = shadow_test(shadow_map4, shadow_coord4, shadow);\n"
"shadow = shadow_test(shadow_map5, shadow_coord5, shadow);\n"
"vec3 le = intensity / (vector_lengthSqr(lv) * float(1.0));\n"
"float hemisphericalProjectedAngle = float(3.1415926);\n"
"vec3 spec;\n"
"if(n<=0.0)\n"
"{\n"
"spec = vec3(0.0, 0.0, 0.0);\n"
"}\n"
"else\n"
"{\n"
"spec = rhos * ( (n+2.0) * pow(max(dot(V,R),0.0),n) / 2.0);\n"
"}\n"
"vec3 brdf = (rhod + spec) / hemisphericalProjectedAngle;\n"
"vec3 color = em + le * brdf * poscos(N, L) / (ss_pdf * ls_pdf);\n"
"gl_FragColor = attach_alpha(color * shadow * invsamples, 1.0);\n"
"}\n";

const char *untyped_arealight_fs = 
"vec3 homogenize(vec4 hp)\n"
"{\n"
"return vec3(hp.xyz) / hp.w;\n"
"}\n"
"float cos(vec3 n, vec3 w)\n"
"{\n"
"return dot(n,w);\n"
"}\n"
"vec4 attach_alpha(vec3 c,float a)\n"
"{\n"
"return vec4(c.xyz, a);\n"
"}\n"
"float vector_lengthSqr(vec3 v)\n"
"{\n"
"return dot(v,v);\n"
"}\n"
"float poscos(vec3 n, vec3 w)\n"
"{\n"
"return max(dot(n,w),0.0);\n"
"}\n"
"varying vec3 norm;\n"
"varying vec3 position;\n"
"varying vec4 shadow_coord0;\n"
"varying vec4 shadow_coord1;\n"
"varying vec4 shadow_coord2;\n"
"varying vec4 shadow_coord3;\n"
"varying vec4 shadow_coord4;\n"
"varying vec4 shadow_coord5;\n"
"uniform vec3 light_pos;\n"
"uniform vec3 eye_pos;\n"
"uniform vec3 intensity;\n"
"uniform vec3 rhod;\n"
"uniform vec3 rhos;\n"
"uniform float n;\n"
"uniform vec3 em;\n"
"uniform vec3 le;\n"
"uniform vec3 ln;\n"
"uniform float sa;\n"
"uniform float invsamples;\n"
"uniform sampler2D shadow_map0;\n"
"uniform sampler2D shadow_map1;\n"
"uniform sampler2D shadow_map2;\n"
"uniform sampler2D shadow_map3;\n"
"uniform sampler2D shadow_map4;\n"
"uniform sampler2D shadow_map5;\n"
"float shadow_test(sampler2D shadow_map,vec4 shadow_coord,float old_shadow)\n"
"{\n"
"float shadow=old_shadow;\n"
"vec3 scoord=homogenize(shadow_coord)-vec3(-1.0,-1.0,-1.0);\n"
"if(scoord.x < 0.0)\n"
"return shadow;\n"
"if(scoord.x > 2.0)\n"
"return shadow;\n"
"if(scoord.y < 0.0)\n"
"return shadow;\n"
"if(scoord.y > 2.0)\n"
"return shadow;\n"
"if(shadow_coord.w>0.0)\n"
"{\n"
"float epsilon=float(0.01);\n"
"if(texture2D(shadow_map,vec2(scoord.x*0.5,scoord.y*0.5)).x<(scoord.z*0.5-epsilon))\n"
"shadow=float(0.0);\n"
"}return shadow;\n"
"}\n"
"void main()\n"
"{\n"
"vec3 lv = light_pos - position;\n"
"vec3 L = normalize(lv);\n"
"vec3 N = norm;\n"
"vec3 V = normalize(eye_pos - position);\n"
"vec3 R = reflect(-L, N);\n"
"if(cos(ln,-L) <= 0.0)\n"
"gl_FragColor = attach_alpha(em * invsamples, 1.0);\n"
"else{\n"
"float ss_pdf = float(vector_lengthSqr(lv) / (sa * cos(ln, -L)));\n"
"float shadow=1.0;\n"
"shadow = shadow_test(shadow_map0, shadow_coord0, shadow);\n"
"shadow = shadow_test(shadow_map1, shadow_coord1, shadow);\n"
"shadow = shadow_test(shadow_map2, shadow_coord2, shadow);\n"
"shadow = shadow_test(shadow_map3, shadow_coord3, shadow);\n"
"shadow = shadow_test(shadow_map4, shadow_coord4, shadow);\n"
"shadow = shadow_test(shadow_map5, shadow_coord5, shadow);\n"
"float hemisphericalProjectedAngle = float(3.141592654);\n"
"vec3 spec;\n"
"if(n<=0.0)\n"
"{\n"
"spec = vec3(0.0, 0.0, 0.0);\n"
"}\n"
"else\n"
"{\n"
"spec = rhos * ((n+2.0) * pow(max(dot(V, R), 0.0), n)/2.0);\n"
"}\n"
"vec3 brdf = (rhod + spec) / hemisphericalProjectedAngle;\n"
"vec3 color = em + le * brdf * poscos(N, L)/ (ss_pdf);\n"
"gl_FragColor = attach_alpha(color* shadow* invsamples, 1.0);\n"
"}\n"
"}\n";


const char *get_shader(shader_id id)
{
    switch(id)
    {
    case SHADOW_MAP_VS_SOURCE:
        return untyped_shadowmap_vs;
    case SHADOW_MAP_FS_SOURCE:
        return untyped_shadowmap_fs;
    case IMAGE_VS_SOURCE:
        return untyped_image_vs;
    case POINTLIGHT_FS_SOURCE:
        return untyped_pointlight_fs;
    case AREALIGHT_FS_SOURCE:
        return untyped_arealight_fs;
    default:
        assert(false);
    }
    return 0;
}