#pragma once
#include "./shader.h"
#include "../core/pipeline.h"


// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 �ü�        ���Ĳ�ֵ��������=> Ƭ�εķ�������uv��zֵ
//                                              ����ռ�����꣬��Դ���꣬�������
void PBRShader::vertex_shader() {
	// �������ԣ������������꣬uv
	//payload_shader.vertexs;
	//payload_shader.normals;
	//payload_shader.texcoords;

	// ���߾���
	// only model matrix can change normal vector in world space ( Normal Matrix: tranverse(inverse(model)) )

	for (int i = 0; i < attribute.vertexs.size(); i++) {
		gl.positions[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i], 1.0f);

		varying.normals[i] = to_vec3(uniform.normal_mat * vec4(attribute.normals[i], 1.0f));

		varying.positions_from_light[i] = uniform.light_vp_mat * uniform.model_mat * vec4(attribute.vertexs[i], 1.0f);

		varying.fragposes[i] = to_vec3(uniform.model_mat * vec4(attribute.vertexs[i], 1.0f));
	}
}


vec4 PBRShader::fragment_shader() {
	//��ֵ
		//���㷨����
		//����vuֵ
		//����
	//��Դ
	//���λ��
	//return gl.FragCoord.z();
	vec3 view_pos = uniform.camera->eye;
	vec3 view_dir = normalize(view_pos - varying.FragPos);

	//vec3 diffuse = texture_sample(texcoords, &payload_shader.model->diffuse());
	vec3 diffuse = attribute.m.albedo;

	vec3 color(0, 0, 0);
	//vec3 amblient(0.1, 0.1, 0.1);
	//color += amblient * diffuse;

	//light
	for (auto light : uniform.lights) {
		// ���Դ�ķ���
		//vec3 light_dir = normalize(light->position - varying.position);
		// �����ķ���
		vec3 light_dir = normalize(light.position);

		float bias = std::max(0.01 * (1.0 - dot(varying.Normal, light_dir)), 0.001);
		vec4 mvp_from_light = varying.Position_From_Light;

		vec3 shadowCoord = mvp_from_light.xyz() / mvp_from_light.w();
		shadowCoord = shadowCoord * 0.5 + 0.5;
		shadowCoord[2] = 1 - shadowCoord[2];

		float visibility = shadow_visibility(shadowCoord, uniform.depthbuffer, bias);

		float distance = (light.position - varying.FragPos).norm();
		float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

		color += attribute.m.eval(-view_dir, light_dir, varying.Normal) * light.power * attenuation * visibility;
	}

	color[0] = pow(color[0] / (color[0] + 1.0f), 1.0f / 2.2);
	color[1] = pow(color[1] / (color[1] + 1.0f), 1.0f / 2.2);
	color[2] = pow(color[2] / (color[2] + 1.0f), 1.0f / 2.2);
	return vec4(color, 1.0f);
}