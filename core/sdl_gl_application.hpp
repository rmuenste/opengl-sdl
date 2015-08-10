#ifndef SDL_GL_APPLICATION_HPP_8SIRAYPZ
#define SDL_GL_APPLICATION_HPP_8SIRAYPZ

#include <GL/gl3w.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_events.h>
#include <stdio.h>
#include <string>

class SDL_GL_application
{

public:

  SDL_Window* window;

  SDL_GLContext context;

  SDL_Event event;

  bool quit; 

  // Contstructor
  SDL_GL_application ();

  virtual ~SDL_GL_application ();

  virtual void init();

  void run();

  virtual void render()
  {
    printf("Base Render function \n");
  };
  
  void setWindowWidth(int w)
  {
    window_width_ = w;
  }

  void setWindowHeight(int h)
  {
    window_height_ = h;
  }

  int getWindowWidth()
  {
    return window_width_;
  }

  int getWindowHeight()
  {
    return window_height_;
  }

  void setTitle(std::string title)
  {
    title_ = title;
  }

  void handleEvent(SDL_Event &event);
  
  virtual void handleResizeEvent(SDL_Event &event);

  virtual void handleKeyPressEvent(SDL_Event &event);

private:
  /* data */
  int window_width_;
  int window_height_;
  std::string title_;

};

#endif /* end of include guard: SDL_GL_APPLICATION_HPP_8SIRAYPZ */
