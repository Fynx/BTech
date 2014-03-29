#ifndef ITEMS_COMBO_BOX_H
#define ITEMS_COMBO_BOX_H

#include <QtWidgets>
#include "BTMapEditor/Utils.h"

/**
 * \class ComboBox
 */
class ComboBox : public QComboBox
{
Q_OBJECT;

public:
	ComboBox();
	virtual ~ComboBox();

signals:
	void itemChosen();

protected slots:
	void chooseItem();
};

/**
 * \class ItemComboBox
 */
template <typename T>
class ItemComboBox : public ComboBox
{
public:
	ItemComboBox();

	void clear();
	bool isEmpty() const;

	void addItem(T *item);
	void removeItem(const T *item);
	void setCurrentItem(const T *item);
	void removeCurrentItem();
	T * getCurrentItem() const;
	QList <T *> getItems() const;

private:
	QList <T *> items;

	void refresh();
};

#endif // ITEMS_COMBO_BOX_H
