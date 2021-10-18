// -*- explicit-buffer-name: "Node.cpp<M1-MOBJ/7>" -*-

#include <limits>
#include "xmlutil.hpp"
#include "node.hpp"
#include "line.hpp"
#include "term.hpp"
#include "net.hpp"
#include "instance.hpp"
#include "cell.hpp"
#include "stringToInt.hpp"

namespace Netlist
{
	using namespace std;

	const size_t  Node::noid = numeric_limits<size_t>::max();

	Node::Node( size_t id ):
		id_(id),
		position_(),
		lines_()
	{}

	Node::~Node()
	{
		while ( not lines_.empty() )
		{
			delete lines_[0];
		}
	}

	void Node::attach( Line* line )
	{
		for( size_t i=0 ; i<lines_.size() ; ++i )
		{
			if(lines_[i] == line) return;
		}
    
		lines_.push_back( line );
	}

	void Node::detach( Line* line )
	{
		vector<Line*>::iterator iline;
		
		for(iline = lines_.begin() ; iline != lines_.end() ; ++iline )
		{
			if((*iline) == line)
			{
				lines_.erase( iline );
				
				break;
			}
		}
	}

	NodePoint::NodePoint( Net* net, size_t id, Point p ):
		Node(id), net_(net)
	{
		setPosition( p );
		net_-> add( this );
	}

	NodePoint::~NodePoint()
	{
		net_-> remove( this );
	}

	Net* NodePoint::getNet() const
	{
		return net_;
	}

	void NodePoint::toXml( ostream& stream ) const
	{
		stream << indent << "<node x=\""  << position_.getX() << "\" y=\""  << position_.getY() << "\" id=\"" << id_ << "\"/>\n";
	}
	
	NodeTerm::NodeTerm( Term* term, size_t id ):
		Node(id),
		term_(term)
	{}

	NodeTerm::~NodeTerm()
	{
		if(getNet()) getNet()-> remove( this );
	}

	Net* NodeTerm::getNet() const
	{
		return term_->getNet();
	}

	void NodeTerm::toXml( ostream& stream ) const
	{
		if(term_->isInternal())
		{
			stream << indent << "<node term=\"" << term_->getName() << "\" instance=\"" << term_->getInstance()->getName() << "\" id=\""       << id_;
		}
		else
		{
			stream << indent << "<node term=\"" << term_->getName() << "\" id=\"" << id_;
		}
    
		stream << "\" x=\"" << position_.getX() << "\" y=\"" << position_.getY() << "\"/>\n";
	}

	bool Node::fromXml( Net* net, xmlTextReaderPtr reader )
	{
		int status = 1;
		int iid = 0;
		int ix = 0;
		int iy = 0;
		
		Term* term = NULL;
		NodePoint* nodepoint = NULL;
		Cell* cell = net-> getCell();
		std::string cellName = cell-> getName();
		const std::string netName = net-> getName();
		
		const xmlChar* nodeTag = xmlTextReaderConstString(reader, (const xmlChar*) "node");
		const xmlChar* junkTag = xmlTextReaderConstString(reader, (const xmlChar*) "#text");
	
		while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag) )
		{
			status = xmlTextReaderRead(reader);
		
			if(status != 1)
			{
				if(status == 0)
				{
					std::cerr << "The end of the XML file is reached" << std::endl;
				}
				else
				{
					std::cerr << "The syntax of the XML file is incorrect" << std::endl;
				}
				
				return false;
			}
		}
		
		if(xmlTextReaderConstLocalName(reader) == nodeTag)
		{
			std::string termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "term"));
			std::string instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "instance"));
			std::string x = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x"));
			std::string y = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y"));
			std::string id = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "id"));
			
			if(id.empty())
			{
				std::cerr << "[ERROR] The id of the node is undefined" << std::endl;
				
				return false;
			}
			else
			{
				try
				{
					iid = stringToInt(id);
				}
				catch(const char& unexp)
				{
					std::cerr << "[ERROR] The character " << unexp << " is not a figure in the attribute id" << std::endl;
					
					return false;
				}
			}
			
			if(termName.empty())
			{
				std::cout << "Connexion d'un NodePoint à la net " << netName << std::endl;
				
				if( (x.empty()) or (y.empty()) )
				{
					std::cerr << "[ERROR] The position of the NodePoint is undefined" << std::endl;
					
					return NULL;
				}
				
				try
				{
					ix = stringToInt(x);
				}
				catch(const char& unexp)
				{
					std::cerr << "[ERRO] The character " << unexp << " is not a figure in the attribute x" << std::endl;
					
					return false;
				}
				
				try
				{
					iy = stringToInt(y);
				}
				catch(const char& unexp)
				{
					std::cerr << "[ERRO] The character " << unexp << " is not a figure in the attribute y" << std::endl;
					
					return false;
				}
				
				try
				{
					nodepoint = new NodePoint(net, iid, Point(ix,iy));
				}
				catch(std::bad_alloc& ba)
				{
					std::cerr << "[ERROR] The construction of a new NodePoint failed because of a " << ba.what() << std::endl;
					
					return false;
				}
				
				std::cout << "The NodePoint " << nodepoint-> getId() << " was added to the net " << netName << " successfully" << std::endl;
				
				return true;
			}
			else
			{
				if(instanceName.empty())
				{
					std::cout << "Connexion between the net " << netName << " to the terminal " << termName << " of the Cell " << cellName << std::endl;
					
					term = cell-> getTerm(termName);
					
					if(term == NULL)
					{
						std::cerr << "The terminal " << termName << " was not found in the Cell " << cellName << std::endl;
						
						return false;
					}
				}
				else
				{
					Instance* instance = NULL;
					instance = cell-> getInstance(instanceName);
					
					if(instance == NULL)
					{
						std::cerr << "[ERROR] The instance " << instanceName << " was not found in the Cell " << cellName << std::endl;
						
						return false;
					}
					
					term = instance-> getTerm(termName);
					
					if(term == NULL)
					{
						std::cerr << "The terminal " << termName << " was not found in the Instance " << instanceName << std::endl;
						
						return false;
					}
				}
				
				(term-> getNode())-> setId(iid);
				term-> setNet(net);
				
				return true;
			}			
		}
	}
	
	Node& Node::operator=(const Node& node)
	{
		id_ = node.getId();
		position_ = node.getPosition();
		lines_ = node.getLines();
		
		return *this;
	}
	
	NodePoint& NodePoint::operator=(const NodePoint& nodePoint)
	{
		id_ = nodePoint.getId();
		position_ = nodePoint.getPosition();
		lines_ = nodePoint.getLines();
		net_ = nodePoint.getNet();
		
		return *this;
	}
	
	NodeTerm& NodeTerm::operator=(const NodeTerm& nodeTerm)
	{
		id_ = nodeTerm.getId();
		position_ = nodeTerm.getPosition();
		lines_ = nodeTerm.getLines();
		term_ = nodeTerm.getTerm();
		
		return *this;
	}
	
	
}  // Netlist namespace.