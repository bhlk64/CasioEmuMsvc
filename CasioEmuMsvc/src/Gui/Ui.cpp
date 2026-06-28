#include "Ui.hpp"
#include "hex.hpp"
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
#ifndef CASIOEMU_CORE_WEB
#include "QrCodeWindow.h"
#endif
#ifndef TEST_BUILD
#include "Rop/RopCompilerUI.h"
#include "PluginLogWindow.hpp"
#include "SnapshotWindow.h"
#include "CalculatorWindow.h"
#include "imgui/imgui.h"
#ifdef CASIOEMU_CORE_WEB
#include "WebDebuggerGui.h"
#else
#include "imgui/imgui_impl_sdl2.h"
#include "imgui/imgui_impl_sdlrenderer2.h"
#endif
#include <Gui.h>
#include <SDL.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
/*#include <fstream>

void DebugLog(const std::string& msg) {
    static std::ofstream log("debug_log.txt", std::ios::app);
    if (log.is_open()) {
        log << msg << std::endl;
        log.flush();
    }
}*/

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
int top_bar_size = 0;
Breakpoints* membp = 0;
SnapshotWindow* snapshot_window = 0;

std::vector<UIWindow*> windows{};

std::string ui_state_fn = "ui_state.txt";
bool ui_ready = false;
void SaveUIState() {
    if (!ui_ready) return;

    std::string tmp = ui_state_fn + ".tmp";

    std::ofstream f(tmp, std::ios::out | std::ios::trunc);
    if (!f.is_open()) return;

    for (auto* w : windows) {
        if (!w) continue;
        f << w->name << "=" << (w->open ? 1 : 0) << "\n";
    }

    f.close();
    std::filesystem::rename(tmp, ui_state_fn);
}

