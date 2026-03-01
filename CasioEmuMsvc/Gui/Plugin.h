#pragma once
#include "Ui.hpp"
class PluginViewer : public UIWindow {
public:
	PluginViewer() : UIWindow("Plugin Manager"){}
	void RenderCore() override;
};
