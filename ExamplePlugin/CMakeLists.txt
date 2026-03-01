cmake_minimum_required(VERSION 3.22)
project(PluginCIOutput)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB IMGUI_SOURCES
    ../CasioEmuMsvc/Gui/imgui/*.cpp
)

add_library(PluginCIOutput SHARED
    PluginCIInput.cpp
    ${IMGUI_SOURCES}
)

target_include_directories(PluginCIOutput PRIVATE
    .
    ../CasioEmuMsvc/Gui/imgui
)


set_target_properties(PluginCIOutput PROPERTIES
    ANDROID_STL c++_static
)