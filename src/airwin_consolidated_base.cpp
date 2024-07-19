/*
* AirwinConsolidated - an adaptation of the airwindows effect suite
* for various open source clients
*
* This source released under the MIT License, found in ~/LICENSE.md.
*
* Copyright 2023 by the authors as described in the github transaction log
*/

#include "airwin_consolidated_base.h"
#include <string>

float AirwinConsolidatedBase::defaultSampleRate{0.f};

bool string2float(const char *txt, float &f)
{
    char *pEnd = nullptr;
    float v = std::strtof(txt, &pEnd);
    if (pEnd == txt)
    {
        return false;

    } else if (v != INFINITY && v != -INFINITY && (v == HUGE_VALF || v == -HUGE_VALF))
    {
        return false;

    } else {
        f = v;
        return true;
    }
}


bool string2dBNorm(const char *txt, float &f)
{
    auto s = std::string(txt);
    if (s.find("-inf") != std::string::npos)
    {
        f = 0;
        return true;
    }

    if (auto ok = string2float(txt, f))
    {
        f = std::pow(10, f/20);
        return true;
    }
    return false;
}