/*
* Airwin2Rack - an adaptation of the airwindows effect suite for VCVRack
*
* This source released under the MIT License, found in ~/LICENSE.md.
*
* Copyright 2023 by the authors as described in the github transaction log
*/

#ifndef AIRWIN2RACK_AIRWINREGISTRY_H
#define AIRWIN2RACK_AIRWINREGISTRY_H

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <map>
#include <iostream>
#include <functional>

#include "airwin2rackbase.h"

struct AirwinRegistry
{
    struct awReg
    {
        std::string name;
        std::string category;
        int catChrisOrdering;
        std::string whatText;
        int nParams{0};
        std::function<std::unique_ptr<Airwin2RackBase>()> generator{[]() { return nullptr; }};
        int ordering{-1};
    };
    static std::vector<awReg> registry;
    static std::set<std::string> categories;
    static std::map<std::string, std::set<std::string>> fxByCategory;
    static std::vector<int> fxAlphaOrdering;
    static std::unordered_map<std::string, int> nameToIndex;

    static int registerAirwindow(const awReg &r)
    {
        if (r.nParams > 10)
        {
            if (r.name == "CStrip" || r.name == "Pafnuty")
            {
                // I know about these
            }
            else
            {
                std::cout << "PROBLEM : " << r.name << " " << r.nParams << std::endl;
            }
        }
        else if (r.whatText.empty())
        {
            // std::cout << r.name << " / " << r.category << " missing what text" << std::endl;
        }
        else
        {
            registry.emplace_back(r);
        }

        return registry.size();
    }
    static int completeRegistry()
    {
        int idx{0};
        for (const auto &r : registry)
        {
            nameToIndex[r.name] = idx;
            categories.insert(r.category);
            fxByCategory[r.category].insert(r.name);
            idx++;
        }

        idx = 0;
        for (const auto &[cat, fxs] : fxByCategory)
        {
            for (const auto &fx : fxs)
            {
                registry[nameToIndex[fx]].ordering = idx;
                fxAlphaOrdering.push_back(nameToIndex[fx]);
                idx++;
            }
        }

        return 0;
    }

    static int neighborIndexFor(int t, int dir)
    {
        auto pos = registry[t].ordering;
        pos += dir;
        if (pos < 0)
            pos = fxAlphaOrdering.size() - 1;
        if (pos >= (int)fxAlphaOrdering.size())
            pos = 0;
        return fxAlphaOrdering[pos];
    }
    static int neighborIndexFor(const std::string &s, int dir)
    {
        return neighborIndexFor(nameToIndex[s], dir);
    }

    static std::string documentationStringFor(int index);

    static void dumpStatsToStdout();
};
#endif // AIRWIN2RACK_AIRWINREGISTRY_H
