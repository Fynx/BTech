#include <QtWidgets>
#include "BTGame/BTGame.h"

int main(int argc, char **argv)
{
	QApplication app(argc, argv);
	BTGame game;
	game.show();
	return app.exec();
}
