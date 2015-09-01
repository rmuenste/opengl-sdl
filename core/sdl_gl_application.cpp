#include <sdl_gl_application.hpp>
#include <iostream>

SDL_GL_application::SDL_GL_application() : quit(false), window(nullptr), window_width_(640), window_height_(480), title_("SDL2 Window")
{
  renderMode_ = GL_FILL;
}

void SDL_GL_application::init()
{
  

  SDL_Surface* image = nullptr;

  if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    printf("SDL_Error: %s\n", SDL_GetError());
    exit(0); 
  }

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
  
  window = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_UNDEFINED, 
                            SDL_WINDOWPOS_UNDEFINED, window_width_, window_height_, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

  if(window==nullptr)
  {
    printf("SDL_Error: %s\n", SDL_GetError());
    exit(0); 
  }

  context = SDL_GL_CreateContext(window);

  if (gl3wInit()) {
    
    fprintf(stderr, "failed to initialize OpenGL, error code: %i\n", gl3wInit());
    int vmajor, vminor;
    glGetIntegerv(GL_MAJOR_VERSION, &vmajor);
    glGetIntegerv(GL_MINOR_VERSION, &vminor);
    printf("Maximal supported version: %i.%i\n",vmajor,vminor );

    exit(0); 
  }
  if (!gl3wIsSupported(3, 2)) {
    fprintf(stderr, "OpenGL 3.2 not supported\n");
    exit(0); 
  }

  printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
         glGetString(GL_SHADING_LANGUAGE_VERSION));

}

void SDL_GL_application::run()
{

  while(!quit)
  {
    while(SDL_PollEvent(&event)) {
      handleEvent(event);      
    }
    render();
  }

  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();

}

SDL_GL_application::~SDL_GL_application()
{
  if(window != nullptr)
  {
    SDL_DestroyWindow(window);
  }
}

void SDL_GL_application::handleEvent(SDL_Event &event)
{
  if (event.type == SDL_QUIT)
  {
    quit = true; 
  }
  else if(event.type == SDL_WINDOWEVENT)
  {
    switch(event.window.event)
    {
      case SDL_WINDOWEVENT_RESIZED:
        handleResizeEvent(event);
        break;
    }
  }
  else if(event.type == SDL_KEYDOWN)
  {
    handleKeyPressEvent(event);
  }
}

void SDL_GL_application::handleResizeEvent(SDL_Event &event)
{
  setWindowWidth(event.window.data1);
  setWindowHeight(event.window.data2);
}

void SDL_GL_application::handleKeyPressEvent(SDL_Event &event)
{
switch(event.key.keysym.sym) {

  case SDLK_RIGHT:
    std::cout << "right arrow pressed" << std::endl;
    break;
  case SDLK_LEFT:
    std::cout << "left arrow pressed" << std::endl;
    break;
  }

}


