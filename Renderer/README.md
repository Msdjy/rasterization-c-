# Renderer

github���� [https://github.com/Msdjy/rasterization-c-](https://github.com/Msdjy/rasterization-c-)

��Ҫ�ļ��ṹ
- core
  - pipeline (���ߣ����¹������̺���ɫ�������ĵ���)
  - camera (���)
  - material (����,ֻ�õ���eval������pbr����)
  - math (��ѧ�����)
- shader
  - shader(��ɫ�����͵Ļ��࣬�������ɫ������Ҫ�Ĳ����ͺ���)
  - phone_shader  (ʹ��blinn-phoneģ����Ⱦ)
  - pbr_shader (ʹ��pbrģ����Ⱦ)
  - light_shader (�ð�ɫ��������ʾ��Դ��λ��)
  - shadow_shader (��Ⱦ���ֵ����Ȼ�����)
- main (�������ã��������ã�render loop)

�������
��Ҫ����
``` 
main(){
    ���ò���

    render loop{
        ��Ⱦ���ֵ

        ��Ⱦ����
    }
}


pipeline(){
    for(vertex){
        ������ɫ��
        NDC�ռ�任
        ��Ļӳ��
    }
    for(Triangle){
        �����޳�
        AABB����{
            ��Ȳ���{
                ���Ĳ�ֵ
                Ƭ����ɫ��
            }
        }
    }
}
```
ʵ��PCF����Ӱ����PBR���ʣ�δ���ü�����������������ֿ��ٻ������




ʵ�ֹ��ܣ������������ǲο��������е�ʵ�ִ��룩
> core & platform
> - [x] ���������
> - [x] ģ�ͼ��ؿ�
> - [x] ͼƬ���ؿ�
> - [x] ���ڿ��ƿ�

- [ ] ���
    - [x] mvp
    - [x] λ�˾���
    >- [x] �ƶ�
    >    - [x] wasd����������
    - [ ] ͸��
- [x] ����
  - [x] ���Ĳ�ֵ��ȷ��
- [ ] ��դ��
    - [ ] �����
- [ ] �ü�
    - [x] ���޳�
- [ ] ��ɫ��
    - [x] bingphone
    - [x] pbr
- [ ] �����Ż�
- [ ] ����
    - [x] ������ͼ
    - [x] ��պ�
    - [ ] mipmap
    - [ ] ������ͼ���Ӳ���ͼ
- [ ] ֡����
    - [x] ��Ӱ
    - [ ] ssao 
    - [ ] ͸�����
    - [ ] �ӳ���Ⱦ     
