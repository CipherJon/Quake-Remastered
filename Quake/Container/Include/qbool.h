#pragma once

#include <iostream>

class QBool
{
public:
	QBool(bool b = false);

	void setTrue();
	void setFalse();
	void set(bool bl);
	bool get() const;

	bool isTrue() const;
	bool isFalse() const;

	operator bool() const;
	void operator=(bool b);
	bool operator==(const QBool& qbool) const;
	bool operator!=(const QBool& qbool) const;
	bool operator<(const QBool& qbool) const;
	bool operator>(const QBool& qbool) const;
	friend std::ostream& operator<<(std::ostream& os, const QBool& qbool);

	//QString toString() const;
private:
	bool _bool;
};

#define QTRUE QBool(true)
#define QFALSE QBool(false)