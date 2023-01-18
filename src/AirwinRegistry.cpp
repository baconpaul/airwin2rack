//
// Created by Paul Walker on 1/16/23.
//


#include "AirwinRegistry.h"

std::vector<AirwinRegistry::awReg> AirwinRegistry::registry;
std::set<std::string> AirwinRegistry::categories;
std::vector<int> AirwinRegistry::fxOrdering;
std::map<std::string, std::set<std::string>> AirwinRegistry::fxByCategory;
std::unordered_map<std::string, int> AirwinRegistry::nameToIndex;


void AirwinRegistry::dumpStatsToStdout()
{
    std::cout << "Airwin Registry Stats\n";

#define LONGEST_LABEL 1
#ifdef LONGEST_LABEL
    std::set<std::string> params;
    int longest{0};
    for (const auto &r : registry)
    {
        auto fx = r.generator();
        for (int i=0; i<r.nParams; ++i)
        {
            char txt[256];
            fx->getParameterName(i, txt);
            params.insert(txt);

            if (strlen(txt) > longest)
                longest = strlen(txt);
        }
    }
    for (const auto &p : params)
    {
        std::cout << "  " << p << std::endl;
    }

    std::cout << "Longest is " << longest << " chars\n";
#endif

    std::cout << std::endl;
}

#include "ModuleAdd.h"
