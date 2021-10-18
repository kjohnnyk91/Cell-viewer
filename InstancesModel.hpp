#ifndef INSTANCESMODEL_HPP
#define INSTANCESMODEL_HPP

#include <QAbstractTableModel>

namespace Netlist
{
	class Cell;
	
	class InstancesModel : public QAbstractTableModel
	{
		Q_OBJECT;
		
		public:
			InstancesModel(QObject* parent=NULL);
			~InstancesModel();
			inline Cell* getCell();
			void setCell(Cell*);
			Cell* getModel(int);
			int rowCount(const QModelIndex& parent = QModelIndex()) const;
			int columnCount(const QModelIndex& parent = QModelIndex()) const;
			QVariant data(const QModelIndex& index, int role = Qt:: DisplayRole) const;
			QVariant headerData(int, Qt:: Orientation, int role = Qt::DisplayRole) const;

		// public slots :
			// void load();
			
		private:
			Cell* cell_;
	};

	inline Cell* InstancesModel::getCell()
	{
		return cell_;
	}
}

#endif