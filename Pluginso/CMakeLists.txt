cmake_minimum_required(VERSION 3.22)
project(CasioPluginSample)

set(CMAKE_CXX_STANDARD 20)

find_library(log-lib log)

# Nên dùng biến đườngdn tuyệt đối dựa trên CMAKE_CURRENT_SOURCE_DIR 
# để tránh lỗi khi build từ các thư mục khác nhau
set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui/imgui")

add_library(PluginCIOutput SHARED 
    PluginCIInput.cpp 
)

target_include_directories(PluginCIOutput PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${IMGUI_DIR}
    # Thêm đường dẫn chứa Ui.hpp nếu cần dùng các helper của emulator
    "${CMAKE_CURRENT_SOURCE_DIR}/../CasioEmuMsvc/Gui"
)

target_link_libraries(PluginCIOutput PRIVATE ${log-lib})

target_compile_definitions(PluginCIOutput PRIVATE __ANDROID__)

# Thêm cái này để chắc chắn load được hàm
set_target_properties(PluginCIOutput PROPERTIES CXX_VISIBILITY_PRESET default)