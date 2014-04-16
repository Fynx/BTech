#include <QtWidgets>
#include "BTGame/BTGame.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	app.setOrganizationName("BTech");
	app.setApplicationName("BTech");
	BTGame game;
	game.show();
	return app.exec();
}
