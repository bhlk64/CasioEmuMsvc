#pragma once
#include "Ui.hpp"
class TestViewer : public UIWindow {
public:
	TestViewer() : UIWindow("Test"){}
	void RenderCore() override;
};
