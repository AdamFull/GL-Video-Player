#pragma once

#include "GShader.hpp"

class GRenderObject
{
public:
    virtual void render(GShader shader) = 0;
};

