#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <QList>
#include "Model.hpp"
#include "InstModel.hpp"

class Entity
{
public:
    Entity();
    ~Entity();

    InstModel * createModel(Model const & model);

private:
    QList<InstModel *> m_modelList;
};

#endif // ENTITY_HPP
