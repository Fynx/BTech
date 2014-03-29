#ifndef MANAGERS_UTILS_H
#define MANAGERS_UTILS_H

#include <QtWidgets>
#include "BTCommon/Hex.h"
#include "BTCommon/Player.h"
#include "BTMapEditor/Utils.h"

/**
 * \class ClickableLabel
 */
class ClickableLabel : public QLabel
{
Q_OBJECT;

public:
	ClickableLabel(QString unitName = QString());

signals:
	void clicked();

private:
	void mousePressEvent(QMouseEvent *event);
};

#endif // MANAGERS_UTILS_H
