#ifndef NETLIST_NET_H
#define NETLIST_NET_H

#include <iostream>
#include <limits>	
#include <vector>
//#include "node.hpp"
//#include "cell.hpp"
#include "term.hpp"
#include "line.hpp"

namespace Netlist
{	
	class Node;
	class Cell;
	
	class Net
	{
		private :
			Cell* owner_;
			std::string name_;
			unsigned int id_;
			Term::Type type_;
			std::vector<Node*> nodes_;
			std::vector<Line*> lines_;
		
		public :
			Net(Cell*, const std::string&, Term::Type);
			~Net();

		public :
			Cell* getCell() const;
			const std::string& getName() const;
			unsigned int getId() const;
			Term::Type getType() const;
			Node* getNode(unsigned int) const;
			const std::vector<Node*>& getNodes() const;
			size_t getFreeNodeId() const;
			
			void add(Node*);
			bool remove(Node*);
			
			void toXml(std::ostream&);
			static bool fromXml(Cell*, xmlTextReaderPtr);
			
			inline const std::vector<Line*>& getLines() const;
			void add(Line*);
			bool remove(Line*);
	};
	
	inline const std::vector<Line*>& Net::getLines() const
	{
		return lines_;
	}
}

#endif