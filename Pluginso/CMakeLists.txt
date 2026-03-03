cmake_minimum_required(VERSION 3.22)
project(CasioPluginSample)

set(CMAKE_CXX_STANDARD 20)

add_library(PluginCIOutput SHARED
    PluginCIInput.cpp
)

target_include_directories(PluginCIOutput PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../CasioEmuMsvc/Gui
    ../CasioEmuMsvc/Gui/imgui
)

find_library(log-lib log)

target_link_libraries(PluginCIOutput PRIVATE
    ${log-lib}
)

# Quan trọng với Android lld
target_link_options(PluginCIOutput PRIVATE
    "-Wl,--unresolved-symbols=ignore-in-shared-libs"
)

set_target_properties(PluginCIOutput PROPERTIES
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)