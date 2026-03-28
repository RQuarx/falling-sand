#include "simulation/board.hh"

using kei::sim::board;


auto
board::size() const noexcept -> sdl::size
{ return m_size; }


void
board::set_size(sdl::size new_size)
{
    m_size = new_size;
    m_cells.clear();
    m_cells.resize(static_cast<std::size_t>(m_size.w) * m_size.h);
}


void
board::clear(sdl::point pos)
{
    (*this)[pos] = cell {};
}


auto
board::operator[](std::size_t x, std::size_t y) -> cell &
{ return m_cells[(y * m_size.w) + x]; }


auto
board::operator[](std::size_t x, std::size_t y) const -> const cell &
{ return m_cells[(y * m_size.w) + x]; }


auto
board::operator[](std::size_t y) -> std::span<cell>
{ return { &m_cells[y * m_size.w], static_cast<std::size_t>(m_size.w) }; }


auto
board::operator[](sdl::point pos) -> cell &
{ return (*this)[pos.x, pos.y]; }


auto
board::operator[](sdl::point pos) const -> const cell &
{ return (*this)[pos.x, pos.y]; }
