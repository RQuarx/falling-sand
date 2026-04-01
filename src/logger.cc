#include <iostream>
#include <thread>
#include <utility>

#include "logger.hh"


using kei::log_level;
using kei::logger;


namespace
{
    [[nodiscard]]
    auto
    get_time(const std::chrono::time_point<std::chrono::system_clock> &now) -> std::string
    {
        using std::chrono::duration;
        using ms = std::chrono::milliseconds;
        using m  = std::chrono::minutes;
        using s  = std::chrono::seconds;

        auto dura { now.time_since_epoch() };
        ms   millis { std::chrono::duration_cast<ms>(dura) % 1000 };
        m    minutes { std::chrono::duration_cast<m>(dura) % 60 };
        s    seconds { std::chrono::duration_cast<s>(dura) % 60 };

        return std::format("{:02}:{:02}.{:03}", minutes.count(), seconds.count(), millis.count());
    }


    constexpr std::array<std::string_view, 6> LABELS {
        "\033[38;2;156;163;175mtrace\033[0m", "\033[38;2;59;130;246mdebug\033[0m",
        "\033[38;2;34;211;238minfo\033[0m ",  "\033[38;2;250;204;21mwarn\033[0m ",
        "\033[38;2;239;68;68merror\033[0m",   "\033[38;2;192;38;211mfatal\033[0m",
    };


    [[nodiscard]]
    auto
    format_log(log_level        level,
               std::string_view time,
               std::string_view domain,
               std::string_view message,
               std::string_view file_name,
               std::uint32_t    line) -> std::string
    {
        const std::string_view &label { LABELS[level] };

        if (level >= log_level::error)
            return std::format("{} [{} at \033[38;2;70;172;173m{}:{}\033[0m]: "
                               "\033[1m{}\033[0m",
                               time, label, file_name, line, message);

        return std::format("{} [{} at \033[38;2;70;172;173m{}\033[0m]: "
                           "\033[1m{}\033[0m",
                           time, label, domain, message);
    }
}


logger::logger(log_level threshold_level) noexcept
    : m_threshold { threshold_level },
      m_worker { [this](std::stop_token st) { mf_process_queue(std::move(st)); } }
{
}


logger::~logger()
{
    m_worker.request_stop();
    m_cv.notify_one();
}


auto
logger::operator[](log_level level, std::string_view domain, std::source_location source) noexcept
    -> log_entry
{ return log_entry { level < m_threshold ? nullptr : this, level, domain, source }; }


void
logger::mf_process_queue(std::stop_token stop_token)
{
    while (!stop_token.stop_requested() || !m_log_queue.empty())
    {
        std::unique_lock lock { m_mutex };

        m_cv.wait(lock,
                  [&] -> bool { return stop_token.stop_requested() || !m_log_queue.empty(); });

        while (!m_log_queue.empty())
        {
            const auto obj { std::move(m_log_queue.front()) };
            m_log_queue.pop();

            lock.unlock();

            const std::string time_str { get_time(obj.time) };

            std::string formatted_console { format_log(obj.level, time_str, obj.domain, obj.message,
                                                       obj.source.file_name(), obj.source.line()) };

            std::clog << formatted_console << '\n';

            lock.lock();
        }
    }
}


void
logger::mf_push_log(log_object &&object)
{
    {
        std::scoped_lock lock { m_mutex };
        m_log_queue.push(std::move(object));
    }

    m_cv.notify_one();
}


logger::log_entry::log_entry(logger              *parent,
                             log_level            level,
                             std::string_view     domain,
                             std::source_location source)
    : m_parent { parent }, m_obj { .time    = std::chrono::system_clock::now(),
                                   .level   = level,
                                   .domain  = domain,
                                   .source  = source,
                                   .message = "" }
{
}
