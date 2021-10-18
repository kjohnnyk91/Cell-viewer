#include <iostream>
#include <QWidget>
#include "Cell.hpp"
#include "CellViewer.hpp"
#include "InstancesWidget.hpp"
#include "OpenCellDialog.hpp"
// #include "SaveCellDialog.hpp"

/*
class CellViewer : public QMainWindow
{
	Q_OBJECT;
	
	public:
		CellViewer( QWidget* parent=NULL );
		virtual ~CellViewer();
		Cell* getCell() const;
		inline CellsLib* getCellsLib();  // TME9+.

	public slots:
		void setCell( Cell* );
		void saveCell();
		void openCell();
		void showCellsLib();  // TME9+.
		void showInstancesWidget();  // TME9+.
		
	signals:
		void cellLoaded();
		
	private:
		CellWidget* cellWidget_;
		CellsLib* cellsLib_;         // TME9+.
		InstancesWidget* instancesWidget_;  // TME9+.
		SaveCellDialog* saveCellDialog_;
		OpenCellDialog* openCellDialog_;
};
*/

namespace Netlist
{
	CellViewer::CellViewer(QWidget* parent) :					// Constructeur de CellViewer
		QMainWindow(parent),					// Appel au constructeur de la classe mère QMainWindow
		cellWidget_(NULL),					// Initialisation de l'attribut cellWidget_ à NULL
		cellsLib_(NULL),					// Initialisation de l'attribut cellsLib_ à NULL
		instancesWidget_(NULL),					// Initialisation de l'attribut instancesWidget_ à NULL
		saveCellDialog_(NULL),					// Initialisation de l'attribut saveCellDialog_ à NULL
		openCellDialog_(NULL)					// Initialisation de l'attribut openCellDialog_ à NULL
	{
		try
		{
			cellWidget_ = new CellWidget;					// Construction de cellWidget_ (CellWidget)
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "The construction of the CellWidget failed because of a " << ba.what() << std::endl;
			
			return;
		}
		
		try
		{
			cellsLib_ = new CellsLib(this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "The construction of the CellsLib failed because of a " << ba.what() << std::endl;
			
			return;
		}
		
		try
		{
			instancesWidget_ = new InstancesWidget(this);
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "The construction of the InstancesWidget failed because of a " << ba.what() << std::endl;
			
			return;
		}
		
		try
		{
			saveCellDialog_ = new SaveCellDialog(this);					// Construction de saveCellDialog_ (SaveCellDialog)
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "The construction of the SaveCellDialog failed because of a " << ba.what() << std::endl;
			
			return;
		}
		
		try
		{
			openCellDialog_ = new OpenCellDialog(this);					// Construction de saveCellDialog_ (SaveCellDialog)
		}
		catch(const std::bad_alloc& ba)
		{
			std::cerr << "The construction of the OpenCellDialog failed because of a " << ba.what() << std::endl;
			
			return;
		}
		
		cellsLib_-> setCellViewer(this);
		
		setCentralWidget(cellWidget_);					// Configuration de cellWidget_ en widget central de la fenetre principale 
		
		QMenu* fileMenu = menuBar()-> addMenu("&File");					// Construction du menu File
		
		QAction* openCellAction = new QAction("&Open cell", this);					// Construction de l'action Ouvrir
			openCellAction-> setStatusTip("Open a new cell");					// Bulle d'info de l'action Ouvrir
			openCellAction-> setShortcut(QKeySequence("CTRL+O"));					// Raccourcis de l'action CTRL+O
			openCellAction-> setVisible(true);					// Configuration de la visibilité de l'action
			
		fileMenu-> addAction(openCellAction);					// Ajout de l'action au Menu File
		QObject::connect(openCellAction, SIGNAL(triggered()), this, SLOT(openCell()));					// Connexion du signal triggered() de l'action au slot openCell() du CellViewer
		
		QAction* saveCellAction = new QAction("&Save cell as", this);					// Construction de l'action Sauvegarder
			saveCellAction-> setStatusTip("Save the cell to disk");					// Bulle d'info de l'action Sauvegarder
			saveCellAction-> setShortcut(QKeySequence("CTRL+S"));					// Raccourcis de l'action CTRL+S
			saveCellAction-> setVisible(true);					// Configuration de la visibilité de l'action 
		
		fileMenu-> addAction(saveCellAction);					// Ajout de l'action Sauvegarder au Menu File
		QObject::connect(saveCellAction, SIGNAL(triggered()), this, SLOT(saveCell()));					// Connexion du signal triggered() de l'action au slot saveCell() du CellViewer
		
		QAction* showCellsLibAction = new QAction("&Open library", this);
			showCellsLibAction-> setStatusTip("Show Cell library");
			showCellsLibAction-> setShortcut(QKeySequence("CTRL+L"));
			showCellsLibAction-> setVisible(true);
			
		fileMenu-> addAction(showCellsLibAction);
		QObject::connect(showCellsLibAction, SIGNAL(triggered()), this, SLOT(showCellsLib()));
		
		QAction* quitAction = new QAction("&Quit", this);					// Construction de l'action Quitter
			quitAction-> setStatusTip("Exit the Netlist Viewer");					// Bulle d'info de l'action Quitter
			quitAction-> setShortcut(QKeySequence("CTRL+Q"));					// Raccourcis de l'action Quitter CTRL+Q
			quitAction-> setVisible(true);					// Configuration de la visibilité de l'action
			
		fileMenu-> addAction(quitAction);					// Ajout de l'action Quitter au Menu File
		QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));					// Connexion du signal triggered() de l'action au slot close() du CellViewer (QMainWindow)
	
