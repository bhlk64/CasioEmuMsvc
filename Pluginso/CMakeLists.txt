cmake_minimum_required(VERSION 3.22)
project(CasioPluginSample)

set(CMAKE_CXX_STANDARD 20)

add_library(PluginCIOutput SHARED
    PluginCIInput.cpp
)

target_include_directories(PluginCIOutput PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui"
    "${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui/imgui"
)

target_link_options(PluginCIOutput PRIVATE
    "-Wl,--allow-shlib-undefined"
)

set_target_properties(PluginCIOutput PROPERTIES
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)