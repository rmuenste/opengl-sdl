#include <gameobject.hpp>

namespace i3d
{

  GameObject::GameObject()
  {

  }

  GameObject::~GameObject()
  {

  }

  void GameObject::render()
  {
    meshObject_->render();
  }
  
} /* i3d */ 