		QObject::connect(openCellDialog_,SIGNAL(textSet(QString)),this, SLOT(openCell(QString)));
		QObject::connect(this,SIGNAL(cellLoaded()),cellsLib_, SLOT(updateDatas()));
	}

	CellViewer::~CellViewer()					// Destructeur de CellViewer
	{
		// delete this;					// On efface ça
	}

	Cell* CellViewer::getCell() const					// Méthode getCell() de CellViewer
	{
		return cellWidget_-> getCell();					// On renvoie la Cell de la CellWidget
	}

	void CellViewer::setCell(Cell* cell)
	{
		cellWidget_-> setCell(cell);
	}

	void CellViewer::saveCell()
	{
		Cell* cell = getCell();
		
		if(cell == NULL) return;
		
		QString cellName = QString::fromStdString((cell-> getName()).c_str());
		
		if(saveCellDialog_-> run(cellName))
		{
			cell-> setName( cellName.toStdString() );
			cell-> save( cellName.toStdString() );
		}
	}

	void CellViewer::openCell()
	{
		// QWidget* widget = new QWidget;
		// QVBoxLayout* vlayout = new QVBoxLayout;
		// QString str("CellViewer::openCell(");
		// QLabel* label = new QLabel(str + name, widget);
		// QPushButton* quit = new QPushButton("Quit");
		// vlayout-> addWidget(label);
		// vlayout-> addWidget(quit);
		
		// QObject::connect(quit,SIGNAL(clicked()),widget,SLOT(hide()));
		
		// widget-> show();

		QString name;
		QString &rname = name;
		
		if( (openCellDialog_-> run(rname)) )					// Si le dialogue est accepté
		{
			Cell* cell = NULL;
		
			std::vector<Cell*> Cells = Cell::getAllCells();					// Chargement des Cells en mémoire
				
			for(std::vector<Cell*>::iterator it = Cells.begin(); it != Cells.end(); it++)					// Boucle pour trouver si la Cell est déjà chargée en mémoire
			{
				QString cellName = QString::fromStdString((*it)-> getName());
				
				// std::cout << "Cell : " << (*it)-> getName() << std::endl;
				
				if(name == cellName)
				{
					cell = (*it);
					setCell(cell);
					emit cellLoaded();
					std::cout << "The Cell " << cellName.toStdString() << " was found in memory." << std::endl;
					
					break;
				}
			}
			
			if( cell )					// Si la Cell a été trouvé
			{
				cellWidget_-> setCell(cell);					// On l'associe au CellWidget
				
				// emit cellLoaded();
			}
			else					// Sinon, on essaye de la charger à partir des fichiers Xml dans le dossier cells
			{
				std::string stdCellName = (name.toStdString()).c_str();
				Cell* cell2 = Cell::load(stdCellName);
				
				if(cell2)
				{
					cellWidget_-> setCell(cell2);
					emit cellLoaded();
					// cellWidget_-> show();
				}
				else					// Si on ne la trouve pas, on affiche un message
				{
					std::cout << "The Cell " << stdCellName << " doesn't exist" << std::endl;
					
					QWidget* widget = new QWidget;
					widget-> move(500,500);
					QPushButton* quit = new QPushButton("Quit");
					QString str1("The Cell ");
					QString str2(" doesn't exist");
					QVBoxLayout* vlayout = new QVBoxLayout(this);
					QLabel* label = new QLabel(str1 + name + str2, cellWidget_);
					// QLabel* label2 = new QLabel(name, cellWidget_);
					// QLabel* label3 = new QLabel(QString::fromStdString(" doesn't exist"), cellWidget_);
					vlayout-> addWidget(label);
					vlayout-> addWidget(quit);
					// vlayout-> addWidget(label2);
					// vlayout-> addWidget(label3);
					widget-> setLayout(vlayout);
					widget-> show();
					
					QObject::connect(quit, SIGNAL(clicked()), widget, SLOT(close()));
				}
			}
		}
		else					// Si le dialogue est rejeté
		{
			std::cout << "The dialog was rejected" << std::endl;
			openCellDialog_-> hide();
		}
	}

	void CellViewer::showCellsLib()
	{
		cellsLib_-> show();
	}

	void CellViewer::showInstancesWidget()
	{
		instancesWidget_-> show();
	}
}