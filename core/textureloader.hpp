#ifndef TEXTURELOADER_HPP_8AM7YLH2
#define TEXTURELOADER_HPP_8AM7YLH2

#include <string>
#include <SDL.h>
#include <SDL_image.h>

class TextureLoader
{
public:
  TextureLoader (){};
  virtual ~TextureLoader (){};

  bool loadTexture(std::string fileName);

  SDL_Surface* getTexture(){ return surface_;};

private:
  /* data */
  SDL_Surface* surface_;
};

#endif /* end of include guard: TEXTURELOADER_HPP_8AM7YLH2 */
