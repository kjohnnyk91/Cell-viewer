#include <iostream>
#include <limits>
#include <vector>
#include <libxml/xmlreader.h>
#include "indentation.hpp"
#include "stringToInt.hpp"
#include "xmlutil.hpp"
#include "point.hpp"
#include "node.hpp"
//#include "net.hpp"
#include "instance.hpp"
//#include "cell.hpp"
#include "term.hpp"

namespace Netlist
{
	class Net;
	class Cell;
	
	Instance::Instance(Cell* owner, Cell* model, const std::string& s):
		name_(s),
		position_()
	{
		owner_ = owner;
		masterCell_ = model;
		
		std::vector<Term*> tmpv = model-> getTerms();
		std::vector<Term*>::iterator itm;
		
		for(itm = tmpv.begin(); itm != tmpv.end(); itm++)
		{
			terms_.push_back(new Term(this,*itm));
		}
		
		owner-> add(this);
	}
			
	Instance::~Instance()
	{
		terms_.clear();
	}
			
	const std::string& Instance::getName() const
	{
		return name_;
	}
			
	Cell* Instance::getMasterCell() const
	{
		return masterCell_;
	}
			
	Cell* Instance::getCell() const
	{
		return owner_;
	}
			
	const std::vector<Term*>& Instance::getTerms() const
	{
		return terms_;
	}
			
	Term* Instance::getTerm(const std::string& s) const
	{
		for(int i = 0; i<terms_.size(); i++)
		{
			if((terms_[i]-> getName()) == s )
			{
				return terms_[i];
			}
		}
				
		return NULL;
	}
			
	Point Instance::getPosition() const
	{
		return position_;
	}
			
	bool Instance::connect(const std::string& s, Net* n)
	{
		if(this-> getTerm(s) == NULL)
		{
			return false;
		}
		else
		{
			this-> getTerm(s)-> setNet(n);
			
			return true;
		}
	}
			
	void Instance::add(Term* t)
	{
		terms_.push_back(t);
	}
	
	void Instance::remove(Term* t)
	{
		Term* pt = (this-> getTerm(t-> getName()));
		
		if(pt != NULL)
		{
			pt = NULL;
		}
	}
			
	void Instance::setPosition(const Point& p)
	{
		position_.setX(p.getX());
		position_.setY(p.getY());
	}
	
	void Instance::setPosition(int x, int y)
	{
		position_.setX(x);
		position_.setY(y);
	}
	
	void Instance::toXml(std::ostream& stream)
	{
		std::string masterCellName = masterCell_-> getName();
		int instanceX = position_.getX();
		int instanceY = position_.getY();
		
		stream << indent << "<instance name=" << '"' << name_ << '"' << " mastercell=" << '"' << masterCellName << '"' << " x=" << '"' << instanceX << '"' << " y=" << '"' << instanceY << '"' << "/>" << std::endl;
	}
	
	Instance* Instance::fromXml(Cell* cell, xmlTextReaderPtr reader)
	{
		Instance* instance = NULL;					// D??claration et initialisation d'un pointeur d'Instance ?? l'adresse NULL
		Cell* instanceMastercell = NULL;					// D??claration et initialisation d'un pointeur de Cell ?? l'adresse NULL
		const xmlChar* instanceTag = xmlTextReaderConstString(reader,(const xmlChar*) "instance" );					// D??claration de l'??tiquette "instance"
		const xmlChar* junkTag = xmlTextReaderConstString(reader,(const xmlChar*) "#text" );					// D??claration de l'??tiquette "#text"
		std::string instanceName;					// D??claration de la chaine de caract??re qui servira ?? stocker le nom de l'instance
		std::string instanceMastercellName;					// D??claration de la chaine de caract??re qui servira ?? stocker le nom du mod??le auquel se r??f??re l'instance
		std::string x;					// D??claration de la chaine de caract??re qui servira ?? stocker l'abscisse de l'instance
		std::string y;					// D??claration de la chaine de caract??re qui servira ?? stocker l'ordonn??e de l'instance
		
		int status = 1;					// D??claration et initialisation ?? 1 de l'entier qui servira ?? stocker la valeur de retour de la fonction de parcours des noeuds
		
		while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag))					// Tant que le passage au noeud suivant se passe bien et que le nom du noeud est "#text"
			status = xmlTextReaderRead(reader);					// On passe au noeud suivant
		
		if(status != 1)					// Si le passage au noeud suivant se passe mal
		{
			if(status == 0)					// Si le fichier xml est termin??
			{
				std::cout << "End of Xml file reached." << std::endl;					// On indique que la fin est atteinte
			}
			else					// Sinon, s'il n'est pas termin??, c'est que la syntaxe xml est incorrecte
			{
				std::cout << "Incorrect Xml file" << std::endl;					// On indique que la syntaxe est incorrecte
			}
			
			return NULL;					// On renvoie l'adresse NULL s'il y a un probl??me
		}
		
		if((xmlTextReaderConstLocalName(reader) == instanceTag) )					// Si le nom du noeud est "instance"
		{
			instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "name"));					// On acquiert le nom de l'instance
			instanceMastercellName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "mastercell" ));					// On acquiert le nom du mod??le ?? partir duquel a ??t?? cr??e l'instance
			x = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x" ));					// On acquiert l'abscisse de l'instance
			y = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y" ));					// On acquiert l'ordonn??e de l'instance
			
			Cell* instanceMastercell = cell-> find(instanceMastercellName);					// On cherche s'il existe un mod??le qui porte ce nom
			
			if(instanceMastercell == NULL)					// S'il n'existe pas
			{
				std::cerr << "[ERROR] Instance's mastercell " << instanceMastercellName << " not found" << std::endl;					// On indique qu'il n'a pas ??t?? trouv??
				
				return NULL;					// On renvoie la valeur NULL
			}
		
			if( not instanceName.empty() )					// Si le nom de l'instance est d??fini
			{
				instance = new Instance(cell, instanceMastercell, instanceName);					// On cr??e une instance portant ce nom ?? partir du mod??le 
			}
			else					// Sinon
			{
				std::cerr << "[ERROR] Instance's name undefined" << std::endl;					// On indique que le nom n'est pas d??fini
			
				return NULL;					// On renvoie l'adresse NULL
			}
		
			if(instance == NULL)					// Si la construction de l'instance a ??chou??
			{
				std::cerr << "[ERROR] The construction of the instance " << instanceName << " failed" << std::endl;					// On indique qu'elle a ??chou??
			
				return NULL;					// On renvoie l'adresse NULL
			}
			
			if( (not x.empty()) and (not y.empty()) )					// Si la position de l'instance est bien d??finie
			{
				int ix = 0;					// On d??clare un entier X qui va stocker l'abscisse
				int iy = 0;					// On d??clare un entier Y qui va stocker l'ordonn??e
			
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
				
				instance-> setPosition(ix,iy);					// On attribue la position de l'instance
				
				std::cout << "The instance " << instanceName << " was added to the Cell " << cell-> getName() << " successfully." << std::endl << std::endl;					// Message de confirmation que l'instance a bien ??t?? ajout?? ?? la liste
				
				return instance;					// On renvoie le pointeur vers l'instance
			}
			else					// Sinon, la position n'est pas d??finie
			{
				std::cerr << "[ERROR] Instance's position undefined" << std::endl;					// On indique que la position n'est pas d??finie
			
				return NULL;
			}
		}
	}
}