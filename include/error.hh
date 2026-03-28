#pragma once
#include <format>
#include <source_location>
#include <string>


namespace kei
{
    class error
    {
    public:
        constexpr error(const std::source_location &source,
                        std::string_view            domain,
                        std::string                 msg) noexcept
            : source { source }, domain { domain }, msg { std::move(msg) }
        {
        }


        [[nodiscard]]
        constexpr auto
        get_message() const noexcept -> const std::string &
        {
            return msg;
        }


        [[nodiscard]]
        constexpr auto
        get_domain() const noexcept -> std::string_view
        {
            return domain;
        }


        [[nodiscard]]
        constexpr auto
        get_source() const noexcept -> const std::source_location &
        {
            return source;
        }

    private:
        std::source_location source;
        std::string_view     domain;
        std::string          msg;
    };


    namespace _impl
    {
        template <typename Formatter> struct error_builder
        {
            [[nodiscard]]
            constexpr auto
            operator[](std::string_view            domain,
                       const std::source_location &source
                       = std::source_location::current()) const
            {
                return error_message_builder { domain, source };
            }

        private:
            class error_message_builder
            {
            public:
                error_message_builder(std::string_view     domain,
                                      std::source_location source)
                    : domain { domain }, source { source }
                {
                }


                template <typename... Args>
                [[nodiscard]]
                auto
                operator()(std::format_string<Args...> fmt,
                           Args &&...args) const -> error
                {
                    return error { source, domain,
                                   formatter(std::format(
                                       fmt, std::forward<Args>(args)...)) };
                }


                [[nodiscard]]
                auto
                operator()() const -> error
                {
                    return error { source, domain, formatter("") };
                }


            private:
                std::string_view     domain;
                std::source_location source;
                Formatter            formatter;
            };
        };


        auto plain_error_formatter { [](std::string msg) -> std::string
                                     { return msg; } };
    }


    inline constexpr _impl::error_builder<
        decltype(_impl::plain_error_formatter)>
        error_builder {};
}
