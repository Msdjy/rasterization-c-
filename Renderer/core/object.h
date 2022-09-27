#pragma once
#include"./camera.h"
#include"./maths.h"
#include"./material.h"

#include<iostream>


//struct Intersection {
//	Material* material = nullptr;
//	vec3 pos;
//	vec3 normal;
//	float distance;
//	bool is_intersect = false;
//	vec3 emission;
//};



class Object {
public:
	Object( Material* m, vec3 emission = 0): emission(emission), material(m) {};

	//virtual float getArea()const = 0;
	//virtual void sample(Intersection& inter, float& pdf)const = 0;
	//virtual Intersection intersect(const vec3& eye, const vec3& dir)const = 0;

	Material* material = nullptr;
	vec3 emission;
	vec3 get_emmission()const;
	bool has_emmission()const;
};

class Sphere : public Object{
public:
	//Sphere() {};
	Sphere(float r, vec3 c, Material* m) :Object(m), radius(r), center(c){};

	//float getArea()const;
	//void sample(Intersection& inter, float& pdf)const;
	//Intersection intersect(const vec3& eye, const vec3& dir)const;

	vec3 center;
	float radius;
};

class Triangle : public Object {
public:
	Triangle(vec3 v0, vec3 v1, vec3 v2, Material* m) :Object(m), vertexs{ v0, v1, v2 } {
		E1 = v1 - v0;
		E2 = v2 - v0;
		normal = normalize(cross(E1, E2));
		area = cross(E1, E2).norm() * 0.5f;
	};

	//float getArea()const;
	//void sample(Intersection& inter, float& pdf)const;
	//Intersection intersect(const vec3& eye, const vec3& dir)const;

	vec3 vertexs[3];
	vec3 E1, E2;
	vec3 normal;
	float area;
};
