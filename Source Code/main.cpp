#include <Windows.h>

#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GL\glfw.h>
#endif

#include <iostream>
#include "GLFWWindow.h"

#include <boost\lexical_cast.hpp>
#include <XMLParser.h>

#include "ParticleSystem.h"

EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)

void loadDefaultValues(XMLParser* xml);

int main(){

	LPTSTR path = new TCHAR[100];
	GetModuleFileName(HINST_THISCOMPONENT,path, 100);

	std::cout << "Executing: " << path  << std::endl;

	XMLParser* xml = new XMLParser();

	try{
		xml->loadXML(".\\particles.xml");

	} catch(std::exception& e){
		std::cout << e.what() <<std::endl;
		std::cout << "Default values will be applied!" <<std::endl;
		
		loadDefaultValues(xml);

		try{
			xml->writeToXML(".\\particles.xml");
		} catch(std::exception& e){
			std::cout << e.what() <<std::endl;
		}
	}

	
	GLFWWindow* wnd;

	try{

		wnd = new GLFWWindow(xml->get<int>("Window", "width"), 
							 xml->get<int>("Window", "height"), 
							 "OpenGL_4.3_Particle_System", 
							 xml->get<bool>("Window", "windowed"));

	} catch(std::exception& e){
		std::cout << e.what() <<std::endl;
		std::cin.get();

		delete xml;

		return -1;
	}

	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	if(glewInit() != GLEW_OK){
		std::cout << "Could not initialize GLEW!" <<std::endl;
		std::cin.get();

		delete wnd;
		delete xml;

		return -1;
	} else {
		std::cout << "GLEW Version: " <<glewGetString(GLEW_VERSION) <<std::endl;
	}

	ParticleSystem* ps;

	try{
		ps = new ParticleSystem(xml->get<int>("Particles", "maxParticles"),
								xml->get<int>("Particles", "iniRadius"),
								xml->get<float>("Particles", "quadLength"),
								xml->get<int>("Window", "maxFPS"),
								xml->get<float>("Camera", "translation_velocity"),
								xml->get<float>("Camera", "rotation_velocity"));

	} catch(std::exception& e){
		std::cout << e.what() <<std::endl;
		std::cin.get();

		delete wnd;
		delete xml;

		return -1;
	}
	
	delete xml;

	try{
		ps->run(wnd);
	} catch(std::exception& e){
		std::cout << e.what() <<std::endl;
		std::cin.get();
	}

	delete ps;
	delete wnd;

	return 0;
}

void loadDefaultValues(XMLParser* xml){
	xml->add("Window", "width", 1024);
	xml->add("Window", "height", 768);
	xml->add("Window", "windowed", true);
	xml->add("Window", "maxFPS", 60);
	
	xml->add("Particles", "maxParticles", 1000);
	xml->add("Particles", "quadLength", 0.01f);
	xml->add("Particles", "iniRadius", 15);

	xml->add("Camera", "translation_velocity", 100.0f);
	xml->add("Camera", "rotation_velocity", 100.0f);
}