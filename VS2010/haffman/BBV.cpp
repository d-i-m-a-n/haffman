#include "stdafx.h"
#include "BBV.h"

BBV::bitK:: bitK(unsigned char* BAIT,unsigned char MASK)
{
	bait = BAIT;
	mask = MASK;
}

BBV::bitK& BBV::bitK::operator ~ ()
{
	*bait ^= mask;

	return *this;
}

BBV::bitK&  BBV::bitK::operator = (BBV::bitK& obj)
{
	if (*(obj.bait) & obj.mask)
	{
		*bait |= mask;
		return *this;
	}
	*bait &= ~mask;
	return *this;
}

BBV::bitK& BBV::bitK::operator = (bool x)
{
	if (x)
		*bait |= mask;
	else
		*bait &= ~mask;

	return *this;
}

BBV::bitK::operator bool()
{
	if (*bait & mask)
		return true;
	return false;
}

std::ostream& operator << (std::ostream& out, BBV::bitK& obj)
{
	if (*(obj.bait) & obj.mask)
		out << '1';
	else
		out << '0';
	return out;
}

std::istream& operator >> (std::istream& in, BBV::bitK& obj)
{
	char a;
	in >> a;

	if (a - 48)
		*(obj.bait) |= obj.mask;
	else
		*(obj.bait) &= obj.mask;

	return in;
}

BBV::BBV()
{
	len = 8;
	size = 8;
	if (len % 8 != 0)
		size++;
	bits = new unsigned char[size];
	for (int i = 0; i < size; bits[i++] = 0);
}

BBV::BBV(int LEN)
{
	if (LEN > 0)
	{
		len = LEN;
		size = LEN / 8;
		if (LEN % 8 != 0)
			size++;
		bits = new unsigned char[size];
		for (int i = 0; i < size; bits[i++] = 0);
	}
}

BBV::BBV(BBV& obj)
{
	len = obj.len;
	size = obj.size;
	bits = new unsigned char[size];

	for (int i = 0; i < size; bits[i] = obj.bits[i], i++);
}

BBV::BBV(std::string& str)
{
	if (str.length())
	{
		len = str.length();

		size = len / 8;
		if (len % 8 != 0)
			size++;

		bits = new unsigned char[size];
		for (int i = 0; i < size; bits[i++] = 0);

		for (int j = 0, k = 0; str[k]; j++)
		{
			unsigned char mask = 1;
			for (int i = 0; i < 8 && str[k]; i++, k++, mask = mask << 1)
			{
				if (str[k] - 48)
					bits[j] |= mask;
			}
		}
	}
}

BBV::BBV(char* str)
{
	if (str)
	{
		len = 0;
		for (char* ptr = str; *ptr; len++, ptr++);

		size = len / 8;
		if (len % 8 != 0)
			size++;

		bits = new unsigned char[size];
		for (int i = 0; i < size; bits[i++] = 0);

		for (int j = 0; *str; j++)
		{
			unsigned char mask = 1;
			for (int i = 0; i < 8 && *str; i++, str++, mask = mask << 1)
			{
				if (*str - 48)
					bits[j] |= mask;
			}
		}
	}
}

BBV::BBV(char* str, int LEN)
{
	if (str && len > 0)
	{
		len = LEN;
		size = len / 8;

		if (len % 8)
			size++;

		for(int i = 0; i < size; bits[i++] = 0)

		for (int i = 0; i < len && *str; i++, str++)
		{
			unsigned char mask = 1;
			for (int j = 0; j < 8 && i < len && *str; i++, str++, j++, mask <<= 1)
				if (*str - 48)
					bits[i] |= mask;
		}
	}
}

BBV::~BBV()
{
	delete[] bits;
	bits = nullptr;
	len = 0;
	size = 0;
}

int BBV::GetLength()
{
	return len;
}

int BBV::GetSize()
{
	return size;
}

