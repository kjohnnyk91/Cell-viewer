#include <QObject>
#include <QAbstractTableModel>
#include "Cell.hpp"
#include "InstancesModel.hpp"
#include "Instance.hpp"

/*
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
*/

namespace Netlist					// Même commentaires que pour CellsModel
{
	InstancesModel::InstancesModel(QObject* parent):
		QAbstractTableModel(parent),
		cell_(NULL)
	{}

	InstancesModel::~InstancesModel()
	{}

	void InstancesModel::setCell(Cell* cell)
	{
		emit layoutAboutToBeChanged();
		cell_ = cell;
		emit layoutChanged();
	}

	int InstancesModel::rowCount(const QModelIndex& parent) const
	{
		return (cell_) ? cell_-> getInstances().size() : 0;
	}

	int InstancesModel::columnCount(const QModelIndex& parent) const
	{
		return 2;
	}

	QVariant InstancesModel::data(const QModelIndex& index, int role ) const
	{
		if(not cell_ or not index.isValid()) return QVariant();
		
		if( role == Qt:: DisplayRole )
		{
			int row = index.row();
		
			switch( index.column() )
			{
				case 0:
					return (((cell_-> getInstances()).at(row))-> getName()).c_str();
					
				case 1:
					return ((((cell_-> getInstances()).at(row))-> getMasterCell())-> getName()).c_str();
			}
		}
		
		return QVariant();
	}

	QVariant InstancesModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if( orientation == Qt::Vertical ) return QVariant();
		
		if( role != Qt::DisplayRole) return QVariant();
		
		switch ( section )
		{
			case 0: return "Instance";
			
			case 1: return "MasterCell";
		}
		
		return QVariant();
	}

	Cell* InstancesModel::getModel(int row)
	{
		if(not cell_) return NULL;
		
		if( row >= (int) cell_-> getInstances().size() ) return NULL;
		
		return cell_-> getInstances()[ row ]-> getMasterCell();
	}
}