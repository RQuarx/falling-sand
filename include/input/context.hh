#pragma once
#include <bitset>
#include <cstdint>
#include <string>

#include <sdl/event.hh>

#include "sdl/typedefs.hh"

namespace kei::sdl { struct renderer; }


namespace kei::input
{
    enum class button_state : std::uint8_t
    {
        /** @brief the button is being held down */
        down,

        /**
         * @brief the button is on a transition state from being released to being pressed
         */
        pressed,

        /**
         * @brief the button is on a transition state from being released to being pressed
         */
        released,

        /** @brief the button is not being held down */
        up,
    };


    class context
    {
        using scancode_bitset = std::bitset<SDL_SCANCODE_COUNT>;


        template <typename Fn> class is_x_states
        {
            template <typename... T> class compare
            {
            public:
                compare(const context &ctx, Fn &&fn, std::tuple<T...> &&buttons)
                    : m_ctx { ctx }, m_fn { std::move(fn) }, m_buttons { std::move(buttons) }
                {
                }


                [[nodiscard]]
                auto
                operator()(auto &&...vals) -> bool
                {
                    return std::apply(
                        [&](auto &&...buttons)
                        {
                            return ((
                                        [&]
                                        {
                                            auto state { std::invoke(m_fn, m_ctx, buttons) };
                                            return ((state == vals) || ...);
                                        }())
                                    && ...);
                        },
                        m_buttons);
                }


            private:
                const context   &m_ctx;
                Fn               m_fn;
                std::tuple<T...> m_buttons;
            };

        public:
            is_x_states(const context &ctx, Fn fn) : m_ctx { ctx }, m_fn { std::move(fn) } {}


            template <typename... T>
            [[nodiscard]]
            auto
            operator[](T &&...buttons) -> compare<T...>
            {
                return compare { m_ctx, std::move(m_fn),
                                 std::make_tuple(std::forward<T>(buttons)...) };
            }

        private:
            const context &m_ctx;
            Fn             m_fn;
        };

    public:
        void connect_signals(sdl::event_handler &event_handler, sdl::renderer &renderer);
        void on_frame_begin();


        [[nodiscard]] auto get_cursor_current_position() const noexcept -> sdl::fpoint;
        [[nodiscard]] auto get_cursor_previous_position() const noexcept -> sdl::fpoint;


        [[nodiscard]] auto get_mouse_wheel_direction() const noexcept -> sdl::fpoint;
        [[nodiscard]] auto get_mouse_button_state(int button_index) const noexcept -> button_state;
        [[nodiscard]] auto get_key_state(SDL_Scancode scancode) const noexcept -> button_state;

        [[nodiscard]] auto get_text_input() const noexcept -> const std::string &;


        [[nodiscard]]
        auto is_button_states() const noexcept
            -> is_x_states<decltype(&context::get_mouse_button_state)>;

        [[nodiscard]]
        auto is_key_states() const noexcept -> is_x_states<decltype(&context::get_key_state)>;

    private:
        struct
        {
            sdl::fpoint current_position;
            sdl::fpoint previous_position;
        } m_cursor;

        std::array<button_state, 6> m_mouse_buttons;
        sdl::fpoint                 m_mouse_wheel;

        scancode_bitset m_keys_up;
        scancode_bitset m_keys_pressed;
        scancode_bitset m_keys_released;
        scancode_bitset m_keys_down;

        std::string m_text_input;


        auto update(const sdl::event &ev, sdl::renderer &renderer) -> sdl::event_return;
    };
}
