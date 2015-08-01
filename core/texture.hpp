#ifndef TEXTURE_HPP_XJ2A6HC5
#define TEXTURE_HPP_XJ2A6HC5

#include<GL/gl3w.h>
#include <string>

class Texture
{
public:
  Texture ();
  virtual ~Texture ();

  void createTextureFromImage(std::string fileName);

  void bind()
  {
    glBindTexture(GL_TEXTURE_2D, id_);
  }

  void makeCheckImage();

private:
  /* data */
  GLuint id_;
  GLubyte checkImage[64][64][4];
  GLubyte* pixels_;
};


#endif /* end of include guard: TEXTURE_HPP_XJ2A6HC5 */
