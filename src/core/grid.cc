#include <cstddef>

#include "core/grid.hh"

using kei::core::grid;


grid::grid() { set_size({ .w = 616, .h = 381 }); }


void
grid::set_size(sdl::size new_size)
{
    m_cells.clear();
    m_cells.resize(static_cast<std::size_t>(new_size.w) * new_size.h);
    m_size = new_size;
}


void
grid::clear()
{ m_cells.clear(); }


auto
grid::size() const noexcept -> sdl::size
{ return m_size; }


auto
grid::operator[](sdl::point p) -> cell &
{ return m_cells.at((p.y * m_size.w) + p.x); }


auto
grid::operator[](sdl::point p) const -> const cell &
{ return m_cells.at((p.y * m_size.w) + p.x); }


auto
grid::begin() noexcept -> iterator
{ return m_cells.begin(); }


auto
grid::end() noexcept -> iterator
{ return m_cells.end(); }


auto
grid::begin() const noexcept -> const_iterator
{ return m_cells.begin(); }


auto
grid::end() const noexcept -> const_iterator
{ return m_cells.end(); }


auto
grid::cbegin() const noexcept -> const_iterator
{ return m_cells.cbegin(); }


auto
grid::cend() const noexcept -> const_iterator
{ return m_cells.cend(); }


auto
grid::rbegin() noexcept -> reverse_iterator
{ return m_cells.rbegin(); }


auto
grid::rend() noexcept -> reverse_iterator
{ return m_cells.rend(); }


auto
grid::rbegin() const noexcept -> const_reverse_iterator
{ return m_cells.rbegin(); }


auto
grid::rend() const noexcept -> const_reverse_iterator
{ return m_cells.rend(); }


auto
grid::crbegin() const noexcept -> const_reverse_iterator
{ return m_cells.crbegin(); }


auto
grid::crend() const noexcept -> const_reverse_iterator
{ return m_cells.crend(); }


