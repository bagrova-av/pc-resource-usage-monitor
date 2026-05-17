cmake_minimum_required(VERSION 3.14)
project(PcResourceMonitor)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    httplib
    URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.14.3.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(httplib)

set(SOURCE_LIST
    app/main.cpp
    app/SystemMonitor/SystemMonitor.cpp
    app/WebServer/WebServer.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_LIST})

target_include_directories (${PROJECT_NAME} PRIVATE
    app/
    app/SystemMonitor/
    app/WebServer/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    nlohmann_json::nlohmann_json
    httplib
)