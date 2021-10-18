#include <iostream>
#include <fstream>
#include "indentation.hpp"
#include "shape.hpp"
#include "box.hpp"
#include "symbol.hpp"
#include "cell.hpp"
#include "term.hpp"
#include "stringToInt.hpp"

namespace Netlist
{
	Shape::Shape( Netlist::Symbol* owner ):
		owner_(owner)
	{}

	Shape::~Shape()
	{
		owner_-> remove( this );
	}

	Box Shape::getBoundingBox() const
	{
		return owner_-> getBoundingBox();
	}

	/* std::string Shape::getNameAlign()
	{
		return "";
	} */
	
	// void Shape::toXml(std::ostream& stream)
	// {
		// std::cout << "Shape::toXml() : Execution" << std::endl;
	// }
	
	Shape* Shape::fromXml( Symbol* owner, xmlTextReaderPtr reader )
	{
		const xmlChar* boxTag = xmlTextReaderConstString( reader, (const xmlChar*)"box" );

		const xmlChar* ellipseTag = xmlTextReaderConstString( reader, (const xmlChar*)"ellipse" );

		const xmlChar* arcTag = xmlTextReaderConstString( reader, (const xmlChar*)"arc" );

		const xmlChar* lineTag = xmlTextReaderConstString( reader, (const xmlChar*)"line" );

		const xmlChar* termTag = xmlTextReaderConstString( reader, (const xmlChar*)"term" );

		const xmlChar* nodeName = xmlTextReaderConstLocalName( reader );

		Shape* shape = NULL;
  
		if( boxTag == nodeName ) shape = BoxShape::fromXml( owner, reader );
		
		if( ellipseTag == nodeName ) shape = EllipseShape::fromXml( owner, reader );

		if( arcTag == nodeName ) shape = ArcShape::fromXml( owner, reader );
  
		if( lineTag == nodeName ) shape = LineShape::fromXml( owner, reader );
  
		if( termTag == nodeName ) shape = TermShape::fromXml( owner, reader );

		if (shape == NULL) std::cerr << "[ERROR] Unknown or misplaced tag <" << nodeName << "> (line:" << xmlTextReaderGetParserLineNumber(reader) << ")." << std::endl;

		std::cout << "--- Shape::fromXml() : Fin d'execution " << std::endl;
		
		return shape;
	}
	
	LineShape::LineShape(Symbol* owner, int x1, int y1, int x2, int y2):
		Shape(owner),
		x1_(x1),
		y1_(y1),
		x2_(x2),
		y2_(y2)
	{
		owner-> add(this);
	}

	LineShape::~LineShape()
	{}

	Box LineShape::getBoundingBox() const
	{
		if(x1_ < x2_)
		{
			if(y1_ < y2_)
			{
				return Box(x1_,y1_,x2_, y2_);
			}
			else
			{
				return Box(x1_,y2_,x2_,y1_);
			}
		}
		else
		{
			if(y1_ < y2_)
			{
				return Box(x2_,y1_,x1_, y2_);
			}
			else
			{
				return Box(x2_,y2_,x1_,y1_);
			}
		}
	}

	void LineShape::toXml(std::ostream& stream)
	{
		// std::cout << "LineShape::toXml() : Execution" << std::endl;
		
		stream << indent << "<line x1=" << '"' << x1_ << '"' << " y1=" << '"' << y1_ << '"' << " x2=" << '"' << x2_ << '"' << " y2=" << '"' << y2_ << '"' << "/>" << std::endl;
	}

