#pragma once

#include "Utils/Property.hpp"

/*
    Defines a look of any character
*/
class Looks
{
    VH_PROPERTY_RO(std::string, Hair);
    VH_PROPERTY_RO(std::string, Face);
    VH_PROPERTY_RO(std::string, Body);
    VH_PROPERTY_RO(std::string, Hands);
    VH_PROPERTY_RO(std::string, Legs);

public:
    Looks() {}
    Looks(const std::string& hair, const std::string& face,
        const std::string& body, const std::string& hands, const std::string& legs)
        : mHair(hair)
        , mFace(face)
        , mBody(body)
        , mHands(hands)
        , mLegs(legs)
    {
    }
};
