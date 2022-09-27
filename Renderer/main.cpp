#pragma once
//#include "./core/macro.h"
#include "./core/pipeline.h"

#include "./core/camera.h"
//#include "./core/model.h"
#include "./core/tgaimage.h"
//#include "./core/object.h"
#include "./platform/win32.h"
#include "./shader/shader.h"
#include<vector>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

//#include"renderer.hpp"
//#include"camera.hpp"
//#include"light.hpp"
//#include"object.hpp"
//#include"scene.hpp"
//#include"sphere.hpp"
//#include"material.hpp"

enum RenderType { RASTERIZER, RAYTRACING, PATHTRACING};



void clear_zbuffer(int width, int height, float* zbuffer);
void clear_framebuffer(int width, int height, unsigned char* framebuffer);
void update_matrix(Camera& camera, mat4& view_mat, mat4& perspective_mat, IShader* shader);

//constexpr Material      ivory = { 1.0, {0.9,  0.5, 0.1, 0.0}, {0.4, 0.4, 0.3},   50. };
//constexpr Material      glass = { 1.5, {0.0,  0.9, 0.1, 0.8}, {0.6, 0.7, 0.8},  125. };
//constexpr Material red_rubber = { 1.0, {1.4,  0.3, 0.0, 0.0}, {0.3, 0.1, 0.1},   10. };
//constexpr Material     mirror = { 1.0, {0.0, 16.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425. };
//Material      ivory = { 1.0, {0.9,  0.5, 0.1, 0.0}, {0.4, 0.4, 0.3},   50. };
//Material      glass = { 1.5, {0.0,  0.9, 0.1, 0.8}, {0.6, 0.7, 0.8},  125. };
//Material red_rubber = { 1.0, {1.4,  0.3, 0.0, 0.0}, {0.3, 0.1, 0.1},   10. };
//Material     mirror = { 1.0, {0.0, 16.0, 0.8, 0.0}, {1.0, 1.0, 1.0}, 1425. };

