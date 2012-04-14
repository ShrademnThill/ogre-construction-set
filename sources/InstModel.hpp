#ifndef INSTMODEL_HPP
#define INSTMODEL_HPP

#include "InstItem.hpp"
#include "Model.hpp"

class InstModel : public InstItem
{
public:
  InstModel(Model const & model);
  ~InstModel();

  Model const &     getModel() const;

  void  load(Ogre::SceneNode * node);
  void  unload(void);

private:
  Model m_model;
};

#endif // INSTMODEL_HPP
