#include "BigData.h"

BigData::BigData(INT64 value = 0)
				: _value(value)
{
	INT64 ret = value;

	string temp;    
	temp.resize(20);

	int iCount = 0;
	int ch = 0;

	while (ret)
	{
		ch = ret % 10;
		if (ch < 0)     
		{
			ch = 0 - ch;
		}
		temp[iCount++] = ch + '0';
		ret /= 10;
	}
	temp.resize(iCount);

	int iSize = temp.size() + 1;
	_pData.resize(iSize);

	if (_value >= 0)
	{
		_pData[0] = '+';
	}
	else
	{
		_pData[0] = '-';
	}

	for (int index = 1; index < iSize; index++)
	{
		_pData[index] = temp[iSize - index - 1];
	}
}

BigData::BigData(const char* pStr)
				: _value(0)
{
	assert(pStr);

	char Symbol = 0;

	while (*pStr == ' ')
	{
		pStr++;
	}

	if (*pStr == '+' || *pStr == '-')
	{
		Symbol = *pStr;
		pStr++;
	}
	else if (*pStr >= '0' && *pStr <= '9')
	{
		Symbol = '+';
	}
	else
	{
		return;
	}

	while (*pStr == '0')
	{
		pStr++;
	}

	int iCount = 1;
	_pData.resize(strlen(pStr) + 1);

	_pData[0] = Symbol;

	while (*pStr >= '0' && *pStr <= '9')
	{
		_value = _value * 10 + *pStr - '0';
		_pData[iCount++] = *pStr;
		pStr++;
	}
	if (Symbol == '-')
	{
		_value = 0 - _value;
	}

	_pData.resize(iCount);
}

BigData BigData::operator + (const BigData& bigdata)
{
	if (!IsOverFlowINT64() && !bigdata.IsOverFlowINT64())
	{
		if (_pData[0] != bigdata._pData[0])
		{
			return BigData(_value + bigdata._value);
		}
		else   // equeal symbol
		{
			if ((_pData[0] == '+' && (LLONG_MAX - _value >= bigdata._value)) ||
				(_pData[0] == '-' && (LLONG_MIN - _value <= bigdata._value)))
			{
				return BigData(_value + bigdata._value);
			}
		}
	}

	if (_pData[0] == bigdata._pData[0])
	{
		return BigData( (char *) Add(_pData, bigdata._pData).c_str());
	}

	return BigData((char *)Sub(_pData, bigdata._pData).c_str());
}

// + +					- - 
string BigData::Add(const string& mLeft, const string& mRight)
{
	string Left(mLeft); 
	string Right(mRight);

	int LSize = Left.size();
	int RSize = Right.size();

	if (LSize < RSize)
	{
		swap(Left, Right);
		swap(LSize, RSize);
	}

	string ret;
	ret.resize(LSize + 1);

	ret[0] = Left[0];

	int LCh = 0;
	int RCh = 0;
	int Carry = 0;

	for (int index = 1; index < LSize; index++)
	{
		LCh = Left[LSize - index] - '0' + Carry;

		RCh = 0;       
		if (index < RSize)
		{
			RCh = Right[RSize - index] - '0';
		}

		RCh += LCh;

		ret[LSize - index + 1] = RCh % 10 + '0';
		Carry = RCh /10;
	}
	ret[1] = Carry + '0';

	return ret;
}

//+ -	
//- +
string BigData::Sub(const string& mLeft, const string& mRight)
{
	string Left(mLeft);
	string Right(mRight);
	int LSize = Left.size();
	int RSize = Right.size();

	if ( (Left.size() < Right.size()) || 
		 (Left.size() == Right.size() && strcmp((char *)Left.c_str() + 1, (char *)Right.c_str() + 1) < 0 ) )
	{
		swap(Left, Right);
		swap(LSize, RSize);
	}

	string ret;
	ret.resize(LSize);

	ret[0] = Left[0];

	int LCh = 0;
	int RCh = 0;
	int Borrow = 0;
	int retCh = 0;

	for (int index = 1; index < LSize; index++)
	{
		LCh = Left[LSize - index] - '0' + Borrow;

		RCh = 0;  
		if (index < RSize)
		{
			RCh = Right[RSize - index] - '0' ; 
		}

		if (LCh >= RCh)
		{
			retCh = LCh - RCh;  
			Borrow = 0;
		}
		else
		{
			retCh = LCh + 10 - RCh;
			Borrow = -1;
		}

		ret[LSize - index] = retCh + '0';
	}

	return  ret;
}

