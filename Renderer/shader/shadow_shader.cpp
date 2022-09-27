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

vec4 fract(vec4 vector4) {
	vec4 tmp;
	tmp[0] = vector4[0] - floor(vector4[0]);
	tmp[1] = vector4[1] - floor(vector4[1]);
	tmp[2] = vector4[2] - floor(vector4[2]);
	tmp[3] = vector4[3] - floor(vector4[3]);

	return tmp;
}

vec4 pack(float depth) {
	// pack�����ǰ�һ��32λ��[0,1)֮��ĸ������ֳ��ķ�
	  // ʹ��rgba 4�ֽڹ�32λ���洢zֵ,1���ֽھ���Ϊ1/256
	const vec4 bitShift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
	const vec4 bitMask = vec4(1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0);
	// gl_FragCoord:ƬԪ������,fract():������ֵ��С������
	vec4 rgbaDepth = fract(depth * bitShift); //����ÿ�����zֵ
	rgbaDepth = rgbaDepth - vec4(rgbaDepth[1], rgbaDepth[2], rgbaDepth[3], rgbaDepth[3]) * bitMask; // Cut off the value which do not fit in 8 bits
	return rgbaDepth;
}

vec3 ShadowShader::fragment_shader() {
	//std::cout << "ShadowShader vertex_shader" << std::endl;

	vec3 color(0, 0, 0);
	color = pack(gl.FragCoordz);
	//color = vec3();

	return color;
}