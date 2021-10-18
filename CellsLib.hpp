#ifndef CELLSLIB_HPP
#define CELLSLIB_HPP

#include <iostream>
#include <QWidget>
#include <QTableView>
#include "CellViewer.hpp"
#include "CellsModel.hpp"

namespace Netlist
{
	class CellViewer;
	
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

	inline CellsModel* CellsLib::getBaseModel()
	{
		return baseModel_;
	}
}

#endif