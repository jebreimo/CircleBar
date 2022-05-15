#include <iostream>

#include <Tungsten/ArrayBufferBuilder.hpp>
#include <Tungsten/Tungsten.hpp>
#include "CircleBarShaderProgram.hpp"

void add_circle_strip(Tungsten::ArrayBuffer<Xyz::Vector2F>& array,
                      uint32_t num_points,
                      Xyz::Vector2F center,
                      float outer_radius,
                      float inner_radius)
{
    constexpr auto PI = Xyz::Constants<float>::PI;
    Tungsten::ArrayBufferBuilder builder(array);
    builder.reserve_vertexes(num_points * 2 + 2);
    builder.reserve_indexes(num_points * 2 + 2);
    auto start_angle = PI / 2.0f;
    auto delta_angle = 2 * PI / float(num_points);
    for (uint32_t i = 0; i <= num_points; ++i)
    {
        auto angle = start_angle + delta_angle * float(i);
        auto p = Xyz::make_vector2(cos(angle), sin(angle));
        builder.add_vertex(center + p * inner_radius);
        builder.add_vertex(center + p * outer_radius);
        builder.add_index(i * 2);
        builder.add_index(i * 2 + 1);
    }
}

class CircleBar : public Tungsten::EventLoop
{
public:
    void on_startup(Tungsten::SdlApplication& app) override
    {
        Tungsten::ArrayBuffer<Xyz::Vector2F> array;
        add_circle_strip(array, 400, {0.0f, 0.0f}, 0.9f, 0.8f);
        m_count = int32_t(array.indexes.size());
        m_vertex_array = Tungsten::generate_vertex_array();
        Tungsten::bind_vertex_array(m_vertex_array);

        m_buffers = Tungsten::generate_buffers(2);
        set_buffers(m_buffers[0], m_buffers[1], array);
        m_program.setup();
        Tungsten::define_vertex_attribute_pointer(m_program.position_attribute, 2,
                                                  GL_FLOAT, false, 0, 0);
        Tungsten::enable_vertex_attribute(m_program.position_attribute);
        glEnable(GL_MULTISAMPLE);
    }

    void on_update(Tungsten::SdlApplication& app) override
    {
        constexpr auto PI = Xyz::Constants<float>::PI;
        m_percentage = 0.5f * (1.0f + cos(PI * float(SDL_GetTicks()) / 10000.0f));
    }

    void on_draw(Tungsten::SdlApplication& app) override
    {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto[w, h] = app.window_size();
        float xs = 1.0, ys = 1.0;
        if (w > h)
            xs = float(h) / float(w);
        else
            ys = float(w) / float(h);
        m_program.view_matrix.set(Xyz::Matrix3F{xs, 0, 0,
                                                0, ys, 0,
                                                0, 0, 1});
        auto count = GLsizei(round(float(m_count) * m_percentage)) & ~GLsizei(1);
        Tungsten::draw_elements(GL_TRIANGLE_STRIP, count,
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
        app.parse_command_line_options(argc, argv);
        auto params = app.window_parameters();
        params.gl_parameters.multi_sampling = {1, 2};
        app.set_window_parameters(params);
        app.run();
    }
    catch (std::exception& ex)
    {
        std::cout << ex.what() << "\n";
        return 1;
    }

    return 0;
}
