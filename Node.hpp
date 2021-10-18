// -*- explicit-buffer-name: "Node.h<M1-MOBJ/7>" -*-

#ifndef NETLIST_NODE_H
#define NETLIST_NODE_H

#include <iostream>
#include <vector>
#include <libxml/xmlreader.h>
#include "indentation.hpp"
#include "point.hpp"

namespace Netlist
{
	class Term;
	class Line;
	class Net;

	class Node
	{
		public:
			static const size_t noid;
    
		public:
			Node( size_t id );
			virtual ~Node();
			inline Point getPosition() const;
			inline const std::vector<Line*>& getLines() const;
			inline size_t getDegree() const;
			inline size_t getId() const;
			virtual Net* getNet() const = 0;
			virtual Term* getTerm() const = 0;
			inline void setId( size_t );
			inline void setPosition( const Point& );
			inline void setPosition( int x, int y );
			void attach( Line* );
			void detach( Line* );
			virtual void toXml( std::ostream& ) const= 0;
			static bool fromXml( Net*, xmlTextReaderPtr );
			
		private:
			Node( const Node& );
			Node& operator=( const Node& );
			
		protected:
			size_t id_;
			Point position_;
			std::vector<Line*> lines_;
	};

	inline const std::vector<Line*>& Node::getLines() const
	{
		return lines_;
	}
	
	inline size_t Node::getDegree() const
	{
		return lines_.size();
	}
	
	inline size_t Node::getId() const
	{
		return id_;
	}
	
	inline Point Node::getPosition() const
	{
		return position_;
	}
	
	inline void Node::setId( size_t id )
	{
		id_ = id;
	}
	
	inline void Node::setPosition( const Point& pos )
	{
		position_ = pos;
	}
	
	inline void Node::setPosition( int x, int y )
	{
		position_ = Point(x,y);
	}

	class NodePoint : public Node
	{
		public:
			NodePoint( Net*, size_t id=Node::noid, Point p=Point() );
			~NodePoint();
			Net* getNet() const;
			void toXml( std::ostream& ) const;
			inline Term* getTerm() const;
			NodePoint& operator=( const NodePoint& );
		private:
			NodePoint( const NodePoint& );
		
		private:
			Net* net_;
	};

	inline Term* NodePoint::getTerm() const
	{
		return NULL;
	}
	
	class NodeTerm : public Node
	{
		public:
			NodeTerm( Term*, size_t id=Node::noid );
			virtual ~NodeTerm();
			Term* getTerm() const;
			virtual Net* getNet() const;
			virtual void toXml( std::ostream& ) const;
			NodeTerm& operator=( const NodeTerm& );
			
		private:
			NodeTerm( const NodeTerm& );
		
		private:
			Term* term_;
	};

	inline Term* NodeTerm::getTerm() const
	{
		return term_;
	}
}  // Netlist namespace.

#endif  // NETLIST_NODE_H
