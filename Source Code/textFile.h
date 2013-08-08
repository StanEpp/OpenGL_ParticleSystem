/****************************************************************************************/
/*	Author: Stanislaw Eppinger															*/
/*	Date: 26.09.2012																	*/
/*																						*/
/*	This class reads a text-file and saves its content in a string as well as			*/
/*	in char-Array. It is possible to disable the saving in a char-Array to save			*/
/*	memory. Only one text-file can be loaded at once. 									*/
/****************************************************************************************/

#ifndef _TEXTFILEREAD_
#define _TEXTFILEREAD_
#include <fstream>
#include <string>

class textFile{
private: 
	std::string		_text;
	char*			_ctext;
	bool			_nonConstantChar, _loadedFile;

public:

	textFile(){
		//No char-Array is used by default.
		_nonConstantChar = false;
		_loadedFile = false;
	}

	textFile(bool enableNonConstantChar){
		_nonConstantChar = enableNonConstantChar;
		_loadedFile = false;
	}

	~textFile(){
		//If _ctext points to an acquired memory block then release it.
		if(_nonConstantChar && _loadedFile){
			delete[] _ctext;
		}
	}

	void loadFile(const char* filepath){
		//Clear the string everytime a new file is loaded.
		_text.clear();

		std::string temp;

		std::ifstream file(filepath);

		if(file.fail()){
			throw std::exception("Corrupt file!");
		} else {
			if(file.is_open()){
				while(file.good()){
					std::getline(file, temp);
					_text+=temp+"\n";
				}
			}
		}

		file.close();

		//Did the user enable saving in a char-Array?
		if(_nonConstantChar){

			//Does _ctext already point to an acquired memory block then
			//release it. 
			if(_loadedFile)
				delete[] _ctext;

			_ctext = new char[_text.length()+1];	//+1 for the null-termination
			strcpy(_ctext, _text.c_str());
			_loadedFile = true;
		}

	}

	std::string str(){
		return _text;
	}

	const char* c_str(){
		return _text.c_str();
	}

	char* nc_str(){
		if(_nonConstantChar && _loadedFile){
			return _ctext;
		} else {
			return "";
		}
		
	}

	void clear(){
		//If _ctext points to an acquired memory block then release it.
		if(_nonConstantChar && _loadedFile){
			delete[] _ctext;
			_loadedFile = false;
		}

		_text.clear();

	}

};


#endif