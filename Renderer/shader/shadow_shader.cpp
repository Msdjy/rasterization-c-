#pragma once
#include "./shader.h"
#include "../core/pipeline.h"




// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 �ü�        ���Ĳ�ֵ��������=> Ƭ�εķ�������uv��zֵ
//                                              ����ռ�����꣬��Դ���꣬�������
void ShadowShader::vertex_shader() {
	for (int i = 0; i < attribute.vertexs.size(); i++) {
		gl.positions[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i], 1.0f);
		varying.normals[i] = to_vec3(uniform.normal_mat * vec4(attribute.normals[i], 1.0f));
	}
}


vec4 ShadowShader::fragment_shader() {
	vec4 color(0, 0, 0, 1.0);
	color = pack(gl.FragCoord.z());

	return color;
}