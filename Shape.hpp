#ifndef NETLIST_SHAPE_HPP
#define NETLIST_SHAPE_HPP

#include <iostream>
#include <fstream>
#include <libxml/xmlreader.h>
#include "box.hpp"
#include "xmlutil.hpp"
// #include "cell.hpp"
// #include "symbol.hpp"

namespace Netlist
{
	// class Box;
	class Symbol;
	class Cell;
	class Term;
	class TermShape;
	class LineShape;
	class BoxShape;
	
	class Shape
	{
		private:
			Symbol* owner_;
			
		public:
			Shape( Symbol* );
			~Shape();
			inline Symbol* getSymbol() const;
			virtual Box getBoundingBox() const = 0;
			inline virtual Term* getTerm();
			inline virtual TermShape* getTermShape();
			// virtual std::string getNameAlign();
			virtual void toXml(std::ostream&) = 0;
			Shape* fromXml( Symbol*, xmlTextReaderPtr);
			virtual std::string getShape() const;
	};
	
	inline Symbol* Shape:: getSymbol() const
	{
		return  owner_;
	}
	
	inline Term* Shape::getTerm()
	{
		return NULL;
	}
	
	inline TermShape* Shape::getTermShape()
	{
		return NULL;
	}
	
	class LineShape : public Shape
	{
		private:
			int x1_, y1_, x2_, y2_;
			
		public:
			LineShape(Symbol*, int x1, int y1, int x2, int y2);
			~LineShape();
			inline int getX1() const;
			inline int getX2() const;
			inline int getY1() const;
			inline int getY2() const;
			Box getBoundingBox() const;
			void toXml(std::ostream&);
			static LineShape* fromXml(Symbol*, xmlTextReaderPtr);
			std::string getShape() const;
	};
	
	inline int LineShape::getX1() const
	{
		return x1_;
	}
	
	inline int LineShape::getX2() const
	{
		return x2_;
	}
	
	inline int LineShape::getY1() const
	{
		return y1_;
	}
	
	inline int LineShape::getY2() const
	{
		return y2_;
	}
	
	class BoxShape : public Shape
	{
		private:
			Box box_;
			
		public:
			BoxShape(Netlist::Symbol*, const Box&);
			~BoxShape();
			inline Box getBoundingBox() const;
			void toXml(std::ostream&);
			static BoxShape* fromXml(Symbol*, xmlTextReaderPtr);
			std::string getShape() const;
	};

	inline Box BoxShape::getBoundingBox() const
	{
		return box_;
	}
	
	class TermShape : public Shape
	{
		public:
			enum NameAlign
			{
				TopLeft = 0,
				TopRight = 1,
				BottomLeft = 2,
				BottomRight = 3
			};
			
		private:
			Term* term_;
			int x1_, y1_;
			NameAlign align_;
		
		public:
			TermShape(Symbol*, std::string, int, int, NameAlign);
			~TermShape();
			inline Box getBoundingBox() const;
			inline Term* getTerm();
			inline TermShape* getTermShape();
			inline int getX();
			inline int getY();
			static std::string NameAlignToString(NameAlign);
			static NameAlign StringToNameAlign(std::string);
			NameAlign getNameAlign() const;
			
			void toXml(std::ostream&);
			static TermShape* fromXml(Symbol*, xmlTextReaderPtr);
			std::string getShape() const;
	};

	inline Box TermShape::getBoundingBox() const
	{
		return Box(x1_,y1_,x1_,y1_);
	}
	
	inline Term* TermShape::getTerm()
	{
		return term_;
	}

	inline TermShape* TermShape::getTermShape()
	{
		return this;
	}
	
	inline int TermShape::getX()
	{
		return x1_;
	}

	inline int TermShape::getY()
	{
		return y1_;
	}
	
	class ArcShape : public Shape
	{
		private:
			Box box_;
			int start_;
			int span_;
			
		public:
			ArcShape(Symbol*, Box, int, int);
			~ArcShape();
			inline Box getBoundingBox() const;
			inline int getX1() const;
			inline int getX2() const;
			inline int getY1() const;
			inline int getY2() const;
			inline int getStart() const;
			inline int getSpan() const;
			void toXml(std::ostream&);
			static ArcShape* fromXml(Symbol*, xmlTextReaderPtr);
			std::string getShape() const;
	};
	
	inline Box ArcShape::getBoundingBox() const
	{
		return box_;
	}
	
	inline int ArcShape::getX1() const
	{
		return getBoundingBox().getX1();
	}
	
	inline int ArcShape::getX2() const
	{
		return getBoundingBox().getX2();
	}
	
	inline int ArcShape::getY1() const
	{
		return getBoundingBox().getY1();
	}
	
	inline int ArcShape::getY2() const
	{
		return getBoundingBox().getY2();
	}
	
	inline int ArcShape::getStart() const
	{
		return start_;
	}
	
	inline int ArcShape::getSpan() const
	{
		return span_;
	}
	
	class EllipseShape : public Shape
	{
		private:
			Box box_;
			
		public:
			EllipseShape(Symbol*, Box);
			~EllipseShape();
			inline Box getBoundingBox() const;
			void toXml(std::ostream&);
			static EllipseShape* fromXml(Symbol*, xmlTextReaderPtr);
			std::string getShape() const;
	};
	
	inline Box EllipseShape::getBoundingBox() const
	{
		return box_;
	}
}

#endif