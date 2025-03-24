# libzedmd-python-pybind11-extension
Project created to use https://github.com/PPUC/libzedmd with python (using pybind11)

** Currently : Tested on Linux **

More info : https://github.com/PPUC/ZeDMD

To be used into MPF, MPF code needs to be changed like this : https://github.com/missionpinball/mpf/compare/0.80.x...PPUC:mpf-zedmd:0.80.x

Demo of MPF use : https://github.com/PPUC/MPF-pinball-ZeDMD-demo

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
