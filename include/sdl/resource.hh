#pragma once
#include <memory>


namespace kei::sdl
{
    template <typename T, void (*F)(T *)> class resource
    {
    public:
        resource() = default;
        resource(std::nullptr_t) : m_data { nullptr } {}
        resource(T *ptr) : m_data { ptr } {}

        auto operator=(const resource &) -> resource & = delete;
        auto
        operator=(resource &&other) noexcept -> resource &
        {
            m_data = std::move(other);
        }


        [[nodiscard]]
        auto
        release() -> T *
        {
            return m_data.release();
        }


        void
        reset(T *ptr = nullptr)
        {
            m_data.reset(ptr);
        }


        [[nodiscard]]
        auto
        get() const -> T *
        {
            return m_data.get();
        }

    private:
        std::unique_ptr<T, decltype([](T *ptr) { F(ptr); })> m_data;
    };
}
