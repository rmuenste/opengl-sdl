#include <3dmodel.h>
#include <objloader.h>

using namespace i3d;

int main(int argc, char *argv[])
{
  Model3D model;
  ObjLoader loader;

  loader.readModelFromFile(&model, "sphere.obj");

  return 0;
}
