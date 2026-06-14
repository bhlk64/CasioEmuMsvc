import re
with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/Emulator.hpp", "r") as f:
    content = f.read()

content = content.replace("std::atomic<bool> screenshot_requested{};", "std::atomic<bool> screenshot_requested{};\\n		std::atomic<bool> screenshot_taken{false};")
with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/Emulator.hpp", "w") as f:
    f.write(content)
