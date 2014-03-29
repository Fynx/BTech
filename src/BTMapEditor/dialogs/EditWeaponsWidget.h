#ifndef EDIT_WEAPONS_WIDGET_H
#define EDIT_WEAPONS_WIDGET_H

#include <QtWidgets>
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"

class EditWeaponsWidget : public AbstractEditMechsWidget
{
Q_OBJECT;

public:
	EditWeaponsWidget(QAbstractItemView *view);

private:
	QAbstractItemView *view;

	/* TODO turn it into a read-only data display
	QLineEdit *nameEdit;

	QRadioButton *typeEnergy, *typeBallistic, *typeMissile;
	ButtonGroup *weaponTypeGroup;

	QSpinBox *minRange;
	QHash <BTech::Range, QSpinBox *> maxRange;
	QList <QSpinBox *> maxRangeList;
	QSpinBox *heat;
	QSpinBox *damage;
	QSpinBox *tonnage;
	QSpinBox *criticalSpaces;
	QSpinBox *ammoPerTon;
	QSpinBox *missilesPerShot;
	*/

	void initLayout();
	void initWidgetMapping();
};

#endif // EDIT_WEAPONS_WIDGET_H