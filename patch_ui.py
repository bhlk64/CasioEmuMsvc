import re
with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/Gui/Ui.cpp", "r") as f:
    content = f.read()

render_func_decl = "void RenderDebuggerToolbar() {"
new_render_func_decl = """static float screenshot_toast_timer = 0.0f;
void RenderDebuggerToolbar() {"""
content = content.replace(render_func_decl, new_render_func_decl)

end_main_menu_bar = "        ImGui::EndMainMenuBar();"
new_end_main_menu_bar = """
		if (m_emu->screenshot_taken.exchange(false)) {
			screenshot_toast_timer = 3.0f;
		}

		if (screenshot_toast_timer > 0.0f) {
			ImGui::SameLine(ImGui::GetWindowWidth() - 250.0f);
			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "\xf0\x9f\x93\xb8 Screenshot Saved!");
			screenshot_toast_timer -= ImGui::GetIO().DeltaTime;
		}

		if (m_emu->recording_active.load()) {
			ImGui::SameLine(ImGui::GetWindowWidth() - (screenshot_toast_timer > 0.0f ? 450.0f : 200.0f));
			ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "\xe2\x97\xbe REC: %u frames", m_emu->recording_frame_count.load());
		}

        ImGui::EndMainMenuBar();"""
content = content.replace(end_main_menu_bar, new_end_main_menu_bar)

with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/Gui/Ui.cpp", "w") as f:
    f.write(content)
