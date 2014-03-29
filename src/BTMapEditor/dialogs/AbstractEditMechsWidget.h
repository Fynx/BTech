#ifndef ABSTRACT_EDIT_MECHS_WIDGET_H
#define ABSTRACT_EDIT_MECHS_WIDGET_H

#include <QtWidgets>

#include "BTCommon/MechBase.h"
#include "BTMapEditor/Utils.h"

class AbstractEditMechsWidget : public QWidget
{
Q_OBJECT;

public:
	AbstractEditMechsWidget();
	virtual ~AbstractEditMechsWidget();

	virtual void setCurrentIndex(const QModelIndex &index);
	virtual void save();
	virtual void discard();

protected:
	QModelIndex currentIndex() const;

	QDataWidgetMapper *mapper;

private:
	void initWidgetMapping();
};

#endif // ABSTRACT_EDIT_MECHS_WIDGET_H
