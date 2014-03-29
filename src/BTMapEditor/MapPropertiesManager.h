#ifndef MAP_PROPERTIES_MANAGER_H
#define MAP_PROPERTIES_MANAGER_H

#include <QtWidgets>
#include "BTCommon/Rules.h"
#include "BTMapEditor/ManagersUtils.h"

/**
 * \class MapPropertiesManager
 */
class MapPropertiesManager : public QWidget
{
Q_OBJECT;

public:
	MapPropertiesManager(QVector <Player *> &players, QString &mapDescription, QList <BTech::GameVersion> &allowedVersions);

	void setPlayers(QVector <Player *> &players);
	void refresh();

public slots:
	void onMapLoaded();

signals:
	void playerChosen(Player *player);
	void playerAdded();
	void playerInfoChanged();
	void playerNeedsRemoving(Player *player);
	void playerRemoved();

private:
	static const int MAX_PLAYERS_SIZE = 8;

	static const int DEFAULT_WIDTH = 200;

	QVector <Player *> &players;
	QString &mapDescriptionRef;
	QList <BTech::GameVersion> &allowedVersions;

	QComboBox *playersComboBox;
	QPushButton *addPlayerButton;
	QPushButton *removePlayerButton;
	QLineEdit *playerName;
	QTextEdit *playerDescription;
	QPushButton *confirmSavePlayerButton;

	QTextEdit *mapDescription;
	QPushButton *confirmSaveMapDescriptionButton;

	QPushButton *versionsButton;

	void initVersionsButton();

	Player * getPlayer(const QString &name) const;
	Player * getCurrentPlayer() const;

private slots:
	void onPlayerChosen();
	void addNewPlayer();
	void removePlayer();
	void savePlayer();
	void saveMapDescription();
	void editVersions();
};

/**
 * \class GameVersionDialog
 */

class GameVersionDialog : public QDialog
{
Q_OBJECT;

public:
	GameVersionDialog(QWidget *parent = nullptr);

	QList <BTech::GameVersion> getAllowedVersions() const;
	void setChecked(BTech::GameVersion version, bool check);
	bool isChecked(BTech::GameVersion version) const;

private:
	static const int DEFAULT_WIDTH  = 200;
	static const int DEFAULT_HEIGHT = 300;

	QHash <BTech::GameVersion, QCheckBox *> checkBox;
	QPushButton *confirmButton;
	QLabel *errorLabel;

private slots:
	void tryConfirm();
};

#endif // MAP_PROPERTIES_MANAGER_H
