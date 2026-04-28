#include "Ui.hpp"
#include "5800FileSystem.h"
#include "AddressWindow.h"
#include "BitmapViewer.h"
#include "CallAnalysis.h"
#include "Chipset/Chipset.hpp"
#include "Chipset/MMU.hpp"
#include "CodeViewer.hpp"
#include "Editors.h"
#include "HwController.h"
#include "Injector.hpp"
#include "LabelFile.h"
#include "LabelViewer.h"
#include "MemBreakPoint.hpp"
#include "Random.hpp"
#include "Theme.h"
#include "VariableWindow.h"
#include "WatchWindow.hpp"
#include "PluginLogWindow.hpp"
#include "SnapshotWindow.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#include <Gui.h>
#include <SDL.h>
#include <filesystem>
#ifdef ENABLE_SENTRY
#include <sentry.h>
#endif
#include <sdl_win32_extra.h>
bool show_sentry_feedback = false;
char sentry_user_comments[1024] = "";
char sentry_user_email[128] = "";
char sentry_user_name[128] = "";



char* n_ram_buffer = 0;
casioemu::MMU* me_mmu = 0;
SDL_Window* window = 0;
SDL_Renderer* renderer = 0;

std::vector<Label> g_labels;

CodeViewer* code_viewer = 0;
Injector* injector = 0;
Breakpoints* membp = 0;

std::vector<UIWindow*> windows{};

