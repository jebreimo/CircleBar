//****************************************************************************
// Copyright © 2021 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2021-11-24.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <cstdint>
#include <vector>
#include <Xyz/XyzException.hpp>
#include <Tungsten/GlBuffer.hpp>

template <typename VertexType>
struct VertexArray
{
public:
    using Vertex = VertexType;

    std::vector<Vertex> vertexes;
    std::vector<uint16_t> indexes;

    [[nodiscard]]
    std::pair<const void*, size_t> vertex_buffer() const
    {
        return {vertexes.data(), vertexes.size() * sizeof(VertexType)};
    }

    [[nodiscard]]
    std::pair<const uint16_t*, size_t> index_buffer() const
    {
        return {indexes.data(), indexes.size() * sizeof(uint16_t)};
    }
};

template <typename Vertex>
void set_buffers(GLuint arrayBuffer, GLuint elementArrayBuffer,
                const VertexArray<Vertex>& array,
                GLenum usage = GL_STATIC_DRAW)
{
    Tungsten::bindBuffer(GL_ARRAY_BUFFER, arrayBuffer);
    auto [v_buf, v_size] = array.vertex_buffer();
    Tungsten::setBufferData(GL_ARRAY_BUFFER, v_size, v_buf, usage);
    Tungsten::bindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementArrayBuffer);
    auto [i_buf, i_size] = array.index_buffer();
    Tungsten::setBufferData(GL_ELEMENT_ARRAY_BUFFER, i_size, i_buf, usage);
}

template <typename Vertex>
class VertexArrayBuilder
{
public:
    explicit VertexArrayBuilder(VertexArray<Vertex>& array)
        : m_array(array),
          m_base_index(array.vertexes.size())
    {}

    void reserve_vertexes(size_t count)
    {
        m_array.vertexes.reserve(m_base_index + count);
    }

    void add_vertex(const Vertex& vertex)
    {
        m_array.vertexes.push_back(vertex);
    }

    [[nodiscard]]
    Vertex& vertex(size_t index)
    {
        return m_array.vertexes[index];
    }

    void reserve_indexes(size_t count)
    {
        m_array.indexes.reserve(m_array.indexes.size() + count);
    }

    void add_index(uint16_t a)
    {
        m_array.indexes.push_back(a + m_base_index);
    }

    void add_indexes(uint16_t a, uint16_t b, uint16_t c)
    {
        m_array.indexes.push_back(a + m_base_index);
        m_array.indexes.push_back(b + m_base_index);
        m_array.indexes.push_back(c + m_base_index);
    }
private:
    VertexArray<Vertex>& m_array;
    uint32_t m_base_index = 0;
};
