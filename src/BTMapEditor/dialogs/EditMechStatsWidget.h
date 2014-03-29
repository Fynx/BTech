#ifndef EDIT_MECH_STATS_WIDGET_H
#define EDIT_MECH_STATS_WIDGET_H

#include <QtWidgets>
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"

/**
 * \class EditMechStatsWidget
 */
class EditMechStatsWidget : public AbstractEditMechsWidget
{
Q_OBJECT;

public:
	EditMechStatsWidget(QAbstractItemView *view, QStatusBar *statusBar);

	void save();

private:
	QLineEdit *type;
	QSpinBox *tonnage;
	QSpinBox *armorValue;
	QSpinBox *maxMovePoints;
	QSpinBox *maxJumpPoints;
	QSpinBox *baseFireRange;
	QSpinBox *heatSinksNumber;

	QHash <BTech::Range, QSpinBox *> rangeDamage;

	QAbstractItemView *view;

	QStatusBar *statusBar;

	void initBoxes();
	void initLayout();
	void initWidgetMapping();

private slots:
	void removeMech();
};

#endif // EDIT_MECH_STATS_WIDGET_H
