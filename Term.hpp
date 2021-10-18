#ifndef NETLIST_TERM_H
#define NETLIST_TERM_H

#include <iostream>
#include <limits>
#include <string>
//#include "point.hpp"
//#include "net.hpp"
#include "instance.hpp"
//#include "cell.hpp"
#include "node.hpp"

namespace Netlist
{
	class Point;
	class Net;
	class Cell;
	
	class Term
	{
		public :
			enum Type
			{
				Internal = 1, External = 2
			};
			
			enum Direction
			{
				In = 0,
				Out = 1,
				Inout = 2,
				Tristate = 3,
				Transcv = 4,
				Unknown = 5
			}; 
			
		private :
			void* owner_;
			std::string name_;
			Direction direction_;
			Type type_;
			Net* net_;
			NodeTerm node_;	
			
		public :	
			Term(Cell*, const std::string&, Direction);
			Term(Instance*, const Term*);
			~Term();
			
			static std::string toString( Type );
			static std::string toString( Direction );
			static Direction toDirection( std::string );
			
			inline bool isInternal() const;
			inline bool isExternal() const;
			inline const std::string& getName() const;
			inline NodeTerm* getNode();
			inline Net* getNet() const;
			inline Cell* getCell() const;
			Cell* getOwnerCell() const;
			inline Instance* getInstance() const;
			inline Direction getDirection() const;
			inline Point getPosition() const;
			inline Type getType() const;
			
			void setNet(Netlist::Net*);
			void setNet(const std::string&);
			inline void setDirection(Direction);
			void setPosition(const Point&);
			void setPosition(int x, int y);
			
			void toXml(std::ostream&);
			static Term* fromXml(Cell*, xmlTextReaderPtr);	
	};
	
	inline bool Term::isInternal() const
	{
		if(this-> type_ == Internal)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	inline bool Term::isExternal() const
	{
		if(this->type_ == External)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	inline const std::string& Term::getName() const
	{
		return this->name_;
	}
	
	inline NodeTerm* Term::getNode()
	{
		return &node_;
	}
	
	inline Net* Term::getNet() const
	{
		return this-> net_;
	}
	
	inline Cell* Term::getCell() const
	{
		if(type_ == External)
		{
			return (Cell*) this-> owner_;
		}
		else
		{
			return NULL;
		}
	}
	
	inline Netlist::Instance* Term::getInstance() const
	{
		if(type_ == Internal)
		{
			return (Instance*) owner_;
		}
		else
		{
			return NULL;
		}
	}

	inline Term::Direction Term::getDirection() const
	{
		return direction_;
	}
	
	inline Netlist::Point Term::getPosition() const
	{
		return ((Instance*)(this-> owner_))-> getPosition();
	}
	
	inline Term::Type Term::getType() const
	{
		return this-> type_;
	}
	
	inline void Term::setDirection(Direction d)
	{
		direction_ = d;
	}
}

#endif