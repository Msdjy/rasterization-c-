#pragma once
#include "./shader.h"
#include "../core/pipeline.h"




// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 裁剪        重心插值（修正）=> 片段的法向量，uv，z值
//                                              世界空间的坐标，光源坐标，相机坐标
void ShadowShader::vertex_shader() {
	// 顶点属性，法向量，坐标，uv
	//std::cout << "ShadowShader vertex_shader" << std::endl;

	for (int i = 0; i < attribute.vertexs.size(); i++) {
		varying.mvp_vertexs[i] = uniform.light_vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		// view空间坐标应该没用到
		//payload_shader.view_vertexs[j] = payload_shader.view_mat * vertexs[j];
	}
	// 法线矩阵
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
	// pack函数是把一个32位的[0,1)之间的浮点数分成四份
	  // 使用rgba 4字节共32位来存储z值,1个字节精度为1/256
	const vec4 bitShift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
	const vec4 bitMask = vec4(1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0);
	// gl_FragCoord:片元的坐标,fract():返回数值的小数部分
	vec4 rgbaDepth = fract(depth * bitShift); //计算每个点的z值
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