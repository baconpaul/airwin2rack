#include "Airwin2Rack.hpp"

#include <unordered_set>

static std::unique_ptr<std::unordered_set<rack::Model *>> models;

rack::Plugin *pluginInstance;
int addAirwin(rack::Model *m)
{
   if (!models)
      models = std::make_unique<std::unordered_set<rack::Model *>>();
   models->insert(m);
   return 0;
}


__attribute__((__visibility__("default"))) void init(rack::Plugin *p)
{
    pluginInstance = p;

    for (auto *m : *models)
    {
       p->addModel(m);
    }

}
