#pragma once
#include "./shader.h"
#include "../core/pipeline.h"




// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 �ü�        ���Ĳ�ֵ��������=> Ƭ�εķ�������uv��zֵ
//                                              ����ռ�����꣬��Դ���꣬�������
void ShadowShader::vertex_shader() {
	// �������ԣ������������꣬uv
	//std::cout << "ShadowShader vertex_shader" << std::endl;

	for (int i = 0; i < attribute.vertexs.size(); i++) {
		varying.mvp_vertexs[i] = uniform.light_vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		// view�ռ�����Ӧ��û�õ�
		//payload_shader.view_vertexs[j] = payload_shader.view_mat * vertexs[j];
	}
	// ���߾���
	// only model matrix can change normal vector in world space ( Normal Matrix: tranverse(inverse(model)) )
	varying.normal_mat = uniform.model_mat.inverse_transpose();
}

vec3 ShadowShader::fragment_shader() {
	//std::cout << "ShadowShader vertex_shader" << std::endl;

	vec3 color(0, 0, 0);



	return color;
}