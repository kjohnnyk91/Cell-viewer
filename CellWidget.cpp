// -*- explicit-buffer-name: "CellWidget.cpp<M1-MOBJ/8-10>" -*-

#include <QResizeEvent>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QApplication>
#include <vector>
#include <QtWidgets/QtWidgets>
#include "CellWidget.hpp"
#include "Term.hpp"
#include "Instance.hpp"
#include "Symbol.hpp"
#include "Shape.hpp"
#include "Cell.hpp"
#include "Line.hpp"
#include "Node.hpp"
#include "Net.hpp"

/*
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
*/

namespace Netlist
{
	using namespace std;

	ostream& operator<<( ostream& o, const QRect& rect )
	{
		o << "<QRect x:" << rect.x() << " y:" << rect.y() << " w:" << rect.width() << " h:" << rect.height() << ">";
		
		return o;
	}

	ostream& operator<<( ostream& o, const QPoint& p )
	{
		o << "<QRect x:" << p.x() << " y:" << p.y() << ">";
		
		return o;
	}

	// public
	
	CellWidget::CellWidget( QWidget* parent ) :
		QWidget(parent),
		cell_(NULL)
	{
		viewport_ = Box(0,0,500,500);
		
		setAttribute( Qt::WA_OpaquePaintEvent );
		setAttribute( Qt::WA_NoSystemBackground );
		setAttribute( Qt::WA_StaticContents );
		setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
		setFocusPolicy( Qt::StrongFocus );
		setMouseTracking( true );
	}

	CellWidget::~CellWidget()
	{}

	void CellWidget::setCell( Cell* cell )
	{
		cell_ = cell;
		repaint();
	}

	QSize CellWidget::minimumSizeHint() const
	{
		return QSize(500,500);
	}

	void CellWidget::resizeEvent( QResizeEvent* event )
	{
		const QSize& size = event-> size();
		
		viewport_.setX2( viewport_.getX1() + size.width  () );
		viewport_.setY1( viewport_.getY2() - size.height () );
		
		std::cerr << "CellWidget::resizeEven() viewport_: " << viewport_  << endl;
	}
	
	// protected
	
	void CellWidget::paintEvent( QPaintEvent* event )
	{
		// QFont bigFont = QFont( "URW Bookman L", 36 );
		QFont bigFont = QFont( "Calibri Light", 18 );					// Police du nom de la Cell affichée
		QString cellName;					// QString pour récupérer le nom de la Cell chargée
		std::vector<Cell*> Cells = Cell::getAllCells();					// Chargement des Cells en mémoire
		
		if(Cells.size() == 0)					// S'il n'y a aucune Cell en mémoire
		{
			return;					// On termine la fonction
		}
		
		int frameWidth = 460;
		int frameHeight = 100;
		// int a = (size().width () - frameWidth)/2;
		// int b = (size().height() - frameHeight)/2;
	
		int a = 0;
		int b = 0;
	
		if(cell_)					// Si on a trouvé la Cell, on récupère son nom et on cherche si elle est présente en mémoire
		{
			cellName = QString::fromStdString(cell_-> getName().c_str());
			
			for(std::vector<Cell*>::iterator it = Cells.begin(); it != Cells.end(); it++)
			{
				if( QString::fromStdString(((*it)-> getName()).c_str()) == cellName )
				{
					break;
				}
			}
		}
		else
		{
			return;
		}
		
		QRect rect = boxToScreenRect(viewport_);					// Définition du champ de vision
		QRect nameRect( a,b, frameWidth, frameHeight );
		
		std::cout << "Viewport = (" << viewport_.getX1() << ',' << viewport_.getY1() << ')' << std::endl;
		std::cout << "Rect = (" << rect.x() << ',' << rect.y() << ')' << std::endl;
		
		QPainter painter(this);					// Construction du QPainter pour dessiner
			painter.setFont( bigFont );					// Définition de la police d'écriture du QPainter
			painter.setBackground( QBrush( Qt::black ) );					// Définition de la couleur de la brosse
			painter.eraseRect( QRect( QPoint(0,0), size() ) );
			painter.setPen( QPen( Qt::darkGreen , 1 ) );
			painter.drawRect( rect );
			painter.setPen( QPen( Qt::red, 0 ) );
			painter.setBrush( QBrush( Qt::red ) );
			// painter.drawRect( nameRect );
			painter.drawText( nameRect, Qt::AlignCenter, cellName );					// Dessin du nom de la Cell
			painter.setViewport(rect);
		query(1,painter);					// Fonction pour savoir quelle Cell dessiner
	}
	
