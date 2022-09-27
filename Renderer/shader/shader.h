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

	//depthbuffer
	unsigned char* depthbuffer;


	// light
	std::vector<Light*> lights;

	// camera
	Camera* camera;

};
struct payload_varying {

	// vertex_shader - output
	mat4 normal_mat;
	std::vector<vec4> mvp_vertexs;

	// world space point attributes
	// fragment_shader - input
	vec3 position;
	vec3 normal;
	vec2 texcoords;
};

struct payload_attribute {

	//
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
	payload_attribute attribute;
	payload_uniform uniform;
	payload_varying varying;


	virtual void vertex_shader() {
		std::cout << "IShader vertex_shader" << std::endl;
	};
	virtual vec3 fragment_shader() { 
		std::cout << "IShader fragment_shader" << std::endl; 
		return vec3(0, 0, 0); 
	};
};

class PhoneShader : public IShader{
	void vertex_shader();
	vec3 fragment_shader();
};


class ShadowShader : public IShader {
	void vertex_shader();
	vec3 fragment_shader();
};

class PBRShader : public IShader {
	void vertex_shader();
	vec3 fragment_shader();
};