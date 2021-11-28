//****************************************************************************
// Copyright © 2017 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2017-04-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#version 100

attribute vec2 a_Position;
uniform mat3 u_ViewMatrix;

void main()
{
    highp vec3 pos2d = u_ViewMatrix * vec3(a_Position, 1.0);
    gl_Position = vec4(pos2d.x, pos2d.y, 0.0, 1.0);
}
