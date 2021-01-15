# graphics-zbuffer

**编程环境：**

- 操作系统：macOS 11.0.1
- 编译工具：cMake 3.18.2
- 库：Eigen 3.3.7，OpenGL，GLUT



**使用：**

```shell
mkdir build
cd build
cmake ..
make

# Run: ./src/render <type> <model_path>
# type can be 1:zbuffer 2:zbuffer(basic) 3:hierarchical zbuffer(basic) 4:hierarchical zbuffer(octree)
# example
./src/render 2 ../src/models/bunny.obj
```



**效果展示**

![depth](img/depth.png)

![diffuse](img/diffuse.png)