void RenderDebuggerToolbar() {
    float padding = 10.0f;
    float spacing = 10.0f;
    float buttonWidth = 100.0f;

    float totalWidth = ImGui::GetIO().DisplaySize.x - padding * 2;
    float comboWidth = totalWidth - buttonWidth * 2 - spacing * 2;

    ImGui::SetNextWindowViewport(ImGui::GetMainViewport()->ID);
    ImGui::SetCursorPosY(40); // đẩy UI xuống dưới toolbar
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0), ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.0f);

    ImGui::Begin("##toolbar", nullptr,
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoSavedSettings);

    static UIWindow* current = nullptr;

    ImGui::SetNextItemWidth(comboWidth);

    if (ImGui::BeginCombo("##cb", current ? current->name : "Select Window")) {
        for (auto* w : windows) {
            if (!w) continue;

            bool selected = (current == w);
            if (ImGui::Selectable(w->name, selected))
                current = w;

            if (selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine(0, spacing);

    if (ImGui::Button("Open", ImVec2(buttonWidth, 0))) {
        if (current) current->open = true;
    }

    ImGui::SameLine(0, spacing);

    if (ImGui::Button("Close all", ImVec2(buttonWidth, 0))) {
        for (auto& w : windows) {
            if (w) w->open = false;
        }
    }

    ImGui::End();
}

void gui_loop() {
	if (!m_emu->Running())
		return;

	ImGuiIO& io = ImGui::GetIO();

#ifdef __ANDROID__
	ThemeManager::Instance().UpdateUIScale();
#endif

	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
#ifndef __ANDROID__
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
#endif
	for (auto win : windows) {
		if (!win || !win->open) continue;
		win->Render();
	}

	//	ImGui::Begin("Testing");
	//	if (ImGui::Button("Crash"_lc)) {
	//		throw 0;
	//	}
	//	// --- 新增：手动反馈选项 ---
	// #ifdef ENABLE_SENTRY
	//	ImGui::SameLine(); // 放在 Crash 按钮旁边
	//	if (ImGui::Button("Send Feedback"_lc)) {
	//		// 重置之前的输入内容
	//		memset(sentry_user_comments, 0, sizeof(sentry_user_comments));
	//		show_sentry_feedback = true;
	//	}
	// #endif
	//	ImGui::End();
	//	// --- Sentry 反馈对话框逻辑 ---
	// #ifdef ENABLE_SENTRY
	//	if (show_sentry_feedback) {
	//		// 确保每一帧都调用 OpenPopup，直到它真正打开
	//		ImGui::OpenPopup("User Feedback");
	//	}
	//
	//	// 使用 Modal 窗口确保反馈过程不被打断
	//	if (ImGui::BeginPopupModal("User Feedback", &show_sentry_feedback, ImGuiWindowFlags_AlwaysAutoResize)) {
	//		ImGui::Text("Help us improve CasioEmuMsvc!");
	//		ImGui::Separator();
	//
	//		ImGui::Text("Email (Optional):");
	//		ImGui::InputText("##email", sentry_user_email, IM_ARRAYSIZE(sentry_user_email));
	//
	//		ImGui::Text("What happened?");
	//		ImGui::InputTextMultiline("##comments", sentry_user_comments, IM_ARRAYSIZE(sentry_user_comments),
	//			ImVec2(350, 120), ImGuiInputTextFlags_AllowTabInput);
	//
	//		if (ImGui::Button("Submit", ImVec2(120, 0))) {
	//			auto uuid = Binary::LoadOrInit("uuid.bin", util::Random::getRandomObject<sentry_uuid_t>());
	//			char buf[37]{};
	//			sentry_uuid_as_string(&uuid, buf);
	//			sentry_value_t feedback = sentry_value_new_feedback(sentry_user_comments, sentry_user_email, buf, 0);
	//			sentry_capture_feedback(feedback);
	//
	//			show_sentry_feedback = false;
	//			ImGui::CloseCurrentPopup();
	//		}
	//
	//		ImGui::SameLine();
	//		if (ImGui::Button("Cancel", ImVec2(120, 0))) {
	//			show_sentry_feedback = false;
	//			ImGui::CloseCurrentPopup();
	//		}
	//		ImGui::EndPopup();
	//	}
	// #endif

	RenderDebuggerToolbar();

	ImGui::Render();
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
#ifndef __ANDROID__
	SDL_RenderPresent(renderer);
#endif
}

CodeViewer* test_gui(bool* guiCreated, SDL_Window* wnd, SDL_Renderer* rnd) {
	SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");

#ifdef SINGLE_WINDOW
	window = wnd;
	renderer = rnd;
#else
#ifdef __ANDROID__
	window = SDL_CreateWindow("CasioEmuMsvc Debugger",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		(int)ThemeManager::Instance().windowWidth,
		(int)ThemeManager::Instance().windowHeight,
		SDL_WINDOW_RESIZABLE);
#else
	window = SDL_CreateWindow("CasioEmuMsvc Debugger",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1600, 1080,
		SDL_WINDOW_RESIZABLE);
#endif
#ifdef _WIN32
	EnableDarkTitleBar(GetSDLWindowHandle(window));
#endif
	renderer = SDL_CreateRenderer(window, -1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
#endif

	if (renderer == nullptr) {
		SDL_Log("Error creating SDL_Renderer!");
		return 0;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

#ifdef __ANDROID__
	ThemeManager::Instance().LoadSettings();
	ThemeManager::Instance().UpdateUIScale();
#endif

	RebuildFont();
	// SetupDefaultTheme();

	io.WantCaptureKeyboard = true;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);
	if (guiCreated)
		*guiCreated = true;
	while (!me_mmu)
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	auto label_file = m_emu->GetModelFilePath("labels.txt");
	if (std::filesystem::exists(label_file))
		g_labels = parseFile(label_file);
	else
		std::cout << "[Warning] labels.txt doesn't exist. You can consider create one for better debugging experiences. Format: address(0x1234),func name(can be quoted)\n";

	if (m_emu->hardware_id == casioemu::HW_FX_5800P) {
		windows.push_back(CreateFx5800FileSystem());
	}

	for (auto item : std::initializer_list<UIWindow*>{
			 new VariableWindow(),
			 new HwController(),
			 new LabelViewer(),
			 new WatchWindow(),
			 CreateCallAnalysisWindow(),
			 code_viewer = new CodeViewer(),
			 injector = new Injector(),
			 membp = new Breakpoints(),
			 CreateAddressWindow(),
			 // MakeAssemblerUI(),
			 MakeThemeWindow(),
			 CreateBitmapViewer(),
			 CreateSnapshotWindow(),
			 new PluginLogWindow()})
		windows.push_back(item);
	for (auto item : GetEditors())
		windows.push_back(item);

#ifdef __ANDROID__
	for (auto item : windows) {
		item->open = false;
	}
#endif

	return 0;
}

void gui_cleanup() {
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}