extern "C"
void PluginInit(PluginApi* api)
{
    if (!api)
        return;

    PLUGINASSERTSTL((*api));

    if (!api->RegisterPlugin("example", "Example Plugin", 1))
        return;

    ExamplePlugin::g_api = api;

    ExamplePlugin::g_window = std::make_unique<ExamplePlugin::ExampleWindow>();
    api->AddWindow(ExamplePlugin::g_window.get());
}