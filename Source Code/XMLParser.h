#ifndef _XMLPARSER_
#define _XMLPARSER_
#include <xercesc\util\PlatformUtils.hpp>
#include <xercesc\parsers\XercesDOMParser.hpp>
#include <xercesc\framework\LocalFileFormatTarget.hpp>
#include <xercesc\util\XMLChar.hpp>
#include <xercesc\dom\DOM.hpp>

#include <boost\any.hpp>
#include <boost\shared_ptr.hpp>
#include <boost\scoped_ptr.hpp>
#include <boost\lexical_cast.hpp>

#include <map>
#include <string>

#include "InhomogenousContainer.h"

typedef std::map<std::string, InhomogenousContainer*> categories;

class XMLParser{
private:
	categories*	_categories;	//Contains the whole xml-tree
	std::string	_inputFile;		//The given path to the xml to load
	std::string _outputFile;	//The given path to the xml to save

	xercesc::XercesDOMParser* _parser;	//DOM Parser

public:

	XMLParser(std::string defaultInputFile, std::string defaultOutputFile){
		_categories		= new categories();
		_inputFile		= defaultInputFile;
		_outputFile		= defaultOutputFile;	
	}

	XMLParser(){
		_categories		= new categories();
	}

	~XMLParser(){
		_categories->clear();
		delete _categories;
		xercesc::XMLPlatformUtils::Terminate(); //Clean up the xerces lib
	}

	void add(std::string category, std::string key, boost::any value);

	void add(std::string categorie, std::string key, const char* value);

	/**Changes the value of a key. If the key is not existent, a new key will be 
		created with the new value.
	**/
	void change(std::string category, std::string key, boost::any newValue);

	void change(std::string category, std::string key, const char* newValue);

	/**returns the value of the given key in the given category and performs
	    the particular typecast. The user is responsible for the right typecast. 
	**/
	template<typename T>
	T get(std::string categorie, std::string key){
		if(_categories->count(categorie) != 0){
		 return (*_categories)[categorie]->get<T>(key);
		} else {
			throw std::logic_error(std::string("Category: ").append(categorie.c_str()).append(" not found!").c_str());
		}
        
	}

	void writeToXML(std::string outputFile = "-1");

	void loadXML(std::string inputFile = "-1");

private:

	/** initializes xerces to be ready for parsing
	**/
	void initialize();

	/**Creates a XML Tree for xerces which is used to write the data to a xml file
	**/
	xercesc::DOMNode* createDOMTree();

	/**Converts the node value to the particular type
	**/
	void convertToAny(xercesc::DOMNode* node, boost::any* value);

	/**Converts the given value to a string
	**/
	void convertToString(std::string* str, boost::any* value);
	
};


#endif

