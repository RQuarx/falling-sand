#pragma once
#include <cstdint>
#include <vector>

#include "sdl/typedefs.hh"
#include "simulation/element.hh"


namespace kei::sim
{
    struct draw_command;


    class cursor
    {
    public:
        enum class shape : std::uint8_t
        {
            square,
            circle,
            triangle,
        } shape { shape::square };

        sdl::size  size;
        sdl::point pos;

        element_id draw_id;


        void draw(sdl::size                  board_size,
                  std::vector<draw_command> &cells_to_draw,
                  bool                       erase) const noexcept;

        void
        get_border_indices(sdl::size                board_size,
                           std::vector<sdl::point> &indices) const noexcept;

    private:
        [[nodiscard]]
        auto mf_is_inside(sdl::fpoint normalized_pos) const noexcept -> bool;
    };
}
