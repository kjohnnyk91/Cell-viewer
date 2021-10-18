#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include <libxml/xmlreader.h>
#include "xmlutil.hpp"
#include "stringToInt.hpp"
#include "indentation.hpp"
#include "Point.hpp"
#include "Net.hpp"
#include "Instance.hpp"
#include "Cell.hpp"
#include "Node.hpp"
#include "Term.hpp"

namespace Netlist
{
	bool operator==(std::string s1, std::string s2)
	{
		int index = 1;
		
		if((s1[0] == '\0')||(s2[0] == '\0'))		// Si l'un ou les premiers caracteres des chaines est le caractere de fin de chaine
		{
			if((s1[0] == '\0')&&(s2[0] == '\0'))	// Si ce sont les deux
			{
				return true;						// Retour de true
			}
			else									// Sinon
			{
				return false;						// Retour de false
			}
		}
		else										// Si les deux caracteres ne sont pas le caractere de fin de chaine
		{
			while(s1[index] == s2[index])			// Tant que les caracteres sont egaux
			{
				if(s1[index] == '\0')				// Si ce sont le caractere de fin de chaine
				{
					return true;					// Retour de true
				}
				index++;							// Sinon on continue de comparer
			}
			
			return false;							// Si les caracteres sont differents, retour de false
		}
	}
	
	Term::Term(Cell* c, const std::string& name, Direction d): 
		owner_(c),
		name_(name),
		direction_(d),
		type_(External),
		net_(NULL),
		node_(this, Node::noid)
	{
		node_.setId(Node::noid);
		c-> add(this);
	}
			
	Term::Term(Netlist::Instance* I, const Term* modelTerm):
		owner_(I),
		name_(modelTerm-> getName()),
		direction_(modelTerm-> getDirection()),
		type_(Internal),
		net_(NULL),
		node_(this, Node::noid)
	{
		I-> add(this);
	}
						
	Term::~Term()
	{}
			
	std::string Term::toString(Type t)
	{
		if(t == Netlist::Term::Internal)
		{
			return "Internal";
		}
		else
		{
			return "External";
		}
	}
		
	std::string Term::toString(Direction d)
	{
		switch(d)
		{
			case In : return "In";
			case Out : return "Out";
			case Inout : return "Inout";
			case Tristate : return "Tristate";
			case Transcv : return "Transcv";
			case Unknown : return "Unknown";
			default : return "";
		}
	}
	
	Term::Direction Term::toDirection(std::string s)
	{
		std::string dir[6] = {"In", "Out", "Inout", "Tristate", "Transcv", "Unknown"};
		int index = 0;
		
		while(( s != dir[index] ) && ( index<6 ))
		{
			index++;
		}
		
		if(index<6)
		{
			return (Direction) index;
		}
		else
		{
			return Unknown;
		}
	}

	Netlist::Cell* Term::getOwnerCell() const
	{
		return (Cell*) this-> owner_;
	}
	
	void Term::setNet(Net* n)
	{
		n-> add(&node_);
		
		net_ = n;
	}
	
	void Term::setPosition(const Point& p)
	{
		node_.setPosition(p);
	}
		
	void Term::setPosition(int x, int y)
	{
		node_.setPosition(x,y);
	}
	
	void Term::toXml(std::ostream& stream)
	{
		int termX = (getNode()-> getPosition()).getX();
		int termY = (getNode()-> getPosition()).getY();
		
		stream << indent << "<term name=" << '"' << name_ << '"' << " direction=" << '"' << toString(direction_) << '"' << " x=" << '"' << termX << '"' << " y=" << '"' << termY << '"' << "/>" << std::endl;
	}
	
	Term* Term::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		Term* term = NULL; 					// Déclaration d'un pointeur de Term qui servira de valeur de retour
		const xmlChar* termTag = xmlTextReaderConstString(reader, (const xmlChar*) "term"); 					// Déclaration de l'étiquette term
		const xmlChar* junkText = xmlTextReaderConstString(reader, (const xmlChar*) "#text"); 					// Déclaration de l'étiquette #text qui est une chaine de texte que renvoie parfois la fonction xmlTextReaderConstLocalName
		std::string termName; 					// Déclaration d'une chaine de caracteres qui contiendra le nom du terminal
		std::string termDirection; 					// Déclaration d'une chaine de caracteres qui contiendra le type du terminal
		std::string x; 					// Déclaration d'une chaine de caracteres qui contiendra l'abscisse du terminal
		std::string y; 					// Déclaration d'une chaine de caracteres qui contiendra l'ordonnée du terminal

