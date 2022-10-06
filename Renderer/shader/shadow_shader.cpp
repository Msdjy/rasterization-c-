#pragma once
#include "./shader.h"
#include "../core/pipeline.h"




// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 裁剪        重心插值（修正）=> 片段的法向量，uv，z值
//                                              世界空间的坐标，光源坐标，相机坐标
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