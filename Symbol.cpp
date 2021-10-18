#include <iostream>
#include <libxml/xmlreader.h>
#include <string>
#include <vector>
#include "Box.hpp"
#include "Indentation.hpp"
#include "xmlutil.hpp"
#include "symbol.hpp"
#include "shape.hpp"
#include "term.hpp"
#include <typeinfo>

namespace Netlist
{
	class Cell;
	
	Symbol::Symbol(Cell* owner):
		owner_(owner)
	{}
	
	Symbol::~Symbol()
	{}
	
	Cell* Symbol::getCell() const
	{
		return owner_;
	}
	
	Box Symbol::getBoundingBox() const
	{
		if(shapes_.size() == 0)					// Si le vecteur de Shape du symbol est vide, ...
		{
			return Box(0, 0, 0, 0);					// alors on renvoit la box vide
		}
		
		int x1 = ((*(shapes_.begin()))-> getBoundingBox()).getX1();					// Sinon, on prend les coordonnées de la premiere Shape pour initialiser l'acquisition de la box du symbol
		int x2 = ((*(shapes_.begin()))-> getBoundingBox()).getX2();
		int y1 = ((*(shapes_.begin()))-> getBoundingBox()).getY1();
		int y2 = ((*(shapes_.begin()))-> getBoundingBox()).getY2();
		
		for(std::vector<Shape*>::const_iterator it = shapes_.begin(); it != shapes_.end(); it++)
		{
			if( ((*it)-> getBoundingBox()).getX1() < x1 )					// Condition if pour récupérer le minimum des x1
			{
				x1 = ((*it)-> getBoundingBox()).getX1();
			}
			
			if( ((*it)-> getBoundingBox()).getX2() > x2 )					// Condition if pour récupérer le maximum des x2
			{
				x2 = ((*it)-> getBoundingBox()).getX2();
			}
			
			if( ((*it)-> getBoundingBox()).getY1() < y1 )					// Condition if pour récupérer le minimum des y1
			{
				y1 = ((*it)-> getBoundingBox()).getY1();
			}
			
			if( ((*it)-> getBoundingBox()).getY2() > y2 )					// Condition if pour récupérer le maximum des y2
			{
				y2 = ((*it)-> getBoundingBox()).getY2();
			}
		}
		
		return Box(x1, y1, x2, y2);					// On renvoie la box
	}
	
	Point Symbol::getTermPosition(Term* term) const
	{
		return (term-> getNode())-> getPosition();
	}
	
	TermShape* Symbol::getTermShape(Term* term) const
	{
		TermShape* termshape = NULL;					// Initialisation d'un pointeur vers un TermShape
		
		for(std::vector<Shape*>::const_iterator it = shapes_.begin(); it != shapes_.end(); it++)					// Boucle for pour parcourir le vecteur de Shape du Symbol
		{
			if( ((*it)-> getTermShape() ) != NULL )					// Si on trouve un TermShape
			{
				termshape = (*it)-> getTermShape();					// On affecte son adresse au pointeur déclaré précédemment
				
				if((termshape-> getTerm()) == term) return termshape;					// Si c'est bien le TermShape qu'on cherchait, on le renvoie
			}
		}
		
		return NULL;
	}
	
	void Symbol::add(Shape* shape)
	{
		shapes_.push_back(shape);
		
		// std::cout << "Symbol::add() : " << shape-> getShape() << "Shape added to the symbol of the Cell " << getCell()-> getName() << std::endl;
		
		// std::cout << "Affichage des shapes du symbol de la cell " << (this-> getCell())-> getName() << std::endl;
		
		// this-> toXml(std::cout);
		
		// (shapes_.at(shapes_.size()-1))->toXml(std::cout);
	}
	
	void Symbol::remove(Shape* shape)
	{
		Box abox, pbox;		// abox: argument box, pbox : parameter box
		
		abox = shape-> getBoundingBox();					// On prend la box de la Shape
		
		for(std::vector<Shape*>::iterator it = shapes_.begin(); it != shapes_.end(); it++)					// Parcours du vecteur de shape
		{
			pbox = (*it)-> getBoundingBox();					// On prend la box du Shape courant
			
			if( (abox.getX1() == pbox.getX1()) and (abox.getX2() == pbox.getX2()) and (abox.getY1() == pbox.getY1()) and (abox.getY2() == abox.getY2()) and ( ((*it)-> getTerm()) == (shape-> getTerm()) ) )					// Si elle est égale à celle de la Shape en argument
			{
				shapes_.erase(it);					// On l'efface
				
				return;
			}
		}
		
		std::cout << "The shape was not found" << std::endl;					// Si on ne trouve pas la Shape, on l'indique
		
		return;
	}
	
	void Symbol::toXml(std::ostream& stream) const
	{
		// std::cout << "Symbol::toXml() : Execution" << std::endl;
		
		// std::cout << "La taille de shapes_ du symbol de la cell " << (this-> getCell())-> getName() << " est " << shapes_.size() << std::endl; 
		
		if(((shapes_.size()) != 0))					// Si le vecteur de Shape du symbole n'est pas vide
		{
			// std::cout << "Symbol::toXml() : Le vecteur de Shape n'est pas vide " << std::endl;
			
			// std::string cellName = owner_-> getName();					// On acquiert son nom
		}
		else
		{
			std::cerr << "Le vecteur de Shape est vide" << std::endl;
			
			return;
		}
		
		stream << indent << "<symbol>" << std::endl;
		
		++indent;
		
		for(std::vector<Shape*>::const_iterator it = shapes_.begin(); it != shapes_.end(); it++)					// Parcours du vecteur
		{
			(*it)-> toXml(std::cout);					// Affichage toXml des différentes Shapes
		}
		
		--indent;
		
		stream << indent << "</symbol>" << std::endl;
	}
	
