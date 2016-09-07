#include <texture.hpp>
#include <iostream>

namespace i3d {

Texture::Texture()
{
  pixels_ = nullptr;
}

Texture::Texture(std::string name)
{
  TextureLoader tLoader;
  if (!tLoader.loadTexture(name))
  {
    exit(1);
  }

  fileName_ = name;

  SDL_Surface* surf = tLoader.getTexture();

  const unsigned bytesPerRow = surf->pitch;
  const byte_type* originalRaw = static_cast<byte_type*>(surf->pixels);
  size_ = bytesPerRow * surf->h;
  pixels_ = new byte_type[size_];

  for (int i = 0; i < surf->h; ++i)
  {
    const byte_type* srcBeg = originalRaw + (bytesPerRow *(surf->h - i - 1));
    const byte_type* srcEnd = srcBeg + bytesPerRow;

    std::copy(srcBeg, srcEnd, pixels_ + bytesPerRow * i);
  }

  glGenTextures(1, &id_);

  bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  GLuint mode = GL_RGB;
  if (surf->format->BytesPerPixel == 4)
  {
    mode = GL_RGBA;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h,
    0, GL_RGBA, GL_UNSIGNED_BYTE, pixels_);
}

Texture::~Texture()
{

  if(pixels_!=nullptr)
  {
    delete[] pixels_;
    pixels_ = nullptr;
  }

}

void Texture::createTextureFromImage(i3dTexType type, std::string fileName)
{

  if(type == i3dTexType::i3d_tex_diffuse_map)
  {
    createTextureFromImage(fileName);
  }
  else
  {
    std::cerr << "Texture type loading not yet implemented." << std::endl;
  }

}

void Texture::createTextureFromImage(std::string fileName)
{

  TextureLoader tLoader;
  if(!tLoader.loadTexture(fileName))
  {
    std::exit(1);
  }

  fileName_ = fileName;
  typedef uint8_t byte_type;

  SDL_Surface* surf = tLoader.getTexture();

  const unsigned bytesPerRow = surf->pitch;
  const byte_type* originalRaw = static_cast<byte_type*>(surf->pixels);
  size_ = bytesPerRow * surf->h;
  pixels_ = new byte_type[bytesPerRow * surf->h];

  for (int i = 0; i < surf->h; ++i)
  {
    const byte_type* srcBeg = originalRaw + (bytesPerRow *(surf->h - i - 1));
    const byte_type* srcEnd = srcBeg + bytesPerRow;

    std::copy(srcBeg, srcEnd, pixels_ + bytesPerRow * i);
  }

  glGenTextures(1,&id_);

  bind();

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


  GLuint mode = GL_RGB;
  if (surf->format->BytesPerPixel == 4)
  {
    mode = GL_RGBA; 
  }

  glTexImage2D(GL_TEXTURE_2D, 0, mode, surf->w, surf->h,
      0, mode, GL_UNSIGNED_BYTE, pixels_);

  glGenerateMipmap(GL_TEXTURE_2D);

}

void Texture::createRenderTexture(unsigned w, unsigned h)
{

  glGenTextures(1, &id_);

  bind();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h,
    0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  unbind();

}

void Texture::createDepthTexture(unsigned w, unsigned h)
{

  glGenTextures(1, &id_);

  bind();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, w, h,
    0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

  unbind();

}

//void Texture::makeCheckImage()
//{
//  int i, j, c;
//  for (i = 0; i < 64; ++i)
//  {
//    for (j = 0; j < 64; ++j)
//    {
//      c = ((((i&0x8)==0)^((j&0x8))==0)) * 255; 
//      checkImage[i][j][0] = (GLubyte)c; 
//      checkImage[i][j][1] = (GLubyte)c; 
//      checkImage[i][j][2] = (GLubyte)c; 
//      checkImage[i][j][3] = (GLubyte)255; 
//    } 
//  }
//
//  glGenTextures(1,&id_);
//  glBindTexture(GL_TEXTURE_2D, id_);
//
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 64, 64,
//      0, GL_RGBA, GL_UNSIGNED_BYTE,checkImage);
//
//}

};
