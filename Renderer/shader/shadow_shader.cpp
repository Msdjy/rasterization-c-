#pragma once
#include "./shader.h"
#include "../core/pipeline.h"




// world space -> NDC space -> screen space -> fagment shader
//            mvp              
//                 裁剪        重心插值（修正）=> 片段的法向量，uv，z值
//                                              世界空间的坐标，光源坐标，相机坐标
void ShadowShader::vertex_shader() {


	for (int i = 0; i < attribute.vertexs.size(); i++) {
		gl.mvp_vertexs[i] = uniform.light_vp_mat * uniform.model_mat * vec4(attribute.vertexs[i]);
		varying.normals[i] = uniform.normal_mat * vec4(attribute.normals[i]);
	}


}



vec3 ShadowShader::fragment_shader() {
	//std::cout << "ShadowShader vertex_shader" << std::endl;
	
	vec4 mvp_vertex_from_light = uniform.light_vp_mat * uniform.model_mat * varying.position;
	vec3 shadowCoord = mvp_vertex_from_light.xyz() / mvp_vertex_from_light.w();
	shadowCoord = shadowCoord * 0.5 + 0.5;
	gl.FragCoord = shadowCoord;
	vec3 color(0, 0, 0);
	color = pack(gl.FragCoord.z());
	//color = vec3(gl.FragCoordz * 5);
	//color = vec3();

	return color;
}