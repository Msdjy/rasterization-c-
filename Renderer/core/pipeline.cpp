#pragma once
#include "./pipeline.h"

#include <thread>
#include <mutex>
#include <cmath>
std::mutex mutex_ins;


// framebuffer�����½�Ϊԭ��
// ��Ļ�ռ����������Ͻ�Ϊԭ��
// TODO staic����
static void set_color(unsigned char* framebuffer, int x, int y, unsigned char color[]) {
	int index = ((WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x) * 4;

	for (int i = 0; i < 4; i++) {
		framebuffer[index + i] = color[i];
	}
}

static void set_color(unsigned char* framebuffer, int x, int y, vec4 color) {
    // color
    unsigned char c[4];
    for (int t = 0; t < 4; t++)
    {
        c[t] = (int)float_clamp(color[t] * 255, 0, 255);
    }

    int index = ((WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x) * 4;
    for (int i = 0; i < 4; i++) {
        framebuffer[index + i] = c[i];
    }
}

static vec3 HDR_ReinhardMap(vec3 color) {
    const float gamma = 2.2;
    vec3 hdrColor = color;

    //// Reinhardɫ��ӳ��
    vec3 mapped;
    for (int w = 0; w < 3; w++) {
        mapped[w] = hdrColor[w] / (hdrColor[w] + 1.0f);
        mapped[w] = std::pow(mapped[w], 1.0 / gamma);
    }
    return mapped;
}

int get_index(int x, int y) {
    return ((WINDOW_HEIGHT - y - 1) * WINDOW_WIDTH + x);
}



static bool is_back_pace(vec3 pos[]) {
    //vec3 ab = pos[1] - pos[0];
    //vec3 bc = pos[2] - pos[1];
    //return (cross(ab, bc)).norm() > 0.0f;
    //TODO
    vec3 a = pos[0];
    vec3 b = pos[1];
    vec3 c = pos[2];
    float signed_area = a.x() * b.y() - a.y() * b.x() +
        b.x() * c.y() - b.y() * c.x() +
        c.x() * a.y() - c.y() * a.x();   //|AB AC|
    return signed_area <= 0;
}

// TODO 
//static void line(unsigned char* framebuffer, vec2 p0, vec2 p1, unsigned char color[]) {
//    bool steep = false;
//    if (std::abs(p0.x() - p1.x()) < std::abs(p0.y() - p1.y())) {
//        std::swap(p0[0], p0[1]);
//        std::swap(p1[0], p1[1]);
//        steep = true;
//    }
//    if (p0.x() > p1.x()) {
//        std::swap(p0, p1);
//    }
//
//    for (int x = p0.x(); x <= p1.x(); x++) {
//        float t = (x - p0.x()) / (float)(p1.x() - p0.x());
//        int y = p0.y() * (1. - t) + p1.y() * t;
//        if (steep) {
//            set_color(framebuffer, y, x, color);
//        }
//        else {
//            set_color(framebuffer, x, y, color);
//        }
//    }
//}
//
//
//static void triangle(unsigned char* framebuffer, vec3 vertexs[], unsigned char color[]) {
//    line(framebuffer, vertexs[0], vertexs[1], color);
//    line(framebuffer, vertexs[1], vertexs[2], color);
//    line(framebuffer, vertexs[2], vertexs[0], color);
//}


static std::tuple<float, float, float> compute_barycentric2D(float x, float y, vec3 vertexs[])
{
    float c1 = (x * (vertexs[1].y() - vertexs[2].y()) + (vertexs[2].x() - vertexs[1].x()) * y + vertexs[1].x() * vertexs[2].y() - vertexs[2].x() * vertexs[1].y()) / (vertexs[0].x() * (vertexs[1].y() - vertexs[2].y()) + (vertexs[2].x() - vertexs[1].x()) * vertexs[0].y() + vertexs[1].x() * vertexs[2].y() - vertexs[2].x() * vertexs[1].y());
    float c2 = (x * (vertexs[2].y() - vertexs[0].y()) + (vertexs[0].x() - vertexs[2].x()) * y + vertexs[2].x() * vertexs[0].y() - vertexs[0].x() * vertexs[2].y()) / (vertexs[1].x() * (vertexs[2].y() - vertexs[0].y()) + (vertexs[0].x() - vertexs[2].x()) * vertexs[1].y() + vertexs[2].x() * vertexs[0].y() - vertexs[0].x() * vertexs[2].y());
    return { c1, c2, 1 - c1 - c2 };
}


static int is_inside_triangle(float alpha, float beta, float gamma)
{
    int flag = 0;
    // here epsilon is to alleviate precision bug
    if (alpha > -EPSILON && beta > -EPSILON && gamma > -EPSILON)
        flag = 1;

    return flag;
}


//static bool insideTriangle(float u, float v, vec2 vertexs[0], vec2 vertexs[1], vec2 vertexs[2]) {
//    vec3 v[3] = { vec3(vertexs[0], 1.0f), vertexs[1], vertexs[2] };
//    vec3 p(u, v, 1.0f)
//
//    f0 = cross();
//    f1 = v[2].cross(v[1]);
//    f2 = v[0].cross(v[2]);
//    Vector3f p(x, y, 1.);
//    if ((p.dot(f0) * f0.dot(v[2]) > 0) && (p.dot(f1) * f1.dot(v[0]) > 0) && (p.dot(f2) * f2.dot(v[1]) > 0))
//        return true;
//    return false;
//}


// ��ͶӰ���󴴽��Ĺ۲���(Viewing Box)����Ϊƽ��ͷ��(Frustum)��ÿ��������ƽ��ͷ�巶Χ�ڵ����궼�����ճ������û�����Ļ�ϡ�
// ���ض���Χ�ڵ�����ת������׼���豸����ϵ�Ĺ��̣������������ױ�ӳ�䵽2D�۲�ռ����꣩����֮ΪͶӰ(Projection)����Ϊʹ��ͶӰ�����ܽ�3D����ͶӰ(Project)��������ӳ�䵽2D�ı�׼���豸����ϵ�С�

//                   mvp�ռ�Ӧ�þ��ǲü��ռ�
// һ�����ж��㱻�任���ü��ռ䣬���յĲ�������͸�ӳ���(Perspective Division)����ִ�У���������������ǽ�λ��������x��y��z�����ֱ�������������w������
// ͸�ӳ����ǽ�4D�ü��ռ�����任Ϊ3D��׼���豸����Ĺ��̡���һ������ÿһ��������ɫ�����е�����Զ�ִ�С�
// ����һ�׶�֮�����յ����꽫�ᱻӳ�䵽��Ļ�ռ��У�ʹ��glViewport�е��趨���������任��Ƭ�Ρ�


// ������ɫ�������Ҫ�����еĶ��㶼�ڲü��ռ��ڣ����������Ǹղ�ʹ�ñ任���������ġ�
// OpenGLȻ��Բü�����ִ��͸�ӳ����Ӷ������Ǳ任����׼���豸���ꡣ
// OpenGL��ʹ��glViewPort�ڲ��Ĳ���������׼���豸����ӳ�䵽��Ļ���꣬ÿ�����궼������һ����Ļ�ϵĵ㣨�����ǵ���������һ��800x600����Ļ����������̳�Ϊ�ӿڱ任��

// ��դ��
static void triangle_draw(unsigned char* framebuffer, float* zbuffer, IShader* shader, 
    std::vector<vec3>& NDC_vertexs,  std::vector<vec3>& screen_vertexs, std::vector<vec4>& mvp_vertexs, std::vector<vec4>& normals) {
    int n = shader->attribute.vertexs.size();

    // ���ñ���������
    for (int id = 0; id < n; id = id + 3)
    {
        vec3 NDC_vertexs3[3] = { NDC_vertexs[id + 0], NDC_vertexs[id + 1], NDC_vertexs[id + 2] };
        if (is_back_pace(NDC_vertexs3)) {
            continue;
        }
        vec3 screen_vertex3[3] = { screen_vertexs[id + 0], screen_vertexs[id + 1], screen_vertexs[id + 2] };
        vec4 mvp_vertex3[3] = { mvp_vertexs[id + 0], mvp_vertexs[id + 1], mvp_vertexs[id + 2] };
       
        vec3 normal3[3] = { normals[id + 0], normals[id + 1], normals[id + 2] };
        vec3 vertex3[3] = { shader->attribute.vertexs[id + 0], shader->attribute.vertexs[id + 1], shader->attribute.vertexs[id + 2] };


        // AABB BOX
        auto minx = std::min(screen_vertex3[0].x(), std::min(screen_vertex3[1].x(), screen_vertex3[2].x()));
        auto maxx = std::max(screen_vertex3[0].x(), std::max(screen_vertex3[1].x(), screen_vertex3[2].x()));
                                                                                                 
        auto miny = std::min(screen_vertex3[0].y(), std::min(screen_vertex3[1].y(), screen_vertex3[2].y()));
        auto maxy = std::max(screen_vertex3[0].y(), std::max(screen_vertex3[1].y(), screen_vertex3[2].y()));

        minx = (int)std::floor(minx); // ��x��������ȡ��
        maxx = (int)std::ceil(maxx); // ��y��������ȡ��
        miny = (int)std::floor(miny); // ��x��������ȡ��
        maxy = (int)std::ceil(maxy); // ��y��������ȡ��


        //std::cout << vertexs[1] << std::endl;
        //std::cout << minx <<" "<< maxx <<" "<<  miny << " " << maxy << std::endl;

        for (int i = minx; i <= maxx; i++) {
            for (int j = miny; j <= maxy; j++) {
                if (i < 0 || i >= WINDOW_WIDTH || j < 0 || j >= WINDOW_HEIGHT)continue;
                // ����Ļ�ռ��е���������
                auto [alpha, beta, gamma] = compute_barycentric2D((float)(i + 0.5), (float)(j + 0.5), screen_vertex3);
                if (alpha > 0 && beta > 0 && gamma > 0) {

                    // interpolation correct term
                    // ��ߵ�����Ҳ��������ʵ�ռ�����ֵ��mvp�ռ��wֵ�������
                    float normalizer = 1.0 / (alpha / mvp_vertex3[0].w()
                                            + beta  / mvp_vertex3[1].w()
                                            + gamma / mvp_vertex3[2].w());
                    //for larger z means away from camera, needs to interpolate z-value as a property			
                    float z = (alpha * screen_vertex3[0].z() / mvp_vertex3[0].w()
                             + beta  * screen_vertex3[1].z() / mvp_vertex3[1].w()
                             + gamma * screen_vertex3[2].z() / mvp_vertex3[2].w()) * normalizer;

                    if (zbuffer[get_index(i, j)] > z) {
                        zbuffer[get_index(i, j)] = z;


                        // ��ֵ������������ռ����꣬uv
                        shader->varying.normal = (alpha * normal3[0] / mvp_vertex3[0].w()
                                                + beta  * normal3[1] / mvp_vertex3[1].w()
                                                + gamma * normal3[2] / mvp_vertex3[2].w()) * normalizer;
                        shader->varying.position = (alpha * vertex3[0] / mvp_vertex3[0].w()
                                                  + beta  * vertex3[1] / mvp_vertex3[1].w()
                                                  + gamma * vertex3[2] / mvp_vertex3[2].w()) * normalizer;
                        shader->gl.screen_vertexs = (alpha * screen_vertex3[0] / mvp_vertex3[0].w()
                                                    + beta * screen_vertex3[1] / mvp_vertex3[1].w()
                                                    + gamma * screen_vertex3[2] / mvp_vertex3[2].w()) * normalizer;
                        //vec2 texcoords = (alpha * shader->attribute.texcoords[id + 0] / mvp_vertex3[0].w()
                        //    + beta * shader->attribute.texcoords[id + 1] / mvp_vertex3[1].w()
                        //    + gamma * shader->attribute.texcoords[id + 2] / mvp_vertex3[2].w()) * normalizer;


                        //��ֵ
                            //���㷨����
                            //����vuֵ
                            //����
                        //��Դ
                        //���λ��
                        vec4 color = shader->fragment_shader();
                        // color
                        set_color(framebuffer, i, j, color);
                    }
                }

            }
        }


    }

       
    
}


// TODO�����Ż����ȶ��㣬�������ε�������
void model_draw(unsigned char* framebuffer, float* zbuffer, IShader* shader) {
    //Model* model = shader->payload_shader.model;
    //
    //for (int i = 0; i < model->nfaces(); i++) {
    //    for (int j = 0; j < 3; j++) {
    //        shader->payload_shader.vertexs[j] = model->vert(i, j);
    //        shader->payload_shader.normals[j] = model->normal(i, j);
    //        shader->payload_shader.texcoords[j] = model->uv(i, j);
    //        // TODO Q:�������ʽ�����ǿ��޸ĵ���ֵ��
    //        //payload_shader.vertexs = vertexs;
    //        //payload_shader.normals = normals;
    //        //payload_shader.texcoords = texcoords;
    //    }
    //    shader->vertex_shader();

    //    triangle_draw(framebuffer, zbuffer, shader);
    //}
    //auto objects = shader->payload_shader.objects;

    int n = shader->attribute.vertexs.size();
    shader->varying.mvp_vertexs.resize(n);

    // ������ɫ�� mvp�任
    shader->vertex_shader();


    auto mvp_vertexs = shader->varying.mvp_vertexs;
    std::vector<vec3> NDC_vertexs(n);
    std::vector<vec3> screen_vertexs(n);
    std::vector<vec4> normals(n);
    
    for (int i = 0; i < n; i++)
    {
        // TODO �ü�
        // 


        // NDC transformation
        // homogeneous division
        NDC_vertexs[i] = vec3(mvp_vertexs[i]) / mvp_vertexs[i].w();


        // ��Ļ�ռ�ӳ��
        screen_vertexs[i][0] = 0.5 * (WINDOW_WIDTH - 1) * (NDC_vertexs[i][0] + 1.0);
        screen_vertexs[i][1] = 0.5 * (WINDOW_HEIGHT - 1) * (NDC_vertexs[i][1] + 1.0);
        //shader->payload_shader.screen_vertexs[i][2] = -shader->payload_shader.view_vertexs[i].z();	//view space z-value
        //screen_vertexs[i][2] = -mvp_vertexs[i].w();	//view space z-value
        // TODO
        screen_vertexs[i][2] = (-NDC_vertexs[i][2] + 1) / 2.0;	//view space z-value
        // ������ֵ��һ���ģ�mvp�ռ��wֵ�������
        //std::cout << shader->payload_shader.view_vertexs[i].z() << "   " << shader->payload_shader.mvp_vertexs[i].w() << std::endl;

        normals[i] = shader->varying.normal_mat * vec4(shader->attribute.normals[i]);
    }


    triangle_draw(framebuffer, zbuffer, shader, NDC_vertexs, screen_vertexs, mvp_vertexs, normals);



    // TODO ������ô��
    //for (int i = 0; i < shader->payload_shader.objects.size(); i++) {
    //    for (int j = 0; j < 3; j++) {
    //        shader->payload_shader.vertexs[j] = model->vert(i, j);
    //        shader->payload_shader.normals[j] = model->normal(i, j);
    //        shader->payload_shader.texcoords[j] = model->uv(i, j);
    //        // TODO Q:�������ʽ�����ǿ��޸ĵ���ֵ��
    //        //payload_shader.vertexs = vertexs;
    //        //payload_shader.normals = normals;
    //        //payload_shader.texcoords = texcoords;
    //    }
    //    shader->vertex_shader();

    //    triangle_draw(framebuffer, zbuffer, shader);
    //}
}



// �����߷��� ȡ������ͼ����ɫ
vec3 get_mapColor(vec3 dir) {
    if (dir.y() == 1.0f || dir.y() == -1.0f)return vec3(1, 1, 1);
    float cosb = dir.y();
    float cosa = dot(normalize(vec3(dir.x(), 0, dir.z())), vec3(1, 0, 0));
    float angle_b = acos(cosb);
    float angle_a = acos(cosa);
    angle_a = dir.z() < 0 ? angle_a : angle_a + 2.0f * PI - 2.0f * angle_a;
    int j = angle_b / PI * envmap_height;
    int i = angle_a / 2.0f / PI * envmap_width;
    
    return envmap[j * envmap_width + i];
}


