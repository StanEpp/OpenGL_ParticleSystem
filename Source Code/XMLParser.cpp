#include "XMLParser.h"
#include <xercesc/sax/HandlerBase.hpp>

void XMLParser::add(std::string category, std::string key, boost::any value){
    //Insert the new category and the container.
    //Every category consists of a container, which holds all the values, which are inside the category.
    _categories->insert( std::pair<std::string, InhomogenousContainer*>(category, new InhomogenousContainer()));

    //Reference to the category's container and add the particular value with its key
    (*_categories)[category]->add(key, value);
}

void XMLParser::add(std::string category, std::string key, const char* value){
    add(category, key, std::string(value));
}

void XMLParser::change(std::string category, std::string key, boost::any newValue){
	if(_categories->count(category) != 0){
		return (*_categories)[category]->change(key, newValue);
	} else {
		throw std::logic_error(std::string("Category: ").append(category.c_str()).append(" not found!").c_str());
	}
}

void XMLParser::change(std::string category, std::string key, const char* newValue){
	if(_categories->count(category) != 0){
		return (*_categories)[category]->change(key, std::string(newValue));
	} else {
		throw std::logic_error(std::string("Category: ").append(category.c_str()).append(" not found!").c_str());
	}
}

void XMLParser::writeToXML(std::string outputFile){
	bool errorOccured = false;
	std::logic_error error("");

    try{
        initialize();
		
        //Creates the needed objects to write a XML file.
		xercesc::DOMImplementation* impl			= xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::chNull);
        xercesc::DOMLSSerializer*	theSerializer	= ((xercesc::DOMImplementationLS*)impl)->createLSSerializer();

		//Enables the prett-print feature, so that the created xml file is well formatted and thus easy to read.
		theSerializer->getDomConfig()->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
	
        //Checks wether the user passed a new output path.
        if(outputFile.compare("-1")!=0){
            theSerializer->writeToURI(createDOMTree(), xercesc::XMLString::transcode(outputFile.c_str()));
        } else {
            theSerializer->writeToURI(createDOMTree(), xercesc::XMLString::transcode(_outputFile.c_str()));
        }

	} catch(std::logic_error& e){
		error = std::logic_error(e.what());
		errorOccured = true;
	} catch(...){
		error = std::logic_error("An error occured during writing the XML file!");
		errorOccured = true;
    }

	 //Clean everything up
	
	 xercesc::XMLPlatformUtils::Terminate();
	 
	 if(errorOccured){
		throw error;
	 }
}

void XMLParser::loadXML(std::string inputFile){

    boost::any* value = new boost::any();
	bool errorOccured = false;
	std::logic_error error("");

	xercesc::ErrorHandler* errHandler = (xercesc::ErrorHandler*) new xercesc::HandlerBase();

    try{
        initialize();

		_parser->setErrorHandler(errHandler);

        //Checks wether the user passed a new output path.
        if(inputFile.compare("-1")!=0){
            _parser->parse(xercesc::XMLString::transcode(inputFile.c_str()));
        } else {
            _parser->parse(xercesc::XMLString::transcode(_inputFile.c_str()));
        }

        xercesc::DOMDocument*	doc			= _parser->getDocument();
        xercesc::DOMNode*		root		= doc->getDocumentElement();

        xercesc::DOMNodeList*	categories	= root->getChildNodes();
        xercesc::DOMNodeList*	keys;

        _categories->clear();	//Clear the current XML tree

        //Load the xml file and save the xml tree
        for(unsigned int i = 0; i < categories->getLength(); ++i){
            if(categories->item(i)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE){
                keys = categories->item(i)->getChildNodes();

                for(unsigned int j = 0; j < keys->getLength(); j++){
                    if(keys->item(j)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE){

                        convertToAny(keys->item(j), value);

                        add(xercesc::XMLString::transcode(categories->item(i)->getNodeName()), 
                            xercesc::XMLString::transcode(keys->item(j)->getNodeName()), 
                            *value);
                    }
                }

            }

        }

        //Clean up
        delete doc;

    } catch (const xercesc::XMLException& toCatch) {
		error = std::logic_error(std::string("Exception message is: ").append(xercesc::XMLString::transcode(toCatch.getMessage())).append("\n").c_str());
		errorOccured = true;
    } catch (const xercesc::DOMException& toCatch) {
		error = std::logic_error(std::string("Exception message is: ").append(xercesc::XMLString::transcode(toCatch.msg)).append("\n").c_str());
		errorOccured = true;
	} catch(const xercesc::SAXParseException& toCatch){
		error = std::logic_error(std::string("Exception message is: ").append(xercesc::XMLString::transcode(toCatch.getMessage())).append("\n").c_str());
		errorOccured = true;
	} catch(std::exception& e){
		error = std::logic_error(e.what());
		errorOccured = true;
	} catch(...){
		error = std::logic_error("An error occured while loading the xml file!");
		errorOccured = true;
	} 

	_parser->reset();
    delete value; 
	delete errHandler;
	xercesc::XMLPlatformUtils::Terminate();

	if(errorOccured){
		throw error;
	}
}

