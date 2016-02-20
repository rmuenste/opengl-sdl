#include <sdl_gl_application.hpp>
#include<GL/gl.h>
#include <basicshader.hpp>
#include <shaderloader.hpp>
#include <stdlib.h>
#include <objloader.h>
#include <3dmodel.h>
#include <3dmesh.h>
#include <texture.hpp>
#include <transform.hpp>
#ifndef WIN32
#include <sys/time.h>
#endif
#include <perspectivetransform.hpp>
#include <cmath>
#include <camera.hpp>

namespace i3d {

  class TransformedTriangle : public SDL_GL_application
  {
    public:
      TransformedTriangle ()
      {
        setTitle(std::string("Transformed Triangle Render"));
        frame=0;
        time_=0;
      };

      virtual ~TransformedTriangle (){};

      void init()
      {
        SDL_GL_application::init();

        perspective_.setPerspectiveTransform(70.0, getWindowWidth(), getWindowHeight(), 0.0, 100.0);
        camera_.initCamera(Vec3(0,0,-3.0), Vec3(1,0,0), Vec3(0,1,0), Vec3(0,0,1));

        shader.initShader();
        shader.addVertexShader("../../shaders/transform.vert");
        shader.addFragmentShader("../../shaders/transform.frag");
        shader.linkShader();
        shader.addUniform(std::string("transform"));
        shader.addUniform(std::string("perspective"));
        shader.addUniform(std::string("camera"));
        shader.addUniform(std::string("cameraRotation"));

        transform_.translation_ = i3d::Vec4(0.0f, 0.0f, 0.0f, 1.0f);
        transform_.setRotationEuler(i3d::Vec3(0.0,0.0,0.0));

        glEnable(GL_TEXTURE_2D);

        earth.createTextureFromImage(std::string("../../textures/australia.png"));
        //earth.makeCheckImage();

        i3d::ObjLoader loader;
        i3d::Model3D model;
        loader.readModelFromFile(&model, "../../meshes/earth_tri2.obj");
        model.prepareIndexing();

        for(auto v : model.meshes_.front().vertices_)
        {
          std::cout << v << std::endl;
        }

        for(auto t : model.meshes_.front().texCoords_)
        {
          std::cout << t << std::endl;
        }

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(3, &buffers[0]);
        glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
        glBufferData(GL_ARRAY_BUFFER, model.meshes_[0].vertices_.size() * 3 * sizeof(float),
            model.meshes_.front().vertices_.data() , GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
        glBufferData(GL_ARRAY_BUFFER, model.meshes_.front().orderedTexCoords_.size() * 2 * sizeof(float),
            model.meshes_.front().orderedTexCoords_.data() , GL_STATIC_DRAW);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(1);

        size = 3 * model.meshes_.front().faces_.size(); 

        glGenBuffers(1, &iao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.meshes_.front().faces_.size() * 3 * sizeof(int),
            model.meshes_.front().indices_, GL_STATIC_DRAW); 

      }

      void render()
      {

        static const GLfloat red[]={0.0f, 0.25f, 0.0f, 1.0f};
        glClearBufferfv(GL_COLOR, 0, red);

        float x = (float)rand()/(float)(RAND_MAX);

        const GLfloat col[]={x, 0.f, 0.0f, 1.0f};

        shader.bind();
        shader.setUniform(std::string("transform"), transform_.getMatrix());
        shader.setUniform(std::string("perspective"), perspective_.getPerspectiveTransform());
        shader.setUniform(std::string("camera"), camera_.getCameraTranslationTransform());
        shader.setUniform(std::string("cameraRotation"), camera_.getCameraCoordinateTransform());

        earth.bind();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iao);
        glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);


#ifndef _MSC_VER
        struct timeval start, end;

        long mtime, seconds, useconds;    

        gettimeofday(&start, NULL);
#endif

        SDL_GL_SwapWindow(window);

#ifndef _MSC_VER
        gettimeofday(&end, NULL);

        seconds  = end.tv_sec  - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;

        mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;
#endif
        //printf("time: %f milliseconds\n", time_);

//        time_+=mtime;
//        transform_.setRotationEuler(i3d::Vec3(0.0,0.0,std::sin(frame*0.001)*3.14));
//
//        frame++;

      }

      void handleResizeEvent(SDL_Event &event)
      {
        SDL_GL_application::handleResizeEvent(event);
        perspective_.setPerspectiveTransform(70.0, getWindowWidth(), getWindowHeight(), 0.0, 100.0);
      }

      void handleKeyPressEvent(SDL_Event &event)
      {
        switch(event.key.keysym.sym) {

          case SDLK_RIGHT:
            camera_.getPos() += 0.01f * camera_.getU();
            break;
          case SDLK_LEFT:
            camera_.getPos() -= 0.01f * camera_.getU();
            break;
          case SDLK_UP:
            camera_.getPos() += 0.01f * camera_.getN();
            break;
          case SDLK_DOWN:
            camera_.getPos() -= 0.01f * camera_.getN();
            break;
          case SDLK_a:
            camera_.rotateY(-0.01f);
            break;
          case SDLK_d:
            camera_.rotateY(0.01f);
            break;
          case SDLK_w:
            camera_.rotateX(-0.01f);
            std::cout << camera_.getN() << std::endl;
            break;
          case SDLK_s:
            camera_.rotateX(0.01f);
            std::cout << camera_.getN() << std::endl;
            break;
          }
      }

    private:
      /* data */
      GLuint program;
      GLuint vao;
      GLuint iao;
      GLuint tiao;
      GLuint buffers[3];
      BasicShader shader;
      Texture earth;
      int size;
      int frame;
      Transform transform_;
      float time_;
      PerspectiveTransform perspective_;
      Camera camera_;
  };
}
int main(int argc, char *argv[])
{

  i3d::TransformedTriangle app;

  app.init();

  app.run();

  return 0;
}

