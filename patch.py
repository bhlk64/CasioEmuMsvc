import re

with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/casioemu.cpp", "r") as f:
    content = f.read()

# Replace std::exit(0) in SDL_WINDOWEVENT_CLOSE
new_close_logic = """			case SDL_WINDOWEVENT_CLOSE: {
				extern SDL_Window* window; // This is the debugger window
				if (event.window.windowID == SDL_GetWindowID(emulator.window)) {
					emulator.return_to_home_requested = true;
				} else if (window && event.window.windowID == SDL_GetWindowID(window)) {
					std::exit(0);
				}
				break;
			}"""
content = re.sub(r'case SDL_WINDOWEVENT_CLOSE:\s*emulator\.Shutdown\(\);\s*std::exit\(0\);\s*break;', new_close_logic, content)

# Remove t3.detach()
content = content.replace("t3.detach();", "// t3.detach(); removed to allow joining")

# Wrap the main loop
# We need to find `if (argv_map["model"].empty()) {` and start the outer loop there.
start_str = 'if (argv_map["model"].empty()) {'
outer_loop_start = '''
	while (true) {
		if (argv_map["model"].empty()) {'''
content = content.replace(start_str, outer_loop_start, 1)

# At the end, we replace the cleanup with a check to break or continue
end_str = '''	running = false;
	if (t3.joinable()) {
		t3.join();
	}
	if (bg_txt) {
		SDL_DestroyTexture(bg_txt);
	}
#ifdef ENABLE_SENTRY
	sentry_close();
#endif
  DiscordRPC::Shutdown();
	return 0;
};'''

new_end_str = '''	running = false;
	if (t3.joinable()) {
		t3.join();
	}
	if (bg_txt) {
		SDL_DestroyTexture(bg_txt);
	}
	
	if (emulator.return_to_home_requested) {
		argv_map["model"] = "";
		continue;
	} else {
		break;
	}
	} // end while(true)
	
#ifdef ENABLE_SENTRY
	sentry_close();
#endif
  DiscordRPC::Shutdown();
	return 0;
};'''
content = content.replace(end_str, new_end_str)

with open("/Users/luongtoi/Documents/Projects/CasioEmuMsvc/CasioEmuMsvc/CasioEmuMsvc/src/casioemu.cpp", "w") as f:
    f.write(content)
