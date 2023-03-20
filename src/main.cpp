#include "songlistdialog.h"
#include <QApplication>
#include <clocale>
#include <QDebug>

int main(int argc, char ** argv) 
{
	QApplication app(argc, argv);
    std::setlocale(LC_CTYPE, "rus");

	SongListDialog dlg;
	dlg.open();

	return app.exec();
}









