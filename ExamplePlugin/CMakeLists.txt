cmake_minimum_required(VERSION 3.22)
project(PluginCIOutput)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(PluginCIOutput SHARED
    PluginCIInput.cpp
)

target_include_directories(PluginCIOutput PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui/imgui
)

set_target_properties(PluginCIOutput PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

target_link_options(PluginCIOutput PRIVATE
    -Wl,--allow-shlib-undefined
    -Wl,--unresolved-symbols=ignore-in-shared-libs
)