#ifdef IOS
#include "iOSNativeBridge.h"
#endif
static float screenshot_toast_timer = 0.0f;
void RenderDebuggerToolbar() {
    bool isCustom = false;
#if defined(IOS)
    isCustom = true;
#endif

    bool opened = false;
    if (isCustom) {
#if defined(IOS)
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        float headerY = viewport->WorkPos.y;
        ImGui::SetNextWindowPos(ImVec2(viewport->WorkPos.x, headerY));
        ImGui::SetNextWindowSize(ImVec2(viewport->WorkSize.x, ImGui::GetFrameHeight() + 8.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, ImGui::GetStyle().FramePadding.y + 4.0f));
        
        opened = ImGui::Begin("##DebuggerToolbar", nullptr, 
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
#endif
    } else {
        opened = ImGui::BeginMainMenuBar();
    }

    if (opened) {
        bool showMenu = true;
        if (isCustom) {
            showMenu = ImGui::BeginMenuBar();
        }
        
        if (showMenu) {
            if (ImGui::BeginTabBar("ToolbarTabs", ImGuiTabBarFlags_FittingPolicyScroll | ImGuiTabBarFlags_NoTooltip)) {
                
                if (ImGui::TabItemButton("Debugger Windows")) {
                    ImGui::OpenPopup("DebuggerMenuPopup");
                }
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
                if (ImGui::BeginPopup("DebuggerMenuPopup")) {
                    for (auto* w : windows) {
                        if (w && ImGui::MenuItem(w->name, nullptr, &w->open)) {
                            SaveUIState();
                        }
                    }
                    ImGui::EndPopup();
                }

                if (std::any_of(windows.begin(), windows.end(), [](UIWindow* w){ return !w->open; })) {
                    if (ImGui::TabItemButton("Open All")) {
                        for (auto* w : windows) if (w) w->open = true;
                    }
                }
                else {
                    if (ImGui::TabItemButton("Close All")) {
                        for (auto* w : windows) if (w) w->open = false;
                    }
                }

#if defined(__ANDROID__) || defined(IOS)
                if (ImGui::TabItemButton("[v] Hide KB")) {
                    SDL_StopTextInput();
                    ImGui::SetWindowFocus(nullptr);
                }
#endif

                bool isPaused = m_emu->GetPaused();
                if (ImGui::TabItemButton(isPaused ? "[>] Resume" : "[||] Pause")) {
                    m_emu->SetPaused(!isPaused);
                }

                if (ImGui::TabItemButton("[C] Screenshot")) {
                    ImGui::OpenPopup("ScreenshotMenuPopup");
                }
                ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
                if (ImGui::BeginPopup("ScreenshotMenuPopup")) {
                    if (ImGui::MenuItem("Full Calculator")) {
                        m_emu->screenshot_full_ui = true;
                        m_emu->screenshot_requested = true;
                    }
                    if (ImGui::MenuItem("Screen Only")) {
                        m_emu->screenshot_full_ui = false;
                        m_emu->screenshot_requested = true;
                    }
                    ImGui::EndPopup();
                }

                if (m_emu->recording_active.load()) {
                    if (ImGui::TabItemButton("[ ] Stop Rec")) {
                        m_emu->recording_stop_requested = true;
                    }
                } else {
                    if (ImGui::TabItemButton("[O] Record")) {
                        ImGui::OpenPopup("RecordMenuPopup");
                    }
                    ImGui::SetNextWindowPos(ImVec2(ImGui::GetItemRectMin().x, ImGui::GetItemRectMax().y));
                    if (ImGui::BeginPopup("RecordMenuPopup")) {
                        if (ImGui::MenuItem("Full Calculator")) {
                            m_emu->recording_full_ui = true;
                            m_emu->recording_requested = true;
                        }
                        if (ImGui::MenuItem("Screen Only")) {
                            m_emu->recording_full_ui = false;
                            m_emu->recording_requested = true;
                        }
                        ImGui::EndPopup();
                    }
                }

                if (ImGui::TabItemButton(ThemeManager::Instance().Settings().isDarkMode ? "Light Theme" : "Dark Theme")) {
                    if (ThemeManager::Instance().Settings().isDarkMode)
                        ThemeManager::Instance().SetLightMode();
                    else
                        ThemeManager::Instance().SetDarkMode();
                }

                ImGui::EndTabBar();
            }

            if (m_emu->screenshot_taken.exchange(false)) {
                screenshot_toast_timer = 3.0f;
            }

            if (screenshot_toast_timer > 0.0f) {
                ImGui::SameLine(ImGui::GetWindowWidth() - 250.0f);
                ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.2f, 1.0f), "[C] Screenshot Saved!");
                screenshot_toast_timer -= ImGui::GetIO().DeltaTime;
            }

            if (m_emu->recording_active.load()) {
                ImGui::SameLine(ImGui::GetWindowWidth() - (screenshot_toast_timer > 0.0f ? 450.0f : 200.0f));
                ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "[O] Recording: %u frames", m_emu->recording_frame_count.load());
            }

            if (isCustom) {
                ImGui::EndMenuBar();
            }
        }
        
        if (isCustom) {
            ImGui::End();
            ImGui::PopStyleVar(4);
        } else {
            ImGui::EndMainMenuBar();
        }
    }
}

void LoadUIState() {
    std::ifstream f(ui_state_fn);
    if (!f.is_open()) return;

    std::unordered_map<std::string, bool> state;

    std::string line;
    while (std::getline(f, line)) {
        auto pos = line.find('=');
        if (pos == std::string::npos) continue;

        std::string name = line.substr(0, pos);
        bool open = line.substr(pos + 1) == "1";

        state[name] = open;
    }

    for (auto* w : windows) {
        if (!w) continue;

        if (state.find(w->name) != state.end())
            w->open = state[w->name];
    }
} 

#ifdef CASIOEMU_CORE_WEB
SDL_Surface* background = nullptr;
SDL_Texture* bg_txt = nullptr;
#endif

static float GetStatusBarHeight() {
	return ImGui::GetFrameHeight() + 4.0f;
}

