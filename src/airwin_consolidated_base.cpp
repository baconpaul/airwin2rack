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
#if defined(__EXCEPTIONS) || defined(__cpp_exceptions) || defined(_CPPUNWIND)
    try
    {
        float v = std::stof(txt);
        f = v;
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
    }
#else
    char *pEnd = nullptr;
    float v = std::strtof(txt, &pEnd);
    if (pEnd == txt)
    {
        return false;
    }
    else if (v != INFINITY && v != -INFINITY && (v == HUGE_VALF || v == -HUGE_VALF))
    {
        return false;
    }
    else
    {
        f = v;
        return true;
    }
#endif
}

bool string2dBNorm(const char *txt, float &f)
{
    auto s = std::string(txt);
    if (s.find("-inf") != std::string::npos)
    {
        f = 0;
        return true;
    }

#if defined(__EXCEPTIONS) || defined(__cpp_exceptions) || defined(_CPPUNWIND)
    try
    {
        float v = std::stof(txt);
        // float2string ((float)(20.0 * log10 (value)), t, num);
        // so db = 20 log10(v)
        // v = 10^(db/20);
        f = std::pow(10, v / 20);
        return true;
    }
    catch (const std::exception &e)
    {

        return false;
    }
#else
    if (auto ok = string2float(txt, f))
    {
        f = std::pow(10, f / 20);
        return true;
    }
#endif
    return false;
}
