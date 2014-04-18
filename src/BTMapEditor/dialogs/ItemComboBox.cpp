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

#include "BTCommon/MechBase.h"
#include "BTCommon/MechPartBase.h"
#include "BTCommon/WeaponBase.h"
#include "BTMapEditor/dialogs/ItemComboBox.h"

/**
 * \class ComboBox
 */

ComboBox::ComboBox()
{
	QWidget::setFixedWidth(BTech::NAME_LINE_WIDTH);
	connect(this, static_cast<void (QComboBox::*)(int)>(&QComboBox::activated), this, &ComboBox::itemChosen);
}

ComboBox::~ComboBox()
{}

void ComboBox::chooseItem()
{
	emit itemChosen();
}

/**
 * \class ItemsComboBox
 */

template <typename T>
ItemComboBox <T>::ItemComboBox()
{}

template <typename T>
void ItemComboBox <T>::clear()
{
	items.clear();
	refresh();
}

template <typename T>
bool ItemComboBox <T>::isEmpty() const
{
	return items.isEmpty();
}

template <typename T>
void ItemComboBox <T>::addItem(T *item)
{
	items.append(item);
	QComboBox::addItem(static_cast<QString>(*item));
}

template <typename T>
void ItemComboBox <T>::removeItem(const T *item)
{
	for (typename QList <T *>::iterator it = items.begin(); it != items.end(); ++it) {
		if (*it == item) {
			items.erase(it);
			return;
		}
	}
}

template <typename T>
void ItemComboBox <T>::setCurrentItem(const T *item)
{
	for (int i = 0; i < items.size(); ++i)
		if (items[i] == item)
			QComboBox::setCurrentIndex(i);
	ComboBox::chooseItem();
}

template <typename T>
void ItemComboBox <T>::removeCurrentItem()
{
	items.removeAt(QComboBox::currentIndex());
	QComboBox::removeItem(QComboBox::currentIndex());
}

template <typename T>
T * ItemComboBox <T>::getCurrentItem() const
{
	if (isEmpty())
		return nullptr;
	return items[QComboBox::currentIndex()];
}

template <typename T>
QList <T *> ItemComboBox <T>::getItems() const
{
	return items;
}

template <typename T>
void ItemComboBox <T>::refresh()
{
	qSort(items.begin(), items.end(), [](const T *lhs, const T *rhs){
		return QString(*lhs) < QString(*rhs);
	});
	QComboBox::clear();
	for (T *item : items)
		QComboBox::addItem(QString(*item));
	ComboBox::chooseItem();
}

template class ItemComboBox <MechBase>;
template class ItemComboBox <const WeaponBase>;
template class ItemComboBox <MechPartBase>;
