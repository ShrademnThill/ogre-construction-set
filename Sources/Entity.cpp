#include "Entity.hpp"

Entity::Entity()
{
}

Entity::~Entity()
{
}

InstModel * Entity::createModel(const Model &model)
{
  InstModel * instModel = new InstModel(model);

  m_modelList.append(instModel);
  return (instModel);
}