int main() 
{
    // initialization
    // --------------
    // malloc memory for zbuffer and framebuffer
    int width = WINDOW_WIDTH, height = WINDOW_HEIGHT;

    float* zbuffer = (float*)malloc(sizeof(float) * width * height);

    unsigned char* framebuffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
    memset(framebuffer, 0, sizeof(unsigned char) * width * height * 4);
	
	unsigned char* depthbuffer = (unsigned char*)malloc(sizeof(unsigned char) * width * height * 4);
    memset(depthbuffer, 0, sizeof(unsigned char) * width * height * 4);

	// TODO
	// 很耗内存
	//int n = -1;
	//unsigned char* pixmap = stbi_load("./image/envmap.jpg", &envmap_width, &envmap_height, &n, 0);
	//if (!pixmap || 3 != n) {
	//	std::cerr << "Error: can not load the environment map" << std::endl;
	//	return -1;
	//}
	//envmap = std::vector<vec3>(envmap_width * envmap_height);
	//for (int j = envmap_height - 1; j >= 0; j--) {
	//	for (int i = 0; i < envmap_width; i++) {
	//		envmap[i + j * envmap_width] = vec3(pixmap[(i + j * envmap_width) * 3 + 0], pixmap[(i + j * envmap_width) * 3 + 1], pixmap[(i + j * envmap_width) * 3 + 2]) * (1 / 255.);
	//	}
	//}
	//stbi_image_free(pixmap);

    // initialize window
    window_init(width, height, "Render");



	// model
	//Model* model = new Model("./objects/african_head/african_head.obj");

	// Material(float ior, float roughness, float metallic, vec3 albedo, MaterialType t = DIFFUSE)

	Material      m_cube = { 2, 0.1, 1, vec3(0.8f,0.6f,0.5f),	MICROFACET };
	Material      m_floot = { 2, 0.5, 0, vec3(0.3f,0.3f,0.3f),	MICROFACET };

	vec3 vertexs[12] = {
		// cube
		vec3(1, 1, 1), vec3(-1, 1, 1), vec3(1, -1, 1), vec3(-1, -1, 1),
		vec3(-1, 1, -1), vec3(1, 1, -1), vec3(-1, -1, -1), vec3(1, -1, -1),
		// 平台
		vec3(5, -1, -5),vec3(-5, -1, -5),vec3(5, -1, 3), vec3(-5, -1, 3)
	};

	Triangle cube[] = {
		Triangle(vertexs[0], vertexs[1], vertexs[2], &m_cube),
		Triangle(vertexs[2], vertexs[1], vertexs[3], &m_cube),
													 
		Triangle(vertexs[5], vertexs[0], vertexs[7], &m_cube),
		Triangle(vertexs[7], vertexs[0], vertexs[2], &m_cube),
													 
		Triangle(vertexs[1], vertexs[4], vertexs[3], &m_cube),
		Triangle(vertexs[3], vertexs[4], vertexs[6], &m_cube),
													 
		Triangle(vertexs[5], vertexs[4], vertexs[0], &m_cube),
		Triangle(vertexs[0], vertexs[4], vertexs[1], &m_cube),
													 
		Triangle(vertexs[4], vertexs[5], vertexs[6], &m_cube),
		Triangle(vertexs[6], vertexs[5], vertexs[7], &m_cube),
													 
		Triangle(vertexs[2], vertexs[3], vertexs[7], &m_cube),
		Triangle(vertexs[7], vertexs[3], vertexs[6], &m_cube),
	};
	Triangle floor[] = {
		Triangle(vertexs[8], vertexs[9], vertexs[10], &m_floot),
		Triangle(vertexs[10], vertexs[9], vertexs[11], &m_floot),
		//Triangle(vertexs[11], vertexs[9], vertexs[10], &m_floot),
	};

	Light* light1 = new Light;
	light1->position = vec3(-7, 5, 5);
	light1->power = vec3(2, 2, 2);
	

	// create camera
	vec3 eye(0, 0, 10);
	vec3 target(0, 0, 0);
	//target = eye + target;
	vec3 up(0, 1, 0);
	float fov = 60;
	float aspect = (float)(width) / height;
	Camera camera(eye, target, up, aspect, fov);
	float zNear = -0.1;
	float zFar = -50;

	// mvp
	mat4 model_mat;
	mat4 view_mat;
	mat4 perspective_mat;

	model_mat		 = mat4::identity();
	view_mat = mat4_lookat(camera.eye, camera.target, camera.up);
	perspective_mat = mat4_perspective(fov, aspect, zNear, zFar);

	// znear大于zfar，top大于bottom
	float top = tan(fov / 2.0f * PI / 180.0f) * abs(zNear);
	float bottom = -top;
	float right = top * aspect;
	float left = -right;
	mat4 ortho_mat = mat4_ortho(left, right, bottom, top, zNear, zFar);
	perspective_mat = ortho_mat * perspective_mat;

	// light mvp
	mat4 light_view_mat;
	mat4 light_perspective_mat;

	light_view_mat = mat4_lookat(light1->position, camera.target, camera.up);
	light_perspective_mat = mat4_perspective(fov, aspect, zNear, zFar);

	light_perspective_mat = ortho_mat * light_perspective_mat;


	// shader payload
	//IShader* shader = new PBRShader();
	IShader* shader = new PhoneShader();
	IShader* shadow_shader = new ShadowShader();

	for (auto& triangle : cube) {
		for (auto vertex : triangle.vertexs) {
			(shader->attribute.vertexs).push_back(vertex);
			(shader->attribute.normals).push_back(triangle.normal);
		}
	}
	for (auto& triangle : floor) {
		for (auto vertex : triangle.vertexs) {
			(shader->attribute.vertexs).push_back(vertex);
			(shader->attribute.normals).push_back(triangle.normal);
		}
			
	}


	shader->uniform.lights.push_back(light1);
	shader->uniform.camera = &camera;

	shader->uniform.light_view_mat = light_view_mat;
	shader->uniform.light_perspective_mat = light_perspective_mat;
	shader->uniform.light_vp_mat = light_perspective_mat * light_view_mat;

	shader->uniform.model_mat = model_mat;
	shader->uniform.view_mat = view_mat;
	shader->uniform.perspective_mat = perspective_mat;
	shader->uniform.vp_mat = perspective_mat * view_mat;

	for (auto& triangle : cube) {
		for (auto vertex : triangle.vertexs) {
			(shadow_shader->attribute.vertexs).push_back(vertex);
			(shadow_shader->attribute.normals).push_back(triangle.normal);
		}
	}
	for (auto& triangle : floor) {
		for (auto vertex : triangle.vertexs) {
			(shadow_shader->attribute.vertexs).push_back(vertex);
			(shadow_shader->attribute.normals).push_back(triangle.normal);
		}

	}


	shadow_shader->uniform.lights.push_back(light1);
	shadow_shader->uniform.camera = &camera;

	shadow_shader->uniform.light_view_mat = light_view_mat;
	shadow_shader->uniform.light_perspective_mat = light_perspective_mat;
	shadow_shader->uniform.light_vp_mat = light_perspective_mat * light_view_mat;

	shadow_shader->uniform.model_mat = model_mat;
	shadow_shader->uniform.view_mat = view_mat;
	shadow_shader->uniform.perspective_mat = perspective_mat;
	shadow_shader->uniform.vp_mat = perspective_mat * view_mat;




	// render loop
	// -----------
	int num_frames = 0;
	float print_time = platform_get_time();


	while (!window->is_close)
	{
		float curr_time = platform_get_time();

		// clear buffer
		clear_framebuffer(width, height, depthbuffer);
		clear_zbuffer(width, height, zbuffer);

		model_draw(depthbuffer, zbuffer, shadow_shader);
		

		// clear buffer
		clear_framebuffer(width, height, framebuffer);
		clear_zbuffer(width, height, zbuffer);

		// handle events and update view, perspective matrix
		handle_events(camera);
		update_matrix(camera, view_mat, perspective_mat, shader);

		shader->uniform.depthbuffer = depthbuffer;
		model_draw(framebuffer, zbuffer, shader);


		// calculate and display FPS
		num_frames += 1;
		if (curr_time - print_time >= 1) {
			int sum_millis = (int)((curr_time - print_time) * 1000);
			int avg_millis = sum_millis / num_frames;
			printf("fps: %3d, avg: %3d ms\n", num_frames, avg_millis);
			num_frames = 0;
			print_time = curr_time;
		}

		// reset mouse information
		window->mouse_info.wheel_delta = 0;
		window->mouse_info.orbit_delta = vec2(0, 0);
		window->mouse_info.fv_delta = vec2(0, 0);

		// send framebuffer to window 
		//window_draw(depthbuffer);
		window_draw(framebuffer);
		msg_dispatch();
	}

	free(depthbuffer);
	free(zbuffer);
	free(framebuffer);
	window_destroy();

	system("pause");
	return 0;

}

void clear_zbuffer(int width, int height, float* zbuffer)
{
	for (int i = 0; i < width * height; i++)
		zbuffer[i] = 100000;
}

void clear_framebuffer(int width, int height, unsigned char* framebuffer)
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			int index = (i * width + j) * 4;

			framebuffer[index + 2] = 80;
			framebuffer[index + 1] = 56;
			framebuffer[index] = 56;
		}
	}
}


void update_matrix(Camera& camera, mat4& view_mat, mat4& perspective_mat, IShader* shader) {
	//TODO
	view_mat = mat4_lookat(camera.eye, camera.target, camera.up);
	mat4 vp = perspective_mat * view_mat;

	shader->uniform.view_mat = view_mat;
	shader->uniform.vp_mat = vp;
}