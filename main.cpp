#include <iostream>
#include <QtWidgets/QtWidgets>
#include "OpenCellDialog.hpp"
#include "SaveCellDialog.hpp"
#include "CellViewer.hpp"
#include "Cell.hpp"
// #include 
// #include 

using namespace Netlist;

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);

	CellViewer* cellViewer = new CellViewer;
	
	cellViewer-> move(500,0);
	cellViewer-> show();

	return app.exec();
}