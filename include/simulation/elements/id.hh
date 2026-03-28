#pragma once
#include "simulation/element.hh"


namespace kei::sim::elements
{
    struct ids
    {
        ids(element_registry &reg);


        element_id air;
        element_id sand;
    };
}
