//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2017-04-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <Tungsten/Tungsten.hpp>
#include <Tungsten/Uniform.hpp>

class CircleBarShaderProgram
{
public:
    void setup();

    Tungsten::ProgramHandle program;

    Tungsten::Uniform<Xyz::Matrix3F> view_matrix;
    GLuint position_attribute;
};
