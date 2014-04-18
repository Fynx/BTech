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

#include "BTCommon/Colors.h"
#include "BTGame/LogWindow.h"
#include "BTGame/Strings.h"

const QColor LogWindow::DefaultLogColor = Qt::white;

const int LogWindow::MAX_WIDTH = 500;

LogWindow::LogWindow(QWidget *parent)
{
	setParent(parent);

	setMouseTracking(true);
	setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	setMaximumWidth(MAX_WIDTH);

	log = new QTextEdit(this);
	log->setReadOnly(true);
	log->setBackgroundRole(QPalette::Window);
	log->setPalette(QPalette(Qt::black));
	log->setAutoFillBackground(true);
	log->setCursorWidth(0);
	log->setMouseTracking(true);

	QLayout *layout = new QVBoxLayout;
	layout->addWidget(log);

	setWidget(new QWidget(this));
	widget()->setMouseTracking(true);
	widget()->setLayout(layout);
}

void LogWindow::print(const QPair <QString, QColor> &msg)
{
	//TODO

	for (int index = 0, length = 0; index + length < msg.first.length(); ++length) {
		bool isEndLine = msg.first[index + length] == '\n';
		if (isEndLine || (length == msg.first.length() - 1)) {
			log->textCursor().insertHtml(BTech::HTML::FontColor
			                             .arg(BTech::colorToString(msg.second))
			                             .arg(msg.first.mid(index, length + int(!isEndLine))));
			if (isEndLine)
				log->textCursor().insertHtml(BTech::HTML::NewLine);
			index += length + 1;
			length = -1;
		}
	}
}

void LogWindow::clear()
{
	log->clear();
}