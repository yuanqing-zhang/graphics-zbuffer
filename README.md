# graphics-zbuffer

#### 要求：实现层次z-buffer算法

- 简单模式(仅包含层次z-buffer)、完整模式(层次z-buffer+场景八叉树)
- 分别对比简单模式和完整模式与扫描线z-buffer算法的加速比
- 评价指标: 面片不少于5000个; 应当含非凸多面体; 算法效率; 是否有加速等，综合考虑。

#### 提交

- 说明文档(编程环境、用户界面使用说明、简单的 数据结构说明、加速等)、源代码和工程文件、实验结果报告。

- 2021年1月15日之前将作业发至邮箱jqfeng@cad.zju.edu.cn，CG+姓名+学号



### 说明文档

编程环境：

- 操作系统：macOS 11.0.1
- 编译工具：cMake 3.18.2
- 库：Eigen 3.3.7，OpenGL，GLUT

使用：

```shell
mkdir build
cd build
cmake ..
make

# Run: ./src/render <type> <model_path>
# type can be 1:zbuffer 2:zbuffer(basic) 3:hierarchical zbuffer(basic) 4:hierarchical zbuffer(octree)
# example
./src/render 2 ../src/models/lowpolytree.obj
```