		int statut = 1; 					// Initialisation de l'entier qui servira pour le parcours des noeuds du fichier xml
		
		while( (statut == 1 ) and (xmlTextReaderConstLocalName(reader) == junkText) ) 					// Tant que le nom local du noeud est l'étiquette indésirable
			statut = xmlTextReaderRead(reader); 					// On passe au noeud suivant
		
		if( statut != 1 ) 					// Si on a atteint la fin du fichier xml et que la syntaxe est incorrecte
		{
			if(statut == 0) 					// Si la fin du fichier a été atteinte
			{
				std::cout << "End of the Xml file" << std::endl; 					// Message d'erreur
			}
			else 					// Sinon c'est qu'il y a un problème avec la syntaxe
			{
				std::cerr << "Incorrect Xml file" << std::endl; 					// Message d'erreur
			}
			
			return NULL; 					// On renvoie le pointeur NULL
		}
		
		if( xmlTextReaderConstLocalName(reader) == termTag ) 					// Si c'est le noeud d'un terminal
		{
			termName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "name")); 					// On prend son nom
			termDirection = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "direction" )); 					// Sa direction
			x = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x" )); 					// Son abscisse
			y = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y" )); 					// Son ordonnées
							
			if( not termName.empty() ) 					// Si son nom est bien défini
			{
				if( not termDirection.empty() ) 					// Si la direction du terminal est bien définie
				{
					term = new Term(cell, termName, Term::toDirection(termDirection)); 					// On construit ce terminal
				}
				else 					// Sinon
				{
					std::cerr << "[ERROR] The direction of the terminal is undefined." << std::endl; 					// Message d'erreur
					
					return NULL; 					// On renvoie le pointeur NULL
				}
			
				if(term == NULL) 					// Si la construction a échoué
				{
					std::cerr << "[ERROR] The construction of the terminal " << termName << " failed." << std::endl; 					// Message d'erreur
				
					return NULL; 					// On renvoie le pointeur NULL
				}
			}
			else 					// Sinon, si le nom est indéfini
			{
				std::cerr << "[ERROR] The name of the terminal is undefined." << std::endl; 					// Message d'erreur
				
				return NULL; 					// On renvoie le pointeur NULL
			}
			
			if( (not x.empty()) and (not y.empty()) ) 					// Si la position du terminal est bien définie
			{
				int ix = 0; 					// Initialisation de l'entier qui sera égal à l'abscisse du terminal
				int iy = 0; 					// Initialisation de l'entier qui sera égal à l'ordonnée du terminal
				
				try
				{
					ix = stringToInt(x);
				}
				catch(const char& unexp)
				{
					std::cerr << "The character " << unexp << " is not a figure in the attribute x" << std::endl;
					
					return NULL;
				}
				
				try
				{
					iy = stringToInt(y);
				}
				catch(const char& unexp)
				{
					std::cerr << "The character " << unexp << " is not a figure in the attribute y" << std::endl;
					
					return NULL;
				}
				
				term-> setPosition(ix,iy); 					//  Si tout s'est bien passé, on attribue la position au terminal
				
				std::cout << "The terminal " << termName << " was added to the Cell " << cell-> getName() << " successfully." << std::endl << std::endl; 					// Message de confirmation
			
				return term; 					// On renvoie le pointeur vers le terminal crée
			}
			else 					// Sinon si la position n'est pas définie
			{
				std::cerr << "[ERROR] The position of the terminal is undefined." << std::endl; 					// Message d'erreur
						
				return NULL; 					// On renvoie le pointeur NULL
			}
		}
		else 					// Sinon, si le noeud n'est pas celui d'un terminal
		{
			return NULL; 					// On renvoie le pointeur NULL
		}
		
	}
}


