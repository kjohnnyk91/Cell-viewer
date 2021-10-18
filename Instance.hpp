#ifndef NETLIST_INSTANCE_H
#define NETLIST_INSTANCE_H

#include <iostream>
#include <limits>
#include <vector>
#include "Point.hpp"
//#include "Node.hpp"
//#include "Net.hpp"
#include "Cell.hpp"
//#include "Term.hpp"

namespace Netlist
{
	class Net;
	class Term;
	
	class Instance
	{
		private :
			Cell* owner_;
			Cell* masterCell_;
			std::string name_;
			std::vector<Term*> terms_;
			Point position_;
		
		public :
			Instance(Cell*, Cell*, const std::string&);
			~Instance();
			
		public :
			const std::string& getName() const;
			Cell* getMasterCell() const;
			Cell* getCell() const;
			const std::vector<Term*>& getTerms() const;
			Term* getTerm(const std::string&) const;
			Point getPosition() const;
			
			bool connect(const std::string&, Net*);
			void add(Term*);
			void remove(Term*);
			void setPosition(const Point&);
			void setPosition(int x, int y);
			
			void toXml(std::ostream&);
			static Instance* fromXml(Cell*, xmlTextReaderPtr);
	};
}

#endif