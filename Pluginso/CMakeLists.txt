cmake_minimum_required(VERSION 3.22)
project(CasioPluginSample)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(log-lib log)

# Đường dẫn tới thư mục imgui của app (chỉ để include header)
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

target_compile_definitions(PluginCIOutput PRIVATE __ANDROID__)

# Quan trọng: không ẩn symbol C++
set_target_properties(PluginCIOutput PROPERTIES
    CXX_VISIBILITY_PRESET default
    VISIBILITY_INLINES_HIDDEN OFF
)