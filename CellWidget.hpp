// -*- explicit-buffer-name: "CellWidget.h<M1-MOBJ/8-10>" -*-

#ifndef NETLIST_CELL_WIDGET_HPP
#define NETLIST_CELL_WIDGET_HPP

#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QRect>
#include <QPoint>
#include "Box.hpp"

class QPainter;

namespace Netlist
{
	class Cell;
	class NodeTerm;

	class CellWidget : public QWidget
	{
		Q_OBJECT;

		private:
			Cell* cell_;
			Box viewport_;
		
		public:
			CellWidget(QWidget* parent = NULL);
			virtual ~CellWidget();
			void setCell(Cell*);
			inline Cell* getCell() const;
			inline int xToScreenX(int) const;
			inline int yToScreenY(int) const;
			inline QRect boxToScreenRect(const Box&) const;
			inline QPoint pointToScreenPoint(const Point&) const;
			inline int screenXToX(int) const;
			inline int screenYToY(int) const;
			inline Box screenRectToBox(const QRect&) const;
			inline Point screenPointToPoint(const QPoint&) const;
			virtual QSize minimumSizeHint() const;
			virtual void resizeEvent(QResizeEvent*);
			void query(unsigned int flags, QPainter& Painter);
		protected:
			virtual void paintEvent(QPaintEvent*);
			virtual void keyPressEvent(QKeyEvent*);
			
		public slots:
			void goLeft();
			void goRight();
			void goUp();
			void goDown();
		
	};

	inline Cell* CellWidget::getCell() const
	{
		return cell_;
	}
	
	inline int CellWidget::xToScreenX( int x ) const
	{
		return x - viewport_.getX1();
	}
	
	inline int CellWidget::yToScreenY( int y ) const
	{
		return viewport_.getY2() - y;
	}
	
	inline int CellWidget::screenXToX( int x ) const
	{
		return x + viewport_.getX1();
	}
	
	inline int CellWidget::screenYToY( int y ) const
	{
		return viewport_.getY2() - y;
	}
	
	inline QRect CellWidget::boxToScreenRect(const Box& box) const
	{
		int x = box.getX1();
		int y = box.getY1();
		int X = xToScreenX(x);
		int Y = xToScreenX(y);
		int width = box.getX2() - box.getX1();
		int height = box.getY2() - box.getY1();
		
		return QRect(X,Y,width,height);
	}
	
	inline QPoint CellWidget::pointToScreenPoint(const Point& point) const
	{
		int X = xToScreenX(point.getX());
		int Y = yToScreenY(point.getY());
		
		return QPoint(X,Y);
	}
	
	inline Box CellWidget::screenRectToBox(const QRect& qRect) const
	{
		int *x1,*x2,*y1,*y2;
		qRect.getCoords(x1,y2,x2,y1);
		
		return Box(screenXToX(*x1),screenYToY(*y1),screenXToX(*x2),screenYToY(*y2));
	}
	
	inline Point CellWidget::screenPointToPoint(const QPoint& qPoint) const
	{
		int x = screenXToX(qPoint.x());
		int y = screenYToY(qPoint.y());
		
		return Point(x,y);
	}
}  // Netlist namespace.

#endif  // NETLIST_CELL_WIDGET_HPP