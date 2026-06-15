#include <iostream>
#define IMGUI_DISABLE_OBSOLETE_FUNCTIONS
#include "imgui.h"
#include "imgui_internal.h"
int main() {
    std::cout << "Style offset: " << offsetof(ImGuiContext, Style) << "\n";
}
