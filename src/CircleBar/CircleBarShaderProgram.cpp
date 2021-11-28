//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2017-04-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "CircleBarShaderProgram.hpp"

#include "Tungsten/ShaderTools.hpp"
#include "CircleBar-vert.glsl.hpp"
#include "CircleBar-frag.glsl.hpp"

void CircleBarShaderProgram::setup()
{
    program = Tungsten::createProgram();
    auto vertexShader = Tungsten::createShader(GL_VERTEX_SHADER,
                                               CircleBar_vert);
    Tungsten::attachShader(program, vertexShader);

    auto fragmentShader = Tungsten::createShader(GL_FRAGMENT_SHADER,
                                                 CircleBar_frag);
    Tungsten::attachShader(program, fragmentShader);
    Tungsten::linkProgram(program);
    Tungsten::useProgram(program);

    position_attribute = Tungsten::getVertexAttribute(program, "a_Position");
    view_matrix = Tungsten::getUniform<Xyz::Matrix3f>(
        program, "u_ViewMatrix");
}
