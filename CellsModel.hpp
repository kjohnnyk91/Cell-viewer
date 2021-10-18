#ifndef CELLSMODEL_HPP
#define CELLSMODEL_HPP

#include <QAbstractTableModel>
#include "Cell.hpp"

namespace Netlist
{
	class CellsLib;
	
	class CellsModel : public QAbstractTableModel
	{
		Q_OBJECT;
		
		public:
			CellsModel(QObject* parent=NULL);
			~CellsModel();
			inline Cell* getCell();
			void setCell(Cell*);
			Cell* getModel(int);
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			int columnCount(const QModelIndex& parent = QModelIndex()) const;
			QVariant data(const QModelIndex& index, int role = Qt:: DisplayRole) const;
			QVariant headerData(int, Qt:: Orientation, int role = Qt::DisplayRole) const;
			QModelIndex index(int, int, const QModelIndex &parent = QModelIndex()) const;
			
		public slots:
			void updateDatas();
			
		private:
			Cell* cell_;
	};

	inline Cell* CellsModel::getCell()
	{
		return cell_;
	}
}

#endif