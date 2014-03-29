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