void RenderStatusBar() {
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	float barHeight = GetStatusBarHeight();
	
	ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - barHeight));
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, barHeight));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8.0f, 2.0f));
#ifdef CASIOEMU_CORE_WEB
	ImVec4 statusBg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
	statusBg.w = std::max(statusBg.w, 0.82f);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, statusBg);
#else
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.08f, 0.08f, 0.12f, 1.0f));
#endif
	
	if (ImGui::Begin("##StatusBar", nullptr, 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_NoDocking)) {
		
		// Run/Pause state status indicator
		if (m_emu->GetPaused()) {
			ImGui::TextColored(UIHelpers::kColorWarning, "[||] %s", "StatusBar.Paused"_lc);  // ⏸
		} else {
			ImGui::TextColored(UIHelpers::kColorSuccess, "[>] %s", "StatusBar.Running"_lc); // ▶
		}
		
		ImGui::SameLine(0.0f, 20.0f);
		ImGui::TextDisabled("|");
		ImGui::SameLine(0.0f, 20.0f);
		
		// Current PC
		ImGui::Text("PC: %05X", pc_cache);
		
		ImGui::SameLine(0.0f, 20.0f);
		ImGui::TextDisabled("|");
		ImGui::SameLine(0.0f, 20.0f);
		
		// Breakpoints count
		int bpCount = code_viewer ? (int)code_viewer->GetBreakpointCount() : 0;
		ImGui::Text("BP: %d", bpCount);
	}
	ImGui::End();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar();
}

static ImGuiID RenderDockSpace(float reservedBottom) {
/*	ImGuiViewport* viewport = ImGui::GetMainViewport();
	float dockHeight = viewport->Size.y - reservedBottom;
	if (dockHeight < 1.0f) dockHeight = 1.0f;

	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, dockHeight));
	ImGui::SetNextWindowViewport(viewport->ID);
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGuiWindowFlags flags =
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGui::Begin("##DebuggerDockSpaceHost", nullptr, flags);
	ImGuiID dockspace_id = ImGui::GetID("DebuggerDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();
	ImGui::PopStyleVar(3);
*/
		// --- BẮT ĐẦU DOCKSPACE ---
	ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);

	ImVec2 dockSize = viewport->WorkSize;
	dockSize.y -= reservedBottom;
	if (dockSize.y < 1.0f)
		dockSize.y = 1.0f;

	ImGui::SetNextWindowSize(dockSize);
	ImGui::SetNextWindowViewport(viewport->ID);
	
	ImGuiWindowFlags host_flags = ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus |
		ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

	ImGui::Begin("MainDockHost", nullptr, host_flags);
	ImGui::PopStyleVar(3);

	// Lệnh này tạo ra vùng để bạn gộp Tab
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	
	ImGui::End(); // Kết thúc Host
	// --- KẾT THÚC DOCKSPACE ---
	return dockspace_id;
}

