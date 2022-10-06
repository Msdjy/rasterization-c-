#pragma once
#include "./shader.h"
#include "../core/pipeline.h"



// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 裁剪        重心插值（修正）=> 片段的法向量，uv，z值
//                                              世界空间的坐标，光源坐标，相机坐标
void LightShader::vertex_shader() {
	//std::cout << "PhoneShader vertex_shader" << std::endl;
	// 顶点属性，法向量，坐标，uv
	//payload_shader.vertexs;
	//payload_shader.normals;
	//payload_shader.texcoords;

	// 法线矩阵
	// only model matrix can change normal vector in world space ( Normal Matrix: tranverse(inverse(model)) )

	for (int i = 0; i < attribute.vertexs.size(); i++) {
		gl.positions[i] = uniform.vp_mat * uniform.model_mat * vec4(attribute.vertexs[i], 1.0f);
	}
}


vec4 LightShader::fragment_shader() {
	vec4 color(1,1,1,1);

	return color;
}