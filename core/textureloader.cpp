#include <textureloader.hpp>
#include <iostream>

bool TextureLoader::loadTexture(std::string fileName)
{

  surface_ = IMG_Load(fileName.c_str());
  if(surface_==NULL)
  {
    std::cout << "Error could not load image: " << fileName << std::endl; 
    return false;
  }
  return true;

}
