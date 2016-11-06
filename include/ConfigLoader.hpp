#ifndef _CONFIG_
#define _CONFIG_

#include <fstream>
#include <sstream>

class Config{
  struct Camera{
    float speed;
    float sensitivity;
    float nearDist;
    float farDist;
    float foV;
    Camera() : speed(100.f), sensitivity(25.f), nearDist(1.f), farDist(1.f), foV(45.f){}
  };
  
  struct Particles{
    int initRadius;
    unsigned int numParticles;
    float sizeOfParticles;
    Particles() : initRadius(15), numParticles(1000), sizeOfParticles(0.01f) {}
  };
  
  struct Window{
    int width, height;
    // int maxFps;
    bool fullscreen;
    Window() : width(1024), height(768), fullscreen(false){}
  };
  
public:
  Camera camera;
  Particles particles;
  Window window;
  
  /*
  Format of the config file is rough and unforgivable! Small deviations will fail! 
  (No need for sophisticated parsing for such a small config ;) )

  Format Type:

  ## Camera
  speed _float_
  sensitivity _float_
  nearDist _float_
  farDist _float_

  ## Particles
  initRadius _int_
  numParticles _int_
  sizeOfParticles _float_

  ## Window
  width _int_
  height _int_
  maxFps _int_
  fullscreen _bool_ //0 or 1

  */
  void loadConfig(){
    std::ifstream file("config", std::ios::in);
    
    if(!file.is_open()){
      std::cerr << "ERROR: Cannot open config file! Make sure there is a config file with the name \"config\" in the same directory as the executable." << std::endl;
      return;
    }
    
    enum Category {Camera, Particles, Window, None};
    
    Category cat = Category::None;
    std::string line, value, descr;
    std::istringstream token;
    
    while(!file.eof()){
      getline(file, line);
      if(!line.empty()){
        
        if(line[0] == '#'){
          if(line.compare("## Camera") == 0){
            cat = Category::Camera;
          } else if (line.compare("## Particles") == 0){
            cat = Category::Particles;
          } else if (line.compare("## Window") == 0){
            cat = Category::Window;
          } else{
            cat = Category::None;
          }
        } else {
          token = std::istringstream(line);
          getline(token, descr, ' ');
          getline(token, value, ' ');
          switch(cat){
            case Camera:{
              if(descr.compare("speed") == 0){ camera.speed = std::stof(value); }
              else if(descr.compare("sensitivity") == 0){ camera.sensitivity = std::stof(value); }
              else if(descr.compare("nearDist") == 0){ camera.nearDist = std::stof(value); }
              else if(descr.compare("farDist") == 0){ camera.farDist = std::stof(value); }
              else if(descr.compare("foV") == 0){ camera.foV = std::stof(value); }
            } break;
            case Particles: {
              if(descr.compare("initRadius") == 0){ particles.initRadius = std::stoi(value); }
              else if(descr.compare("numParticles") == 0){ particles.numParticles = std::stoul(value); }
              else if(descr.compare("sizeOfParticles") == 0){ particles.sizeOfParticles = std::stof(value); }
            } break;
            case Window: {
              if(descr.compare("width") == 0){ window.width = std::stoi(value); }
              else if(descr.compare("height") == 0){ window.height = std::stoi(value); }
              // else if(descr.compare("maxFps") == 0){ window.maxFps = std::stoi(value); }
              else if(descr.compare("fullscreen") == 0){ window.fullscreen = std::stoi(value); }
            } break;
            default:
            break;
          }
        }
      }
    }
  }
  
};




#endif