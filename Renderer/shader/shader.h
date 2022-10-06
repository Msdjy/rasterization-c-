#pragma once
#include "../core/maths.h"
#include "../core/camera.h"
#include "../core/tgaimage.h"
#include "../core/model.h"
#include"../core/object.h"
#include <vector>


//vec3 texture_sample(vec2 uv, const TGAImage* image)
//{
//	uv[0] = fmod(uv[0], 1);
//	uv[1] = fmod(uv[1], 1);
//	//printf("%f %f\n", uv[0], uv[1]);
//	int uv0 = uv[0] * image->width();
//	int uv1 = uv[1] * image->width();
//	TGAColor c = image->get(uv0, uv1);
//	vec3 res;
//	for (int i = 0; i < 3; i++)
//		res[2 - i] = (float)c[i] / 255.f;
//	return res;
//}


struct Light {
	vec3 position;
	vec3 power;

	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
};

struct payload_uniform {
	// shadow
	mat4 light_view_mat;
	mat4 light_perspective_mat;
	mat4 light_vp_mat;

	// mat
	mat4 model_mat;
	mat4 view_mat;
	mat4 perspective_mat;
	mat4 vp_mat;

	// normal_mat
	mat4 normal_mat;

	//depthbuffer
	unsigned char* depthbuffer;

	// light
	std::vector<Light> lights;

	// camera
	Camera* camera;
};
struct payload_varying {

	// vertex_shader - output
	std::vector<vec3> fragposes;
	std::vector<vec4> positions_from_light;
	std::vector<vec3> normals;
	//std::vector<vec3> texcoordss;

	// world space point attributes
	// fragment_shader - input
	vec3 FragPos;
	vec4 Position_From_Light;
	vec3 Normal;
	//vec2 texcoords;
};

struct payload_gl {
	// out
	std::vector<vec4> positions;

	vec4 Position;

	// in
	vec3 FragCoord;
};

struct payload_attribute {
	// 材质
	Material m;
	//model
	Model* model;

	std::vector<Triangle> triangles;
	std::vector<vec3> vertexs;
	std::vector<vec3> normals;
	std::vector<vec2> texcoords;
};


class IShader {
public:
	// 模仿opengl的内置参数
	payload_attribute attribute;
	payload_uniform uniform;
	payload_varying varying;
	payload_gl gl;

	bool is_shadow_shader;

	virtual void vertex_shader() {};
	virtual vec4 fragment_shader() { return vec4(); };

	vec4 fract(vec4 vector4);

	vec4 pack(float depth);

	float unpack(vec4 rgbaDepth);

	float get_depth(vec3 shadowCoord, unsigned char* depthbuffer);

	float LinearizeDepth(float depth);

	float PCF(vec3 shadowCoord, unsigned char* depthbuffer, float bias);

	float shadow_visibility(vec3 shadowCoord, unsigned char* depthbuffer, float bias);
};

class PhoneShader : public IShader{
	void vertex_shader();
	vec4 fragment_shader();

};
class ShadowShader : public IShader {
public:
	ShadowShader() {
		this->is_shadow_shader = true;
	}
	void vertex_shader();
	vec4 fragment_shader();
};

class PBRShader : public IShader {
	void vertex_shader();
	vec4 fragment_shader();
};
class LightShader : public IShader {
	void vertex_shader();
	vec4 fragment_shader();
};