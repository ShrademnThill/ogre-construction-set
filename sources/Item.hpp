#ifndef ITEM_HPP
#define ITEM_HPP

class Item
{
public:
  Item();

  bool  isDeleted(void) const;

  void  setDeleted(bool deleted);

protected:
  bool  m_isDeleted;
};

#endif // ITEM_HPP
