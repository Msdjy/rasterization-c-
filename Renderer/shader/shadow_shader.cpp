#pragma once
#include "./shader.h"
#include "../core/pipeline.h"




// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 �ü�        ���Ĳ�ֵ��������=> Ƭ�εķ�������uv��zֵ
//                                              ����ռ�����꣬��Դ���꣬�������
void ShadowShader::vertex_shader() {
	for (int i = 0; i < attribute.vertexs.size(); i++) {
		gl.positions[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		varying.normals[i] = uniform.normal_mat * vec4(attribute.normals[i]);

	}
}



vec3 ShadowShader::fragment_shader() {
	//std::cout << "ShadowShader vertex_shader" << std::endl;
	
	//vec4 mvp_vertex_from_light = uniform.light_vp_mat * uniform.model_mat * varying.position;
	//vec3 shadowCoord = mvp_vertex_from_light.xyz() / mvp_vertex_from_light.w();
	//shadowCoord = shadowCoord * 0.5 + 0.5;
	//gl.FragCoord = shadowCoord;
	vec3 color(0, 0, 0);
	//return vec3(gl.FragCoord.z());
	color = pack(gl.FragCoord.z());
	//color = vec3(gl.FragCoord.z() / 2);
	//color = vec3();
	//color = varying.Normal;
	return color;
}