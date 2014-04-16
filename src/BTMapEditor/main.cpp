#include <QtWidgets>
#include "BTMapEditor/BTMapEditor.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("BTech");
	app.setApplicationName("BTMapEditor");
	BTMapEditor editor;
	editor.show();
	return app.exec();
}
