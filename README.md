# libzedmd-python-pybind11-extension
Project created to use https://github.com/PPUC/libzedmd with python (using pybind11)

** Currently : Tested on Linux 64 and Win 64**

More info : https://github.com/PPUC/ZeDMD

To be used into MPF, MPF code needs to be changed like this : https://github.com/missionpinball/mpf/compare/0.80.x...PPUC:mpf-zedmd:0.80.x

Demo of MPF use : https://github.com/PPUC/MPF-pinball-ZeDMD-demo

## Windows 64
### Requirements
Install Python (Check Add Python to system PATH)

Install Pip if needed : python -m ensurepip --upgrade

Install Pybind11 : pip install pybind11

Install Visual Studio Build Tools (Check C++ build tools with : MSVC v142, Windows SDK and Clang)

Install Cmake.msi from the official website (Check Add Cmake to system PATH)

Check installs by launching VS Build Tools Console:
```shell
python --version

pip --version

python
>>> import pybind11
>>> print(pybind11.__version__)
>>> exit()

cl

cmake --version
```

How to build :
```shell
cd extern/libzedmd/
platforms/win/x64/external.sh
cmake -G "Visual Studio 17 2022" -DPLATFORM=win -DARCH=x64 -B build
cmake --build build --config Release

cd ../../build
cmake -Dpybind11_DIR=C:\PATH\TO\PYBIND11\CMAKE\PYBIND11\DIR ..
cmake --build . --config Release 
```
Notice : In order to find pybind11_DIR you can use this command : 
```shell
python -c "import pybind11; print(pybind11.get_cmake_dir())"
```

### Running
```shell
pip install pillow
pip install numpy

python extending.py 
```

## Linux
### Requirements
```shell
cd extern/libzedmd/
platforms/linux/x64/external.sh
cmake -DPLATFORM=linux -DARCH=x64 -DCMAKE_BUILD_TYPE=Release -B build
cmake --build build
```
Building :
```shell
cd ../..
g++ -O3 -Wall -shared -std=c++11 -fPIC $(python3-config --includes) -Iextern/pybind11/include -Iextern/libzedmd/src -Lextern/libzedmd/build extending.cpp  -o build/extending$(python3-config --extension-suffix) -lzedmd -lserialport
```
Notice : You can use cmake too (cf Windows section to see cmake command)

### Running
```shell
pip install pillow
python extending.py 
```
