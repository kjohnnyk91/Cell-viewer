#include <iostream>
#include <limits>
#include <vector>
#include "Node.hpp"
#include "Net.hpp"
#include "Cell.hpp"
#include "xmlutil.hpp"
#include "stringToInt.hpp"
//#include "Term.hpp"

namespace Netlist
{
	class Net;
	
	Net::Net(Cell* c, const std::string& s, Term::Type t):
		name_(s),
		type_(t)
	{
		owner_ = c;
		this-> id_ = (c-> newNetId());
		c-> add(this);
	}
			
	Net::~Net()
	{}
	
	Cell* Net::getCell() const
	{
		return owner_;
	}
			
	const std::string& Net::getName() const
	{
		return name_;
	}
			
	unsigned int Net::getId() const
	{
		return id_;
	}
			
	Term::Type Net::getType() const
	{
		return type_;
	}
	
	Node* Net::getNode(unsigned int id) const
	{
		// std::cout << "Net::getNode() : Looking for the node " << id << std::endl;
		
		for(std::vector<Node*>::const_iterator it = nodes_.begin(); it != nodes_.end(); it++)
		{
			// std::cout << "Net::getNode() : The current node is " << (*it)-> getId() << std::endl;
			
			if(((*it)-> getId()) == id)
			{
				// std::cout << "Net::getNode() : The node " << id << " was found" << std::endl;
				
				return *it;
			}
		}
		
		// std::cout << "The node " << id << " was not found" << std::endl;
		
		return NULL;
	}
	
	const std::vector<Node*>& Net::getNodes() const
	{
		return nodes_;
	}
			
	size_t Net::getFreeNodeId() const
	{
		int i = 0;
		
		for(std::vector<Node*>::const_iterator it = nodes_.begin(); it != nodes_.end(); it++)				// Tant que l'index du noeud est plus petit que la taille du vecteur
		{
			if((*it) == NULL)			// Si le noeud i du vecteur est vide
			{
				return i;					// La fonction renvoie la valeur de l'index
			}
			
			i++;							// Sinon on incrémente l'index pour chercher ce qu'il y a au noeud suivant
		}
		
		return nodes_.size();				// Si le vecteur ne contient pas de noeud vide, alors on renvoie la taille du vecteur de noeud
	}
	
	void Net::add(Node* node)
	{
		int n = this-> getFreeNodeId();
		int m = this-> nodes_.size();
		
		node-> setId(n);
		
		if(n<m)
		{
			nodes_.at(n) = node;
			nodes_.at(n)-> setId(n);
			id_ = n;
		}
		else
		{
			nodes_.push_back(node);
			id_ = m;
		}
		
		if(owner_-> getNet(name_) == NULL )
		{
			owner_-> add(this);
		}
	}
			
	bool Net::remove(Node* node)
	{
		for(std::vector<Node*>::iterator it = this-> nodes_.begin(); it != this-> nodes_.end(); it++)
		{
			if(*it == node)
			{
				this-> nodes_.erase(it);
				
				return true;
			}
		}
		
		return false;
	}
	
	void Net::toXml(std::ostream& stream)
	{
		stream << indent << "<net name=" << '"' << name_ << '"' << " type=" << '"' << Term::toString(type_) << '"' << "/>" << std::endl;
		
		indent++;
		
		for(std::vector<Node*>::iterator it = nodes_.begin(); it != nodes_.end(); it++)
		{			
			(*it)-> toXml(stream);
		}
		
		for(std::vector<Line*>::iterator it = lines_.begin(); it != lines_.end(); it++)
		{			
			(*it)-> toXml(stream);
		}
		
		indent--;
		
		stream << indent << "</net>" << std::endl;
	}
	
	bool Net::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		int status = 1;
		
		Net* net = NULL;
		Term* term = NULL;
		Node* node = NULL;
		
