#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "OpenCellDialog.hpp"

/*
class OpenCellDialog : public QDialog
	{
		Q_OBJECT;
		
		public :
			OpenCellDialog(QWidget* parent = NULL);
			bool run(QString& name);
			const QString getCellName() const;
			void setCellName(const QString&);
			
		public slots :
			void accept();
			void reject();
			
		signals:
			void textSet(QString&);
			
		protected :
			QLineEdit* lineEdit_;
	};
*/

namespace Netlist
{
	OpenCellDialog::OpenCellDialog(QWidget* parent):					//  Même commentaires que pour SaveCellDialog
		QDialog(parent),
		lineEdit_(NULL)
	{
		setFixedSize(300,200);
		setModal(true);
		setWindowTitle("Open cell as");
		
		QLabel* label = new QLabel("Enter the cell name", this);
		
		lineEdit_ = new QLineEdit(this);
		
		QPushButton* ok = new QPushButton("Ok");
		QPushButton* cancel = new QPushButton("Cancel");
		
		QVBoxLayout* vlayout = new QVBoxLayout;
		QHBoxLayout* hlayout = new QHBoxLayout;
		QFrame* separator1 = new QFrame(this);					// Construction d'un QFrame (Séparateur)
			separator1-> setFrameShape( QFrame::HLine );					// Configuration de la forme du QFrame en ligne
			separator1-> setFrameShadow( QFrame::Sunken );					// Configuration de l'ombre du QFrame en contrebas
		QFrame* separator2 = new QFrame(this);					// Construction d'un QFrame (Séparateur)
			separator2-> setFrameShape( QFrame::HLine );					// Configuration de la forme du QFrame en ligne
			separator2-> setFrameShadow( QFrame::Sunken );					// Configuration de l'ombre du QFrame en contrebas
		
		hlayout-> addStretch();					// Ajout d'un espace étirable
		hlayout-> addWidget(ok);					// Ajout du bouton ok
		hlayout-> addStretch();					// etc ...
		hlayout-> addWidget(cancel);
		hlayout-> addStretch();
		
		vlayout-> addStretch();
		vlayout-> addWidget(label);
		vlayout-> addStretch();
		vlayout-> addWidget(separator1);
		vlayout-> addStretch();
		vlayout-> addWidget(lineEdit_);
		vlayout-> addStretch();
		vlayout-> addWidget(separator2);
		vlayout-> addStretch();
		vlayout-> addLayout(hlayout);
		vlayout-> addStretch();
		
		setLayout(vlayout);
		
		QObject::connect(ok,SIGNAL(clicked()),this, SLOT(accept()));
		QObject::connect(cancel,SIGNAL(clicked()),this, SLOT(reject()));
	}

	bool OpenCellDialog::run(QString& name)
	{
		setCellName(name);					// Initialisation du texte dans la QLineEdit
		
		int dialogResult = exec();
		
		name = getCellName();
		
		if(dialogResult == QDialog::Accepted)
		{
			std::cout << "Accepted" << std::endl;
		}
		else
		{
			std::cout << "Rejected" << std::endl;
		}
		
		return (dialogResult == QDialog::Accepted);
	}

	const QString OpenCellDialog::getCellName() const
	{
		return lineEdit_-> text();
	}

	void OpenCellDialog::setCellName(const QString& name)
	{
		lineEdit_-> setText(name);
	}
	
	void OpenCellDialog::accept()
	{
		if( not (lineEdit_-> text()).isEmpty())
		{
			QString name = lineEdit_-> text();
			
			setCellName(name);
			
			/*
			QWidget* widget = new QWidget;
				widget-> setFixedSize(300,300);
				
			QPushButton* quit = new QPushButton("Quit", widget);
			QLabel* label = new QLabel(name,widget);
			QVBoxLayout* layout = new QVBoxLayout(widget);
			layout-> addWidget(label);
			layout-> addWidget(quit);
			widget-> setLayout(layout);
			
			QObject::connect(quit,SIGNAL(clicked()),widget,SLOT(close()));
			
			widget-> show();
			*/
			
			hide();
			
			emit textSet(name);
			
			setResult(QDialog::Accepted);
		}
	}
	
	void OpenCellDialog::reject()
	{
		lineEdit_-> setText(QString());
		
		setResult(QDialog::Rejected);
		
		hide();
	}
}