BigData BigData::operator - (const BigData& bigdata)
{
	//BigData ret;
	BigData Left((char *)_pData.c_str());
	BigData Right((char *)bigdata._pData.c_str());

	if (bigdata._pData[0] == '+')
	{
		Right._pData[0] = '-';
		Right._value = 0 - Right._value;
	}
	else
	{
		Right._pData[0] = '+';
		Right._value = 0 - Right._value;
	}

	return  BigData((char *)(Left + Right)._pData.c_str());
}

string  BigData::Mul(const string& mLeft, const string& mRight)
{
	string Left(mLeft);
	string Right(mRight);

	int LSize = Left.size();
	int RSize = Right.size();

	if (LSize > RSize)
	{
		swap(Left, Right);
		swap(LSize, RSize);
	}

	string strRet;
	strRet.resize(LSize + RSize - 1, '0'); // 1

	strRet[0] = '-';
	if (Left[0] == Right[0])
	{
		strRet[0] = '+';
	}

	int Datalen = strRet.size();
	
	//000000000000000000000000000000222
	//111111111111111111111111111111111

	//222222222222222222222222222222222

	char cStep = 0;
	char offSet = 0;

	for (int iLIdx = 1; iLIdx < LSize; iLIdx++)
	{
		int cLeft = Left[LSize - iLIdx] - '0';
		if (cLeft == 0)
		{
			offSet++;
			continue;
		}
		cStep = 0; 

		for (int iRIdx = 1; iRIdx < RSize; iRIdx++)
		{
			int cRet = cLeft* (Right[RSize - iRIdx] - '0');

			cRet += cStep;
			cRet += (strRet[Datalen - iRIdx - offSet] - '0');

			strRet[Datalen - iRIdx - offSet] = cRet % 10 + '0';
			cStep = cRet / 10;
		}

		strRet[Datalen - RSize - offSet] = (cStep + '0');
		offSet++;
	}

	return strRet;
}

BigData BigData::operator* (const BigData& bigdata)
{
	if (!IsOverFlowINT64() && !bigdata.IsOverFlowINT64())
	{
		if (_value == 0 || bigdata._value == 0)
		{
			return BigData((INT64) 0);
		}
		//10 /2  = 5 >= 3     10 / -2  = -5 <= -4
		if (_pData[0] == bigdata._pData[0]) 
		{
			if ((_pData[0] == '+' && LLONG_MAX / _value >= bigdata._value) ||
				(_pData[0] == '-' && LLONG_MAX / _value <= bigdata._value))
			{
				return BigData(_value * bigdata._value);
			}
		}
		else
		{
			// -10 / 2 = -5 <= -4  -10 / -2 = 5 >= 4 
			if ((_pData[0] == '+' && (LLONG_MIN /_value <= bigdata._value)) ||
				(_pData[0] == '-' && (LLONG_MIN / _value >= bigdata._value)))
			{
				return BigData(_value * bigdata._value);
			}
		}
	}

	return BigData((char *)Mul(_pData, bigdata._pData).c_str());
}

BigData BigData::operator / (const BigData& bigdata)
{
	if (bigdata._pData == "+0")
	{
		assert(false);
		std::cout << "FileName :" <<__FILE__ << "Line :" << __LINE__ << std::endl;
	}

	if (_pData == "+0")
	{
		return BigData((INT64)0);
	}

	int LSize = _pData.size();
	int RSize = bigdata._pData.size();

	if (LSize < RSize  || 
		(LSize == RSize  && strcmp(_pData.c_str() + 1, bigdata._pData.c_str() + 1) < 0))
	{
		return BigData((INT64)0);
	}

	if (strcmp(_pData.c_str() + 1, bigdata._pData.c_str() + 1) == 0)
	{
		if (_pData[0] == bigdata._pData[0])
		{
			return BigData((INT64)1);
		}
		else
		{
			return BigData((INT64)-1);
		}
	}

	if (bigdata._pData == "+1")
	{
		return *this;
	}

	if (bigdata._pData == "-1")
	{
		std::string  strRet(_pData);

		if (strRet[0] == '+')
		{
			strRet[0] = '-';
			_value = 0 - _value;
		}
		else
		{
			strRet[0] = '+';
			_value = 0 - _value;
		}

		return BigData((char *)strRet.c_str());
	}

	//LSize > RSize
	return BigData((char *)Div(_pData, bigdata._pData).c_str());
}