		const xmlChar* netTag = xmlTextReaderConstString(reader, (const xmlChar*) "net");
		const xmlChar* nodeTag = xmlTextReaderConstString(reader, (const xmlChar*) "node");
		const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*) "line");
		const xmlChar* junkTag = xmlTextReaderConstString(reader, (const xmlChar*) "#text");
		
		std::string netName;
		std::string netType;
		std::string termName;
		std::string nodeId;
		std::string instanceName;
		
		if(cell == NULL)
		{
			std::cerr << "{ERROR] The Cell in parameter is undefined" << std::endl;
			
			return false;
		}
		
		while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag) )
		{
			status = xmlTextReaderRead(reader);
			
			if(status != 1)
			{
				if(status == 0)
				{
					std::cout << "End of XML file reached" << std::endl;
				}
				else
				{
					std::cerr << "[ERROR] Incorrect XML syntax" << std::endl;
				}
				
				return false;
			}
		}
		
		if( (status == 1) and (xmlTextReaderConstLocalName(reader) == netTag) and (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT) )
		{
			netName = xmlCharToString( xmlTextReaderGetAttribute(reader, (const xmlChar*) "name"));
			netType = xmlCharToString( xmlTextReaderGetAttribute(reader, (const xmlChar*) "type"));
			
			if(netName.empty())
			{
				std::cerr << "[ERROR] The name of the net is undefined" << std::endl;
				
				return false;
			}
			
			if(netType.empty())
			{
				std::cerr << "[ERROR] The type of the net is undefined" << std::endl;
				
				return false;
			}
			
			if(netType == "External")
			{
				try
				{
					net = new Net(cell, netName, Term::External);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "The construction of a new net failed because of a " << ba.what() << std::endl;
					
					return false;
				}
			}
			else
			{
				try
				{
					net = new Net(cell, netName, Term::Internal);
				}
				catch(const std::bad_alloc& ba)
				{
					std::cerr << "The construction of a new net failed because of a " << ba.what() << std::endl;
					
					return false;
				}
			}
			
			status = xmlTextReaderRead(reader);
			
			
			while( not (( xmlTextReaderNodeType(reader) == XML_READER_TYPE_END_ELEMENT ) and ( xmlTextReaderConstLocalName(reader) == netTag)) )
			{
				while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag) )
				{
					status = xmlTextReaderRead(reader);
					
					if(status != 1)
					{
						if(status == 0)
						{
							std::cout << "End of XML file reached" << std::endl;
						}
						else
						{
							std::cerr << "[ERROR] Incorrect XML syntax" << std::endl;
						}
						
						return false;
					}
				}
				
				bool fromXml;
				
				if(xmlTextReaderConstLocalName(reader) == nodeTag)
				{
					fromXml = Node::fromXml(net,reader);
					
					if(fromXml == false)
					{
						std::cerr << "[ERROR] Node::fromXml() failed" << std::endl;
						
						return false;
					}
				}
				
				if(xmlTextReaderConstLocalName(reader) == lineTag)
				{
					fromXml = Line::fromXml(net,reader);
					
					if(fromXml == false)
					{
						std::cerr << "[ERROR] Line::fromXml() failed" << std::endl;
						
						return false;
					}
				}
				
				status = xmlTextReaderRead(reader);
				
				while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag) )
				{
					status == xmlTextReaderRead(reader);
					
					if(status != 1)
					{
						if(status == 0)
						{
							std::cout << "End of XML file reached" << std::endl;
						}
						else
						{
							std::cerr << "[ERROR] Incorrect XML syntax" << std::endl;
						}
						
						return false;
					}	
				}
			}
			
			std::cout << "Net load complete" << std::endl;
			
			return true;
		}
	}
	
	void Net::add( Line* line )
	{
		if(line) lines_.push_back( line );
	}

	bool Net::remove( Line* line )
	{
		if(line)
		{
			for( std::vector<Line*>::iterator il = lines_.begin(); il != lines_.end(); ++il )
			{
				if (*il == line)
				{
					lines_.erase( il );
					
					return true;
				}
			}
		}
	
		return false;
	}
}