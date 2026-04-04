#include "game.hh"
#include "logger.hh"


auto
main() -> int
try
{
    return kei::game {}.run();
}
catch (const std::exception &e)
{
    kei::logger[kei::log_level::fatal, "main"]("what(): {}", e.what());
}
