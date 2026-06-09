#include "MemBreakPoint.hpp"
#include "Chipset/CPU.hpp"
#include "Chipset/Chipset.hpp"
#include "Emulator.hpp"
#include "Gui/Hooks.h"
#include "Ui.hpp"
#include "imgui/imgui.h"
#include <Localization.h>
#include <cstdint>
#include <cstdlib>
#include <stdlib.h>

Breakpoints* membp_cv = 0;

struct RegBP {
	int reg;			// register type
	int mode;		   // compare mode
	uint32_t value;	 // target value
	bool enabled;	   // bật/tắt
};

enum RegType {
	REG_SP,
	REG_ER0,
	REG_ER2,
	REG_ER4,
	REG_ER6,
	REG_ER8,
	REG_ER10,
	REG_ER12,
	REG_ER14,
	REG_EA,
	REG_LR,
	REG_PC
};

std::vector<RegBP> reg_bps;

inline uint16_t GET_ER(casioemu::CPU& cpu, int n) {
	return (cpu.reg_r[n + 1] << 8) | cpu.reg_r[n];
}

uint32_t GetRegisterValue(casioemu::CPU& cpu, int reg) {
	switch (reg) {
	case REG_SP: return cpu.reg_sp;
	case REG_ER0: return GET_ER(cpu, 0);
	case REG_ER2: return GET_ER(cpu, 2);
	case REG_ER4: return GET_ER(cpu, 4);
	case REG_ER6: return GET_ER(cpu, 6);
	case REG_ER8: return GET_ER(cpu, 8);
	case REG_ER10: return GET_ER(cpu, 10);
	case REG_ER12: return GET_ER(cpu, 12);
	case REG_ER14: return GET_ER(cpu, 14);
	case REG_EA: return cpu.reg_ea;
	case REG_LR: return cpu.reg_lr;
	case REG_PC: return cpu.reg_pc;
	default: return 0;
	}
}

static uint32_t last_reg_value[16] = {0};
static bool last_break_regs[16] = {0};
static bool reg_first_run = true;

