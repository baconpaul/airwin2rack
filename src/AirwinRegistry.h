//
// Created by Paul Walker on 1/16/23.
//

#ifndef AIRWIN2RACK_AIRWINREGISTRY_H
#define AIRWIN2RACK_AIRWINREGISTRY_H

#include <string>
#include <memory>
#include <utility>
#include "airwin2rackbase.h"
#include <set>
#include <iostream>
#include <unordered_map>
#include <map>

struct AirwinRegistry
{
    struct awReg
    {
        std::string name;
        std::string category;
        std::string whatText;
        int nParams{0};
        std::function<std::unique_ptr<Airwin2RackBase>()> generator{[](){return nullptr;}};
        int ordering{-1};
    };
    static std::vector<awReg> registry;
    static std::set<std::string> categories;
    static std::map<std::string, std::set<std::string>> fxByCategory;
    static std::vector<int> fxOrdering;
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
                fxOrdering.push_back(nameToIndex[fx]);
                idx++;
            }
        }

        return 0;
    }

    static int neighborIndexFor(int t, int dir)
    {
        auto pos = registry[t].ordering;
        pos += dir;
        if (pos < 0) pos = fxOrdering.size()-1;
        if (pos >= (int)fxOrdering.size()) pos = 0;
        return fxOrdering[pos];
    }
    static int neighborIndexFor(const std::string &s, int dir)
    {
        return neighborIndexFor(nameToIndex[s], dir);
    }

    static void dumpStatsToStdout();
};
#endif // AIRWIN2RACK_AIRWINREGISTRY_H