	Symbol* Symbol::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		Symbol* symbol = cell-> getSymbol();
		const xmlChar* symbolTag = xmlTextReaderConstString(reader, (const xmlChar*) "symbol" );
		const xmlChar* lineTag = xmlTextReaderConstString(reader, (const xmlChar*) "line" );
		const xmlChar* boxTag = xmlTextReaderConstString(reader, (const xmlChar*) "box" );
		const xmlChar* arcTag = xmlTextReaderConstString(reader, (const xmlChar*) "arc" );
		const xmlChar* ellipseTag = xmlTextReaderConstString(reader, (const xmlChar*) "ellipse" );
		const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*) "term" );
		const xmlChar* junkTag = xmlTextReaderConstString(reader, (const xmlChar*) "#text" );
		
		int status = 1;
		
		if(cell == NULL)					// Verification de la cell en argument
		{
			std::cerr << "[ERROR] The cell in argument is undefined." << std::endl;
			
			return NULL;
		}
		
		if( xmlTextReaderConstLocalName(reader) != symbolTag )					// Vérification du nom du noeud
		{
			std::cerr << "[ERROR] The node isn't a symbol node." << std::endl;
			
			return NULL;
		}
	/*	else
		{
			symbol = new Symbol(cell);
			
			if(symbol == NULL)
			{
				std::cerr << "[ERROR] The construction of a new symbol failed." << std::endl;
				
				return NULL;
			}
		}
	*/	
		while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag) )
		{
			status = xmlTextReaderRead(reader);
			
			// std::cout << "Symbol::fromXml() : Noeud suivant" << std::endl;
		}
		
		if(status != 1)
		{
			if(status == 0)
			{
				std::cerr << "[ERROR] End of the XML file reached." << std::endl;
			}
			else
			{
				std::cerr << "[ERROR] Incorrect XML syntax." << std::endl;
			}
			
			return NULL;
		}
		
		while( (status != 0) and (status != -1) )
		{
			if(xmlTextReaderConstLocalName(reader) == lineTag)					// Si le noeud est celui d'une LineSHape
			{
				std::cout << "Symbol::fromXml() : Building a LineShape" << std::endl;
				
				LineShape* lineshape = NULL;
				
				lineshape = LineShape::fromXml(symbol,reader);					// On l'acquiert
				
				if(lineshape == NULL)
				{
					std::cerr << "The construction of the LineShape failed." << std::endl;
					
					return NULL;
				}
			}
			
			if(xmlTextReaderConstLocalName(reader) == boxTag)					// Idem pour une BoxShape
			{
				std::cout << "Symbol::fromXml() : Building BoxShape" << std::endl;
				
				BoxShape* boxshape = NULL;
				
				boxshape = BoxShape::fromXml(symbol,reader);
				
				if(boxshape == NULL)
				{
					std::cerr << "The construction of the BoxShape failed." << std::endl;
					
					return NULL;
				}
			}
			
			if(xmlTextReaderConstLocalName(reader) == termTag)					// Idem pour une TermSHape
			{
				std::cout << "Symbol::fromXml() : Building TermShape" << std::endl;
				
				TermShape* termshape = NULL;
				
				termshape = TermShape::fromXml(symbol,reader);
				
				if(termshape == NULL)
				{
					std::cerr << "[ERROR] The construction of the TermShape failed." << std::endl;
					
					return NULL;
				}
			}
			
			if(xmlTextReaderConstLocalName(reader) == arcTag)
			{
				std::cout << "Symbol::fromXml() : Building an ArcShape" << std::endl;
				
				ArcShape* arcshape = NULL;
				
				arcshape = ArcShape::fromXml(symbol,reader);
				
				if(arcshape == NULL)
				{
					std::cerr << "[ERROR] The construction of the ArcShape failed." << std::endl;
					
					return NULL;
				}
			}
			
			if(xmlTextReaderConstLocalName(reader) == ellipseTag)
			{
				std::cout << "Symbol::fromXml() : Building an EllipseShape" << std::endl;
				
				EllipseShape* ellipseshape = NULL;
				
				ellipseshape = EllipseShape::fromXml(symbol,reader);
				
				if(ellipseshape == NULL)
				{
					std::cerr << "[ERROR] The construction of the EllipseShape failed." << std::endl;
					
					return NULL;
				}
			}
			
			status = xmlTextReaderRead(reader);					// On avance d'un noeud
			
			while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag) )					// Vérification
			{
				status = xmlTextReaderRead(reader);
				
				if(status != 1)
				{
					if(status == -1)
					{
						std::cerr << "The XML syntax is incorrect" << std::endl;
						
						return NULL;
					}
					else
					{
						return symbol;
					}
				}
			}
		}
	}
}