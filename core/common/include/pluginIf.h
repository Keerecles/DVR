#ifndef PLUGINIF
#define PLUGINIF
#include "base.h"
class PluginIF {

public:
    PluginIF()
        : mClassName("PluginIF"){}

    ~PluginIF(){}

    std::string mClassName;

    std::string mDestnation;

    Base *ctxPlugin;
};
#endif // PLUGINIF

