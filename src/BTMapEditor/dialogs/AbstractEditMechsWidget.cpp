#include "BTMapEditor/dialogs/AbstractEditMechsWidget.h"

AbstractEditMechsWidget::AbstractEditMechsWidget()
{
	initWidgetMapping();
}

AbstractEditMechsWidget::~AbstractEditMechsWidget()
{}

void AbstractEditMechsWidget::setCurrentIndex(const QModelIndex &index)
{
	mapper->setRootIndex(index.parent());
	mapper->setCurrentIndex(index.row());
}

void AbstractEditMechsWidget::save()
{
	mapper->submit();
}

void AbstractEditMechsWidget::discard()
{
	mapper->revert();
}

QModelIndex AbstractEditMechsWidget::currentIndex() const
{
	return mapper->rootIndex().child(mapper->currentIndex(), 0);
}

void AbstractEditMechsWidget::initWidgetMapping()
{
	mapper = new QDataWidgetMapper(this);
	mapper->setModel(&MechModel::getInstance());
	mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
}
