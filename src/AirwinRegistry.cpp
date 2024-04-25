/*
* Airwin2Rack - an adaptation of the airwindows effect suite for VCVRack
*
* This source released under the MIT License, found in ~/LICENSE.md.
*
* Copyright 2023 by the authors as described in the github transaction log
*/

#include "AirwinRegistry.h"

#include <cmrc/cmrc.hpp>

CMRC_DECLARE(awdoc_resources);

std::vector<AirwinRegistry::awReg> AirwinRegistry::registry;
std::set<std::string> AirwinRegistry::categories;
std::vector<int> AirwinRegistry::fxAlphaOrdering;
std::map<std::string, std::vector<std::string>> AirwinRegistry::fxByCategory;
std::map<std::string, std::vector<std::string>> AirwinRegistry::fxByCategoryChrisOrder;
std::unordered_map<std::string, int> AirwinRegistry::nameToIndex;

std::string AirwinRegistry::documentationStringFor(int index)
{
    auto nm = registry[index].name;
    auto fs = cmrc::awdoc_resources::get_filesystem();
    auto doc =  std::string("res/awpdoc/") + nm + ".txt";

    try
    {
        if (fs.is_file(doc))
        {
            auto fn = fs.open(doc);
            return std::string(fn.begin(), fn.end());
        }
    }
    catch (std::exception &e)
    {
    }
    return "";
}

void AirwinRegistry::dumpStatsToStdout()
{
    std::cout << "Airwin Registry Stats\n";

#define LONGEST_LABEL 0
#if LONGEST_LABEL
    std::set<std::string> params;
    size_t longest{0};
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

#define LABEL_BY_PLUG 1
#if LABEL_BY_PLUG

    for (const auto &ord : fxAlphaOrdering)
    {
        const auto &r = registry[ord];
        auto fx = r.generator();
        std::cout << r.name << " (" << r.category << ")\n";
        for (int i=0; i<r.nParams; ++i)
        {
            char txt[256];
            fx->getParameterName(i, txt);
            std::cout << "    " << i << ": " << txt << "\n";
        }
    }
#endif

    std::cout << std::endl;
}

#include "ModuleAdd.h"