void BBV::Set0(int i)
{
	if (i > -1 && i < len)
	{
		int k = i / 8;
		unsigned char mask = 1 << (i % 8);

		bits[k] &= ~mask;
		return;
	}
	throw "memory error";
}

void BBV::Set0InRange(int i, int N = 1)
{
	if (i > -1 && N > 0 && (i + N - 1) < len)
	{
		int b1 = i / 8;
		int p1 = i % 8;
		unsigned char mask1;

		if(i + N > 8)
			mask1 = (1 << p1) - 1;
		else
		{
			mask1 = 1 << p1;
			for (int k = 0; k < N - 1; k++)
				mask1 = mask1 | (mask1 << 1);
			mask1 = ~mask1;
		}

		bits[b1] &= mask1;

		N -= (8 - p1);
		int b2 = (i + N) / 8;
		int p2 = N % 8;
		unsigned char mask2 = (1 << p2) - 1;

		if (b1 != b2)
		{
			for (int k = b1 + 1; k < b2; bits[k++] = 0);

			bits[b2] &= ~mask2;
		}
		return;
	}
	throw "memory error";
}

void BBV::Set1(int i)
{
	if (i > -1 && i < len)
	{
		int b = i / 8;
		int p = i % 8;
		unsigned char mask = 1 << p;

		bits[b] |= mask;
		return;
	}
	throw "memory error";
}

void BBV::Set1InRange(int i, int N = 1)
{
	if (i > -1 && N > 0 && (i + N - 1) < len)
	{
		int b1 = i / 8;
		int p1 = i % 8;
		unsigned char mask1;

		if (i + N > 8)
			mask1 = (1 << p1) - 1;
		else
		{
			mask1 = 1 << p1;
			for (int k = 0; k < N - 1; k++)
				mask1 = mask1 | (mask1 << 1);
			mask1 = ~mask1;
		}

		bits[b1] |= ~mask1;

		N -= (8 - p1);
		int b2 = (i + N) / 8;
		int p2 = N % 8;
		unsigned char mask2 = (1 << p2) - 1;

		if (b1 != b2)
		{
			for (int k = b1 + 1; k < b2; bits[k++] = UCHAR_MAX);

			bits[b2] |= mask2;
		}
		return;
	}
	throw "memory error";
}

void BBV::Inversion(int i)
{
	if (i > -1 && i < len)
	{
		int b = i / 8;
		unsigned char mask = 1 << (i % 8);

		bits[b] ^= mask;
		return;
	}
	throw "memory error";
}

void BBV::InversionInRange(int i, int N = 1)
{
	if (i > -1 && N > 0 && (i + N - 1) < len)
	{
		int b1 = i / 8;
		int p1 = i % 8;
		unsigned char mask1;

		if (i + N > 8)
			mask1 = (1 << p1) - 1;
		else
		{
			mask1 = 1 << p1;
			for (int k = 0; k < N - 1; k++)
				mask1 = mask1 | (mask1 << 1);
			mask1 = ~mask1;
		}

		bits[b1] ^= ~mask1;

		N -= (8 - p1);
		int b2 = (i + N) / 8;
		int p2 = N % 8;
		unsigned char mask2 = (1 << p2) - 1;

		if (b1 != b2)
		{
			for (int k = b1 + 1; k < b2; bits[k++] = 0);

			bits[b2] ^= mask2;
		}
		return;
	}
	throw "memory error";
}

int BBV::Weight()
{
	int res = 0;
	for (int i = 0, k = 0; i < len; k++)
	{
		unsigned char mask = 1;
		for (int j = 0; j < 8 && i < len; j++, i++, mask <<= 1)
			if (bits[k] & mask)
				res++;
	}
	return res;
}

BBV& BBV::operator = (BBV& obj)
{
	if (this != &obj)
	{
		delete[] bits;
		len = obj.len;
		size = obj.size;
		bits = new unsigned char[size];

		for (int i = 0; i < size; bits[i] = obj.bits[i++]);
	}
	return *this;
}

