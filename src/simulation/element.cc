#include "simulation/element.hh"
#include "simulation/elements/air.hh"
#include "simulation/elements/id.hh"
#include "simulation/elements/sand.hh"

using kei::sim::element_registry;
using kei::sim::elements::ids;


auto
element_registry::operator<<(element_def elem) -> element_id
{
    elem.id = m_elements.size();
    m_elements.emplace_back(elem);
    return elem.id;
}


auto
element_registry::operator[](element_id id) const -> const element_def &
{
    return m_elements[id];
}


ids::ids(element_registry &reg)
    : air { reg << elements::air() }, sand { reg << elements::sand() }
{
}
