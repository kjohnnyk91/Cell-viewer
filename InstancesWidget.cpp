#include <iostream>
#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include "CellViewer.hpp"
#include "InstancesModel.hpp"

/*
class InstancesWidget : public QWidget
{
	Q_OBJECT;
	
	public:
		InstancesWidget( QWidget* parent=NULL );
		void setCellViewer( CellViewer* );
		void goTo( int );
		int getSelectedRow() const;
		inline void setCell( Cell* );

	public slots:
		void load();
	
	private:
		CellViewer* cellViewer_;
		InstancesModel* baseModel_;
		QTableView* view_;
		QPushButton* load_;
};
*/

namespace Netlist
{
	InstancesWidget::InstancesWidget( QWidget* parent):					// Constructeur de InstancesWidget
		QWidget(parent),					// Appel au constructeur de la classe mère QWidget
		cellViewer_(NULL),					// Initialisation de l'attribut à NULL
		baseModel_(NULL),					// Initialisation de l'attribut à NULL
		view_(new QTableView(this)),					// Construction de l'attribut
		load_(new QPushButton(this))					// Construction de l'attribut
	{
		setAttribute(Qt::WA_QuitOnClose, false);					// Configuration de l'action close
		setAttribute(Qt::WA_DeleteOnClose, false);					// Configuration de l'action close
		
		setContextMenuPolicy(Qt::ActionsContextMenu);					// Configuration du mode du menu contextuel
		
		view_-> setShowGrid(false);					// Désactivation de l'affichage des grilles
		view_-> setAlternatingRowColors(true);					// Configuration des couleurs alternées des lignes
		view_-> setSelectionBehavior(QAbstractItemView::SelectRows);					// Configuration de la sélection d'une ligne
		view_-> setSelectionMode(QAbstractItemView::SingleSelection);					// Configuration du mode Selection
		view_-> setSortingEnabled(true);					// Activation du classement
		view_-> setModel(baseModel_);					// Définition du modèle
		
		QHeaderView* hHeaderView = view_-> horizontalHeader();					// Configuration d'une en-tête horizontal
		hHeaderView-> setDefaultAlignment(Qt:: AlignHCenter);					// Configuration de l'alignement du texte
		hHeaderView-> setMinimumSectionSize(300);					// Configuration de la taille minimum d'une section
		hHeaderView-> setStretchLastSection(true);					// Dernière section étirable
		
		QHeaderView* vHeaderView = view_ -> verticalHeader();					// Construction d'une en-tête verticale
		vHeaderView-> setVisible(false);					// En-tête invisible
		
		load_-> setText("Load");					// Définition de l'étiquette du bouton
		
		connect(load_, SIGNAL(clicked()), this, SLOT(load()));					// Connexion du signal clicked du bouton load au slot load() de l'InstancesWidget
	}

	int InstancesWidget::getSelectedRow() const					// Méthode getSelectedRow() de InstancesWidget
	{
		QModelIndexList selecteds = view_-> selectionModel()-> selection().indexes();					// Acquisition de l'indice de la ligne sélectionnée

		if(selecteds.empty()) return -1;					// Si aucune ligne n'est sélectionnée, on renvoie -1

		return selecteds.first().row();					// Sinon, on renvoie l'indice entier de la ligne sélectionnée
	}

	void InstancesWidget::load()					// Slot load() de InstancesWidget
	{
		int selectedRow = getSelectedRow();

		if(selectedRow < 0) return;
		
		cellViewer_-> setCell(baseModel_-> getModel(selectedRow));
	}
}