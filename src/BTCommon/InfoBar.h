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

#ifndef INFOBAR_H
#define INFOBAR_H

#include <QtWidgets>
#include "BTCommon/Hex.h"
#include "BTCommon/MechEntity.h"

class InfoBar : public QDockWidget
{
Q_OBJECT;

public:
	InfoBar();

	void show();
	void hide();
	bool isVisible() const;

	enum class DisplayState : quint8 {
		Full,
		HexWindowOnly,
		Hidden
	};

	void setDisplayState(const DisplayState &state);

public slots:
	void setMech(const MechEntity *mech);
	void removeMech();
	void setHex(const Hex *hex);
	void removeHex();
	void update();

	void updateStatWindow();
	void updateHexWindow();

private:
	QImage *image;
	QVector <QString> information;
	const MechEntity *curMech;
	const Hex *curHex;

	QWidget *widget;
	QVBoxLayout *layout;
	QTextEdit *statWindow;
	QTextEdit *hexWindow;
	QLabel *imageWindow;
	QTimer *timer;

	int tX;

	static const int MAXIMUM_WIDTH = 300;
	static const int MAXIMUM_HEIGHT = 2000;
	static const int MINIMUM_WIDTH = 0;
	static const int MINIMUM_HEIGHT = 600;
	static const int HIDE_SHOW_SPEED = 4;

	void initSettings();
	void initWidget();
	void initImageWindow();
	void initStatWindow();
	void initHexWindow();
	void initLayout();
	void initTimer();

private slots:
	void advance();
};

#endif // INFOBAR_H