void Breakpoints::DrawContent() {
	if (break_point_hash.empty()) {
		ImGui::PushStyleColor(ImGuiCol_Text, UIHelpers::kColorMuted);
		ImGui::TextWrapped("No memory breakpoints set. Enter an address in hex and click 'Add' below to monitor memory access.");
		ImGui::PopStyleColor();
		return;
	}
	ImGuiListClipper c;
	static int selected = -1;
	c.Begin(break_point_hash.size());
	char buf[5] = {0};
	while (c.Step()) {

		for (int i = c.DisplayStart; i < c.DisplayEnd; i++) {
			MemBPData_t& data = break_point_hash[i];
			snprintf(buf, sizeof(buf), "%lx", (unsigned long)data.addr);
			ImGui::PushID(i);
			if (ImGui::Selectable(buf, selected == i)) {
				selected = i;
			}
			ImGui::PopID();
			if (ImGui::BeginPopupContextItem()) {
				selected = i;

				ImGui::TextUnformatted("MemBP.BPType"_lc);
				if (ImGui::Button("HexEditors.ContextMenu.MonitorRead"_lc)) {
					target_addr = i;
					data.enableWrite = 0;
					data.records.clear();
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::Button("HexEditors.ContextMenu.MonitorWrite"_lc)) {
					data.enableWrite = true;
					target_addr = i;
					data.records.clear();
					ImGui::CloseCurrentPopup();
				}
				ImGui::Separator();
				if (ImGui::Button("MemBP.Delete"_lc)) {
					data.records.clear();
					if (target_addr == i) {
						target_addr = -1;
					}
					break_point_hash.erase(break_point_hash.begin() + i);
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}
}

void Breakpoints::DrawFindContent() {
	if (target_addr == -1) {
		ImGui::TextColored(~UIHelpers::kColorWarning, "%s", "MemBP.NoBPHint"_lc);
		return;
	}
	int write = break_point_hash[target_addr].enableWrite;
	static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;
	ImGui::Text("MemBP.MonitoringHint"_lc,
		break_point_hash[target_addr].addr);
	ImGui::SameLine();
	static const char* fx = "";
	if (ImGui::Button("MemBP.ClearRec"_lc)) {
		break_point_hash[target_addr].records.clear();
	}
	if (ImGui::BeginTable("##outputtable", 2, flags)) {
		ImGui::TableSetupScrollFreeze(0, 1);
		ImGui::TableSetupColumn("MemBP.ColPC"_lc);
		ImGui::TableSetupColumn("");
		ImGui::TableHeadersRow();
		int i = 0;
		for (auto kv : break_point_hash[target_addr].records) {
			uint32_t pc_addr = kv.first;
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			// Clickable address → left-click navigates CodeViewer; right-click offers memory jump
			UIHelpers::ClickableAddress(pc_addr, UIHelpers::JumpTarget::Code);
			ImGui::TableSetColumnIndex(1);
			ImGui::PushID(i++);
			if (ImGui::Button("MemBP.ViewCallstack"_lc)) {
				fx = kv.second.stacktrace.c_str();
				SDL_ShowSimpleMessageBox(0, "", fx, 0);
			}
			ImGui::PopID();
		}
		ImGui::EndTable();
	}
}


void Breakpoints::SetupHooks() {
	SetupHook(on_memory_read, [&](casioemu::MMU& sender, MemoryEventArgs& mea) {
		if (break_on_cv) {
			if (target_addr == -1) {
				return;
			}
			MemBPData_t& bp = break_point_hash.at(target_addr);
			if (bp.addr == mea.offset && !bp.enableWrite) {
				SetDebugbreak();
			}
		}
		else {
			TryTrigBp(mea.offset, 0);
		}
	});
	SetupHook(on_memory_write, [&](casioemu::MMU& sender, MemoryEventArgs& mea) {
		if (break_on_cv) {
			if (target_addr == -1) {
				return;
			}
			MemBPData_t& bp = break_point_hash.at(target_addr);
			if (bp.addr == mea.offset && bp.enableWrite) {
				SetDebugbreak();
			}
		}
		else {
			TryTrigBp(mea.offset, 1);
		}
	});
	SetupHook(on_instruction, [&](casioemu::CPU& sender, InstructionEventArgs& iea) {
	
		// ===== SP breakpoint =====
		
		if (break_on_sp) {
			bool trig = false;
	
			switch (reg_compare_mode) {
			case 1: trig = sender.reg_sp == target_sp; break;
			case 2: trig = sender.reg_sp != target_sp; break;
			case 3: trig = sender.reg_sp > target_sp; break;
			case 4: trig = sender.reg_sp < target_sp; break;
			case 5: trig = sender.reg_sp >= target_sp; break;
			case 6: trig = sender.reg_sp <= target_sp; break;
			}
	
			if (trig) {
				SetDebugbreak();
			}
		}
	
		// ===== REGISTER BREAKPOINT =====
		for (auto& bp : reg_bps) {
			if (!bp.enabled) continue;
	
			uint32_t cur = GetRegisterValue(sender, bp.reg);
	
			if (reg_first_run) {
				last_reg_value[bp.reg] = cur;
				continue;
			}
	
			bool trig = false;
	
			switch (bp.mode) {
			case 1: trig = cur == bp.value; break;
			case 2: trig = cur != bp.value; break;
			case 3: trig = cur > bp.value; break;
			case 4: trig = cur < bp.value; break;
			case 5: trig = cur >= bp.value; break;
			case 6: trig = cur <= bp.value; break;
			}
			
	
			if (trig and !last_break_regs[bp.reg]) {
				printf("Reg %d hit BP: %X\n", bp.reg, cur);
				last_break_regs[bp.reg] = true;
				SetDebugbreak();
			} else if (!trig and last_break_regs[bp.reg]) {
				last_break_regs[bp.reg] = false;
			}
			last_reg_value[bp.reg] = cur;
		}
	
		reg_first_run = false;
		membp_cv = this;
	});
}

void Breakpoints::TryTrigBp(uint32_t addr, bool write) {
	if (target_addr == -1) {
		return;
	}
	MemBPData_t& bp = break_point_hash.at(target_addr);
	if (bp.addr == addr && bp.enableWrite == write) {
		bp.records[(m_emu->chipset.cpu.reg_csr << 16) | m_emu->chipset.cpu.reg_pc] = Record{m_emu->chipset.cpu.GetBacktrace(), (unsigned int)(m_emu->chipset.cpu.reg_lcsr << 16) | m_emu->chipset.cpu.reg_lr};
	}
}

void Breakpoints::RenderCore() {
	if (ImGui::BeginTabBar("Breakpoints")) {
		if (ImGui::BeginTabItem("Memory")) {
			static char buf[10] = {0};
			ImGui::BeginChild("##srcollingmbp", ImVec2(0, break_on_cv ? ImGui::GetWindowHeight() - ImGui::GetTextLineHeightWithSpacing() * 6 : ImGui::GetWindowHeight() / 3));
			DrawContent();
			ImGui::EndChild();
			ImGui::SetNextItemWidth(ImGui::CalcTextSize("F").x * 8);
			ImGui::InputText(
				"##addressin",
				buf, 10, ImGuiInputTextFlags_CharsHexadecimal);
			ImGui::SameLine();
			if (ImGui::Button("MemBP.AddAddr"_lc)) {
				break_point_hash.push_back({.addr = (uint32_t)strtol(buf, nullptr, 16)});
			}
			ImGui::Checkbox("MemBP.BreakWhenHit"_lc,
				&break_on_cv);
			if (!break_on_cv) {
				ImGui::BeginChild("##findoutput");
				DrawFindContent();
				ImGui::EndChild();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Register")) {
			static char buf[10] = {0};
			ImGui::Combo("BP.RegCmpMode"_lc, &reg_compare_mode, "Disabled\0Equal\0Not Equal\0Greater\0Less\0Greater or Equal\0Less or Equal\0");
			ImGui::Separator();
			ImGui::SetNextItemWidth(ImGui::CalcTextSize("F").x * 8);
			if (ImGui::InputText(
					"##addressin2",
					buf, 10, ImGuiInputTextFlags_CharsHexadecimal)) {
				target_sp = (uint16_t)strtol(buf, nullptr, 16);
			}
			ImGui::SameLine();
			ImGui::Checkbox("BP.SPHint"_lc, &break_on_sp);
			
			// ===== REGISTER BREAKPOINT UI =====
			ImGui::Separator();
			ImGui::Text("Register Breakpoints");

			static int selected_reg = 0;
			static int selected_mode = 0;
			static int value = 0;

			const char* reg_names[] = {
				"SP","ER0","ER2","ER4","ER6","ER8","ER10","ER12","ER14","EA","LR","PC"
			};

			ImGui::Combo("Register", &selected_reg, reg_names, IM_ARRAYSIZE(reg_names));

			ImGui::Combo("Mode", &selected_mode,
				"Equal\0Not Equal\0Greater\0Less\0GreaterEq\0LessEq\0");

			//ImGui::InputInt("Value", &value);
			ImGui::InputScalar("ValueHex", ImGuiDataType_U32, &value, NULL, NULL, "%X");

			if (ImGui::Button("Add Reg BP")) {
				reg_bps.push_back({
					selected_reg,
					selected_mode + 1, // lệch index -> fix
					(uint32_t)value,
					true
				});
			}

			ImGui::Separator();

			for (int i = 0; i < (int)reg_bps.size(); i++) {
				auto& bp = reg_bps[i];

				ImGui::PushID(i);

				ImGui::Checkbox("##en", &bp.enabled);
				ImGui::SameLine();

				ImGui::Text("Reg %s | Mode %d | Val %X",
					reg_names[bp.reg],
					bp.mode,
					bp.value
				);

				ImGui::SameLine();
				if (ImGui::Button("Delete")) {
					reg_bps.erase(reg_bps.begin() + i);
					ImGui::PopID();
					break;
				}

				ImGui::PopID();
			}
			
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
}

void Breakpoints::ExternalAddBp(uint32_t addr, bool write) {
	break_point_hash.push_back({.enableWrite = write, .addr = addr});
	target_addr = break_point_hash.size() - 1;
}

void SetMemBp(uint32_t addr, bool write) {
	if (membp_cv) {
		membp_cv->ExternalAddBp(addr, write);
	}
}
