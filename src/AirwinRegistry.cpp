//
// Created by Paul Walker on 1/16/23.
//


#include "AirwinRegistry.h"

std::vector<AirwinRegistry::awReg> AirwinRegistry::registry;
std::set<std::string> AirwinRegistry::categories;
std::vector<int> AirwinRegistry::fxOrdering;
std::map<std::string, std::set<std::string>> AirwinRegistry::fxByCategory;
std::unordered_map<std::string, int> AirwinRegistry::nameToIndex;


#include "ModuleAdd.h"
