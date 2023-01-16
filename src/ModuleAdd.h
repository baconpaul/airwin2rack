#include "autogen_airwin/Chamber.h"
int Chamber_unused = AW2RModule::registerAirwindow("Chamber", []() { return new airwin2rack::Chamber::Chamber(0); });
#include "autogen_airwin/Galactic.h"
int Galactic_unused = AW2RModule::registerAirwindow("Galactic", []() { return new airwin2rack::Galactic::Galactic(0); });
#include "autogen_airwin/Hombre.h"
int Hombre_unused = AW2RModule::registerAirwindow("Hombre", []() { return new airwin2rack::Hombre::Hombre(0); });