static void RenderDebuggerGuiWindows() {
#if !defined(__ANDROID__) && !defined(IOS)
	ImGuiID dockspace_id = RenderDockSpace(GetStatusBarHeight());
#endif
	for (auto win : windows) {
		if (!win) continue;
#if !defined(__ANDROID__) && !defined(IOS)
		if (dockspace_id != 0) {
			ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		}
#endif
		win->Render();
	}

    RenderDebuggerToolbar();

    top_bar_size = ImGui::GetCursorPosY();
#if !defined(__ANDROID__) && !defined(IOS)
	RenderStatusBar();
#endif

    ImGui::Render();
    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    
    SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    SDL_RenderSetScale(renderer, 1.0f, 1.0f);
    
    #ifndef SINGLE_WINDOW
    SDL_RenderPresent(renderer);
    #endif

/*#ifdef SINGLE_WINDOW
	ImGui::SetNextWindowBgAlpha(0.0f);
	ImGui::Begin("Overlay", nullptr,
		ImGuiWindowFlags_NoDecoration |
		ImGuiWindowFlags_NoDocking |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove);

	auto& tm = ThemeManager::Instance();
	float safeAreaPadding = tm.padding * 1.5f;
	ImGui::SetWindowPos(ImVec2(safeAreaPadding, safeAreaPadding));

	float displayWidth = ImGui::GetIO().DisplaySize.x;
	float totalWidth = displayWidth - (safeAreaPadding * 2);
	float spacingBetweenElements = tm.padding * 1.2f;
	float buttonWidth = (totalWidth - spacingBetweenElements * 2) * 0.25f;
	float comboWidth = totalWidth - (buttonWidth * 2) - (spacingBetweenElements * 2);

	static UIWindow* current_filter = 0;
	ImGui::SetNextItemWidth(comboWidth);
	if (ImGui::BeginCombo("##cb", current_filter ? current_filter->name : 0)) {
		for (int n = 0; n < windows.size(); n++) {
			bool is_selected = (current_filter == windows[n]);
			if (ImGui::Selectable(windows[n]->name, is_selected))
				current_filter = windows[n];
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::SameLine(0, spacingBetweenElements);
	ImVec2 buttonSize(buttonWidth, tm.buttonHeight * 1.2f);
	if (ImGui::Button("Open", buttonSize)) {
		if (current_filter != 0)
			current_filter->open = true;
	}

	ImGui::SameLine(0, spacingBetweenElements);
	if (ImGui::Button("Close all", buttonSize)) {
		for (auto& win : windows) {
			win->open = false;
		}
	}
	// Let's record where we are.
	top_bar_size = ImGui::GetCursorPosY();
	ImGui::End();
#endif*/
}

void gui_loop() {
	if (!m_emu->Running())
		return;

	ImGuiIO& io = ImGui::GetIO();

#if defined(__ANDROID__) || defined(MACOS) || defined(IOS)
	ThemeManager::Instance().UpdateUIScale();
#endif

#ifndef CASIOEMU_CORE_WEB
	ImGui_ImplSDLRenderer2_NewFrame();
	ImGui_ImplSDL2_NewFrame();
#endif
	ImGui::NewFrame();
	RenderDebuggerGuiWindows();
	ImGui::Render();
#ifndef CASIOEMU_CORE_WEB
#ifdef SINGLE_WINDOW
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
#else
	ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
	SDL_RenderPresent(renderer);
#endif
#endif
}

static CodeViewer* CreateDebuggerGuiWindows() {
	while (!me_mmu)
		std::this_thread::sleep_for(std::chrono::microseconds(1));
	std::filesystem::path label_file = m_emu->GetModelFilePath("labels.txt");
	if (std::filesystem::exists(label_file))
		g_labels = parseFile(label_file.string());
	else
		std::cout << "[Warning] " << label_file.string() << " doesn't exist. You can consider create one for better debugging experiences. Format: address(0x1234),func name(can be quoted)\n";

	if (m_emu->hardware_id == casioemu::HW_FX_5800P) {
		windows.push_back(CreateFx5800FileSystem());
	}

	if (m_emu->hardware_id != casioemu::HW_SOLARII) {
		windows.push_back(new VariableWindow());
	}

	for (auto item : std::initializer_list<UIWindow*>{
			 new CalculatorWindow(),
			 new HwController(),
			 new LabelViewer(),
			 new WatchWindow(),
			 CreateCallAnalysisWindow(),
			 code_viewer = new CodeViewer(),
			 injector = new Injector(),
			 membp = new Breakpoints(),
			 CreateAddressWindow(),
			 // MakeAssemblerUI(),
#if !defined(TEST_BUILD)
			 CreateRopCompilerWindow(),
#endif
#if !defined(TEST_BUILD) && !defined(CASIOEMU_CORE_WEB)
			 new PluginLogWindow(),
#endif
#if !defined(TEST_BUILD)
			 snapshot_window = static_cast<SnapshotWindow*>(CreateSnapshotWindow()),
#endif
#ifndef CASIOEMU_CORE_WEB
			 new QrCodeWindow(),
#endif
			 MakeThemeWindow(),
			 CreateBitmapViewer(), })
		windows.push_back(item);
	for (auto item : GetEditors())
		windows.push_back(item);

#if defined(__ANDROID__) || defined(IOS)
	for (auto item : windows) {
		item->open = false;
	}
#else
	if (!std::filesystem::exists(ui_state_fn)) {
		for (auto* w : windows) {
			if (w) {
				w->open = true;
				w->bring_to_front_requested = false;
			}
		}
	}
#endif
	LoadUIState();
	ui_ready = true;

	return code_viewer;
}

