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

struct AirwinRegistry
{
    struct awReg
    {
        std::string name;
        std::string category;
        std::string whatText;
        int nParams;
        std::function<std::unique_ptr<Airwin2RackBase>()> generator;
    };
    static std::vector<awReg> registry;
    static std::set<std::string> categories;
    static int registerAirwindow(const awReg &r)
    {
        if (r.nParams > 11)
        {
            // std::cout << "PROBLEM : " << r.name << " " << r.nParams << std::endl;
        }
        else
        {
            registry.emplace_back(r);
        }
        return registry.size();
    }
    static int completeRegistry()
    {
        for (const auto &r : registry)
            categories.insert(r.category);
        return 0;
    }
};
#endif // AIRWIN2RACK_AIRWINREGISTRY_H