/** initializes xerces to be ready for parsing
**/
void XMLParser::initialize(){
    try{
        xercesc::XMLPlatformUtils::Initialize();
    } catch( const xercesc::XMLException& toCatch){
		throw std::logic_error(xercesc::XMLString::transcode(toCatch.getMessage()));;
    }

    _parser			= new xercesc::XercesDOMParser();
    _parser->setDoNamespaces(true);
    _parser->setValidationConstraintFatal(false);
}

/**Creates a XML Tree for xerces which is used to write the data to a xml file
**/
xercesc::DOMNode* XMLParser::createDOMTree(){

    xercesc::DOMImplementation* impl			= xercesc::DOMImplementationRegistry::getDOMImplementation(xercesc::XMLString::transcode("Core"));
    xercesc::DOMDocument*	doc					= impl->createDocument(0, xercesc::XMLString::transcode("root"), 0);
    xercesc::DOMElement*	rootElement			= doc->getDocumentElement();
    xercesc::DOMElement*	categorie;
    xercesc::DOMElement*	key;
    xercesc::DOMText*		text;

    //Create an iterator for the categories and one for the values
    categories::iterator itc = _categories->begin();
    t_map::iterator itk;
        
    std::string str; 

    for(; itc != _categories->end(); itc++){
        categorie = doc->createElement(xercesc::XMLString::transcode(itc->first.c_str()));
        itk = itc->second->begin();

        for(; itk != itc->second->end(); itk++){
            key = doc->createElement(xercesc::XMLString::transcode(itk->first.c_str()));

            //Check if the value is a string and adapt "string" to the type-information of the key
            if(std::string(typeid(std::string).name()).compare(itk->second.type().name()) == 0){
                key->setAttribute(xercesc::XMLString::transcode("type"), xercesc::XMLString::transcode("string"));
            } else {
                key->setAttribute(xercesc::XMLString::transcode("type"), xercesc::XMLString::transcode(itk->second.type().name()));
            }

            convertToString(&str, &itk->second);	//Converts the value to a string, so it can bee saved
            text = doc->createTextNode(xercesc::XMLString::transcode(str.c_str()));
            key->appendChild(text);

            categorie->appendChild(key);
        }
        rootElement->appendChild(categorie);
    }

    return rootElement;
}

/**Converts the node value to the particular type
**/
void XMLParser::convertToAny(xercesc::DOMNode* node, boost::any* value){

    //Get the type information of the value. 
    std::string type = std::string(xercesc::XMLString::transcode(node->getAttributes()->item(0)->getTextContent()));
    //Get the value
    std::string val = std::string(xercesc::XMLString::transcode(node->getTextContent()));

    //Convert the value to the particular type and save it in "boost::any value"
    try{
        if(type.compare("bool") == 0){
            *value = boost::lexical_cast<bool>(val);
        } else if(type.compare("int") == 0){
            *value = boost::lexical_cast<int>(val);
        } else if(type.compare("unsigned int") == 0){
            *value = boost::lexical_cast<unsigned int>(val);
        } else if(type.compare("short int") == 0){
            *value = boost::lexical_cast<short int>(val);
        } else if(type.compare("float") == 0){
            *value = boost::lexical_cast<float>(val);
        } else if(type.compare("double") == 0){
            *value = boost::lexical_cast<double>(val);
        } else if(type.compare("long double") == 0){
            *value = boost::lexical_cast<long double>(val);
        } else if(type.compare("long") == 0){
            *value = boost::lexical_cast<long>(val);
        } else if(type.compare("unsigned long") == 0){
            *value = boost::lexical_cast<unsigned long>(val);
        } else if(type.compare("short") == 0){
            *value = boost::lexical_cast<short>(val);
        } else if(type.compare("string") == 0){
            *value = val;
        }

    } catch(boost::bad_lexical_cast e){
		throw std::logic_error(e.what());
    }
}

/**Converts the given value to a string
**/
void XMLParser::convertToString(std::string* str, boost::any* value){
    std::string type(value->type().name());	//Get the type of the value

    //Convert the value to a string and save the string in "str"
    try{
        if(type.compare("bool") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<bool>(*value));
        } else if(type.compare("int") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<int>(*value));
        } else if(type.compare("unsigned int") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<unsigned int>(*value));
        } else if(type.compare("short int") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<short int>(*value));
        } else if(type.compare("float") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<float>(*value));
        } else if(type.compare("double") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<double>(*value));
        } else if(type.compare("long double") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<long double>(*value));
        } else if(type.compare("long") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<long>(*value));
        } else if(type.compare("unsigned long") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<unsigned long>(*value));
        } else if(type.compare("short") == 0){
            *str = boost::lexical_cast<std::string>(boost::any_cast<short>(*value));
        } else if(type.compare(typeid(std::string).name()) == 0){
            *str = boost::any_cast<std::string>(*value);
        }
    }catch(boost::bad_lexical_cast e){
		throw std::logic_error(e.what());
    }
}
	