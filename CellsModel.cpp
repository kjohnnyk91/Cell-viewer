#include <QAbstractTableModel>
#include "CellsModel.hpp"
#include "Cell.hpp"

/*
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
*/

namespace Netlist
{
	CellsModel::CellsModel(QObject* parent):
		QAbstractTableModel(parent)
	{
		emit layoutAboutToBeChanged();
		cell_ = Cell::load("gnd");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("and2");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("or2");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("vdd");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("TransistorN");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("TransistorP");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("xor2");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("inverter");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("halfadder");					// Constructeur qui charge la Cell gnd 
		cell_ = Cell::load("fulladder");					// Constructeur qui charge la Cell gnd 
		emit layoutChanged();
	}

	CellsModel::~CellsModel()
	{}

	void CellsModel::setCell(Cell* cell)					// Définition de la Cell du CellsModel
	{
		emit layoutAboutToBeChanged();					// Emission du signal pour mettre à jour le modèle
		cell_ = cell;
		emit layoutChanged();
	}

	Cell* CellsModel::getModel(int row)					// Récupération du modèle à la ligne d'indice row
	{
		std::vector<Cell*> Cells = Netlist::Cell::getAllCells();
		
		if( not cell_ ) return NULL;
		
		if( row >= (int) (Cells.size()) )					// Si l'indice est plus grand que la taille du vecteur de Cell
		{
			return NULL;					// On renvoie NULL
		}
		else
		{
			return Cells.at(row);					// Sinon, on renvoie la Cell
		}
	}

	int CellsModel::rowCount(const QModelIndex& parent) const					// Définition du nombre de lignes du CellsModel
	{
		if( cell_ == NULL )
		{
			return 0;
		}
		else
		{
			std::vector<Cell*> Cells = Netlist::Cell::getAllCells();					// Le nombre de ligne est égal au nombre de Cell
			
			return Cells.size();
		}
	}

	int CellsModel::columnCount(const QModelIndex& parent) const
	{
		return 1;
	}

	QVariant CellsModel::data(const QModelIndex& index, int role) const					// Définition des données du modèle
	{
		if( (not cell_) or (not index.isValid())) return QVariant();
		
		std::vector<Cell*> Cells = Netlist::Cell::getAllCells();
		
		if(role == Qt::DisplayRole)
		{
			int row = index.row();
			
			std::cout << "The Cell is " << ((Cells.at(row))-> getName()).c_str() << std::endl;
			
			return ((Cells.at(row))-> getName()).c_str();
		}
		else
		{
			return QVariant();
		}
	}

	QVariant CellsModel::headerData(int section, Qt::Orientation orientation, int role) const					// Définition des entêtes
	{
		/* 
		if( (orientation == Qt::Vertical) or (role != Qt::DisplayRole) ) return QVariant();
		
		if(section != 0)
		{
			return QVariant();
		}
		else
		{
			return "Cell";
		}
		*/
		 
		if(role == Qt::DisplayRole)
		{
			if(orientation == Qt::Vertical)
			{
				return section+1;
			}
			else
			{
				if(section == 0)
				{
					return "Cell";
				}
				else
				{
					return QVariant();
				}
			}
		}
		else
		{
			return QVariant();
		}
	}
	
	QModelIndex CellsModel::index(int row, int column, const QModelIndex& parent) const					// Non utilisée
	{
		if( not hasIndex(row, column, parent) )
		{
			return QModelIndex();
		}
		
		Cell* cell = NULL;
		
		if( parent.isValid() )
		{
			cell = static_cast <Cell*> (parent.internalPointer());
		}
		else
		{
			return QModelIndex();
		}
		
		return createIndex(row,column,cell);
	}
	
	void CellsModel::updateDatas()
	{
		// emit layoutAboutToBeChanged();
		setCell(cell_);
		// emit layoutChanged();
	}
}