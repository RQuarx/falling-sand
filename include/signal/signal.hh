#pragma once
#include <atomic>
#include <functional>
#include <list>
#include <mutex>


namespace kei::sig
{
    template <typename T, typename... Ts> class signal;

    /**
     * @brief this class represents a connection between a signal
     *        and its callback or slot
     *
     * @tparam T     the return type of the callback function
     * @tparam Ts... the parameter types accepted by the
     *               callback function
     */
    template <typename T, typename... Ts> class connection
    {
        using slot_type      = std::function<T(Ts...)>;
        using container_type = std::list<connection<T, Ts...>>;

        friend class signal<T, Ts...>;

    public:
        connection(const connection &) = delete;
        connection(connection &&)      = default;

        auto operator=(const connection &) -> connection & = delete;
        auto operator=(connection &&) -> connection &      = default;


        auto
        operator()(Ts &&...params) const -> T
        {
            return m_slot(std::forward<Ts>(params)...);
        }


        /** @brief disconnects the connection from the signal */
        void
        disconnect()
        {
            if (m_iter != m_container.end()) m_iter = m_container.erase(m_iter);
        }

    private:
        slot_type                         m_slot;
        container_type                   &m_container;
        typename container_type::iterator m_iter;


        connection(slot_type slot, container_type &container)
            : m_slot { slot }, m_container { container }
        {
        }


        auto
        set_iterator(typename container_type::iterator iter) noexcept
            -> connection &
        {
            m_iter = iter;
            return *this;
        }
    };


    template <typename Iterator, typename T, typename... Ts>
    class slot_iterator_base
    {
    public:
        using connection    = connection<T, Ts...>;
        using base_iterator = Iterator;
        using slot_type     = std::function<T(Ts...)>;

        using iterator_category = std::input_iterator_tag;
        using value_type        = slot_type;
        using pointer           = slot_type *;
        using reference         = slot_type &;


        slot_iterator_base(base_iterator current, base_iterator end) noexcept
            : m_current { current }, m_end { end }
        {
        }


        auto
        operator++() noexcept -> slot_iterator_base &
        {
            return ++m_current, *this;
        }


        auto
        operator++(int) noexcept -> slot_iterator_base
        {
            slot_iterator_base tmp { *this };
            ++(*this);
            return tmp;
        }


        auto
        operator!=(const slot_iterator_base &other) const noexcept
        {
            return m_current != other.m_current;
        }


        auto
        operator==(const slot_iterator_base &other) const noexcept
        {
            return m_current == other.m_current;
        }


        auto
        operator*() const -> slot_type
        {
            return *m_current;
        }


    private:
        Iterator m_current;
        Iterator m_end;
    };

    template <typename T, typename... Ts>
    using slot_iterator
        = slot_iterator_base<typename std::list<connection<T, Ts...>>::iterator,
                             T,
                             Ts...>;

    template <typename T, typename... Ts>
    using const_slot_iterator = slot_iterator_base<
        typename std::list<connection<T, Ts...>>::const_iterator,
        T,
        Ts...>;


    /**
     * @brief the signal class manages a collection of callback functions and
     *        provides a couple of interfaces to them
     *
     * @tparam T       the return type of the callback functions
     * @tparam Ts... the parameter types accepted by the
     *                     callback functions
     */
    template <typename T, typename... Ts> class signal
    {
    public:
        using slot_type = std::function<T(Ts...)>;
        using emit_result
            = std::conditional_t<std::is_same_v<T, void>, void, std::vector<T>>;
        using connection = connection<T, Ts...>;


        signal() = default;


        /**
         * @brief connect a new slot (callback function) to the signal
         *
         * @param slot the callback function to connect
         * @return a reference to the created connection
         */
        auto
        connect(slot_type slot) noexcept -> connection &
        {
            std::scoped_lock lock { m_mtx };

            m_slots.emplace_back(connection { slot, m_slots });
            return m_slots.back().set_iterator(std::prev(m_slots.end()));
        }


        /**
         * @brief emits the signal, invoking all connected slots or callbacks
         *
         * @param params the arguments to forward to the callback functions
         * @return a vector of return values from the functions
         */
        auto
        emit(Ts &&...params) -> emit_result
        {
            std::scoped_lock lock { m_mtx };

            if constexpr (std::is_same_v<T, void>)
                for (auto &slot : m_slots) slot(std::forward<Ts>(params)...);
            else
            {
                if (m_collect_retvals)
                {
                    std::vector<T> return_values;
                    return_values.reserve(m_slots.size());

                    for (auto &slot : m_slots)
                        return_values.emplace_back(
                            slot(std::forward<Ts>(params)...));

                    return return_values;
                }

                for (auto &slot : m_slots) slot(std::forward<Ts>(params)...);
                return {};
            }
        }


        /**
         * @brief enables or disables the collection of return values
         *        from the emitted slots
         *
         * @param collect @e true to collect return values, @e false otherwise
         * @return a reference to @e *this
         */
        auto
        set_collect_return_values(bool collect) noexcept -> signal &
        {
            return m_collect_retvals.exchange(collect), *this;
        }


        [[nodiscard]]
        auto
        begin() noexcept -> slot_iterator<T, Ts...>
        {
            return { m_slots.begin(), m_slots.end() };
        }


        [[nodiscard]]
        auto
        end() noexcept -> slot_iterator<T, Ts...>
        {
            return { m_slots.end(), m_slots.end() };
        }


        [[nodiscard]]
        auto
        begin() const noexcept -> const_slot_iterator<T, Ts...>
        {
            return { m_slots.cbegin(), m_slots.cend() };
        }


        [[nodiscard]]
        auto
        end() const noexcept -> const_slot_iterator<T, Ts...>
        {
            return { m_slots.cend(), m_slots.cend() };
        }


        [[nodiscard]]
        auto
        cbegin() const noexcept -> const_slot_iterator<T, Ts...>
        {
            return { m_slots.cbegin(), m_slots.cend() };
        }


        [[nodiscard]]
        auto
        cend() const noexcept -> const_slot_iterator<T, Ts...>
        {
            return { m_slots.cend(), m_slots.cend() };
        }

    private:
        mutable std::mutex    m_mtx;
        std::list<connection> m_slots;

        std::atomic<bool> m_collect_retvals { true };
    };


    template <typename Signal>
    class signal_connect
    {
        Signal &m_signal;

    public:
        using slot_type = typename Signal::slot_type;

        explicit signal_connect(Signal &signal) noexcept : m_signal { signal }
        {
        }


        template <typename S>
        auto
        connect(S &&slot) noexcept -> Signal::connection &
            requires std::convertible_to<S, slot_type>
        {
            return m_signal.connect(std::forward<S>(slot));
        }

        template <typename S>
        auto
        operator|(S &&slot) noexcept -> signal_connect &
            requires std::convertible_to<S, slot_type>
        {
            return connect(std::forward<S>(slot)), *this;
        }
    };
}
