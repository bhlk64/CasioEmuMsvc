#include <iostream>
#include "src/Gui/ThemeManager.h"
int main() {
    std::cout << "isDarkMode offset: " << offsetof(ThemeSettings, isDarkMode) << "\n";
    std::cout << "igs_light offset: " << offsetof(ThemeSettings, igs_light) << "\n";
    std::cout << "igs_dark offset: " << offsetof(ThemeSettings, igs_dark) << "\n";
}
ImGuiStyle::ImGuiStyle() {}
