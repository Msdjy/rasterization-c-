#pragma once
#include"./maths.h"

enum MaterialType { DIFFUSE, MICROFACET, REFR, SPEC };

class Material {
public:
	MaterialType t;
	// �������ֵ��ior�õ�
	// kd��ks��������;��淴���ɷ������õ���
	// ��TODO ����ϵ����ô���ǣ�ֱ�ӵ���kd��
	// TODO �����ĸ�ֵ
	// https://zhuanlan.zhihu.com/p/21961722
	// ����û��������
	// �ǽ�����ֵ��Ϊ������0.04, 0.04, 0.04��
	float roughness;
	float metallic;
	float ior;
	vec3 albedo;

	// phoneģ������Ĳ���
	//float f4[4] = { 2,0,0,0 };
	//float specular_exponent = 0;
	Material() {};
	Material(float ior, float roughness, float metallic, vec3 albedo, MaterialType t) :ior(ior), roughness(roughness), metallic(metallic), albedo(albedo), t(t) {
	};
	vec3 eval(const vec3& in_dir, const vec3& out_dir, const vec3& normal);
	vec3 sample(const vec3& in_dir, const vec3& norma);
	float pdf(const vec3& in_dir, const vec3& out_dir, const vec3& normal);
	void ImporttanceSampleGGX(const vec3& normal, const vec3& in_dir, vec3& out_dir, float& pdf);
};
