#include "EntityGroup.hpp"

EntityGroup::EntityGroup(QString const & name) :
  m_name(name)
{
}

int EntityGroup::getGroupIdx(void) const
{
  return (m_groupIdx);
}
