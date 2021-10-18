#include <iostream>
#include <QWidget>
#include <QTableView>
#include "CellsLib.hpp"

/*
class CellsLib : public QWidget
{
	Q_OBJECT;
	
	public:
		CellsLib( QWidget* parent=NULL );
		void setCellViewer( CellViewer* );
		int getSelectedRow() const;
		inline CellsModel* getBaseModel();
  
	public slots:
		void load();
		void updateDatas();
  
	private:
		CellViewer* cellViewer_;
		CellsModel* baseModel_;
		QTableView* view_;
		QPushButton* load_;
};
*/

namespace Netlist
{
	CellsLib::CellsLib(QWidget* parent):
		cellViewer_(NULL),
		baseModel_(NULL)
	{
		QVBoxLayout* layout = new QVBoxLayout;					// Création d'un layout
		QPushButton* quit = new QPushButton("Quit");					// Création d'un bouton pour quitter
		load_ = new QPushButton(parent);					// Création d'un bouton pour charger le modèle
		view_= new QTableView(parent);					// Création d'un QTableView
		baseModel_ = new CellsModel(parent);
		setAttribute(Qt::WA_QuitOnClose, false);					// Configuration de l'action close
		setAttribute(Qt::WA_DeleteOnClose, false);					// Configuration de l'action close
		
		setContextMenuPolicy(Qt::ActionsContextMenu);					// Configuration du mode du menu contextuel
		
		view_-> setShowGrid(false);					// Désactivation de l'affichage des grilles
		view_-> setAlternatingRowColors(true);					// Configuration des couleurs alternées des lignes
		view_-> setSelectionBehavior(QAbstractItemView::SelectRows);					// Configuration de la sélection d'une ligne
		view_-> setSelectionMode(QAbstractItemView::SingleSelection);					// Configuration du mode Selection
		view_-> setSortingEnabled(true);					// Activation du classement
		view_-> setModel(baseModel_);					// Définition du modèle
		
		for(int i = 0; i < (baseModel_-> rowCount()); i++)
		{
			if(view_->	isRowHidden(i) == true)
			{
				std::cout << "The row " << i << " is hidden" << std::endl;
			}
			else
			{
				std::cout << "The row " << i << " is not hidden" << std::endl;
			}
		}
		
		QHeaderView* hHeaderView = view_-> horizontalHeader();					// Configuration d'une en-tête horizontal
		hHeaderView-> setDefaultAlignment(Qt:: AlignHCenter);					// Configuration de l'alignement du texte
		hHeaderView-> setMinimumSectionSize(300);					// Configuration de la taille minimum d'une section
		hHeaderView-> setStretchLastSection(true);					// Dernière section étirable
		
		QHeaderView* vHeaderView = view_ -> verticalHeader();					// Construction d'une en-tête verticale
		vHeaderView-> setVisible(true);					// En-tête invisible
		
		load_-> setText("Load");					// Définition de l'étiquette du bouton
		
		layout-> addWidget(view_);					// Ajout du QTableView au layout
		layout-> addWidget(load_);					// Ajout du bouton pour charger un modèle au layout
		layout-> addWidget(quit);					// Ajout du bouton quitter au layout
		
		setLayout(layout);					// Définir le layout de la CellsLib
		
		QObject::connect(load_, SIGNAL(clicked()), this, SLOT(load()));					// Connexion du signal clicked du bouton load au slot load() de l'InstancesWidget
		QObject::connect(cellViewer_, SIGNAL(cellLoaded()), this, SLOT(updateDatas()));					// Connexion du signal CellLoaded au slot updateDatas
		QObject::connect(quit, SIGNAL(clicked()), this, SLOT(close()));
	}

	void CellsLib::setCellViewer(CellViewer* viewer)
	{
		CellsLib* cellsLib = viewer-> getCellsLib();
		baseModel_ = cellsLib-> getBaseModel();
		view_-> setModel(baseModel_);
		cellViewer_ = viewer;
	}

	int CellsLib::getSelectedRow() const
	{
		QModelIndexList selecteds = ((view_-> selectionModel())-> selection()).indexes();					// Acquisition de l'indice de la ligne sélectionnée

		if(selecteds.empty()) return -1;					// Si aucune ligne n'est sélectionnée, on renvoie -1

		return selecteds.first().row();					// Sinon, on renvoie l'indice entier de la ligne sélectionnée
	}

	void CellsLib::load()					// Slot load() de InstancesWidget
	{
		int selectedRow = getSelectedRow();					// Récupération de l'indice de la ligne sélectionnée

		if(selectedRow < 0) return;					// Si aucune ligne n'est sélectionnée, on termine la fonction
		
		cellViewer_-> setCell(baseModel_-> getModel(selectedRow));					// Sinon, on définit le modèle du CellsModel
	}
	
	void CellsLib::updateDatas()
	{
		Cell* cell = cellViewer_-> getCell();					// On récupère la Cell associée au CellViewer

		baseModel_-> setCell(cell);					// On définit la Cell du CellsModel

		view_-> setModel(baseModel_);					// On définit le modèle du QTableView
	}
}