#pragma once
#include "./shader.h"
#include "../core/pipeline.h"



// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 裁剪        重心插值（修正）=> 片段的法向量，uv，z值
//                                              世界空间的坐标，光源坐标，相机坐标
void PhoneShader::vertex_shader() {
	//std::cout << "PhoneShader vertex_shader" << std::endl;
	// 顶点属性，法向量，坐标，uv
	//payload_shader.vertexs;
	//payload_shader.normals;
	//payload_shader.texcoords;

	for (int i = 0; i < attribute.vertexs.size(); i++) {
		varying.mvp_vertexs[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		// view空间坐标应该没用到
		//payload_shader.view_vertexs[j] = payload_shader.view_mat * vertexs[j];
	}
	// 法线矩阵
	// only model matrix can change normal vector in world space ( Normal Matrix: tranverse(inverse(model)) )
	varying.normal_mat = uniform.model_mat.inverse_transpose();
}

float unpack(vec4 rgbaDepth) {
	const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
	return dot(rgbaDepth, bitShift);
}

vec3 PhoneShader::fragment_shader() {
	//std::cout << "PhoneShader fragment_shader" << std::endl;
	//插值
		//顶点法向量
		//顶点vu值
		//纹理
	//光源
	//相机位姿
	
	

	vec3 view_pos = uniform.camera->eye;
	vec3 view_dir = normalize(view_pos - varying.position);
	

	//vec3 diffuse = texture_sample(texcoords, &payload_shader.model->diffuse());
	vec3 diffuse = attribute.m.albedo;
	//std::cout << texcoords << std::endl;

	vec3 color(0,0,0);
	vec3 amblient(0.1,0.1,0.1);
	color += amblient;

	//light
	for (auto light : uniform.lights) {
		float visibility;

		vec3 shadowCoord = varying.mvp_vertex_from_light.xyz() / varying.mvp_vertex_from_light.w();
		shadowCoord = shadowCoord * 0.5 + 0.5;
		int i = (WINDOW_WIDTH - 1) * (shadowCoord[0] );
		int j = (WINDOW_HEIGHT - 1) * (shadowCoord[1]);
		if (shadowCoord.x() > 1 || shadowCoord.x() < 0 || shadowCoord.y() > 1 || shadowCoord.y() < 0)continue;
		//std::cout << shadowCoord<<" "<<i<<" "<<j << std::endl;
		float depth = unpack(get_color(uniform.depthbuffer, i, j));
		//std::cout << depth<<" "<< shadowCoord.z() << std::endl;
		if (depth > shadowCoord.z())continue;

		vec3 light_dir = normalize(light->position - varying.position);
		vec3 half_dir = normalize(light_dir + view_dir);
		//if (depth < (light->position - payload_shader.position).norm())continue;
		float distance = (light->position - varying.position).norm();
		float attenuation = 1.0 / (light->constant + light->linear * distance + light->quadratic * (distance * distance));

		float diff = std::max(dot(light_dir, varying.normal), 0.0);
		color += diff  * light->power * attenuation;

		float spec = pow(std::max(dot(half_dir, varying.normal), 0.0), 128.0f);
		//color += spec * diffuse * light->power * attenuation;
		color += spec  * light->power * attenuation;

		//color += payload_shader.m.eval(light_dir, view_dir, normal) * light->power * attenuation;
	}

	//std::cout << color << std::endl;


	return color;
}