#ifndef _GLEW_
#define _GLEW_
	#include <GL\glew.h>
	#include <GLFW\glfw3.h>
#endif

#include <iostream>
#include <string>
#include <stdexcept>

#include <boost\lexical_cast.hpp>

#include "GLFWWindow.h"
#include "XMLParser.h"
#include "ParticleSystem.h"

void loadDefaultValues(XMLParser* xml);

int main(){

	std::auto_ptr<XMLParser> xml(new XMLParser());

	try{
		xml->loadXML(".\\particles.xml");
	} catch(std::exception& e){
		std::cout << e.what() <<std::endl;
		std::cout << "Default values will be applied!" <<std::endl;

		loadDefaultValues(xml.get());

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

		return -1;
	}

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cout << "Could not initialize GLEW!" <<std::endl;
		std::cin.get();

		delete wnd;

		return -1;
	} else {
		std::cout << "GLEW Version: " <<glewGetString(GLEW_VERSION) <<std::endl;
	}
	
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

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

		return -1;
	}

	try{
		std::cout << "Executing Particle System" <<std::endl;
		ps->run(wnd);
	} catch(std::exception& e){
		std::cout << e.what() <<std::endl;
		std::cin.get();
	} catch(...){
		std::cout << "FATAL ERROR occured during execution of the particle system!" <<std::endl;
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