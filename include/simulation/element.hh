#pragma once
#include <string_view>
#include <vector>

#include "sdl/color.hh"


namespace kei::sim
{
    inline float G { 10'000 }; /* In cell/frame */


    using element_id = std::size_t;

    struct element_def
    {
        element_id       id { 0 };
        std::string_view name;

        sdl::color color;
        int        color_random_range;

        float mass;
        float friction;
    };


    namespace _impl
    {
        class element_registry
        {
        public:
            struct ids
            {
                ids(element_registry &reg);


                const element_id air;
                const element_id sand;
            };


            element_registry();

            [[nodiscard]] auto operator->() const -> const ids *;

            [[nodiscard]]
            auto operator[](element_id id) const -> const element_def &;


        private:
            std::vector<element_def> m_elements;
            ids                      m_ids;


            [[nodiscard]] auto operator<<(element_def elem) -> element_id;
        };
    }


    inline _impl::element_registry element_registry {};
}