	LineShape* LineShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*) "line");
		
		if(owner == NULL)
		{
			std::cerr << "[ERROR] The Symbol in argument is undefined" << std::endl;
			
			return NULL;
		}
		
		if(xmlTextReaderConstLocalName(reader) != lineTag)
		{
			std::cerr << "[ERROR] The node is not a LineShape node." << std::endl;
			
			return NULL;
		}
		
		int ix1 = 0;
		int iy1 = 0;
		int ix2 = 0;
		int iy2 = 0;
		std::string x1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x1"));
		std::string x2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x2"));
		std::string y1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y1"));
		std::string y2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y2"));
		LineShape* lineshape = NULL;
		
		if( (x1.empty()) or (x2.empty()) or (y1.empty()) or (y2.empty()) )					// Vérification des coordonnées
		{
			std::cerr << "[ERROR] The positions of the line is undefined." << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix1 = stringToInt(x1);
		}
		catch(const char& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix2 = stringToInt(x2);
		}
		catch(const char& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x2" << std::endl;
			
			return NULL;
		}
		
		try
		{
			iy1 = stringToInt(y1);
		}
		catch(const char& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			iy2 = stringToInt(y2);
		}
		catch(const char& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y2" << std::endl;
			
			return NULL;
		}
		
		try
		{
			lineshape = new LineShape(owner, ix1, iy1, ix2, iy2);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "[ERROR] The construction of a new LineShape failed because of a " << ba.what() << std::endl;
			
			return NULL;
		}
		
		std::cout << "The LineShape of the Cell " << (owner-> getCell())-> getName() << " was constructed successfully" << std::endl << std::endl;
		
		return lineshape;
	}

	BoxShape::BoxShape( Symbol* owner , const  Box& box ): 
		Shape(owner), box_(box)
	{
		owner-> add(this);
	}

	BoxShape::~BoxShape()
	{}

	/* Box BoxShape::getBoundingBox() const
	{
		return  box_;
	} */

	void BoxShape::toXml(std::ostream& stream)
	{
		// std::cout << "BoxShape::toXml() : Execution" << std::endl;
		
		int boxshapeX1 = getBoundingBox().getX1();
		int boxshapeY1 = getBoundingBox().getY1();
		int boxshapeX2 = getBoundingBox().getX2();
		int boxshapeY2 = getBoundingBox().getY2();
		
		stream << indent << "<box x1=" << '"' << boxshapeX1 << '"' << " y1=" << '"' << boxshapeY1 << '"' << " x2=" << '"' << boxshapeX2 << '"' << " y2=" << '"' << boxshapeY2 << '"' << "/>" << std::endl;
	}

	BoxShape* BoxShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		const xmlChar* boxTag = xmlTextReaderConstString(reader, (const xmlChar*) "box");
		
		if(owner == NULL)					// Vérification
		{
			std::cerr << "[ERROR] The Symbol in argument is undefined." << std::endl;
			
			return NULL;
		}
		
		if(xmlTextReaderConstLocalName(reader) != boxTag)					// Verification
		{
			std::cerr << "[ERROR] The node is not a BoxShape node." << std::endl;
			
			return NULL;
		}
		
		int ix1 = 0;					// Initialisations
		int ix2 = 0;
		int iy1 = 0;
		int iy2 = 0;
		std::string x1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x1"));					// Acquisitions
		std::string x2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x2"));
		std::string y1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y1"));
		std::string y2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y2"));
		BoxShape* boxshape = NULL;
		
		if( (x1.empty()) or (x2.empty()) or (y1.empty()) or (y2.empty()) )					// Vérification
		{
			std::cerr << "[ERROR] The position of the box is undefined." << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix1 = stringToInt(x1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix2 = stringToInt(x2);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x2" << std::endl;
			
			return NULL;
		}
		
		try
		{
			iy1 = stringToInt(y1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			iy2 = stringToInt(y2);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y2" << std::endl;
			
			return NULL;
		}
		
		Box* pbox = NULL;
		pbox = new Box(ix1,iy1,ix2,iy2);
		
		if(pbox == NULL)
		{
			std::cout << "La construction d'une box a échoué." << std::endl;
			
			return NULL;
		}
		
		try
		{
			boxshape = new BoxShape(owner, *pbox);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "[ERROR] The construction of a new BoxShape failed because of a " << ba.what() << std::endl;
			
			return NULL;
		}
		
		std::cout << "The BoxShape of the Cell " << (owner-> getCell())-> getName() << " was constructed successfully" << std::endl << std::endl;
		
		return boxshape;
	}

	TermShape::TermShape(Symbol* owner, std::string name, int x1, int y1, NameAlign align):
		Shape(owner),
		term_(NULL),
		x1_(x1),
		y1_(y1),
		align_(align)
	{
		Cell* cell = owner-> getCell();
		
		term_ = cell-> getTerm(name);
		
		owner-> add(this);
	}

	TermShape::~TermShape()
	{}

	std::string TermShape::NameAlignToString(NameAlign align)
	{
		switch(align)
		{
			case TermShape::TopLeft :
				return "top_left";
				break;
				
			case TermShape::TopRight :
				return "top_right";
				break;
				
			case TermShape::BottomLeft :
				return "bottom_left";
				break;
				
			case TermShape::BottomRight :
				return "bottom_right";
				break;
				
			default :
				return NULL;
				break;
		}
	}

	TermShape::NameAlign TermShape::StringToNameAlign(std::string align)
	{
		if(align == "top_left") return TermShape::TopLeft;
		
		if(align == "bottom_left") return TermShape::BottomLeft;
		
		if(align == "top_right") return TermShape::TopRight;
		
		if(align == "bottom_right") return TermShape::BottomRight;
		
		throw std::string("[ERROR] Unknown align");
	}

	TermShape::NameAlign TermShape::getNameAlign() const
	{
		return align_;
	}
	
	void TermShape::toXml(std::ostream& stream)
	{
		// std::cout << "TermShape::toXml() : Execution" << std::endl;
		
		Term* term = getTerm();
		
		stream << indent << "<term name=" << '"' << term-> getName() << '"' << " x1=" << '"' << x1_ << '"' << " y1=" << '"' << y1_ << " align=" << '"' << NameAlignToString(align_) << '"' << "/>" << std::endl;
	}

	TermShape* TermShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		// std::cout << "--------- TermShape::fromXml() : Execution" << std::endl;
		
		const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*) "term");
		
		if(owner == NULL)
		{
			std::cerr << "[ERROR] The Symbol in argument is undefined." << std::endl;
			
			return NULL;
		}
		
		if(xmlTextReaderConstLocalName(reader) != termTag)
		{
			std::cerr << "[ERROR] The node is not a TermShape node." << std::endl;
			
			return NULL;
		}
		
		int ix1 = 0;
		int iy1 = 0;
		std::string name = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "name"));
		std::string x1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x1"));
		std::string y1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y1"));
		std::string align = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "align"));
		Term* term = NULL;
		TermShape* termshape = NULL;
		
		if(name.empty())
		{
			std::cerr << "[ERROR] The Termshape term's name is undefined." << std::endl;
			
			return NULL;
		}

		term = (owner-> getCell())-> getTerm(name);
		
		if(term == NULL)
		{
			std::cerr << "The term " << name << " doesn't exist." << std::endl;
			
			return NULL;
		}
		
		if( (x1.empty()) or (y1.empty()) )
		{
			std::cerr << "[ERROR] The position of the TermShape is undefined" << std::endl;
			
			return NULL;
		}
		
		if(align.empty())
		{
			std::cerr << "[ERROR] The NameAlign is undefined." << std::endl;
			
			return NULL;
		}
		
		if( (align != "top_right") and (align != "top_left") and (align != "bottom_right") and (align != "bottom_left") )
		{
			std::cerr << "[ERROR] The NameAlign " << align << " is not valid." << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix1 = stringToInt(x1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure" << std::endl;
			
			return NULL;
		}
		
		try
		{
			iy1 = stringToInt(y1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure" << std::endl;
		}
		
		// std::cout << "--- Shape::fromXml() : Construction du termshape " << std::endl;
		
		try
		{
			termshape = new TermShape(owner, name, ix1, iy1, StringToNameAlign(align));
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "{ERROR] The construction of a TermShape failed because of a " << ba.what() << std::endl;
			
			return NULL;
		}
		
		std::cout << "The NameAlign of the TermShape " << name << " is " << align << std::endl;
		
		std::cout << "The TermShape of the Cell " << (owner-> getCell())-> getName() << " was constructed successfully" << std::endl << std::endl;
		
		// std::cout << "--------- TermShape::fromXml() : Fin execution" << std::endl;
		
		return termshape;
	}
	
	ArcShape::ArcShape(Symbol* owner, Box box, int start, int span):
		Shape(owner),
		box_(box),
		start_(start),
		span_(span)
	{
		owner-> add(this);
	}
	
	ArcShape::~ArcShape()
	{}
	
	void ArcShape::toXml(std::ostream& stream)
	{
		// std::cout << "ArcShape::toXml() : Execution" << std::endl;
		
		stream << indent << "<arc x1=" << '"' << getX1() << '"' << " y1=" << '"' << getY1() << '"' << " x2=" << '"' << getX2() << '"' << " y2=" << '"' << getY2() << '"' << " start=" << '"' << start_ << '"' << " span=" << '"' << span_ << '"' << "/>" << std::endl;
	}
	
	ArcShape* ArcShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		const xmlChar* arcTag = xmlTextReaderConstString( reader, (const xmlChar*) "arc");
		ArcShape* arcshape = NULL;
		
		if (owner == NULL )
		{
			std::cerr << "The Symbol in argument is undefined" << std::endl;
		}
		
		if( xmlTextReaderConstLocalName(reader) != arcTag )
		{
			std::cerr << "The node is not an ArcShape node" << std::endl;
		}
		
		int ix1 = 0;
		int ix2 = 0;
		int iy1 = 0;
		int iy2 = 0;
		int istart = 0;
		int ispan = 0;
		
		std::string x1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x1"));
		std::string x2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x2"));
		std::string y1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y1"));
		std::string y2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y2"));
		std::string start = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "start"));
		std::string span = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "span"));
		
		if( (x1.empty()) or (x2.empty()) or (y1.empty()) or (y2.empty()) )
		{
			std::cerr << "[ERROR] The position of the ArcShape is undefined" << std::endl;
			
			return NULL;
		}
		
		if( (start.empty()) or (span.empty()) )
		{
			std::cerr << "[ERROR] The angle of the ArcShape is undefined" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix1 = stringToInt(x1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix2 = stringToInt(x2);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x2" << std::endl;
			
			return NULL;
		}
		try
		{
			iy1 = stringToInt(y1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y1" << std::endl;
			
			return NULL;
		}
		try
		{
			iy2 = stringToInt(y2);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y2" << std::endl;
			
			return NULL;
		}
		
		try
		{
			istart = stringToInt(start);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute start" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ispan = stringToInt(span);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute span" << std::endl;
			
			return NULL;
		}
		
		try
		{
			arcshape = new ArcShape(owner, Box(ix1,iy1,ix2,iy2),istart,ispan);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "[ERROR] The construction of a new ArcShape failed because of a " << ba.what() << std::endl;
			
			return NULL;
		}
		
		std::cout << "The ArcShape of the Cell " << (owner-> getCell())-> getName() << " was constructed successfully" << std::endl << std::endl;
		
		return arcshape;
	}
	
	EllipseShape::EllipseShape(Symbol* owner, Box box):
		Shape(owner),
		box_(box)
	{
		owner-> add(this);
	}
	
	EllipseShape::~EllipseShape()
	{}
	
	void EllipseShape::toXml(std::ostream& stream)
	{
		// std::cout << "EllipseShape::toXml() : Execution" << std::endl;
		int x1 = box_.getX1();
		int x2 = box_.getX2();
		int y1 = box_.getY1();
		int y2 = box_.getY2();
		
		stream << "<ellipse x1=" << '"' << x1 << '"' << " y1=" << '"' << y1 << '"' << " x2=" << '"' << x2 << '"' << " y2=" << '"' << y2 << '"' << "/>" << std::endl;
	}
	
	EllipseShape* EllipseShape::fromXml(Symbol* owner, xmlTextReaderPtr reader)
	{
		EllipseShape* ellipseshape = NULL;
		const xmlChar* ellipseTag = xmlTextReaderConstString(reader, (const xmlChar*) "ellipse");
		
		if( owner == NULL )
		{
			std::cerr << "[ERROR] The Symbol in argument is undefined" << std::endl;
			
			return NULL;
		}
		
		if( xmlTextReaderConstLocalName(reader) != ellipseTag )
		{
			std::cerr << "[ERROR] The node is not an EllipseShape node" << std::endl;
			
			return NULL;
		}
		
		int ix1 = 0;
		int ix2 = 0;
		int iy1 = 0;
		int iy2 = 0;
		std::string x1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x1"));
		std::string x2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x2"));
		std::string y1 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y1"));
		std::string y2 = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y2"));
		
		if( (x1.empty()) or (x2.empty()) or (y1.empty()) or (y2.empty()) )
		{
			std::cerr << "[ERROR] The position of the EllipseShape is undefined." << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix1 = stringToInt(x1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ix2 = stringToInt(x2);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute x2" << std::endl;
		}
		
		try
		{
			iy1 = stringToInt(y1);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y1" << std::endl;
			
			return NULL;
		}
		
		try
		{
			iy2 = stringToInt(y2);
		}
		catch(std::string const& unexp)
		{
			std::cerr << "The character " << unexp << " is not a figure in the attribute y2" << std::endl;
			
			return NULL;
		}
		
		try
		{
			ellipseshape = new EllipseShape(owner, Box(ix1,iy1,ix2,iy2));
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "{ERROR] The construction of a new EllipseShape failed because of a " << ba.what() << std::endl;
			
			return NULL;
		}
		
		std::cout << "The construction of the EllipseShape of the Cell " << (owner-> getCell())-> getName() << " was constructed successfully" << std::endl << std::endl;
		
		return ellipseshape;
	}
	
	std::string Shape::getShape() const
	{
		return "Shape";
	}
	
	std::string LineShape::getShape() const
	{
		return "Line";
	}
	
	std::string BoxShape::getShape() const
	{
		return "Box";
	}
	
	std::string TermShape::getShape() const
	{
		return "Term";
	}
	
	std::string ArcShape::getShape() const
	{
		return "Arc";
	}
	
	std::string EllipseShape::getShape() const
	{
		return "Ellipse";
	}
}