BBV BBV::operator & (BBV& obj)
{
	if (len == obj.len)
	{
		BBV rez;
		rez.len = len;
		rez.size = size;
		rez.bits = new unsigned char[size];

		for (int i = 0; i < size; rez.bits[i] = bits[i] & obj.bits[i], i++);

		return rez;
	}
	throw "different length";
}

BBV& BBV::operator &= (BBV& obj)
{
	if (len == obj.len)
	{
		for (int i = 0; i < size; bits[i] &= obj.bits[i], i++);

		return *this;
	}
	throw "different length";
}

BBV BBV::operator | (BBV& obj)
{
	if (len == obj.len)
	{
		BBV rez;
		rez.len = len;
		rez.size = size;
		rez.bits = new unsigned char[size];

		for (int i = 0; i < size; rez.bits[i] = bits[i] | obj.bits[i], i++);

		return rez;
	}
	throw "different length";
}

BBV& BBV::operator |= (BBV& obj)
{
	if (len == obj.len)
	{
		for (int i = 0; i < size; bits[i] |= obj.bits[i], i++);

		return *this;
	}
	throw "different length";
}

BBV BBV::operator ^ (BBV& obj)
{
	if (len == obj.len)
	{
		BBV rez;
		rez.len = len;
		rez.size = size;
		rez.bits = new unsigned char[size];

		for (int i = 0; i < size; rez.bits[i] = bits[i] ^ obj.bits[i], i++);

		return rez;
	}
	throw "different length";
}

BBV& BBV::operator ^= (BBV& obj)
{
	if (len == obj.len)
	{
		for (int i = 0; i < size; bits[i] ^= obj.bits[i], i++);

		return *this;
	}
	throw "different length";
}

BBV BBV::operator ~ ()
{
	BBV res(len);
	for (int i = 0; i < size; res.bits[i] = ~bits[i], i++);

	int lastB = len % 8;
	if (lastB)
	{
		unsigned char mask = (1 << lastB) - 1;
		res.bits[size - 1] &= mask;
	}

	return res;
}

BBV BBV::operator << (int N)
{
	if (N > 0)
	{
		BBV res(len);
		if (N >= len)
		{
			for (int i = 0; i < size; res.bits[i++] = 0);
		}
		else
		{
			int n1 = N / 8, n2 = N % 8, i;

			unsigned char mask1 = (1 << (8 - n2)) - 1;
			unsigned char mask2 = ~mask1;

			for (i = size - 1; i > n1; i--)
				res.bits[i] = ((bits[i - n1] & mask1) << n2) | ((bits[i - n1 - 1] & mask2) >> (8 - n2));

			res.bits[i] = bits[i] << n2;

			for (i--; i > 0; res.bits[i--] = 0);

			if (len % 8)
			{
				unsigned char mask = (1 << (len % 8)) - 1;
				res.bits[size - 1] &= mask;
			}
		}
		return res;
	}
	return *this;
}

BBV BBV::operator >> (int N)
{
	if (N > 0)
	{
		BBV res(len);
		if (N >= len)
		{
			for (int i = 0; i < size; res.bits[i++] = 0);
		}
		else
		{
			int n1 = N / 8, n2 = N % 8, i;

			unsigned char mask2 = (1 << n2) - 1;
			unsigned char mask1 = ~mask2;

			for (i = 0; i < size - 1 - n1; i++)
				res.bits[i] = ((bits[i + n1] & mask1) >> n2) | ((bits[i + n1 + 1] & mask2) << (8 - n2));

			res.bits[i] = bits[i] >> n2;

			for (i++; i < size; res.bits[i++] = 0);
		}
		return res;
	}
	return *this;
}

BBV& BBV::operator >>= (int N)
{
	if (N > 0)
	{
		if (N >= len)
		{
			for (int i = 0; i < size; bits[i++] = 0);
		}
		else
		{
			int n1 = N / 8, n2 = N % 8, i;

			unsigned char mask2 = (1 << n2) - 1;
			unsigned char mask1 = ~mask2;

			for (i = 0; i < size - 1 - n1; i++)
				bits[i] = ((bits[i + n1] & mask1) >> n2) | ((bits[i + n1 + 1] & mask2) << (8 - n2));

			bits[i] >>= n2;

			for (i++; i < size; bits[i++] = 0);
		}
	}
	return *this;
}

