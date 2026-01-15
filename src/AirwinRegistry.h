/*
 * AirwinConsolidated - an adaptation of the airwindows effect suite
 * for various open source clients
 *
 * This source released under the MIT License, found in ~/LICENSE.md.
 *
 * Copyright 2023 by the authors as described in the github transaction log
 */

#ifndef AIRWINCONSOLIDATED_AIRWINREGISTRY_H
#define AIRWINCONSOLIDATED_AIRWINREGISTRY_H

#include <string>
#include <memory>
#include <utility>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <iostream>
#include <functional>
#include <algorithm>

#include "airwin_consolidated_base.h"

struct AirwinRegistry
{
    struct awReg
    {
        std::string name;
        std::string category;
        int catChrisOrdering;
        bool isMono;
        std::string whatText;
        int nParams{0};
        std::string firstCommitDate;
        std::function<std::unique_ptr<AirwinConsolidatedBase>()> generator{
            []() { return nullptr; }};
        int ordering{-1};
        std::vector<std::string> collections{};
    };
    static std::vector<awReg> registry;
    static std::set<std::string> categories;
    static std::map<std::string, std::vector<std::string>> fxByCategory;
    static std::map<std::string, std::vector<std::string>> fxByCategoryChrisOrder;
    static std::vector<int> fxAlphaOrdering, fxChrisOrdering;
    static std::unordered_map<std::string, int> nameToIndex;

    static std::map<std::string, std::unordered_set<std::string>> namesByCollection;

    static int registerAirwindow(const awReg &r)
    {
        if (r.whatText.empty())
        {
            std::cout << r.name << " / " << r.category << " missing what text" << std::endl;
        }
        registry.emplace_back(r);

        return registry.size();
    }

    static void filterAndRebuildRegistry(std::function<bool(const awReg &)> removePredicate)
    {
        nameToIndex.clear();
        categories.clear();
        fxByCategory.clear();
        fxByCategoryChrisOrder.clear();
        fxAlphaOrdering.clear();
        fxChrisOrdering.clear();
        registry.erase(std::remove_if(registry.begin(), registry.end(), removePredicate),
                       registry.end());
        completeRegistry();
    }

    static int completeRegistry()
    {
        int idx{0};
        for (const auto &r : registry)
        {
            nameToIndex[r.name] = idx;
            categories.insert(r.category);
            fxByCategory[r.category].push_back(r.name);
            fxByCategoryChrisOrder[r.category].push_back(r.name);
            idx++;
        }

        for (auto &[cat, dat] : fxByCategory)
        {
            std::sort(dat.begin(), dat.end(), [](const auto &a, const auto &b) { return a < b; });
        }

        for (auto &[cat, dat] : fxByCategoryChrisOrder)
        {
            std::sort(dat.begin(), dat.end(), [](const auto &a, const auto &b) {
                // this double lookup is a bit of a bummer
                auto ai = AirwinRegistry::nameToIndex[a];
                auto bi = AirwinRegistry::nameToIndex[b];
                return AirwinRegistry::registry[ai].catChrisOrdering <
                       AirwinRegistry::registry[bi].catChrisOrdering;
            });
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

        idx = 0;
        for (const auto &[cat, fxs] : fxByCategoryChrisOrder)
        {
            for (const auto &fx : fxs)
            {
                fxChrisOrdering.push_back(nameToIndex[fx]);
                idx++;
            }
        }

        for (const auto &r : registry)
        {
            for (const auto &c : r.collections)
            {
                namesByCollection[c].insert(r.name);
            }
        }

        return 0;
    }

    static int neighborChrisIndexFor(int t, int dir)
    {
        int chrisIndex{-1};
        for (size_t i = 0; i < fxChrisOrdering.size(); ++i)
        {
            if (fxChrisOrdering[i] == t)
            {
                chrisIndex = (int)i;
                break;
            }
        }
        if (chrisIndex < 0)
        {
            return neighborIndexFor(t, dir);
        }
        auto pos = chrisIndex;
        pos += dir;
        if (pos < 0)
            pos = fxChrisOrdering.size() - 1;
        if (pos >= (int)fxChrisOrdering.size())
            pos = 0;
        return fxChrisOrdering[pos];
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
#endif // AIRWINCONSOLIDATED_AIRWINREGISTRY_H
