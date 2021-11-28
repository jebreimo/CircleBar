#include <iostream>

#include <Tungsten/Tungsten.hpp>
#include "CircleBarShaderProgram.hpp"
#include "VertexArray.hpp"

void add_circle_strip(VertexArray<Xyz::Vector2f>& array,
                      uint32_t num_points,
                      Xyz::Vector2f center,
                      float outer_radius,
                      float inner_radius)
{
    constexpr auto PI = Xyz::Constants<float>::PI;
    VertexArrayBuilder builder(array);
    builder.reserve_vertexes(num_points * 2 + 2);
    builder.reserve_indexes(num_points * 2 + 2);
    for (uint16_t i = 0; i < num_points + 1; ++i)
    {
        auto angle = PI * (0.5f - 2.0f * float(i) / float(num_points));
        auto v = Xyz::makeVector2(cos(angle), sin(angle));
        builder.add_vertex(center + v * inner_radius);
        builder.add_vertex(center + v * outer_radius);
        builder.add_index(i * 2);
        builder.add_index(i * 2 + 1);
    }
}

class CircleBar : public Tungsten::EventLoop
{
public:
    void onStartup(Tungsten::SdlApplication& app) override
    {
        VertexArray<Xyz::Vector2f> array;
        add_circle_strip(array, 400, {0.0f, 0.0f}, 0.9f, 0.8f);
        m_count = int32_t(array.indexes.size());
        m_vertex_array = Tungsten::generateVertexArray();
        Tungsten::bindVertexArray(m_vertex_array);

        m_buffers = Tungsten::generateBuffers(2);
        set_buffers(m_buffers[0], m_buffers[1], array);
        m_program.setup();
        Tungsten::defineVertexAttributePointer(m_program.position_attribute, 2,
                                               GL_FLOAT, false, 0, 0);
        Tungsten::enableVertexAttribute(m_program.position_attribute);
    }

    void onUpdate(Tungsten::SdlApplication& app) override
    {
        constexpr auto PI = Xyz::Constants<float>::PI;
        m_percentage = 0.5f * (1.0f + cos(PI * float(SDL_GetTicks()) / 10000.0f));
    }

    void onDraw(Tungsten::SdlApplication& app) override
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto[w, h] = app.windowSize();
        float xs = 1.0, ys = 1.0;
        if (w > h)
            xs = float(h) / float(w);
        else
            ys = float(w) / float(h);
        m_program.view_matrix.set(Xyz::Matrix3f{xs, 0, 0,
                                                0, ys, 0,
                                                0, 0, 1});
        auto count = GLsizei(float(m_count) * m_percentage) & ~GLsizei(1);
        Tungsten::drawElements(GL_TRIANGLE_STRIP, count,
                               GL_UNSIGNED_SHORT, 0);
    }

private:
    std::vector<Tungsten::BufferHandle> m_buffers;
    Tungsten::VertexArrayHandle m_vertex_array;
    CircleBarShaderProgram m_program;
    int32_t m_count = 0;
    float m_percentage = 1.0;
};

int main(int argc, char* argv[])
{
    try
    {
        Tungsten::SdlApplication app("CircleBar",
                                     std::make_unique<CircleBar>());
        app.parseCommandLineOptions(argc, argv);
        app.run();
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << "\n";
        return 1;
    }

    return 0;
}
