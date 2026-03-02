cmake_minimum_required(VERSION 3.22)
project(CasioPluginSample)

set(CMAKE_CXX_STANDARD 20)

find_library(log-lib log)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui/imgui")

add_library(PluginCIOutput SHARED
    PluginCIInput.cpp
)

target_include_directories(PluginCIOutput PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui"
)

target_link_libraries(PluginCIOutput PRIVATE
    ${log-lib}
)

# 👇 Quan trọng nhất
target_link_options(PluginCIOutput PRIVATE
    "-Wl,--allow-shlib-undefined"
    "-Wl,--unresolved-symbols=ignore-in-shared-libs"
)

set_target_properties(PluginCIOutput PROPERTIES
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)