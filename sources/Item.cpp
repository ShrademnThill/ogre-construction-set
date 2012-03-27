#include "Item.hpp"

Item::Item() :
  m_isDeleted(false)
{
}


bool  Item::isDeleted(void) const
{
  return (m_isDeleted);
}

void  Item::setDeleted(bool deleted)
{
  m_isDeleted = deleted;
}
