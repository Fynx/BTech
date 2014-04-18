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

#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTMapEditor/Utils.h"

/**
 * \class NewMapDialog
 * NewMapDialog is a QDialog for creating new maps.
 */
class NewMapDialog : public QDialog
{
Q_OBJECT;

public:
	NewMapDialog(QWidget *parent = nullptr);
	~NewMapDialog();

	int getMapWidth() const;
	int getMapHeight() const;

private:
	QSpinBox *widthLine;
	QSpinBox *heightLine;
	QPushButton *confirmButton;

	static const int MIN_SIZE = 4;
	static const int MAX_SIZE = 100;
	static const int DEFAULT_SIZE = 10;
};

#endif // NEWMAPDIALOG_H
