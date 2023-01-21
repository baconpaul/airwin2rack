/*
* Airwin2Rack - an adaptation of the airwindows effect suite for VCVRack
*
* This source released under the MIT License, found in ~/LICENSE.md.
*
* Copyright 2023 by the authors as described in the github transaction log
*/

#include "airwin2rackbase.h"
#include <iostream>

bool string2float(const char *txt, float &f)
{
    try {
        float v = std::stof(txt);
        f = v;
        return true;
    }
    catch (const std::exception &e)
    {
        return false;
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

    try {
        float v = std::stof(txt);
        // float2string ((float)(20.0 * log10 (value)), t, num);
        // so db = 20 log10(v)
        // v = 10^(db/20);
        f = std::pow(10, v/20);
        return true;
    }
    catch (const std::exception &e)
    {

        return false;
    }
    return false;
}