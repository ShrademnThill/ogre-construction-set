#ifndef INSTMODEL_HPP
#define INSTMODEL_HPP

#include <Ogre.h>
#include "Model.hpp"

class InstModel
{
public:
    InstModel(Model const & model);
    ~InstModel();

    void setRoot(Ogre::SceneNode * node);
    Ogre::SceneNode * getRoot(void);

private:
    Ogre::SceneNode * m_root;
};

#endif // INSTMODEL_HPP
