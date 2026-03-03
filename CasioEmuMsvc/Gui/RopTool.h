#pragma once

#include "Ui.hpp"
#include <vector>
#include <string>
#include <map>
#include <set>
#include <cstdint>

// =========================================================
// Forward Declarations
// =========================================================
struct InsnEffect;
struct GadgetInfo;
struct DagNode;
struct DagAnalysis;
struct ChainEntry;
struct CompileResult;
struct Label;

// =========================================================
// Instruction Effect
// =========================================================
struct InsnEffect {
    uint32_t address = 0;
    uint16_t raw_bytes[2] = {};
    int byte_size = 2;

    uint16_t regs_read = 0;
    uint16_t regs_written = 0;

    bool reads_sp = false;
    bool writes_sp = false;
    bool writes_lr = false;
    bool writes_ea = false;
    bool writes_psw = false;
    bool reads_ea = false;
    bool reads_mem = false;
    bool writes_mem = false;
    bool reads_psw = false;

    bool is_ret = false;
    bool is_branch = false;

    uint32_t branch_target = 0;
    int sp_delta = 0;

    char disasm[64] = {};
};

// =========================================================
// Gadget Info
// =========================================================
struct GadgetInfo {
    enum GadgetType {
        GT_POP_RT,
        GT_POP_PC,
        GT_MOV_RT,
        GT_LOAD_RT,
        GT_STORE_RT,
        GT_ADD_SP_RT,
        GT_ARITH_RT,
        GT_LEA_RT,
        GT_CUSTOM_RT,
        GT_SWI
    };

    uint32_t address = 0;
    std::vector<InsnEffect> insns;

    uint16_t regs_read = 0;
    uint16_t regs_written = 0;

    bool modifies_sp = false;
    bool modifies_lr = false;
    bool modifies_ea = false;
    bool modifies_psw = false;

    int sp_delta = 0;
    int pop_count = 0;
    bool controllable_pc = false;

    int score = 0;
    GadgetType type = GT_CUSTOM_RT;

    std::string user_name;

    std::string GetDisasmString() const;
    std::string GetHexString() const;
    std::string GetTypeString() const;
    int ClobberCount() const;
    std::string GetClobberString() const;
};

// =========================================================
// DAG Node
// =========================================================
struct DagNode {
    enum RegState {
        UNKNOWN,
        CONTROLLED,
        CLOBBERED
    };

    int gadget_idx = -1;
    const GadgetInfo* gadget = nullptr;

    int false_successor = -1;
    int true_successor = -1;

    bool is_branch_point = false;
    bool is_skip = false;
    std::string skip_reason;

    int sp_offset = 0;

    RegState reg_state_in[16] = {};
    RegState reg_state_out[16] = {};
};

// =========================================================
// DAG Analysis
// =========================================================
struct DagAnalysis {
    struct PathResult {
        std::vector<int> node_indices;
        int final_sp_offset = 0;
        DagNode::RegState final_regs[16] = {};
    };

    std::vector<DagNode> nodes;
    std::vector<std::pair<int,int>> edges;
    std::vector<PathResult> paths;
};

// =========================================================
// Chain Entry
// =========================================================
struct ChainEntry {
    enum Type {
        CE_DATA,
        CE_PADDING,
        CE_GADGET_ADDR
    };

    Type type = CE_DATA;
    uint32_t value = 0;
    int byte_count = 0;
    int source_line = 0;

    const GadgetInfo* chosen_gadget = nullptr;
    std::string comment;
};

// =========================================================
// Compile Result
// =========================================================
struct CompileResult {
    struct GadgetChoice {
        int source_line = 0;
        std::string operation;

        const GadgetInfo* chosen = nullptr;
        int chosen_score = 0;
        int alternatives_count = 0;

        std::vector<std::pair<const GadgetInfo*, int>> all_candidates;
    };

    bool success = false;

    std::vector<ChainEntry> entries;
    std::vector<uint8_t> raw_bytes;

    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    std::vector<GadgetChoice> choices;
};

// =========================================================
// Label
// =========================================================
struct Label {
    std::string name;
    uint32_t address;
};

// =========================================================
// ROP Tool Window
// =========================================================
class UiWindow;
UIWindow* CreateRopToolWindow();