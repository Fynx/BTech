#ifndef FILEIO_HPP
#define FILEIO_HPP

#include <QtCore>

template <typename T>
bool loadFromFile(T &obj, const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
		return false;

	QDataStream input(&file);
	input >> obj;

	return true;
}

template <typename T>
bool saveToFile(const T &obj, const QString &fileName)
{
	QSaveFile tmpFile(fileName);

	if (!tmpFile.open(QIODevice::WriteOnly))
		return false;

	QDataStream output(&tmpFile);
	output << obj;
	return tmpFile.commit();
}

#endif
