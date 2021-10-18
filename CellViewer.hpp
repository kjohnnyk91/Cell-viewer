#ifndef CELLVIEWER_HPP
#define CELLVIEWER_HPP

#include <iostream>
#include <QtWidgets/QtWidgets>
#include "OpenCellDialog.hpp"
#include "SaveCellDialog.hpp"
#include "CellWidget.hpp"
#include "CellsLib.hpp"
#include "InstancesWidget.hpp"
#include "CellsModel.hpp"

class Cell;

namespace Netlist
{
	class CellsLib;
	class InstancesWidget;
	
	class CellViewer : public QMainWindow
	{
		Q_OBJECT;
		
		public:
			CellViewer( QWidget* parent=NULL );
			virtual ~CellViewer();
			Cell* getCell() const;
			inline CellsLib* getCellsLib() const;  // TME9+.

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
			InstancesWidget* instancesWidget_;
			SaveCellDialog* saveCellDialog_;
			OpenCellDialog* openCellDialog_;
	};

	inline CellsLib* CellViewer::getCellsLib() const
	{
		return cellsLib_;
	}
}

#endif