	void CellWidget::keyPressEvent(QKeyEvent* event)
	{
		// event-> ignore();
		
		if( event-> modifiers() & (Qt:: ControlModifier|Qt:: ShiftModifier ) ) return;
		
		switch(event-> key())
		{
			case Qt:: Key_Up :
				std::cout << "Key : Up" << std::endl;
				goUp();
				break;
			
			case Qt:: Key_Down :
				std::cout << "Key : Down" << std::endl;
				goDown();
				break;
			
			case Qt:: Key_Left :
				std::cout << "Key : Left" << std::endl;
				goLeft();
				break;
			
			case Qt::Key_Right :
				std::cout << "Key : Right" << std::endl;
				goRight();
				break;
			
			default:
				return;
		}

		event-> accept();
	}
	
	void CellWidget::query(unsigned int flags, QPainter& painter)
	{
		if( (not cell_) or (not flags) ) return;					// Si la Cell n'est pas définie ou que l'on ne souhaite pas la dessiner, on termine la fonction
		
		Symbol* cellSymbol = cell_-> getSymbol();					// Sinon, on récupère son symbole
		
		const std::vector<Shape*> shapes = cellSymbol-> getShapes();					// et ses shapes
		
		const std::vector<Instance*> cellInstances = cell_-> getInstances();					// et ses instances
		
		std::vector<Symbol*> symbols;					// On initialise un vecteur de symbol vide qui contiendra le symbol de la Cell en premier et ensuite, ceux des instances
		
		symbols.push_back(cellSymbol);					// Ajout du symbol de la Cell
		
		for(std::vector<Instance*>::const_iterator it = cellInstances.begin(); it != cellInstances.end(); it++)					// Boucle pour remplir le vecteur de symbol
		{
			Cell* instanceCell = (*it)-> getMasterCell();
			Symbol* instanceSymbol = instanceCell-> getSymbol();
			symbols.push_back(instanceSymbol);
		}
		
		for(std::vector<Symbol*>::iterator it = symbols.begin(); it != symbols.end(); it++)					// Boucle pour dessiner les shapes de tous les éléments
		{
			Cell* cell = (*it)-> getCell();
			std::vector<Net*> Nets = cell-> getNets();
			int i = it - symbols.begin();
			Point center(250,250);
			Point coord;					// Définition d'un point courant qui sera soit égal au centre ou à la position de l'instance
			
			if(i == 0)					// Comme le premier symbol est celui de la Cell, on trace ses shapes à partir du centre
			{
				coord = center;
			}
			else					// Sinon, on trace les shapes à partir de la position de l'instance
			{
				coord = (cellInstances.at(i-1))-> getPosition();
			}
			
			/*	Ne trace pas les fils au bons endroits 
			for(std::vector<Net*>::iterator it = Nets.begin(); it != Nets.end(); it++)
			{
				std::vector<Line*> Lines = (*it)-> getLines();
				
				QPen wirePen(Qt::cyan,1);
				painter.setPen(wirePen);
				
				for(std::vector<Line*>::iterator itl = Lines.begin(); itl != Lines.end(); itl++)
				{
					Point sourcePos = (*itl)-> getSourcePosition();
					Point targetPos = (*itl)-> getTargetPosition();
					
					QPoint sourceQPos = pointToScreenPoint(sourcePos.translate(coord));
					QPoint targetQPos = pointToScreenPoint(targetPos.translate(coord));
						
					painter.drawLine(sourceQPos,targetQPos);
				}
			}
			 */
			 
			std::vector<Shape*> shapes = (*it)-> getShapes();					// On récupère les shapes du symbol courant
		
			for(std::vector<Shape*>::const_iterator it = shapes.begin(); it != shapes.end(); it++)					// Boucle pour savoir quel shape tracer
			{
				const std::string boxTag("Box");					// Définitions d'étiquettes
				const std::string lineTag("Line");
				const std::string arcTag("Arc");
				const std::string termTag("Term");
				const std::string ellipseTag("Ellipse");
				
				QPen linePen(Qt::darkGreen,3);					// Définitions de stylos
				QPen intTermPen(Qt::red,7);
				QPen extTermPen(Qt::red,15);
				QPen namePen(Qt::red,1);
				// QPen penNode(Qt::cyan,10);
				
				QFont nameFont("Courier New",10);					// Définition d'une police
				painter.setFont(nameFont);					// Définition de la police d'écriture du QPainter
				
				std::string shape = (*it)-> getShape();					// Récupération du type de la Shape sous forme de chaine de caractères
				
				if(shape == boxTag)					// Si la shape est une BoxShape, on trace une box etc...
				{
					painter.setPen(linePen);
					Box box = (*it)-> getBoundingBox();
					QRect rect = boxToScreenRect(box.translate(coord));
					painter.drawRect(rect);
				}
				
				if(shape == lineTag)
				{
					painter.setPen(linePen);
					LineShape* lineshape = dynamic_cast<LineShape*> (*it);
					
					if(lineshape == NULL)
					{
						std::cerr << "[ERROR] A dynamic cast failed" << std::endl;
						
						continue;
					}
					
					const int x1 = (lineshape-> getX1()) + coord.getX();
					const int y1 = lineshape-> getY1() + coord.getY();
					const int x2 = lineshape-> getX2() + coord.getX();
					const int y2 = lineshape-> getY2() + coord.getY();
					
					const int X1 = xToScreenX(x1);
					const int Y1 = xToScreenX(y1);
					const int X2 = xToScreenX(x2);
					const int Y2 = xToScreenX(y2);
					
					painter.drawLine(X1,Y1,X2,Y2);
					
					continue;
				}
				
				if(shape == arcTag)
				{
					painter.setPen(linePen);
					Box box = (*it)-> getBoundingBox();
					QRect rect = boxToScreenRect(box.translate(coord));
					ArcShape* arcshape = dynamic_cast<ArcShape*> (*it);
					
					if(arcshape == NULL)
					{
						std::cerr << "[ERROR] A dynamic cast failed" << std::endl;
						
						continue;
					}
					
					const int start = arcshape-> getStart();
					const int span = arcshape-> getSpan();
					
					const int startAngle = 16*start;
					const int spanAngle = 16*span;
					
					painter.drawArc(rect, startAngle, spanAngle);
					
					continue;
				}
				
				if(shape == ellipseTag)
				{
					painter.setPen(linePen);
					Box box = (*it)-> getBoundingBox();
					Box tbox = box.translate(coord);
					QRect rect = boxToScreenRect(tbox);
					
					painter.drawEllipse(rect);
					
					continue;
				}
				
				if(shape == termTag)
				{
					TermShape* termshape = dynamic_cast<TermShape*> (*it);
					Term* term = termshape-> getTerm();
					// Term::Direction dir = term-> getDirection();
					TermShape::NameAlign align = termshape-> getNameAlign();
					const QString termName = QString::fromStdString(term-> getName());
					
					if(termshape == NULL)
					{
						std::cerr << "[ERROR] A dynamic cast failed" << std::endl;
						
						continue;
					}
					
					const int x = (termshape-> getX()) + coord.getX();
					const int y = termshape-> getY() + coord.getY();
					
					const int X = xToScreenX(x);
					const int Y = xToScreenX(y);
					
					if(not(term-> isInternal()))
					{
						painter.setPen(intTermPen);
					}
					else
					{
						painter.setPen(extTermPen);
					}
					
					painter.drawPoint(X,Y);
					painter.setPen(namePen);
					
					switch(align)
					{
						case TermShape::TopLeft :
							painter.drawText(X-10,Y-10, termName);
							break;
						
						case TermShape::TopRight :
							painter.drawText(X+10,Y-10, termName);
							break;
						
						case TermShape::BottomLeft :
							painter.drawText(X-10,Y+10, termName);
							break;
						
						case TermShape::BottomRight :
							painter.drawText(X+10,Y+10, termName);
							break;
						
						default :
							painter.drawText(X,Y, termName);
							break;
					}
					
					continue;
				}
			}
		}
	}
	
	void CellWidget::goLeft()
	{
		viewport_.translate( Point(-20,0) );
		repaint();
	}
	
	void CellWidget::goRight()
	{
		viewport_.translate( Point (20,0) );
		repaint();
	}

	void CellWidget::goUp()
	{
		viewport_.translate( Point (0,20) );
		repaint();
	}
	
	void CellWidget::goDown()
	{
		viewport_.translate( Point(0,-20) );
		repaint();
	}
}  // Netlist namespace.
