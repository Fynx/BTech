#ifndef CLICK_MODE_MANAGER_H
#define CLICK_MODE_MANAGER_H

#include <QtWidgets>
#include "BTMapEditor/ManagersUtils.h"
#include "BTMapEditor/Utils.h"

/**
 * \class DirectionStar
 */
class DirectionStar : public QWidget
{
Q_OBJECT;

public:
	DirectionStar();

	void setEnabled(bool enabled);
	void clear();
	void setDirection(Direction direction);

signals:
	void directionChosen(Direction direction);

private:
	ClickableLabel *directionLabel[Direction::NUMBER];

	void initLabels();

private slots:
	void onDirectionChosen(const QString &directionName);

};

/**
 * \class ClickModeManager
 */
class ClickModeManager : public QWidget
{
Q_OBJECT;

public:
	ClickModeManager();

public slots:
	void onHexClicked(Hex *hex);

signals:
	void mechTypeSelected(quint32 serialNumber);

	void hexInfoChanged();
	void mechInfoChanged();
	void mechNeedsRemoving(MechEntity *mech);
	void mechRemoved();

private:
	QComboBox *terrainComboBox;
	QSpinBox *heightSpinBox;
	QComboBox *unitTypeComboBox;
	DirectionStar *directionStar;
	QPushButton *removeUnitButton;

	void initBoxes();
	void initTerrainComboBox();
	void initHeightSpinBox();
	void initUnitTypeComboBox();

	BTech::Terrain getCurrentTerrain() const;
	QString getCurrentMechType() const;
	quint32 getCurrentMechTypeSerialNumber() const;

	Hex *currentHex;

	void refresh();

private slots:
	void onTerrainChosen();
	void changeHexHeight(int height);
	void onMechTypeSelected();
	void onMechDirectionChosen(Direction direction);
	void onRemoveUnit();
};

#endif // CLICK_MODE_MANAGER_H
