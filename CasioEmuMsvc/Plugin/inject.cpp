#include "pch.h"
#include "PluginApi.h"
#include <vector>
#include <string>
#include <format>
#include <iomanip>
#include <sstream>
using namespace std;
PluginApi* api = nullptr;
IMMU* mmu = nullptr;
uint32_t targetaddr;
vector<uint8_t> targetbytes;
vector<string> logs;
void log(const string& msg)
{
    logs.push_back(msg);
    if(logs.size()>100) logs.erase(logs.begin());
}
string tohex8(uint8_t v) 
{
    stringstream ss;
    ss << "0x" << setfill('0') << setw(2)
       << hex << uppercase << (int)v;
    return ss.str();
}
string tohex32(uint32_t v) 
{
    stringstream ss;
    ss << "0x" << setfill('0') << setw(4)
       << hex << uppercase << v;
    return ss.str();
}
bool catcherror(const string& s)
{
    for (char c : s)
    {
        if (isspace((unsigned char)c))
            continue;
        if (!isxdigit((unsigned char)c))
            return false;
    }
    return true;
}
class inject : public UIWindow 
{
    private:
        char inputaddr[16]="";
        char inputhex[196608] = "";
    public:
        inject() : UIWindow("Injector") {}
        void RenderCore() override
        {
            ImGui::Text("Inject hex");
            ImGui::Separator();
            ImGui::Text("Địa chỉ");
            ImGui::SameLine();
            ImGui::InputText("##addr", inputaddr, IM_ARRAYSIZE(inputaddr));
            ImGui::Text("Giá trị Hex");
            ImGui::InputTextMultiline(
                "##hex",
                inputhex,
                IM_ARRAYSIZE(inputhex),
                ImVec2(-FLT_MIN, 120)
            );
            if (ImGui::Button("Inject")) 
            {
                if (!catcherror(inputhex))
                {
                    log("Lỗi: Có ký tự không hợp lệ.");
                    return;
                }
                try
                {
                    targetaddr = stoul(inputaddr, nullptr, 16);
                }
                catch (...)
                {
                    log("Lỗi: Địa chỉ không hợp lệ.");
                    return;
                }
                targetaddr = stoul(inputaddr, nullptr, 16);
                targetbytes.clear();
                stringstream ss(inputhex);
                string bytestr;
                while (ss >> bytestr)
                    targetbytes.push_back((uint8_t)stoul(bytestr, nullptr, 16));
                for (size_t i = 0; i < targetbytes.size(); ++i)
                    mmu->WriteData(targetaddr + i, targetbytes[i]);
                log("Inject thành công.");
            }
            ImGui::Separator();
            ImGui::Text("Log:");
            ImGui::BeginChild("LogRegion", ImVec2(0,150), true);
            for(auto& log:logs) ImGui::TextUnformatted(log.c_str());
            if(ImGui::GetScrollY()>=ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);
            ImGui::EndChild();
            if(ImGui::Button("Xóa log")) logs.clear();
        }
};
extern "C" __declspec(dllexport)
void fPluginLoad(PluginApi* pApi)
{
    api = pApi;
    if(!api) return;
    if(!api->RegisterPlugin("injection", "injection plugin",1)) return;
    mmu=api->QueryInterface<IMMU>();
    if(!mmu) return;
    api->AddWindow(new inject());
    log("Plugin load thành công");
}
BOOL APIENTRY DllMain(HMODULE, DWORD, LPVOID) {
    return TRUE;
}