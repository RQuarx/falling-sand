#pragma once
#include <functional>

#include "simulation/board.hh"


namespace kei::sim
{
    struct draw_command
    {
        sdl::point point;
        element_id id;
        bool       erase;
    };


    class system
    {
    public:
        using draw_fn
            = std::function<void(sdl::size, std::vector<draw_command> &)>;


        system();


        [[nodiscard]] auto board() const noexcept -> const board &;


        auto enqueue_draw(const draw_fn &fn) -> system &;
        void update(float dt);

        [[nodiscard]]
        auto get_present_cells() const
            -> std::span<const std::reference_wrapper<cell>>;

    private:
        class board m_board;

        std::vector<std::reference_wrapper<cell>> m_cells;
        std::vector<draw_command>                 m_draw_queue;
    };
}
