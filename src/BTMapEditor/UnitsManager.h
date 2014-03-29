#ifndef UNITS_MANAGER_H
#define UNITS_MANAGER_H

#include <QtWidgets>
#include "BTMapEditor/ManagersUtils.h"

/**
 * \class UnitsManager
 */
class UnitsManager : public QWidget
{
Q_OBJECT;

public:
	UnitsManager(QVector <Player *> &players);

	Player * getCurrentPlayer() const;

public slots:
	void onMapLoaded();
	void refresh();

signals:
	void unitChosen(UID unitUid);
	void playerChosen(Player *player);

private:
	QVector <Player *> &players;

	void initMechList();

	QComboBox *playersComboBox;
	QVBoxLayout *mechListLayout;

	void updatePlayersComboBox();

private slots:
	void onUnitChosen(const QString &unitName);
	void onPlayerChosen();
};

#endif // UNITS_MANAGER_H
