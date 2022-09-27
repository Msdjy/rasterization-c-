#pragma once
#include "./shader.h"
#include "../core/pipeline.h"



// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 �ü�        ���Ĳ�ֵ��������=> Ƭ�εķ�������uv��zֵ
//                                              ����ռ�����꣬��Դ���꣬�������
void PhoneShader::vertex_shader() {
	//std::cout << "PhoneShader vertex_shader" << std::endl;
	// �������ԣ������������꣬uv
	//payload_shader.vertexs;
	//payload_shader.normals;
	//payload_shader.texcoords;

	for (int i = 0; i < attribute.vertexs.size(); i++) {
		varying.mvp_vertexs[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		// view�ռ�����Ӧ��û�õ�
		//payload_shader.view_vertexs[j] = payload_shader.view_mat * vertexs[j];
	}
	// ���߾���
	// only model matrix can change normal vector in world space ( Normal Matrix: tranverse(inverse(model)) )
	varying.normal_mat = uniform.model_mat.inverse_transpose();
}

vec3 PhoneShader::fragment_shader() {
	//std::cout << "PhoneShader fragment_shader" << std::endl;
	//��ֵ
		//���㷨����
		//����vuֵ
		//����
	//��Դ
	//���λ��
	


	vec3 view_pos = uniform.camera->eye;
	vec3 view_dir = normalize(view_pos - varying.position);
	

	//vec3 diffuse = texture_sample(texcoords, &payload_shader.model->diffuse());
	vec3 diffuse = attribute.m.albedo;
	//std::cout << texcoords << std::endl;

	vec3 color(0,0,0);
	//vec3 amblient(0.1,0.1,0.1);
	//color += amblient;

	//light
	for (auto light : uniform.lights) {
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