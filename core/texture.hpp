#ifndef TEXTURE_HPP_XJ2A6HC5
#define TEXTURE_HPP_XJ2A6HC5

#include<GL/gl3w.h>
#include <string>
#include <textureloader.hpp>

namespace i3d {

typedef uint8_t byte_type;

enum class i3dTexType {

  i3d_tex_diffuse_map,
  i3d_tex_normal_map,
  i3d_tex_shadow_map

};

class Texture
{

public:

  Texture ();

  Texture(std::string name);

  Texture& operator=(const Texture &copy)
  {
    id_ = copy.id_;
    fileName_ = copy.fileName_;
    return *this;
  }

  Texture(const Texture &copy)
  {

    id_ = copy.id_;
    fileName_ = copy.fileName_;
    printf("copy constructor called %s \n",fileName_.c_str());
    pixels_ = new byte_type[copy.size_];
    size_ = copy.size_;
    for (unsigned i = 0; i < size_; i++)
      pixels_[i] = copy.pixels_[i];

  }

  Texture(Texture &&copy)
  {
    id_ = copy.id_;
    fileName_ = copy.fileName_;
    pixels_ = copy.pixels_;
    copy.pixels_ = nullptr;
  }

  virtual ~Texture ();

  void createTextureFromImage(std::string fileName);

  void createTextureFromImage(i3dTexType type, std::string fileName);

  void createDepthTexture(unsigned w, unsigned h);

  void createRenderTexture(unsigned w, unsigned h);

  void bind()
  {
    glBindTexture(GL_TEXTURE_2D, id_);
  }

  void unbind()
  {
    glBindTexture(GL_TEXTURE_2D, 0);
  }

  void bindAlternate()
  {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id_);
  }

  //void makeCheckImage();

  GLuint id_;

  std::string fileName_;

  i3dTexType texType_;

private:
  /* data */

  //GLubyte checkImage[64][64][4];
  byte_type* pixels_;

  unsigned size_;


};

};
#endif /* end of include guard: TEXTURE_HPP_XJ2A6HC5 */