#ifndef CASIOEMU_CORE_WEB
CodeViewer* test_gui(bool* guiCreated, SDL_Window* wnd, SDL_Renderer* rnd) {
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    
    if (window || renderer) {
        gui_cleanup();
        window = nullptr;
        renderer = nullptr;
    }

#ifdef SINGLE_WINDOW
    window = wnd;
    renderer = rnd;
#else
#if defined(__ANDROID__) || defined(IOS)
    window = SDL_CreateWindow("CasioEmuMsvc Debugger",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        (int)ThemeManager::Instance().windowWidth,
        (int)ThemeManager::Instance().windowHeight,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#else
	int winX = ThemeManager::Instance().Settings().windowX;
	int winY = ThemeManager::Instance().Settings().windowY;
	int winW = ThemeManager::Instance().Settings().windowW;
	int winH = ThemeManager::Instance().Settings().windowH;

	SDL_Rect bounds;
	if (SDL_GetDisplayUsableBounds(0, &bounds) == 0) {
		if (winW > bounds.w) winW = bounds.w;
		if (winH > bounds.h) winH = bounds.h;

		if (winX != SDL_WINDOWPOS_CENTERED) {
			if (winX < bounds.x) winX = bounds.x;
			if (winX + winW > bounds.x + bounds.w) winX = bounds.x + bounds.w - winW;
		}
		if (winY != SDL_WINDOWPOS_CENTERED) {
			if (winY < bounds.y) winY = bounds.y;
			if (winY + winH > bounds.y + bounds.h) winY = bounds.y + bounds.h - winH;
		}
	}

	window = SDL_CreateWindow("CasioEmuMsvc Debugger",
		winX,
		winY,
		winW, winH,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
#endif
#ifdef _WIN32
    EnableDarkTitleBar(GetSDLWindowHandle(window));
#endif
    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
#endif

    if (!renderer) {
        SDL_Log("Error creating SDL_Renderer!");
        return nullptr;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

#if defined(__ANDROID__) || defined(IOS)
    ThemeManager::Instance().LoadSettings();
    ThemeManager::Instance().UpdateUIScale();
#endif

	// RebuildFont();
	// SetupDefaultTheme();

    io.IniFilename = "imgui.ini";
    io.WantCaptureKeyboard = true;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
	ImGui_ImplSDLRenderer2_Init(renderer);

	ThemeManager::Instance().RequestFontRebuild();
	ThemeManager::Instance().ProcessFontRebuild();

	if (guiCreated)
		*guiCreated = true;
	return CreateDebuggerGuiWindows();
}
#endif

#ifdef CASIOEMU_CORE_WEB
void InitWebDebuggerGuiWindows() {
	if (windows.empty()) {
		CreateDebuggerGuiWindows();
	}
}

void RenderWebDebuggerGuiWindows() {
	RenderDebuggerGuiWindows();
}

void CleanupWebDebuggerGuiWindows() {
	for (auto* win : windows) {
		delete win;
	}
	windows.clear();
	code_viewer = nullptr;
	injector = nullptr;
	membp = nullptr;
	g_labels.clear();
}
#endif

namespace UIHelpers {

	void JumpToMemory(uint32_t addr) {
		// Try Ram first
		for (auto* win : windows) {
			const char* n = win->name;
			if (n && strcmp(n, "Ram") == 0) {
				win->GotoMemoryAddress(addr);
				win->BringToFront();
				return;
			}
			// Track first editor-like window as fallback
			if (!fallback && n && (strcmp(n, "Rom") == 0 || strcmp(n, "All") == 0
				|| strcmp(n, "PRam") == 0 || strcmp(n, "Flash") == 0)) {
				fallback = win;
			}
		}
		if (fallback) {
			fallback->GotoMemoryAddress(addr);
			fallback->BringToFront();
		}
	}

	void ClickableAddress(uint32_t addr, JumpTarget defaultTarget) {
		char addrLabel[16];
		snprintf(addrLabel, sizeof(addrLabel), "%05X", addr);
		ImGui::PushID(addrLabel);
		const ImVec2 textSize = ImGui::CalcTextSize(addrLabel);
		const ImVec2 textPos = ImGui::GetCursorScreenPos();
		ImGui::InvisibleButton("##clickable_address", textSize);
		const bool hovered = ImGui::IsItemHovered();
		const bool leftClicked = ImGui::IsItemClicked(ImGuiMouseButton_Left);
		const bool rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
		ImDrawList* drawList = ImGui::GetWindowDrawList();
		const ImU32 textColor = ImGui::GetColorU32(hovered ? ImVec4(0.55f, 0.72f, 1.0f, 1.0f) : kColorInfo);
		drawList->AddText(textPos, textColor, addrLabel);
		if (hovered) {
			const float underlineY = textPos.y + textSize.y;
			drawList->AddLine(ImVec2(textPos.x, underlineY), ImVec2(textPos.x + textSize.x, underlineY), textColor);
		}

		if (hovered) {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
			ImGui::BeginTooltip();
			if (defaultTarget == JumpTarget::Code) {
				ImGui::Text("ClickableAddress.CodeJumpTooltip"_lc, addr);
				ImGui::TextDisabled("%s", "ClickableAddress.RightClickHint"_lc);
			} else if (defaultTarget == JumpTarget::Memory) {
				ImGui::Text("ClickableAddress.MemJumpTooltip"_lc, addr);
				ImGui::TextDisabled("%s", "ClickableAddress.RightClickHint"_lc);
			} else {
				ImGui::Text("ClickableAddress.BothTooltip"_lc, addr);
			}
			ImGui::EndTooltip();
		}

		// Left-click: default action
		if (leftClicked) {
			if (defaultTarget == JumpTarget::Code || defaultTarget == JumpTarget::Both) {
				if (code_viewer) {
					code_viewer->JumpTo(addr);
					code_viewer->BringToFront();
				}
			} else {
				JumpToMemory(addr);
			}
		}

		// Right-click: context menu with both options
		char popupId[32];
		snprintf(popupId, sizeof(popupId), "##ca_popup_%05X", addr);
		if (rightClicked) {
			ImGui::OpenPopup(popupId);
		}
		if (ImGui::BeginPopup(popupId)) {
			ImGui::TextDisabled("0x%05X", addr);
			ImGui::Separator();
			if (ImGui::MenuItem("ClickableAddress.CodeJump"_lc)) {
				if (code_viewer) {
					code_viewer->JumpTo(addr);
					code_viewer->BringToFront();
				}
			}
			if (ImGui::MenuItem("ClickableAddress.MemJump"_lc)) {
				JumpToMemory(addr);
			}
			ImGui::EndPopup();
		}
		ImGui::PopID();
	}
}


void gui_cleanup() {
#ifndef CASIOEMU_CORE_WEB
#if !defined(__ANDROID__) && !defined(IOS)
#ifndef SINGLE_WINDOW
	if (window) {
		int x, y, w, h;
		SDL_GetWindowPosition(window, &x, &y);
		SDL_GetWindowSize(window, &w, &h);

		ThemeManager::Instance().Settings().windowX = x;
		ThemeManager::Instance().Settings().windowY = y;
		ThemeManager::Instance().Settings().windowW = w;
		ThemeManager::Instance().Settings().windowH = h;
		ThemeManager::Instance().SaveSettings();
	}
#endif
#endif
	SaveUIState();
	ImGui_ImplSDLRenderer2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	windows.clear();

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
#else
	CleanupWebDebuggerGuiWindows();
#endif
}
