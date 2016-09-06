#ifndef GAMEOBJECT_HPP_GBVAOEUP
#define GAMEOBJECT_HPP_GBVAOEUP

#include <material.hpp>
#include <mesh.hpp>
#include <simpleshader.hpp>
#include <memory>

namespace i3d {

  class GameObject
  {

    public:

      Mesh *meshObject_;

      Material *material_;

      SimpleShader *shader_;

      std::shared_ptr<BasicShader> bshader_;

      GameObject (void);

      virtual ~GameObject ();

      virtual void render();

    private:
      /* data */

  };

}

#endif /* end of include guard: GAMEOBJECT_HPP_GBVAOEUP */

