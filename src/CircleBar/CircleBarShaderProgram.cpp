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
    program = Tungsten::create_program();
    auto vertexShader = Tungsten::create_shader(GL_VERTEX_SHADER,
                                               CircleBar_vert);
    Tungsten::attach_shader(program, vertexShader);

    auto fragmentShader = Tungsten::create_shader(GL_FRAGMENT_SHADER,
                                                  CircleBar_frag);
    Tungsten::attach_shader(program, fragmentShader);
    Tungsten::link_program(program);
    Tungsten::use_program(program);

    position_attribute = Tungsten::get_vertex_attribute(program, "a_Position");
    view_matrix = Tungsten::get_uniform<Xyz::Matrix3F>(
        program, "u_ViewMatrix");
}
