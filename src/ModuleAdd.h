#include "autogen_airwin/Chamber.h"
typedef AW2RModule<airwin2rack::Chamber::Chamber, airwin2rack::Chamber::kNumParameters> Chamber_model;
int Chamber_throwaway = addAirwin(rack::createModel<Chamber_model, AW2RModuleWidget<Chamber_model>>("Chamber"));

#include "autogen_airwin/Galactic.h"
typedef AW2RModule<airwin2rack::Galactic::Galactic, airwin2rack::Galactic::kNumParameters> Galactic_model;
int Galactic_throwaway = addAirwin(rack::createModel<Galactic_model, AW2RModuleWidget<Galactic_model>>("Galactic"));

#include "autogen_airwin/Hombre.h"
typedef AW2RModule<airwin2rack::Hombre::Hombre, airwin2rack::Hombre::kNumParameters> Hombre_model;
int Hombre_throwaway = addAirwin(rack::createModel<Hombre_model, AW2RModuleWidget<Hombre_model>>("Hombre"));

