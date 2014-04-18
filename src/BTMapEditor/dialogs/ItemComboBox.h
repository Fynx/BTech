/*
Copyright (C) 2014 by Piotr Majcherczyk <fynxor [at] gmail [dot] com>
This file is part of BTech Project.

	BTech Project is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	BTech Project is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with BTech.  If not, see <http://www.gnu.org/licenses/>.
*/

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
