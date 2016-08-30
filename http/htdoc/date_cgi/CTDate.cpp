#include "CTDate.h"

CTDate CTDate::operator + (int d)
{
	if (d < 0)
	{
		return *this - (0 - d);
	}

	CTDate temp(*this);

	temp.m_day += d;

	while (temp.m_day >  DaysInMonth(temp.m_year,temp.m_month ))
	{
		temp.m_day -= DaysInMonth(temp.m_year, temp.m_month);

		if (temp.m_month == 12)
		{
			temp.m_month = 1;
			temp.m_year++;
		}
		else
		{
			temp.m_month++;
		}
	}
	return temp;
}

CTDate CTDate::operator - (int d)
{
	if (d < 0)
	{
		return *this + (0 - d);
	}

	CTDate temp(*this);

	temp.m_day -= d;

	while (temp.m_day <= 0)
	{
		if (temp.m_month == 1)
		{
			temp.m_month = 12;
			temp.m_year--;
		}
		else
		{
			temp.m_month--;
		}

		temp.m_day += DaysInMonth(temp.m_year, temp.m_month);
	}
	return temp;
}

int CTDate::operator -(const CTDate &d)   
{
	CTDate Max(*this);
	CTDate Min(d);
	int count = 0;

	if (Min > Max)
	{
		Min = *this;
		Max = d;
	}

	while (Min < Max)
	{
		count++;
		Min++;
	}

	return count;
}

CTDate& CTDate::operator = (const CTDate &d)
{
	if (this != &d)
	{
		m_year = d.m_year;
		m_month = d.m_month;
		m_day = d.m_day;
	 
		//强盗式逻辑
//		CTDate tem = d;
//		swap(m_year, tem.m_year);
//		swap(m_month, tem.m_month);
//		swap(m_day, tem.m_day);
	}
	return *this;
}

CTDate&  CTDate::operator++() 
{
	(*this) = (*this) + 1;
	return *this;
}

CTDate CTDate::operator++(int) 
{
	CTDate temp(*this);

	(*this) = (*this) + 1;
	return temp;
}

CTDate& CTDate::operator--() 
{
	(*this) = (*this) - 1;
	return *this;
}

CTDate CTDate::operator--(int) 
{
	CTDate temp(*this);

	(*this) = (*this) - 1;
	return temp;
}

bool CTDate::operator > (const CTDate &d)
{
	if (m_year > d.m_year)
	{
		return true;
	}
	else if ((m_year == d.m_year) && (m_month > d.m_month))
	{
		return true;
	}
	else if ((m_year == d.m_year) &&
		(m_month == d.m_month) &&
		(m_day > d.m_day))
	{
		return true;
	}

	return false;
}

bool CTDate::operator == (const CTDate &d)
{
	return ((m_year == d.m_year) && (m_month == d.m_month) && (m_day == d.m_day));
}

bool CTDate::operator <(const CTDate &d)
{
	return  !((*this > d) || (*this == d));
}

ostream& operator <<(ostream &_cout, const CTDate &d)
{
	_cout << d.m_year << "-" << d.m_month << "-" << d.m_day;
	return _cout;
}

istream& operator >>(istream &_cin, CTDate &d)
{
	_cin >> d.m_year >> d.m_month >> d.m_day;
	return _cin;
}

//int main()
//{
//	CTDate date(2016,8,9);
//
//	std::cout << date + 1 << std::endl;
//
//	return 0;
//}


