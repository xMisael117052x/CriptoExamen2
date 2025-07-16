# Versión mínima de CMake requerida.
cmake_minimum_required(VERSION 3.16)

# Definición del proyecto.
project(CriptoExamen2 LANGUAGES CXX)

# Establece el estándar de C++ a C++20.
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Esto solo se aplica si estás usando el compilador GCC/MinGW en Windows.
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "Compilador GNU detectado. Aplicando flags para enlazado estático.")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
endif()

# Creamos el ejecutable 'CriptoExamen2' a partir de nuestro archivo fuente principal.
add_executable(CriptoExamen2 src/main.cpp)

# Añade el directorio 'include' a la ruta de búsqueda de headers para el target 'CriptoExamen2'.
target_include_directories(CriptoExamen2 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Mensaje para el usuario después de la configuración de CMake.
message(STATUS "Proyecto configurado. Para compilar, navega al directorio 'build' y ejecuta 'make' o 'ninja'.")
message(STATUS "Para ejecutar la aplicación: ./CriptoExamen2")
