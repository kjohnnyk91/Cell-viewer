#ifndef OPENCELLDIALOG_HPP
#define OPENCELLDIALOG_HPP

#include <iostream>
#include <QDialog>
#include <QLineEdit>

namespace Netlist
{
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
}
#endif