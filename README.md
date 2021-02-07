# graphics-zbuffer

### Usage:
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


### Result:

![depth](img/depth.png)

![diffuse](img/diffuse.png)

