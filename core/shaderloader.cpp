#include <shaderloader.hpp>
#include <fstream>
#include <iostream>

bool ShaderLoader::load(std::string fileName)
{

  std::string line;
  std::ifstream file(fileName.c_str());
  shaderCode_.clear();
  while(std::getline(file,line)) {

    shaderCode_.append(line);
    shaderCode_.append("\n");
    
  }

  std::cout << shaderCode_ << std::endl;

  return true;
}

