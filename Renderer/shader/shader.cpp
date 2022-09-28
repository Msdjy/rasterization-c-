
#include "./shader.h"
#include "../core/pipeline.h"

vec4 IShader::fract(vec4 vector4) {
	vec4 tmp;
	tmp[0] = vector4[0] - floor(vector4[0]);
	tmp[1] = vector4[1] - floor(vector4[1]);
	tmp[2] = vector4[2] - floor(vector4[2]);
	tmp[3] = vector4[3] - floor(vector4[3]);

	return tmp;
}

vec4 IShader::pack(float depth) {
	// pack函数是把一个32位的[0,1)之间的浮点数分成四份
	  // 使用rgba 4字节共32位来存储z值,1个字节精度为1/256
	const vec4 bitShift = vec4(1.0, 256.0, 256.0 * 256.0, 256.0 * 256.0 * 256.0);
	const vec4 bitMask = vec4(1.0 / 256.0, 1.0 / 256.0, 1.0 / 256.0, 0.0);
	// gl_FragCoord:片元的坐标,fract():返回数值的小数部分
	vec4 rgbaDepth = fract(depth * bitShift); //计算每个点的z值
	rgbaDepth = rgbaDepth - vec4(rgbaDepth[1], rgbaDepth[2], rgbaDepth[3], rgbaDepth[3]) * bitMask; // Cut off the value which do not fit in 8 bits
	return rgbaDepth;
}



float IShader::unpack(vec4 rgbaDepth) {
	const vec4 bitShift = vec4(1.0, 1.0 / 256.0, 1.0 / (256.0 * 256.0), 1.0 / (256.0 * 256.0 * 256.0));
	return dot(rgbaDepth, bitShift);
}

float IShader::get_depth(vec3 shadowCoord, unsigned char* depthbuffer) {
	float i = (WINDOW_WIDTH) * (shadowCoord[0]);
	float j = (WINDOW_HEIGHT) * (shadowCoord[1]);
	float i_floor = i - floor(i);
	float j_floor = j - floor(j);
	int x1, x2, y1, y2, xw, yw;
	if (i_floor > 0.5) {
		x1 = (int)(i);
		x2 = x1 + 1;
		xw = i_floor - 0.5;
	}
	else {
		x1 = (int)(i - 1);
		x2 = x1 + 1;
		xw = i_floor + 0.5;
	}

	if (j_floor > 0.5) {
		y1 = (int)(j);
		y2 = y1 + 1;
		yw = j_floor - 0.5;
	}
	else {
		y1 = (int)(j - 1);
		y2 = y1 + 1;
		yw = j_floor + 0.5;
	}
	float depth11 = unpack(get_color(depthbuffer, x1, y1));
	float depth12 = unpack(get_color(depthbuffer, x1, y2));
	float depth21 = unpack(get_color(depthbuffer, x2, y1));
	float depth22 = unpack(get_color(depthbuffer, x2, y2));

	//std::cout << shadowCoord<<" "<<i<<" "<<j << std::endl;
	float depth = (depth11 * yw + depth12 * (1 - yw)) * xw + (depth21 * yw + depth22 * (1 - yw)) * (1 - xw);
	return depth;
}

float IShader::LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // Back to NDC 
	//return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));
	return (2.0 * -1 * -20) / (-20 + -1 - z * (-20 - -1));
}


float IShader::PCF(vec3 shadowCoord, unsigned char* depthbuffer, float bias) {

	//float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	//一个0.005的偏移就能帮到很大的忙，但是有些表面坡度很大，仍然会产生阴影失真。有一个更加可靠的办法能够根据表面朝向光线的角度更改偏移量：使用点乘：

	float visibility = 0.0;
	int cnt = 0;
	if (shadowCoord.x() > 1 || shadowCoord.x() < 0 || shadowCoord.y() > 1 || shadowCoord.y() < 0)return 1;
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			int x = (WINDOW_WIDTH - 1) * (shadowCoord[0]);
			int y = (WINDOW_HEIGHT - 1) * (shadowCoord[1]);
			if (x + i < 0 || x + i > WINDOW_WIDTH - 1 || y + j < 0 || y + j > WINDOW_HEIGHT - 1)continue;
			cnt++;
			float depth = unpack(get_color(depthbuffer, x + i, y + j));
			//float depth = get_depth(shadowCoord + vec3(i, j, 0), depthbuffer);
			if (depth > shadowCoord.z() - 2 * bias)visibility += 1.0f;
		}
	}

	return visibility / cnt;
}

float IShader::shadow_visibility(vec3 shadowCoord, unsigned char* depthbuffer, float bias) {
	return PCF(shadowCoord, depthbuffer, bias);

	float depth;
	if (shadowCoord.x() > 1 || shadowCoord.x() < 0 || shadowCoord.y() > 1 || shadowCoord.y() < 0)depth = 0;
	else depth = get_depth(shadowCoord, depthbuffer);

	//float depth =  get_color(uniform.depthbuffer, shadowCoord.x()*(WINDOW_WIDTH - 1), shadowCoord.y()* (WINDOW_HEIGHT - 1)).z();

	//std::cout << depth<<" "<< shadowCoord.z() << std::endl;
	//return vec3(shadowCoord.z() * 10);
	//return vec3(depth * 10);
	//return vec3(shadowCoord.z() - depth > 0 ? 1 : 0);
	if (depth > shadowCoord.z() - bias)return 0;
	else return 1;
}