#include "Airwin2Rack.hpp"

#include <unordered_set>

static std::unique_ptr<std::unordered_set<rack::Model *>> models;

rack::Plugin *pluginInstance;

__attribute__((__visibility__("default"))) void init(rack::Plugin *p)
{
    pluginInstance = p;

    p->addModel(airwin2RackModel);
}
