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
		Instance* instance = NULL;					// Déclaration et initialisation d'un pointeur d'Instance à l'adresse NULL
		Cell* instanceMastercell = NULL;					// Déclaration et initialisation d'un pointeur de Cell à l'adresse NULL
		const xmlChar* instanceTag = xmlTextReaderConstString(reader,(const xmlChar*) "instance" );					// Déclaration de l'étiquette "instance"
		const xmlChar* junkTag = xmlTextReaderConstString(reader,(const xmlChar*) "#text" );					// Déclaration de l'étiquette "#text"
		std::string instanceName;					// Déclaration de la chaine de caractère qui servira à stocker le nom de l'instance
		std::string instanceMastercellName;					// Déclaration de la chaine de caractère qui servira à stocker le nom du modèle auquel se réfère l'instance
		std::string x;					// Déclaration de la chaine de caractère qui servira à stocker l'abscisse de l'instance
		std::string y;					// Déclaration de la chaine de caractère qui servira à stocker l'ordonnée de l'instance
		
		int status = 1;					// Déclaration et initialisation à 1 de l'entier qui servira à stocker la valeur de retour de la fonction de parcours des noeuds
		
		while( (status == 1) and (xmlTextReaderConstLocalName(reader) == junkTag))					// Tant que le passage au noeud suivant se passe bien et que le nom du noeud est "#text"
			status = xmlTextReaderRead(reader);					// On passe au noeud suivant
		
		if(status != 1)					// Si le passage au noeud suivant se passe mal
		{
			if(status == 0)					// Si le fichier xml est terminé
			{
				std::cout << "End of Xml file reached." << std::endl;					// On indique que la fin est atteinte
			}
			else					// Sinon, s'il n'est pas terminé, c'est que la syntaxe xml est incorrecte
			{
				std::cout << "Incorrect Xml file" << std::endl;					// On indique que la syntaxe est incorrecte
			}
			
			return NULL;					// On renvoie l'adresse NULL s'il y a un problème
		}
		
		if((xmlTextReaderConstLocalName(reader) == instanceTag) )					// Si le nom du noeud est "instance"
		{
			instanceName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "name"));					// On acquiert le nom de l'instance
			instanceMastercellName = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "mastercell" ));					// On acquiert le nom du modèle à partir duquel a été crée l'instance
			x = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "x" ));					// On acquiert l'abscisse de l'instance
			y = xmlCharToString(xmlTextReaderGetAttribute(reader, (const xmlChar*) "y" ));					// On acquiert l'ordonnée de l'instance
			
			Cell* instanceMastercell = cell-> find(instanceMastercellName);					// On cherche s'il existe un modèle qui porte ce nom
			
			if(instanceMastercell == NULL)					// S'il n'existe pas
			{
				std::cerr << "[ERROR] Instance's mastercell " << instanceMastercellName << " not found" << std::endl;					// On indique qu'il n'a pas été trouvé
				
				return NULL;					// On renvoie la valeur NULL
			}
		
			if( not instanceName.empty() )					// Si le nom de l'instance est défini
			{
				instance = new Instance(cell, instanceMastercell, instanceName);					// On crée une instance portant ce nom à partir du modèle 
			}
			else					// Sinon
			{
				std::cerr << "[ERROR] Instance's name undefined" << std::endl;					// On indique que le nom n'est pas défini
			
				return NULL;					// On renvoie l'adresse NULL
			}
		
			if(instance == NULL)					// Si la construction de l'instance a échoué
			{
				std::cerr << "[ERROR] The construction of the instance " << instanceName << " failed" << std::endl;					// On indique qu'elle a échoué
			
				return NULL;					// On renvoie l'adresse NULL
			}
			
			if( (not x.empty()) and (not y.empty()) )					// Si la position de l'instance est bien définie
			{
				int ix = 0;					// On déclare un entier X qui va stocker l'abscisse
				int iy = 0;					// On déclare un entier Y qui va stocker l'ordonnée
			
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
				
				std::cout << "The instance " << instanceName << " was added to the Cell " << cell-> getName() << " successfully." << std::endl << std::endl;					// Message de confirmation que l'instance a bien été ajouté à la liste
				
				return instance;					// On renvoie le pointeur vers l'instance
			}
			else					// Sinon, la position n'est pas définie
			{
				std::cerr << "[ERROR] Instance's position undefined" << std::endl;					// On indique que la position n'est pas définie
			
				return NULL;
			}
		}
	}
}