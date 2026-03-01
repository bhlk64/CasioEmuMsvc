cmake_minimum_required(VERSION 3.22)
project(PluginCIOutput)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(IMGUI_SOURCES
    ../CasioEmuMsvc/Gui/imgui/imgui.cpp
    ../CasioEmuMsvc/Gui/imgui/imgui_draw.cpp
    ../CasioEmuMsvc/Gui/imgui/imgui_widgets.cpp
    ../CasioEmuMsvc/Gui/imgui/imgui_tables.cpp
)

# Nếu KHÔNG phải Android thì mới thêm SDL backend ok
if(NOT ANDROID)
    list(APPEND IMGUI_SOURCES
        ../CasioEmuMsvc/Gui/imgui/imgui_impl_sdl2.cpp
        ../CasioEmuMsvc/Gui/imgui/imgui_impl_sdlrenderer2.cpp
    )
endif()

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