#ifndef LOG_WINDOW_H
#define LOG_WINDOW_H

#include <QtWidgets>

class LogWindow : public QDockWidget
{
Q_OBJECT;

public:
	LogWindow(QWidget *parent = nullptr);

	void print(const QPair <QString, QColor> &msg);
	void clear();

private:
	static const QColor DefaultLogColor;
	static const int MAX_WIDTH;

	QTextEdit *log;
};

#endif // LOG_WINDOW_H