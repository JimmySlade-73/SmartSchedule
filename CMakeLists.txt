cmake_minimum_required(VERSION 3.10)
project(ScheduleApp)

# 1. Configurar C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 2. Buscar SFML (Debe estar instalado en tu sistema/MSYS2)
find_package(SFML COMPONENTS Graphics Window System REQUIRED)

# 3. Escanear automáticamente TODOS los .cpp en src y sus subcarpetas
file(GLOB_RECURSE SOURCES "src/*.cpp")

# 4. Incluir la carpeta de headers
include_directories(include)

# 5. Crear el ejecutable
add_executable(App ${SOURCES})

include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG        v3.11.3
)
FetchContent_MakeAvailable(nlohmann_json)

target_link_libraries(App
    PRIVATE
    SFML::Graphics
    SFML::Window
    SFML::System
    nlohmann_json::nlohmann_json
)