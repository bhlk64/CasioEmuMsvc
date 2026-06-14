import re

with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/casioemu.cpp", "r") as f:
    content = f.read()

content = content.replace("emulator.return_to_home_requested = true;\\n					emulator.Shutdown();", "emulator.return_to_home_requested = true;\\n					emulator.Shutdown();")

with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/casioemu.cpp", "w") as f:
    f.write(content)
