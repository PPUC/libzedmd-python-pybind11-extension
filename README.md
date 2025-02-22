# libzedmd-pybind11-extension
Project created to use libzedmd with python (using pybind11)


## Requirements
```shell
pip install pillow
pip install numpy

cd extern/libzedmd/
platforms/linux/x64/external.sh
cmake -DPLATFORM=linux -DARCH=x64 -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build

cd ../..
./extending_build
```

## Running
```shell
python extending.py 
```
