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

	// 法线矩阵
	// only model matrix can change normal vector in world space ( Normal Matrix: tranverse(inverse(model)) )
	
	for (int i = 0; i < attribute.vertexs.size(); i++) {
		gl.positions[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		// view空间坐标应该没用到
		//payload_shader.view_vertexs[j] = payload_shader.view_mat * vertexs[j];
		varying.normals[i] = uniform.normal_mat * vec4(attribute.normals[i]);

		varying.positions_from_light[i] = vec3(uniform.light_vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]));
		varying.fragposes[i] = vec3(uniform.model_mat * vec4(attribute.vertexs[i]));
	}
}

float unpack(vec4 rgbaDepth) {
	const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
	return dot(rgbaDepth, bitShift);
}

float get_depth(vec3 shadowCoord, unsigned char* depthbuffer) {
	float i = (WINDOW_WIDTH) * (shadowCoord[0]);
	float j = (WINDOW_HEIGHT) * (shadowCoord[1]);
	float i_floor = i - floor(i);
	float j_floor = j - floor(j);
	int x1, x2, y1, y2, xw, yw;
	if (i_floor > 0.5) {
		x1 = (int)(i);
		x2 = x1 + 1;
		xw = i_floor - 0.5;
	}
	else {
		x1 = (int)(i - 1);
		x2 = x1 + 1;
		xw = i_floor + 0.5;
	}

	if (j_floor > 0.5) {
		y1 = (int)(j);
		y2 = y1 + 1;
		yw = j_floor - 0.5;
	}
	else {
		y1 = (int)(j - 1);
		y2 = y1 + 1;
		yw = j_floor + 0.5;
	}
	float depth11 = unpack(get_color(depthbuffer, x1, y1));
	float depth12 = unpack(get_color(depthbuffer, x1, y2));
	float depth21 = unpack(get_color(depthbuffer, x2, y1));
	float depth22 = unpack(get_color(depthbuffer, x2, y2));

	//std::cout << shadowCoord<<" "<<i<<" "<<j << std::endl;
	float depth = ( depth11 * yw + depth12 * (1 - yw) ) * xw + (depth21 * yw + depth22 * (1 - yw)) * (1 - xw);
	return depth;
}

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // Back to NDC 
	//return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
	return (2.0 * -1 * -20) / (-20 + -1 - z * (-20 - -1));
}

static float bias;
float PCF(vec3 shadowCoord, unsigned char* depthbuffer) {

	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	//一个0.005的偏移就能帮到很大的忙，但是有些表面坡度很大，仍然会产生阴影失真。有一个更加可靠的办法能够根据表面朝向光线的角度更改偏移量：使用点乘：

	float visibility = 0.0;
	int cnt = 0;
	if (shadowCoord.x() > 1 || shadowCoord.x() < 0 || shadowCoord.y() > 1 || shadowCoord.y() < 0)return 1;
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			int x = (WINDOW_WIDTH - 1) * (shadowCoord[0]);
			int y = (WINDOW_HEIGHT - 1) * (shadowCoord[1]);
			if (x + i < 0 || x + i > WINDOW_WIDTH - 1 || y + j < 0 || y + j > WINDOW_HEIGHT - 1)continue;
			cnt++;
			float depth = unpack(get_color(depthbuffer, x + i, y + j));
			//float depth = get_depth(shadowCoord + vec3(i, j, 0), depthbuffer);
			if (depth < shadowCoord.z() - bias)visibility += 1.0f;
		}
	}

	return visibility / cnt;
}

float shadow_visibility(vec3 shadowCoord, unsigned char* depthbuffer) {
	return PCF(shadowCoord, depthbuffer);

	float depth;
	if (shadowCoord.x() > 1 || shadowCoord.x() < 0 || shadowCoord.y() > 1 || shadowCoord.y() < 0)depth = 0;
	else depth = get_depth(shadowCoord, depthbuffer);

	//float depth =  get_color(uniform.depthbuffer, shadowCoord.x()*(WINDOW_WIDTH - 1), shadowCoord.y()* (WINDOW_HEIGHT - 1)).z();

	//std::cout << depth<<" "<< shadowCoord.z() << std::endl;
	//return vec3(shadowCoord.z() * 10);
	//return vec3(depth * 10);
	//return vec3(shadowCoord.z() - depth > 0 ? 1 : 0);
	if (depth > shadowCoord.z() - bias)return 0;
	else return 1;
}

vec3 PhoneShader::fragment_shader() {
	//std::cout << "PhoneShader fragment_shader" << std::endl;
	//插值
		//顶点法向量
		//顶点vu值
		//纹理
	//光源
	//相机位姿
	//return vec3(gl.FragCoord.z());
	vec3 view_pos = uniform.camera->eye;
	vec3 view_dir = normalize(view_pos - varying.FragPos);
	//return varying.Position_From_Light;

	//vec3 diffuse = texture_sample(texcoords, &payload_shader.model->diffuse());
	vec3 diffuse = attribute.m.albedo;

	vec3 color(0,0,0);
	vec3 amblient(0.1,0.1,0.1);
	color += amblient;
	
	//light
	for (auto light : uniform.lights) {
		// 点光源的方向
		//vec3 light_dir = normalize(light->position - varying.position);
		// 方向光的方向
		vec3 light_dir = normalize(light->position);
		vec3 half_dir = normalize(light_dir + view_dir);
		
		bias = std::max(0.001 * (1.0 - dot(varying.Normal, light_dir)), 0.0001);
		vec4 mvp_from_light = varying.Position_From_Light;
		// 这两相等
		//std::cout << varying.Position_From_Light << " ads" << uniform.light_vp_mat * uniform.model_mat * varying.FragPos << "" << std::endl;
		vec3 shadowCoord = mvp_from_light.xyz() / mvp_from_light.w();
		shadowCoord = shadowCoord * 0.5 + 0.5;
		shadowCoord[2] = 1 - shadowCoord[2];

		float visibility = shadow_visibility(shadowCoord, uniform.depthbuffer);
		//visibility = 1;
		

		float distance = (light->position - varying.FragPos).norm();
		float attenuation = 1.0 / (light->constant + light->linear * distance + light->quadratic * (distance * distance));

		float diff = std::max(dot(light_dir, varying.Normal), 0.0);

		float spec = pow(std::max(dot(half_dir, varying.Normal), 0.0), 128.0f);
		//color += spec * diffuse * light->power * attenuation;
		color += (spec+ diff)* light->power * attenuation * visibility;

		//color += payload_shader.m.eval(light_dir, view_dir, normal) * light->power * attenuation;
	}

	//std::cout << color << std::endl;


	return color;
}