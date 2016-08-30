#pragma once

#include <iostream>
using namespace std;

class CTDate
{
	friend ostream& operator <<(ostream &_cout,const CTDate &d);

	friend istream& operator >>(istream &_cin, CTDate &d);

public:

	CTDate(int year = 1900, int month = 1, int day = 1)
		: m_year(year)
		, m_month(month)
		, m_day(day)
	{
		if ((year < 1900) ||
			(month <= 0 || month > 12) ||
			day < 0 && day > DaysInMonth(year, month))
		{
			m_year = 1900;
			m_month = 1;
			m_day = 1;
		}
	}

	CTDate(const CTDate &d)
		: m_year(d.m_year)
		, m_month(d.m_month)
		, m_day(d.m_day)
	{  }

	CTDate operator+(int d);

	CTDate operator-(int d);

	int operator -(const CTDate &d);

	CTDate& operator=(const CTDate &d);

	CTDate& operator++(); 
	
	CTDate operator++(int);
	
	CTDate& operator--(); 
	
	CTDate operator--(int);

	bool operator > (const CTDate &d);

	bool operator == (const CTDate &d);

	bool operator <(const CTDate &d);

private:

	bool IsLeap(int year)
	{
		return (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0));
	}

	int DaysInMonth(int year, int month)
	{
		int day[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

		if (IsLeap(year) && month == 2)
		{
			day[month]++;
		}

		return day[month];
	}

private:
	int m_year;
	int m_month;
	int m_day;
};


