#pragma once
#include "sdl/typedefs.hh"


namespace kei::core
{
    struct cell
    {
        friend class grid;

    private:
        bool m_is_updated { false };

    public:
        int element;

        sdl::fpoint velocity;
        float       temperature;
        int         life;


        constexpr cell(int element = -1) noexcept : element { element } {}
    };
}
