g
	template <>
	const int Screen<HW_CLASSWIZ_II>::N_ROW = 63;
	template <>
	const int Screen<HW_CLASSWIZ_II>::ROW_SIZE = 32;
	template <>
	const int Screen<HW_CLASSWIZ_II>::OFFSET = 32;
	template <>
	const int Screen<HW_CLASSWIZ_II>::ROW_SIZE_DISP = 24;
	template <>
	const int Screen<HW_CLASSWIZ_II>::SPR_MAX = 19;

	template <>
	const int Screen<HW_CLASSWIZ>::N_ROW = 63;
	template <>
	const int Screen<HW_CLASSWIZ>::ROW_SIZE = 32;
	template <>
	const int Screen<HW_CLASSWIZ>::OFFSET = 32;
	template <>
	const int Screen<HW_CLASSWIZ>::ROW_SIZE_DISP = 24;
	template <>
	const int Screen<HW_CLASSWIZ>::SPR_MAX = 21;

	template <>
	const int Screen<HW_ES_PLUS>::N_ROW = 31;
	template <>
	const int Screen<HW_ES_PLUS>::ROW_SIZE = 16;
	template <>
	const int Screen<HW_ES_PLUS>::OFFSET = 16;
	template <>
	const int Screen<HW_ES_PLUS>::ROW_SIZE_DISP = 12;
	template <>
	const int Screen<HW_ES_PLUS>::SPR_MAX = 19;

	// that's meaningless, just make compiler happy xd
	template <>
	const int Screen<HW_EPS6800>::N_ROW = 31;
	template <>
	const int Screen<HW_EPS6800>::ROW_SIZE = 16;
	template <>
	const int Screen<HW_EPS6800>::OFFSET = 16;
	template <>
	const int Screen<HW_EPS6800>::ROW_SIZE_DISP = 12;
	template <>
	const int Screen<HW_EPS6800>::SPR_MAX = 19;

	template <>
	const SpriteBitmap Screen<HW_CLASSWIZ_II>::sprite_bitmap[] = {
		{"rsd_pixel", 0, 0},
		{"rsd_s", 0x01, 0x01},
		{"rsd_math", 0x01, 0x03},
		{"rsd_d", 0x01, 0x04},
		{"rsd_r", 0x01, 0x05},
		{"rsd_g", 0x01, 0x06},
		{"rsd_fix", 0x01, 0x07},
		{"rsd_sci", 0x01, 0x08},
		{"rsd_e", 0x01, 0x0A},
		{"rsd_cmplx", 0x01, 0x0B},
		{"rsd_angle", 0x01, 0x0C},
		{"rsd_wdown", 0x01, 0x0D},
		{"rsd_verify", 0x01, 0x0E},
		{"rsd_left", 0x01, 0x10},
		{"rsd_down", 0x01, 0x11},
		{"rsd_up", 0x01, 0x12},
		{"rsd_right", 0x01, 0x13},
		{"rsd_pause", 0x01, 0x15},
		{"rsd_sun", 0x01, 0x16}};

	template <>
	const SpriteBitmap Screen<HW_CLASSWIZ>::sprite_bitmap[] = {
		{"rsd_pixel", 0, 0},
		{"rsd_s", 0x01, 0x00},
		{"rsd_a", 0x01, 0x01},
		{"rsd_m", 0x01, 0x02},
		{"rsd_sto", 0x01, 0x03},
		{"rsd_math", 0x01, 0x05},
		{"rsd_d", 0x01, 0x06},
		{"rsd_r", 0x01, 0x07},
		{"rsd_g", 0x01, 0x08},
		{"rsd_fix", 0x01, 0x09},
		{"rsd_sci", 0x01, 0x0A},
		{"rsd_e", 0x01, 0x0B},
		{"rsd_cmplx", 0x01, 0x0C},
		{"rsd_angle", 0x01, 0x0D},
		{"rsd_wdown", 0x01, 0x0F},
		{"rsd_left", 0x01, 0x10},
		{"rsd_down", 0x01, 0x11},
		{"rsd_up", 0x01, 0x12},
		{"rsd_right", 0x01, 0x13},
		{"rsd_pause", 0x01, 0x15},
		{"rsd_sun", 0x01, 0x16}};

	template <>
	const SpriteBitmap Screen<HW_ES_PLUS>::sprite_bitmap[] = {
		{"rsd_pixel", 0, 0},
		{"rsd_s", 0x10, 0x00},
		{"rsd_a", 0x04, 0x00},
		{"rsd_m", 0x10, 0x01},
		{"rsd_sto", 0x02, 0x01},
		{"rsd_rcl", 0x40, 0x02},
		{"rsd_stat", 0x40, 0x03},
		{"rsd_cmplx", 0x80, 0x04},
		{"rsd_mat", 0x40, 0x05},
		{"rsd_vct", 0x01, 0x05},
		{"rsd_d", 0x20, 0x07},
		{"rsd_r", 0x02, 0x07},
		{"rsd_g", 0x10, 0x08},
		{"rsd_fix", 0x01, 0x08},
		{"rsd_sci", 0x20, 0x09},
		{"rsd_math", 0x40, 0x0A},
		{"rsd_down", 0x08, 0x0A},
		{"rsd_up", 0x80, 0x0B},
		{"rsd_disp", 0x10, 0x0B}};

	template <>
	const SpriteBitmap Screen<HW_EPS6800>::sprite_bitmap[] = {
		{"rsd_pixel", 0, 0},
		{"rsd_s", 0x10, 0x00},
		{"rsd_a", 0x04, 0x00},
		{"rsd_m", 0x10, 0x01},
		{"rsd_sto", 0x02, 0x01},
		{"rsd_rcl", 0x40, 0x02},
		{"rsd_stat", 0x40, 0x03},
		{"rsd_cmplx", 0x80, 0x04},
		{"rsd_mat", 0x40, 0x05},
		{"rsd_vct", 0x01, 0x05},
		{"rsd_d", 0x20, 0x07},
		{"rsd_r", 0x02, 0x07},
		{"rsd_g", 0x10, 0x08},
		{"rsd_fix", 0x01, 0x08},
		{"rsd_sci", 0x20, 0x09},
		{"rsd_math", 0x40, 0x0A},
		{"rsd_down", 0x08, 0x0A},
		{"rsd_up", 0x80, 0x0B},
		{"rsd_disp", 0x10, 0x0B}};

	template <HardwareId hardware_id>
	void Screen<hardware_id>::Initialise() {
		if (!inited) {
			renderer = emulator.GetRenderer();
			interface_texture = emulator.GetInterfaceTexture();
			sprite_info.resize(SPR_MAX);
			for (int ix = 0; ix != SPR_MAX; ++ix)
				sprite_info[ix] = emulator.ModelDefinition.sprites[sprite_bitmap[ix].name];

			ink_colour = emulator.ModelDefinition.ink_color;
			if constexpr (hardware_id == HW_TI) {
				screen_buffer = new uint8_t[192 * 9];
				// TODO: remove this
				memset(screen_buffer, 0, 192 * 9);
				// fillRandomData(screen_buffer, 192*9);
			}
			else {
				screen_buffer = new uint8_t[(N_ROW + 1) * ROW_SIZE];
				fillRandomData(screen_buffer, (N_ROW + 1) * ROW_SIZE);
			}
			if constexpr (hardware_id == HW_CLASSWIZ || hardware_id == HW_CLASSWIZ_II) {
				region_power.Setup(
					0xF03D, 1, "Screen/Power", this,
					[](MMURegion* region, size_t offset) {
						return ((Screen*)region->userdata)->screen_power;
					},
					[](MMURegion* region, size_t offset, uint8_t data) {
						bool a = (((Screen*)region->userdata)->screen_power & 1) ^ (data & 1);
						((Screen*)region->userdata)->screen_power = data & 0xf;
						if (a && ((data & 1) == 0)) { // 关闭屏幕
							((Screen*)region->userdata)->Uninitialise();
						}
						else {
							((Screen*)region->userdata)->Initialise();
						}
					},
					emulator);
			}
			if constexpr (hardware_id == HW_CLASSWIZ_II) {
				screen_buffer1 = new uint8_t[(N_ROW + 1) * ROW_SIZE];
				fillRandomData(screen_buffer1, (N_ROW + 1) * ROW_SIZE);
			}
			inited = true;
		}
		if constexpr (hardware_id == HW_TI) {
			auto pp = emulator.chipset.QueryInterface<IPortProvider>();
			pp->SetPortOutputCallback(7, [&](uint8_t data) {
				ti_port7 = data;
			});
			pp->SetPortOutputCallback(5, [&](uint8_t data) {
				// ti_port5 = data;
				if (ti_a0 && !(data & 0x40)) {
					if ((data & 0x10)) {
						auto bit_off = ti_col;
						auto off = bit_off + ti_page * 192;
						if (off > 192 * 9) {
							return;
						}
						if (off > 192 * 8) {
							std::cout << std::dec << off - 192 * 8 << " <- 0x" << std::hex << ti_port7 << "\n";
						}
						screen_buffer[off] = ti_port7;
						ti_col++;
						if (ti_col >= 192) {
							ti_col = 0;
							ti_page++;
						}
					}
					else {
						auto data = ti_port7;
						switch (ti_port_status) {
						case 0: {
							auto dh = data >> 4;
							if (dh == 0) {
								ti_col = (ti_col & 0xf0) | (data & 0xf);
							}
							else if (dh == 1) {
								ti_col = (ti_col & 0xf) | ((data & 0xf) << 4);
							}
							else if ((dh & 0b1100) == 0b0100) {
								// std::cout << "Set Scroll line " << (data & 0x3f) << "\n";
							}
							else if (dh == 0b1011) {
								// std::cout << "Set page  " << (data & 0xf) << "\n";
								ti_page = (data & 0xf);
							}
							else if ((data >> 3) == 17) {
								// std::cout << "Set addressing mode\n";
							}
							else if ((data >> 2) == 58) {
								// std::cout << "Set bias\n";
							}
							else if ((data >> 2) == 40) {
								// std::cout << "Set frame rate\n";
							}
							else if ((data >> 1) == 82) {
								// std::cout << "Clear all display segments\n";
							}
							else if ((data >> 1) == 83) {
								// std::cout << "Set inverse display\n";
							}
							else if ((data & 0xf9) == 0xc0) {
								// std::cout << "Set Com Seg Scan Direction\n";
							}
							else if (data == 0xe3) {
								// std::cout << "Nop\n";
							}
							else if (data == 0xe2) {
								// std::cout << "Software reset\n";
							}
							else if (data == 0xaf) {
								// std::cout << "Enabled screen!\n";
								ti_enabled = 1;
							}
							else if (data == 0x81) {
								ti_port_status = 1;
							}
							else if (data == 0xae) {
								// std::cout << "Disabled screen!\n";
								ti_enabled = 0;
							}
							else {
								std::cout << "[Screen][Warn] Unknown ST7525 command: 0x" << std::hex << (int)data << "\n";
							}
							break;
						}
						case 1:
							// std::cout << "Set contrast!\n";
							ti_contrast = data;
							ti_port_status = 0;
							break;
						}
					}
				}
				ti_a0 = (data & 0x40);
			});
			return;
		}
		if (!(hardware_id == HW_CLASSWIZ || hardware_id == HW_CLASSWIZ_II) || (!enabled_2 && (screen_power & 1))) {
			if constexpr (hardware_id != HW_CLASSWIZ_II) {
				region_buffer.Setup(
					0xF800, (N_ROW + 1) * ROW_SIZE, "Screen/Buffer", this, [](MMURegion* region, size_t offset) {
				offset -= region->base;
				if (offset % ROW_SIZE >= ROW_SIZE_DISP)
					return (uint8_t)0;
				return ((Screen*)region->userdata)->screen_buffer[offset]; },
					[](MMURegion* region, size_t offset, uint8_t data) {
					offset -= region->base;
					if (offset % ROW_SIZE >= ROW_SIZE_DISP)
						return;

                                        auto this_obj = (Screen*)region->userdata;
                                        this_obj->screen_buffer[offset] = data; },
					emulator);
			}
			else {
				region_buffer.Setup(
					0xF800, (N_ROW + 1) * ROW_SIZE, "Screen/Buffer", this,
					[](MMURegion* region, size_t offset) {
						offset -= region->base;
						if (offset % ROW_SIZE >= ROW_SIZE_DISP)
							return (uint8_t)0;
						if (((Screen*)region->userdata)->screen_select & 0x04) {
							return ((Screen*)region->userdata)->screen_buffer1[offset];
						}
						else {
							return ((Screen*)region->userdata)->screen_buffer[offset];
						}
					},
					[](MMURegion* region, size_t offset, uint8_t data) {
						offset -= region->base;
						if (offset % ROW_SIZE >= ROW_SIZE_DISP)
							return;

						auto this_obj = (Screen*)region->userdata;
						if (!(this_obj->screen_mode & 0x40)) {
							this_obj->screen_buffer1[offset] = this_obj->screen_buffer[offset] = data;
							return;
						}
						if (this_obj->screen_select & 0x04) {
							this_obj->screen_buffer1[offset] = data;
						}
						else {
							this_obj->screen_buffer[offset] = data;
						}
					},
					emulator);
				if (!emulator.ModelDefinition.real_hardware) {
					// region_buffer.Setup(
					//	0xF800, (N_ROW + 1) * ROW_SIZE, "Screen/Buffer", this,
					//	[](MMURegion* region, size_t offset) {
					//		offset -= region->base;
					//		if (offset % ROW_SIZE >= ROW_SIZE_DISP)
					//			return (uint8_t)0;
					//		return ((Screen*)region->userdata)->screen_buffer[offset];
					//	},
					//	[](MMURegion* region, size_t offset, uint8_t data) {
					//		offset -= region->base;
					//		if (offset % ROW_SIZE >= ROW_SIZE_DISP)
					//			return;

					//                auto this_obj = (Screen*)region->userdata;
					//                this_obj->screen_buffer[offset] = data;
					//        },
					//        emulator);
					region_buffer1.Setup(
						0x89000, (N_ROW + 1) * ROW_SIZE, "Screen/Buffer1", this,
						[](MMURegion* region, size_t offset) {
							offset -= region->base;
							if (offset % ROW_SIZE >= ROW_SIZE_DISP)
								return (uint8_t)0;
							return ((Screen*)region->userdata)->screen_buffer1[offset];
						},
						[](MMURegion* region, size_t offset, uint8_t data) {
							offset -= region->base;
							if (offset % ROW_SIZE >= ROW_SIZE_DISP)
								return;

							auto this_obj = (Screen*)region->userdata;
							this_obj->screen_buffer1[offset] = data;
						},
						emulator);
				}
			}
			if constexpr (hardware_id == HW_CLASSWIZ || hardware_id == HW_CLASSWIZ_II) {
				region_range.Setup(0xF030, 1, "Screen/Range", &screen_range, MMURegion::DefaultRead<uint8_t, 0x2F>,
					MMURegion::DefaultWrite<uint8_t, 0x2F>, emulator);
			}
			else {
				region_range.Setup(0xF030, 1, "Screen/Range", &screen_range, MMURegion::DefaultRead<uint8_t, 0x07>,
					MMURegion::DefaultWrite<uint8_t, 0x07>, emulator);
			}

			if constexpr (hardware_id == HW_CLASSWIZ_II) {
				region_mode.Setup(
					0xF031, 1, "Screen/Mode", this,
					[](MMURegion* region, size_t offset) {
						auto screen = ((Screen*)region->userdata);
						return screen->screen_mode;
					},
					[](MMURegion* region, size_t offset, uint8_t data) {
						auto screen = ((Screen*)region->userdata);
						auto old = screen->screen_mode & 0b1000;
						auto new_ = data & 0b1000;
						if (old ^ new_) {
							auto sb = screen->screen_buffer;
							for (int iy = 0; iy != (N_ROW + 1); ++iy) {
								for (int ix = 0; ix != ROW_SIZE_DISP; ++ix) {
									sb[ix + iy * ROW_SIZE] = bit_lookup_table[sb[(ix) + iy * ROW_SIZE]];
								}
							}
							for (int iy = 0; iy != (N_ROW + 1); ++iy) {
								for (int ix = 0; ix != (ROW_SIZE_DISP / 2); ++ix) {
									std::swap(sb[ix + iy * ROW_SIZE], sb[(ROW_SIZE_DISP - 1 - ix) + iy * ROW_SIZE]);
								}
							}
							if constexpr (hardware_id == HW_CLASSWIZ_II) {
								sb = screen->screen_buffer1;
								for (int iy = 0; iy != (N_ROW + 1); ++iy) {
									for (int ix = 0; ix != ROW_SIZE_DISP; ++ix) {
										sb[ix + iy * ROW_SIZE] = bit_lookup_table[sb[(ix) + iy * ROW_SIZE]];
									}
								}
								for (int iy = 0; iy != (N_ROW + 1); ++iy) {
									for (int ix = 0; ix != (ROW_SIZE_DISP / 2); ++ix) {
										std::swap(sb[ix + iy * ROW_SIZE], sb[(ROW_SIZE_DISP - 1 - ix) + iy * ROW_SIZE]);
									}
								}
							}
						}
						screen->screen_mode = data & 127;
					},
					emulator);
			}
			else if constexpr (hardware_id == HW_CLASSWIZ) {
				region_mode.Setup(0xF031, 1, "Screen/Mode", &screen_mode, MMURegion::DefaultRead<uint8_t, 63>,
					MMURegion::DefaultWrite<uint8_t, 63>, emulator);
			}
			else {
				region_mode.Setup(0xF031, 1, "Screen/Mode", &screen_mode, MMURegion::DefaultRead<uint8_t, 0x07>,
					MMURegion::DefaultWrite<uint8_t, 0x07>, emulator);
			}
			if constexpr (hardware_id == HW_CLASSWIZ || hardware_id == HW_CLASSWIZ_II) {
				region_contrast.Setup(0xF032, 1, "Screen/Contrast", &screen_contrast, MMURegion::DefaultRead<uint8_t, 0x3F>,
					MMURegion::DefaultWrite<uint8_t, 0x3F>, emulator);
				region_unk1.Setup(
					0xF03E, 1, "Screen/Unk1", this,
					[](MMURegion* region, size_t offset) {
						return (uint8_t)0;
					},
					[](MMURegion* region, size_t offset, uint8_t data) {
						((Screen*)region->userdata)->emulator.chipset.mmu.WriteData(0xF817, data);
					},
					emulator);
				region_unk2.Setup(
					0xF03F, 1, "Screen/Unk2", this,
					[](MMURegion* region, size_t offset) {
						return (uint8_t)0;
					},
					[](MMURegion* region, size_t offset, uint8_t data) {
						((Screen*)region->userdata)->emulator.chipset.mmu.WriteData(0xF817, data);
					},
					emulator);
			}
			else {
				region_contrast.Setup(0xF032, 1, "Screen/Contrast", &screen_contrast, MMURegion::DefaultRead<uint8_t, 0x1f>,
					MMURegion::DefaultWrite<uint8_t, 0x1f>, emulator);
			}

			if constexpr (hardware_id == HW_CLASSWIZ || hardware_id == HW_CLASSWIZ_II) {
				region_select.Setup(0xF037, 1, "Screen/Select", &screen_select, MMURegion::DefaultRead<uint8_t, 0x04 | 1>,
					MMURegion::DefaultWrite<uint8_t, 0x04 | 1>, emulator);

				region_brightness.Setup(0xF033, 1, "Screen/Brightness", &screen_brightness, MMURegion::DefaultRead<uint8_t, 0x07>,
					MMURegion::DefaultWrite<uint8_t, 0x07>, emulator);

				/*
cwx中F03B的值应该是由屏幕扫描和F035/F036决定的
1.每行扫描的时间大概是( [0xF034] * 25 ) us
2.F03B的mask是3，屏幕每扫描( [0xF036] == 0 ? 64 : [0xF035] )行后F03B的基础值会在0和3之间切换，如果F036是0的话这个循环的半周期和屏幕扫描应该是对齐的，也就是F03B的基础值切换后对应屏幕的第0行扫描（注：F035.0始终为1）
3.扫描屏幕的第0行 (对应bit0?) 及第32行 (对应bit1?) 时，F03B对应的bit会反转

n为行扫描计数，[0xF03B] = ( ( n / ( [0xF036] == 0 ? 64 : [0xF035] ) ) % 2 ? 3 : 0 ) ^ ( n % 64 == 0 ? 1 : ( n % 64 == 32 ? 2 : 0)  )
				*/

				region_scan_report_op1.Setup(0xF035, 1, "Screen/ScanReportOption1", &screen_scan_report_op1, MMURegion::DefaultRead<uint8_t, 0x1E>,
					MMURegion::DefaultWrite<uint8_t, 0x1E>, emulator);

				region_scan_report_en.Setup(0xF036, 1, "Screen/ScanReportOptionEnable", &screen_scan_report_en, MMURegion::DefaultRead<uint8_t, 0b1001>,
					MMURegion::DefaultWrite<uint8_t, 0b1001>, emulator);

				region_scan_report.Setup(0xF03B, 1, "Screen/ScanReport", &screen_scan_report, MMURegion::DefaultRead<uint8_t, 0x3>,
					MMURegion::IgnoreWrite, emulator);
			}
			else {
				screen_scan_report_op1 = 0x17;
				screen_scan_report_en = 1;
			}

			if constexpr (hardware_id == HardwareId::HW_ES_PLUS) {
				region_refresh_rate.Setup(0xF034, 1, "Screen/Unknown_F034", &unk_f034, MMURegion::DefaultRead<uint8_t, 0b11>,
					MMURegion::DefaultWrite<uint8_t, 0b11>, emulator);
			}
			else {
				region_offset.Setup(0xF039, 1, "Screen/DSPOFST", &screen_offset, MMURegion::DefaultRead<uint8_t, 0x3F>,
					MMURegion::DefaultWrite<uint8_t, 0x3F>, emulator);

				// 25us
				region_refresh_rate.Setup(0xF034, 1, "Screen/RefreshRate", &screen_refresh_rate, MMURegion::DefaultRead<uint8_t, 0x7F>,
					MMURegion::DefaultWrite<uint8_t, 0x7F>, emulator);
			}
			enabled_2 = true;
		}
	}

	template <HardwareId hardware_id>
	void Screen<hardware_id>::Uninitialise() {
		fillRandomData(screen_buffer, (N_ROW + 1) * ROW_SIZE);
		if constexpr (hardware_id == HW_CLASSWIZ_II) {
			fillRandomData(screen_buffer1, (N_ROW + 1) * ROW_SIZE);
		}
		if constexpr (hardware_id != HW_CLASSWIZ_II) {
			region_buffer.Kill();
		}
		else {
			if (!emulator.ModelDefinition.real_hardware) {
				region_buffer.Kill();
				region_buffer1.Kill();
			}
			else {
				region_buffer.Kill();
			}
		}
		screen_range = 0;
		region_range.Kill();
		screen_mode = 0;
		region_mode.Kill();
		screen_contrast = 0;
		region_contrast.Kill();
		if constexpr (hardware_id == HW_CLASSWIZ || hardware_id == HW_CLASSWIZ_II) {
			screen_select = 0;
			region_select.Kill();
			screen_scan_report_op1 = 0;
			region_scan_report_op1.Kill();
			screen_scan_report_en = 0;
			region_scan_report_en.Kill();
			screen_scan_report = 0;
			region_scan_report.Kill();
			region_unk1.Kill();
			region_unk2.Kill();
			screen_brightness = 0;
			region_brightness.Kill();
		}
		screen_refresh_rate = 0;
		region_refresh_rate.Kill();
		if constexpr (hardware_id != HardwareId::HW_ES_PLUS) {
			screen_offset = 0;
			region_offset.Kill();
		}
		enabled_2 = false;
	}
	// Function to capture the current screen, save as PNG file and copy to clipboard
	void CaptureScreenshot(SDL_Renderer* renderer, const std::vector<SDL_Rect>& spriteRects, const std::vector<SDL_Rect>& pixelRects) {
		// Get current time to generate a unique filename
		std::time_t t = std::time(nullptr);
		std::tm tm = *std::localtime(&t);
		std::ostringstream filename;

		filename << "screenshot-"
				 << std::put_time(&tm, "%Y-%m-%d-%H-%M-%S-")
				 << util::Random::uniform_uint32(0, 999)
				 << ".png";

		// Calculate the bounding box of the rendering area from both sprite and pixel rectangles
		int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

		// Traverse all sprite rectangles
		for (const auto& rect : spriteRects) {
			minX = std::min(minX, rect.x);
			minY = std::min(minY, rect.y);
			maxX = std::max(maxX, rect.x + rect.w);
			maxY = std::max(maxY, rect.y + rect.h);
		}

		// Traverse all pixel rectangles (representing the screen pixels)
		for (const auto& rect : pixelRects) {
			minX = std::min(minX, rect.x);
			minY = std::min(minY, rect.y);
			maxX = std::max(maxX, rect.x + rect.w);
			maxY = std::max(maxY, rect.y + rect.h);
		}

		// Calculate the width and height of the capture area
		int captureWidth = maxX - minX;
		int captureHeight = maxY - minY;

		// Create a surface to capture the screen content
		SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, captureWidth, captureHeight, 32,
			0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);

		if (screenSurface != nullptr) {
			// Define the area to capture
			SDL_Rect captureRect = {minX, minY, captureWidth, captureHeight};

			// Copy the renderer to the surface
			if (SDL_RenderReadPixels(renderer, &captureRect, SDL_PIXELFORMAT_RGBA32,
					screenSurface->pixels, screenSurface->pitch) == 0) {

#ifdef __ANDROID__
				// Save to MediaStore
				auto str = filename.str();
				bool success = saveImageToMediaStore(screenSurface->pixels, screenSurface->w, screenSurface->h, screenSurface->pitch, str.c_str());
				if (!success) {
					SDL_Log("Error saving screenshot using MediaStore API");
				}
				else {
					SDL_Log("Screenshot saved successfully with MediaStore API");
				}

				// Copy to clipboard on Android using JNI
				JNIEnv* env = (JNIEnv*)SDL_AndroidGetJNIEnv();
				jobject activity = (jobject)SDL_AndroidGetActivity();

				if (env && activity) {
					// Create a Java direct ByteBuffer from the pixel data
					jobject byteBuffer = env->NewDirectByteBuffer(screenSurface->pixels,
						screenSurface->h * screenSurface->pitch);

					// Call the Java method to copy to clipboard
					jclass activityClass = env->GetObjectClass(activity);
					jmethodID copyToClipboardMethod = env->GetMethodID(activityClass, "copyImageToClipboard",
						"(Ljava/nio/ByteBuffer;III)Z");

					if (copyToClipboardMethod != NULL) {
						jboolean result = env->CallBooleanMethod(activity, copyToClipboardMethod,
							byteBuffer, screenSurface->w,
							screenSurface->h, screenSurface->pitch);
						if (result) {
							SDL_Log("Screenshot copied to clipboard");
						}
						else {
							SDL_Log("Failed to copy screenshot to clipboard");
						}
					}
					else {
						SDL_Log("copyImageToClipboard method not found. Add it to your Java activity.");
					}

					env->DeleteLocalRef(byteBuffer);
					env->DeleteLocalRef(activityClass);
					env->DeleteLocalRef(activity);
				}
#else
				// Save to file on Windows/Desktop
				auto str = filename.str();
				if (IMG_SavePNG(screenSurface, str.c_str()) != 0) {
					SDL_Log("Error saving screenshot: %s", IMG_GetError());
				}
				else {
					SDL_Log("Screenshot saved to %s", str.c_str());
				}

// Copy to clipboard on Windows/Desktop
#ifdef _WIN32
				// Convert SDL_Surface to Windows DIB format for clipboard
				HDC hdcScreen = GetDC(NULL);
				HDC hdcMem = CreateCompatibleDC(hdcScreen);

				BITMAPINFOHEADER bi;
				ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
				bi.biSize = sizeof(BITMAPINFOHEADER);
				bi.biWidth = screenSurface->w;
				bi.biHeight = -screenSurface->h; // Negative for top-down
				bi.biPlanes = 1;
				bi.biBitCount = 32;
				bi.biCompression = BI_RGB;

				void* bits = NULL;
				HBITMAP hBitmap = CreateDIBSection(hdcMem, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &bits, NULL, 0);

				if (hBitmap) {
					// Copy pixels from SDL surface to DIB
					SelectObject(hdcMem, hBitmap);

					// Convert RGBA to BGRA and copy to DIB
					uint8_t* src = (uint8_t*)screenSurface->pixels;
					uint8_t* dst = (uint8_t*)bits;

					for (int y = 0; y < screenSurface->h; y++) {
						for (int x = 0; x < screenSurface->w; x++) {
							// RGBA to BGRA
							dst[0] = src[2]; // B
							dst[1] = src[1]; // G
							dst[2] = src[0]; // R
							dst[3] = src[3]; // A

							src += 4;
							dst += 4;
						}
					}

					// Copy to clipboard
					if (OpenClipboard(NULL)) {
						EmptyClipboard();
						SetClipboardData(CF_BITMAP, hBitmap);
						CloseClipboard();
						SDL_Log("Screenshot copied to clipboard");
					}
					else {
						SDL_Log("Failed to open clipboard");
						DeleteObject(hBitmap);
					}

					DeleteDC(hdcMem);
				}
				else {
					SDL_Log("Failed to create DIB section for clipboard");
				}

				ReleaseDC(NULL, hdcScreen);
#else
				// For other desktop platforms like Linux/macOS
				// Use platform-specific clipboard APIs if needed
				SDL_Log("Clipboard copy not implemented for this platform");
#endif
#endif
			}
			else {
				SDL_Log("Error capturing screen pixels: %s", SDL_GetError());
			}
			SDL_FreeSurface(screenSurface); // Free the surface after use
		}
		else {
			SDL_Log("Error creating surface: %s", SDL_GetError());
		}
	}

	void UpdatePreview(SDL_Renderer* renderer, ScreenMirror* sm, const std::vector<SDL_Rect>& spriteRects, const std::vector<SDL_Rect>& pixelRects) {

		// Calculate the bounding box of the rendering area from both sprite and pixel rectangles
		int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

		// Traverse all sprite rectangles
		for (const auto& rect : spriteRects) {
			minX = std::min(minX, rect.x);
			minY = std::min(minY, rect.y);
			maxX = std::max(maxX, rect.x + rect.w);
			maxY = std::max(maxY, rect.y + rect.h);
		}

		// Traverse all pixel rectangles (representing the screen pixels)
		for (const auto& rect : pixelRects) {
			minX = std::min(minX, rect.x);
			minY = std::min(minY, rect.y);
			maxX = std::max(maxX, rect.x + rect.w);
			maxY = std::max(maxY, rect.y + rect.h);
		}

		// Calculate the width and height of the capture area
		int captureWidth = maxX - minX;
		int captureHeight = maxY - minY;

		// Create a surface to capture the screen content
		SDL_Surface* screenSurface = SDL_CreateRGBSurface(0, captureWidth, captureHeight, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
		if (screenSurface != nullptr) {
			// Define the area to capture
			SDL_Rect captureRect = {minX, minY, captureWidth, captureHeight};

			// Copy the renderer to the surface
			if (SDL_RenderReadPixels(renderer, &captureRect, SDL_PIXELFORMAT_RGBA32, screenSurface->pixels, screenSurface->pitch) == 0) {
				sm->update(screenSurface->pixels, screenSurface->pitch);
			}
			else {
				SDL_Log("Error capturing screen pixels: %s", SDL_GetError());
			}
			SDL_FreeSurface(screenSurface); // Free the surface after use
		}
		else {
			SDL_Log("Error creating surface: %s", SDL_GetError());
		}
	}

	std::pair<int, int> GetSize(const std::vector<SDL_Rect>& spriteRects, const std::vector<SDL_Rect>& pixelRects) {

		// Calculate the bounding box of the rendering area from both sprite and pixel rectangles
		int minX = INT_MAX, minY = INT_MAX, maxX = INT_MIN, maxY = INT_MIN;

		// Traverse all sprite rectangles
		for (const auto& rect : spriteRects) {
			minX = std::min(minX, rect.x);
			minY = std::min(minY, rect.y);
			maxX = std::max(maxX, rect.x + rect.w);
			maxY = std::max(maxY, rect.y + rect.h);
		}

		// Traverse all pixel rectangles (representing the screen pixels)
		for (const auto& rect : pixelRects) {
			minX = std::min(minX, rect.x);
			minY = std::min(minY, rect.y);
			maxX = std::max(maxX, rect.x + rect.w);
			maxY = std::max(maxY, rect.y + rect.h);
		}

		// Calculate the width and height of the capture area
		int captureWidth = maxX - minX;
		int captureHeight = maxY - minY;
		return {captureWidth, captureHeight};
	}
	// Function to collect all sprite and pixel rectangles
	template <HardwareId hardware_id>
	void Screen<hardware_id>::Frame() {
		int x = 0;
		int screenWidth = 0, screenHeight = 0;

		// Get the renderer output size if not already available
		SDL_GetRendererOutputSize(renderer, &screenWidth, &screenHeight);

		if (!emulator.ModelDefinition.enable_new_screen) {
			SDL_SetTextureColorMod(interface_texture, ink_colour.r, ink_colour.g, ink_colour.b);
		}

		// Store all the rendering rectangles (sprites and pixel areas)
		std::vector<SDL_Rect> spriteRects;
		std::vector<SDL_Rect> pixelRects;

		// Set texture transparency and copy sprites as before
		for (int ix = 1; ix != SPR_MAX; ++ix) {
			SDL_SetTextureAlphaMod(interface_texture, Uint8(std::clamp((int)screen_ink_alpha[x], 0, 255)));
			x++;
			SDL_Rect tmp1 = sprite_info[ix].src;
			SDL_Rect tmp2 = sprite_info[ix].dest;
			SDL_RenderCopy(renderer, interface_texture, &tmp1, &tmp2);
			// Store the sprite rectangle for later
			spriteRects.push_back(sprite_info[ix].dest);
		}

		static constexpr auto SPR_PIXEL = 0;
		int gap = 1;
		
		SDL_Rect dest = Screen<hardware_id>::sprite_info[SPR_PIXEL].dest;
		
		for (int iy2 = 1; iy2 != (N_ROW + 1); ++iy2) {
		
			int x = 0;
		
			dest.x = sprite_info[SPR_PIXEL].dest.x;
			dest.y = sprite_info[SPR_PIXEL].dest.y + (iy2 - 1) * (sprite_info[SPR_PIXEL].src.h + gap);
		
			for (int ix = 0; ix != ROW_SIZE_DISP; ++ix) {
		
				for (uint8_t mask = 0x80; mask; mask >>= 1, dest.x += sprite_info[SPR_PIXEL].src.w + gap) {
		
					if (screen_ink_alpha[x + iy2 * 192] > 255) {
						SDL_SetTextureColorMod(
							interface_texture,
							std::max(0, ink_colour.r - (int)(screen_ink_alpha[x + iy2 * 192] - 255)),
							std::max(0, ink_colour.g - (int)((screen_ink_alpha[x + iy2 * 192] - 255) * 0.8)),
							std::max(0, ink_colour.b - (int)((screen_ink_alpha[x + iy2 * 192] - 255) * 0.1))
						);
						SDL_SetTextureAlphaMod(interface_texture, 255);
					}
		
					SDL_RenderCopy(renderer, interface_texture,
								   &sprite_info[SPR_PIXEL].src, &dest);
		
					x++;
				}
		
			}
		}
					else {
						SDL_SetTextureColorMod(interface_texture, ink_colour.r, ink_colour.g, ink_colour.b);
						SDL_SetTextureAlphaMod(interface_texture, Uint8(std::clamp((int)screen_ink_alpha[x + iy2 * 192], 0, 255)));
					}
					x++;
					SDL_Rect tmp1 = sprite_info[SPR_PIXEL].src;
					SDL_RenderCopy(renderer, interface_texture, &tmp1, &dest);
					// Store the pixel rectangle for later
					pixelRects.push_back(dest);
				}
			}
		}

		// If screenshot is requested, capture only the rendered screen region
		if (emulator.screenshot_requested.load()) {
			// Capture the region using both sprite and pixel rectangles
			CaptureScreenshot(renderer, spriteRects, pixelRects);
			emulator.screenshot_requested.store(false);
		}
		static ScreenMirror* mirror = nullptr;
		if (emulator.mirroring_requested.load()) {
			auto p = GetSize(spriteRects, pixelRects);
			auto sm = new ScreenMirror(p.first, p.second);
			sm->create();
			mirror = sm;
			emulator.mirroring_requested.store(false);
		}
		if (mirror) {
			UpdatePreview(renderer, mirror, spriteRects, pixelRects);
		}
	}

	template <HardwareId hardware_id>
	void Screen<hardware_id>::Reset() {
	}

	Peripheral* CreateScreen(Emulator& emulator) {
		switch (emulator.hardware_id) {
		case HW_FX_5800P:
		case HW_ES_PLUS:
			return new Screen<HW_ES_PLUS>(emulator);

		case HW_CLASSWIZ:
			return new Screen<HW_CLASSWIZ>(emulator);

		case HW_CLASSWIZ_II:
			return new Screen<HW_CLASSWIZ_II>(emulator);

		case HW_TI:
			return new Screen<HW_TI>(emulator);
		case HW_EPS6800:
			return new Screen<HW_EPS6800>(emulator);
		default:
			PANIC("Unknown hardware id\n");
		}
		std::abort();
	}
} // namespace casioemu
