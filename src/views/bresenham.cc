#include "views/bresenham.hh"

using kei::views::_impl::bresenham_view;


bresenham_view::iterator::iterator(sdl::point a, sdl::point b, bool is_end)
    : current { a }, end { b }, done { is_end }
{
    distance = { .x = std::abs(b.x - a.x), .y = -std::abs(b.y - a.y) };
    step     = { .x = (a.x < b.x ? 1 : -1), .y = (a.y < b.y ? 1 : -1) };
    err      = distance.x + distance.y;
    if (is_end) done = true;
}


auto
bresenham_view::iterator::operator*() const -> sdl::point
{ return current; }


auto
bresenham_view::iterator::operator++() -> iterator &
{
    if (done) return *this;
    if (current.x == end.x && current.y == end.y)
    {
        done = true;
        return *this;
    }

    int e2 = 2 * err;
    if (e2 >= distance.y)
    {
        err       += distance.y;
        current.x += step.x;
    }
    if (e2 <= distance.x)
    {
        err       += distance.x;
        current.y += step.y;
    }
    return *this;
}

auto
bresenham_view::iterator::operator==(const iterator &other) const -> bool
{ return done == other.done; }


auto
bresenham_view::iterator::operator!=(const iterator &other) const -> bool
{ return !(*this == other); }
