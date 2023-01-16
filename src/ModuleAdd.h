#include "autogen_airwin/Chamber.h"
int Chamber_unused = AW2RModule::registerAirwindow({"Chamber", airwin2rack::Chamber::kNumParameters, []() { return std::make_unique<airwin2rack::Chamber::Chamber>(0); }});
#include "autogen_airwin/Galactic.h"
int Galactic_unused = AW2RModule::registerAirwindow({"Galactic", airwin2rack::Galactic::kNumParameters, []() { return std::make_unique<airwin2rack::Galactic::Galactic>(0); }});
#include "autogen_airwin/Hombre.h"
int Hombre_unused = AW2RModule::registerAirwindow({"Hombre", airwin2rack::Hombre::kNumParameters, []() { return std::make_unique<airwin2rack::Hombre::Hombre>(0); }});
