# Renderer

github链接 [https://github.com/Msdjy/rasterization-c-](https://github.com/Msdjy/rasterization-c-)

主要文件结构
- core
  - pipeline (管线，大致管线流程和着色器函数的调用)
  - camera (相机)
  - material (材质,只用到了eval函数的pbr计算)
  - math (数学运算库)
- shader
  - shader(着色器类型的基类，存放了着色器所需要的参数和函数)
  - phone_shader  (使用blinn-phone模型渲染)
  - pbr_shader (使用pbr模型渲染)
  - light_shader (用白色立方体显示光源的位置)
  - shadow_shader (渲染深度值到深度缓冲中)
- main (顶点设置，参数设置，render loop)

代码概括
主要流程
``` 
main(){
    设置参数

    render loop{
        渲染深度值

        渲染物体
    }
}


pipeline(){
    for(vertex){
        顶点着色器
        NDC空间变换
        屏幕映射
    }
    for(Triangle){
        背面剔除
        AABB遍历{
            深度测试{
                重心插值
                片段着色器
            }
        }
    }
}
```
实现PCF软阴影，和PBR材质，未做裁剪，相机靠近物体会出现卡顿或卡死情况




实现功能，（框起来的是参考别人已有的实现代码）
> core & platform
> - [x] 向量运算库
> - [x] 模型加载库
> - [x] 图片加载库
> - [x] 窗口控制库

- [ ] 相机
    - [x] mvp
    - [x] 位姿矩阵
    >- [x] 移动
    >    - [x] wasd，鼠标键滑轮
    - [ ] 透镜
- [x] 重心
  - [x] 重心插值正确项
- [ ] 光栅化
    - [ ] 抗锯齿
- [ ] 裁剪
    - [x] 面剔除
- [ ] 着色器
    - [x] bingphone
    - [x] pbr
- [ ] 性能优化
- [ ] 纹理
    - [x] 纹理贴图
    - [x] 天空盒
    - [ ] mipmap
    - [ ] 法线贴图，视差贴图
- [ ] 帧缓冲
    - [x] 阴影
    - [ ] ssao 
    - [ ] 透明混合
    - [ ] 延迟渲染     
