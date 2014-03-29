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
