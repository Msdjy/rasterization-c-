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

	//
	mat4 normal_mat;

	//depthbuffer
	unsigned char* depthbuffer;


	// light
	std::vector<Light*> lights;

	// camera
	Camera* camera;

};
struct payload_varying {

	// vertex_shader - output
	


	// world space point attributes
	// fragment_shader - input
	vec3 FragPos;
	vec3 Position_From_Light;
	vec3 Normal;
	//vec2 texcoords;


	std::vector<vec3> fragposes;
	std::vector<vec3> positions_from_light;
	std::vector<vec3> normals;
	//std::vector<vec3> texcoordss;

	//vec4 mvp_vertex_from_light;
};

struct payload_gl {
	// out
	std::vector<vec4> positions;

	vec4 Position;

	// in
	vec3 FragCoord;
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
	payload_gl gl;

	bool is_shadow_shader = false;

	virtual void vertex_shader() {
		std::cout << "IShader vertex_shader" << std::endl;
	};
	virtual vec3 fragment_shader() { 
		std::cout << "IShader fragment_shader" << std::endl; 
		return vec3(0, 0, 0); 
	};
	vec4 fract(vec4 vector4) {
		vec4 tmp;
		tmp[0] = vector4[0] - floor(vector4[0]);
		tmp[1] = vector4[1] - floor(vector4[1]);
		tmp[2] = vector4[2] - floor(vector4[2]);
		tmp[3] = vector4[3] - floor(vector4[3]);

		return tmp;
	}

	vec4 pack(float depth) {
		// pack函数是把一个32位的[0,1)之间的浮点数分成四份
		  // 使用rgba 4字节共32位来存储z值,1个字节精度为1/256
		const vec4 bitShift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
		const vec4 bitMask = vec4(1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0);
		// gl_FragCoord:片元的坐标,fract():返回数值的小数部分
		vec4 rgbaDepth = fract(depth * bitShift); //计算每个点的z值
		rgbaDepth = rgbaDepth - vec4(rgbaDepth[1], rgbaDepth[2], rgbaDepth[3], rgbaDepth[3]) * bitMask; // Cut off the value which do not fit in 8 bits
		return rgbaDepth;
	}
};

class PhoneShader : public IShader{
	bool is_shadow_shader = false;
	void vertex_shader();
	vec3 fragment_shader();
};


class ShadowShader : public IShader {
	bool is_shadow_shader = true;
	void vertex_shader();
	vec3 fragment_shader();
};

class PBRShader : public IShader {
	void vertex_shader();
	vec3 fragment_shader();
};