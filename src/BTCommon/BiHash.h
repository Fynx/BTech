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

#ifndef BIHASH_H
#define BIHASH_H

#include <QtCore>
#include <initializer_list>

template <typename T, typename U>
class BiHash {

public:
	BiHash(std::initializer_list <std::pair <T, U> > initList);
	BiHash() = default;
	BiHash(const BiHash &biHash) = default;
	BiHash(BiHash &&biHash) = default;
	~BiHash() = default;

	BiHash & operator = (const BiHash &biHash) = default;
	BiHash & operator = (BiHash &&biHash) = default;

	// non-const operators should not be used - not until they work properly
	U & operator [] (const T &arg);
	U operator [] (const T &arg) const;
	T & operator [] (const U &arg);
	T operator [] (const U &arg) const;

	void insert(const T &lhs, const U &rhs);
	void insert(const U &lhs, const T &rhs);

	int size() const;

private:
	QHash <T, U> left;
	QHash <U, T> right;
};

#endif // BIHASH_H
