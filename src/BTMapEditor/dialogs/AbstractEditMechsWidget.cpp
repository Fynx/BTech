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