BBV& BBV::operator <<= (int N)
{
	if (N > 0)
	{
		if (N >= len)
		{
			for (int i = 0; i < size; bits[i++] = 0);
		}
		else
		{
			int n1 = N / 8, n2 = N % 8, i;

			unsigned char mask1 = (1 << (8 - n2)) - 1;
			unsigned char mask2 = ~mask1;

			for (i = size - 1; i > n1; i--)
				bits[i] = ((bits[i - n1] & mask1) << n2) | ((bits[i - n1 - 1] & mask2) >> (8 - n2));

			bits[i] = bits[i] << n2;

			for (i--; i > 0; bits[i--] = 0);

			if (len % 8)
			{
				unsigned char mask = (1 << (len % 8)) - 1;
				bits[size - 1] &= mask;
			}
		}
	}
	return *this;
}

BBV::bitK BBV::operator [] (int N)
{
	int n = N / 8;
	int k = N % 8;

	bitK rez(bits + n, 1 << k);

	return rez;
}

bool BBV::operator == (BBV& obj)
{
	if (len == obj.len)
	{
		int i;
		for (i = 0; i < size && bits[i] == obj.bits[i]; i++);

		if (i == size)
			return true;
	}
	return false;
}

bool BBV::operator != (BBV& obj)
{
	if (len == obj.len)
	{
		int i;
		for (i = 0; i < size && bits[i] == obj.bits[i]; i++);

		if (i == size)
			return false;
	}
	return true;
}

BBV::operator char*()
{
	char* str = new char[len + 1];

	for (int i = 0, k = 0; i < len; k++)
	{
		unsigned char mask = 1;
		for (int j = 0; j < 8; j++, i++, mask <<= 1)
			if (bits[k] & mask)
				str[i] = '1';
			else
				str[i] = '0';
	}

	str[len] = '\0';

	return str;
}

BBV::operator bool()
{
	int i = 0;
	for (; i < size && !bits[i]; i++);
	if (i == size)
		i--;
	if (bits[i])
		return true;
	return false;
}

BBV::operator std::string()
{
	std::string str;
	str.resize(len + 1);

	for (int i = 0, k = 0; i < len; k++)
	{
		unsigned char mask = 1;
		for (int j = 0; j < 8; j++, i++, mask <<= 1)
			str[i] = bits[k] & mask + 48;
	}

	str[len] = '\0';

	return str;
}

std::ostream& operator << (std::ostream& out, BBV& obj)
{
	int p = obj.len % 8;
	int i = obj.size - 1;
	if (p)
	{
		unsigned char mask = 1 << (p - 1);
		for (int j = 0; j < p; j++, mask >>= 1)
			if (obj.bits[i] & mask)
				std::cout << '1';
			else
				std::cout << '0';
		i--;
	}
	
	for (; i > -1; i--)
	{
		unsigned char mask = ~((1 << 7) - 1);
		for (int j = 0; j < 8; j++, mask >>= 1)
			if (obj.bits[i] & mask)
				std::cout << '1';
			else
				std::cout << '0';
	}

	return out;
}

std::istream& operator >> (std::istream& in, BBV& obj)
{
	if (obj.bits)
		delete[] obj.bits;

	std::string str;

	std::getline(in, str);
	obj.len = str.length();
	obj.size = obj.len / 8;

	obj.bits = new unsigned char[obj.size];

	for (int i = 0, k = 0; i < obj.len; k++)
	{
		unsigned char mask = 1;
		for (int j = 0; j < 8 && i < obj.len; i++, j++, mask <<= 1)
		{
			if (str[i] - 48)
				obj.bits[k] |= mask;
			else
				obj.bits[k] &= ~mask;
		}
	}

	return in;
}