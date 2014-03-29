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