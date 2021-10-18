// -*- explicit-buffer-name: "Line.cpp<M1-MOBJ/7>" -*-

#include <iostream>
#include "xmlutil.hpp"
#include "line.hpp"
#include "cell.hpp"
#include "net.hpp"
#include "stringToInt.hpp"

namespace Netlist
{
	Line::Line ( Node* source, Node* target ):
		source_(source),
		target_(target)
	{
		source_-> attach( this );
		target_-> attach( this );
	}
	
	Line::~Line()
	{
		source_-> detach( this );
		target_-> detach( this );
		target_-> getNet()->remove( this );
	}

	void  Line::toXml ( std::ostream& stream ) const
	{
		stream << indent << "<Line source=\"" << source_-> getId() << "\" target=\"" << target_-> getId() << "\"/>\n";
	}

	bool Line::fromXml( Net* net, xmlTextReaderPtr reader )
	{
		const xmlChar* lineTag  = xmlTextReaderConstString        ( reader, (const xmlChar*)"line" );
		const xmlChar* nodeName = xmlTextReaderConstLocalName     ( reader );

		if (lineTag == nodeName)
		{
			int iidSource = 0;
			int iidTarget = 0;
			std::string idSource;
			std::string idTarget;
			idSource = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*) "source"));
			idTarget = xmlCharToString(xmlTextReaderGetAttribute( reader, (const xmlChar*) "target"));
			
			try
			{
				iidSource = stringToInt(idSource);
			}
			catch(const char& unexp)
			{
				std::cerr << "The character " << unexp << " is not a figure in the attribute source" << std::endl;
				
				return NULL;
			}
			
			try
			{
				iidTarget = stringToInt(idTarget);
			}
			catch(const char& unexp)
			{
				std::cerr << "The character " << unexp << " is not a figure in the attribute target" << std::endl;
				
				return NULL;
			}
			
			Node* source = net-> getNode( iidSource );
			Node* target = net-> getNode( iidTarget );

			if(not source)
			{
				std::cerr << "[ERROR] Line::fromXml(): Unknown source node id:" << idSource << " (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
	
				return false;
			}
      
			if (not target)
			{
				std::cerr << "[ERROR] Line::fromXml(): Unknown target node id:" << idTarget << " (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;
        
				return false;
			}

			net-> add( new Line(source,target) );
			
			return true;
		}

		return false;
	}
}  // Netlist namespace.
