#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "SaveCellDialog.hpp"
#include "OpenCellDialog.hpp"

namespace Netlist
{
	SaveCellDialog::SaveCellDialog(QWidget* parent):					// Constructeur de SaveCellDialog
		QDialog(parent),					// Appel au constructeur de la classe mère QDialog
		lineEdit_(NULL)					// Initialisation de l'attribut lineEdit_ à NULL
	{
		setFixedSize(300,200);					// Configuration de la taille fixe de la fenetre à 300 pixel de longueur et 200 de hauteur
		setModal(true);					// Configuration de la fenetre en mode modale
		setWindowTitle("Save cell");					// Configuration du titre de la fenetre
		
		QLabel* label = new QLabel("Enter the cell name", this);					// Construction d'un label
		
		lineEdit_ = new QLineEdit(this);					// Définition de l'attribut lineEdit_
		
		QPushButton* ok = new QPushButton("Ok");					// Construction du bouton Ok
		QPushButton* cancel = new QPushButton("Cancel");					// Construction du bouton Cancek
		
		QVBoxLayout* vlayout = new QVBoxLayout;					// Construction d'un layout vertical
		QHBoxLayout* hlayout = new QHBoxLayout;					// Construction d'un layout horizontal
		QFrame* separator1 = new QFrame;					// Construction d'un QFrame (Séparateur)
			separator1-> setFrameShape( QFrame::HLine );					// Configuration de la forme du QFrame en ligne
			separator1-> setFrameShadow( QFrame::Sunken );					// Configuration de l'ombre du QFrame en contrebas
		QFrame* separator2 = new QFrame;					// Construction d'un QFrame (Séparateur)
			separator2-> setFrameShape( QFrame::HLine );					// Configuration de la forme du QFrame en ligne
			separator2-> setFrameShadow( QFrame::Sunken );					// Configuration de l'ombre du QFrame en contrebas
		
		hlayout-> addStretch();
		hlayout-> addWidget(ok);					// Ajout du bouton Ok au layout horizontal
		hlayout-> addStretch();
		hlayout-> addWidget(cancel);					// Ajout du bouton Cancel au layout horizontal
		hlayout-> addStretch();
		
		vlayout-> addStretch();
		vlayout-> addWidget(label);					// Ajout du label au layout vertical
		vlayout-> addStretch();
		vlayout-> addWidget(separator1);					// Ajout d'un separateur au layout vertical
		vlayout-> addStretch();
		vlayout-> addWidget(lineEdit_);					// Ajout de la ligne d'édtition au layout vertical
		vlayout-> addStretch();
		vlayout-> addWidget(separator2);					// Ajout d'un separateur au layout vertical
		vlayout-> addStretch();
		vlayout-> addLayout(hlayout);					// Ajout du layout horizontal au layout vertical
		vlayout-> addStretch();
		
		setLayout(vlayout);
		
		QObject::connect(ok,SIGNAL(clicked()),this, SLOT(accept()));					// Connexion du signal clicked() du bouton Ok au slot accept() du QDialog
		QObject::connect(cancel,SIGNAL(clicked()),this, SLOT(reject()));					// Connexion du signal clicked() du bouton Cancel au slot reject() du QSialog
	}

	bool SaveCellDialog::run(QString& name)
	{
		setCellName(name);
		
		int dialogResult = exec();
		
		name = getCellName();
		
		return (dialogResult == Accepted);
	}

	const QString SaveCellDialog::getCellName()	const				// Méthode getCellName() de SaveCellDialog
	{
		return lineEdit_-> text();					// On renvoie le nom de la ligne d'édition
	}

	void SaveCellDialog::setCellName(const QString& name)					// Méthode setCellName() de SaveCellDialog
	{
		lineEdit_-> setText(name);					// On définit le texte de la ligne d'édition avec le nom
	}
}