string BigData::Div(const string& Left, const string& Right)
{
	char Symbol = '+';
	if (Left[0] != Right[0])
	{
		Symbol = '-';
	}

	int LSize = Left.size() - 1;
	int RSize = Right.size() - 1;

	string strRet;
	strRet.append(1, Symbol);

	char *pLeft = (char *)Left.c_str() + 1;
	char *pRight = (char *)Right.c_str() + 1;

	int iDataLen = RSize;
	int idx = 0;

	while (idx + iDataLen <= LSize)
	{
		//"99000000099" "33"
		//"0000000000099"
		while (*pLeft == '0')
		{
			strRet.append(1, '0');
			idx++;
			pLeft++;
		}

		if (*pLeft != '\0')
		{
			if (!IsLeftBig(pLeft, iDataLen, pRight, RSize))
			{
				strRet.append(1, '0');
				iDataLen++;
				continue;
			}
			else
			{
				int Len = iDataLen;
				strRet.append(1, LoopSub(pLeft, iDataLen, pRight, RSize));
				idx += Len - iDataLen;
				iDataLen++;
			}
		}
	}

	return strRet;
}

ostream& operator << (ostream& _cout, const BigData& bigdata)
{
	if (!bigdata.IsOverFlowINT64())
	{
		_cout << bigdata._value;
	}
	else
	{
		_cout << bigdata._pData.c_str();
	}

	return _cout;
}

 bool  BigData::IsOverFlowINT64() const
 {
	 string Max(INT64_MAX);
	 string Min(INT64_MIN);

	 if (_pData.size() > Max.size())
	 {
		 return true;
	 }

	 //"-9223372036854775808"
	 //"-9223372036854775809"
	 if ((_pData[0] == '+' && Max < _pData) ||(_pData[0] == '-' && Min < _pData) )  
	 {
		 return true;
	 }

	 return false;
 }

 bool BigData::IsLeftBig(char *Left, int LSize, char* Right, int RSize)
 {
	 assert(Left);
	 assert(Right);

	 if ((LSize > RSize) ||(LSize == RSize && strncmp(Left, Right, LSize) >= 0 ))
	 {
		 return true;
	 }

	 return false;
 }

 char BigData::LoopSub(char *&Left, int& LSize, char* &Right, int RSize)
 {
	 assert(Left);
	 assert(Right);

	 int RCh = 0;
	 int LCh = 0;
	 int Borrow = 0;
	 int RetCh = 0;
	 char iCount = '0';

	 while (IsLeftBig(Left, LSize, Right, RSize))
	 {
		 for (int index = 0; index < LSize; index++)
		 {
			 LCh = Left[LSize - 1 - index] - '0' + Borrow;

			 RCh = 0;  
			 if (index < RSize)
			 {
				 RCh = Right[RSize - 1 - index] - '0';
			 }

			 if (LCh >= RCh)
			 {
				 RetCh = LCh - RCh;
				 Borrow = 0;
			 }
			 else
			 {
				 RetCh = LCh + 10 - RCh;
				 Borrow = -1;
			 }

			 Left[LSize - 1 - index] = RetCh + '0';
		 }

		 //"99000000099" "33"
		 //"0000000000099"
		 while (*Left == '0' && LSize > 0)
		 {
			 Left++;
			 LSize--;
		 }
		 iCount++;
	 }

	 return iCount;
 }

//int main()
//{
//	BigData d1("1111111111111111111");
//	BigData d2("22");
//
//	std::cout << d1*d2 << std::endl;
//
//}

