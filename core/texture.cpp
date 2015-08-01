#include <texture.hpp>
#include <textureloader.hpp>

Texture::Texture()
{

}

Texture::~Texture()
{

}

void Texture::createTextureFromImage(std::string fileName)
{

  TextureLoader tLoader;
  if(!tLoader.loadTexture(fileName))
    exit(1);

  glGenTextures(1,&id_);

  bind();

  SDL_Surface* surf = tLoader.getTexture();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surf->w, surf->h,
               0, GL_RGB, GL_UNSIGNED_BYTE,surf->pixels);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  

}

