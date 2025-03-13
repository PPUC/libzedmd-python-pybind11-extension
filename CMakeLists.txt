cmake_minimum_required(VERSION 3.10)

# Nom du projet
project(ExtendingProject)

# Indiquer la version de C++ à utiliser
set(CMAKE_CXX_STANDARD 11)

# Options de compilation
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")

# Trouver Python (utilisé pour obtenir les paramètres d'inclusion et de suffixe)
find_package(Python3 REQUIRED)
include_directories(${Python3_INCLUDE_DIRS})

# Ajouter les chemins d'inclusion externes
include_directories(
    extern/pybind11/include
    extern/libzedmd/src
)

# Ajouter le fichier source
pybind11_add_module(extending extending.cpp)

# Ajouter les répertoires de bibliothèque
target_link_directories(extending PRIVATE
    extern/libzedmd/build
)

# Lier les bibliothèques nécessaires
target_link_libraries(extending
    PRIVATE
    ${Python3_LIBRARIES}   # Bibliothèque Python (liée à partir de la configuration CMake)
    dl
    pthread
    util
    zedmd
    serialport
)

# Définir le nom du fichier de sortie en utilisant le suffixe de l'extension Python
set_target_properties(extending
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build
    SUFFIX ${Python3_EXTENSION_SUFFIX}
)

