#ifndef EDITMECHPARTSWIDGET_H
#define EDITMECHPARTSWIDGET_H

#include <QtWidgets>
#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"
#include "BTMapEditor/dialogs/ItemComboBox.h"

/**
 * \class EditMechPartsWidget
 */

class EditMechPartsWidget : public AbstractEditMechsWidget
{
Q_OBJECT;

public:
	EditMechPartsWidget(QAbstractItemView *view);

	void setCurrentIndex(const QModelIndex &index);
	void setModel(QAbstractItemModel *model);

private:
	QSpinBox *maxArmorValue;
	QSpinBox *maxInternalValue;

	QPushButton *addWeaponButton;
	QPushButton *removeWeaponButton;
	QPushButton *moveWeaponUpButton;
	QPushButton *moveWeaponDownButton;

	QAbstractItemView *view;

	QListView *attachedWeapons;
	QListView *allWeapons;

	void initBoxes();
	void initButtons();
	void initListView();
	void initLayout();
	void initWidgetMapping();

	QList <int> getSelectedIndexes() const;

private slots:
	void addWeapons();
	void removeWeapons();
	void moveWeaponsUp();
	void moveWeaponsDown();
};

#endif // EDITMECHPARTSWIDGET_H
