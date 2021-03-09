#pragma once
#include <iostream>
#include <string>

class BoolMx;

class BBV
{
public:

	friend BoolMx;

	class bitK
	{
	public:
		bitK(unsigned char* BAIT,unsigned char MASK);

		bitK& operator ~ ();

		bitK& operator = (bitK& obj);

		bitK& operator = (bool x);

		operator bool();

		friend std::istream& operator >> (std::istream& in, bitK& obj);
		friend std::ostream& operator << (std::ostream& out, bitK& obj);

	private:
		unsigned char* bait;
		unsigned char mask;
	};

	BBV();

	explicit BBV(int LEN);

	BBV(BBV& obj);

	explicit BBV(std::string& str);

	explicit BBV(char* str);

	BBV(char* str, int LEN);

	~BBV();

	int GetLength();

	int GetSize();

	void Set0(int i);

	void Set0InRange(int i, int len);

	void Set1(int i);

	void Set1InRange(int i, int len);

	void Inversion(int i);

	void InversionInRange(int i, int len);

	int Weight();

	BBV& operator = (BBV& obj);

	BBV operator & (BBV& obj);

	BBV& operator &= (BBV& obj);

	BBV operator | (BBV& obj);

	BBV& operator |= (BBV& obj);

	BBV operator ^ (BBV& obj);

	BBV& operator ^= (BBV& obj);

	BBV operator ~ ();

	BBV operator >> (int N);

	BBV operator << (int N);

	BBV& operator >>= (int N);

	BBV& operator <<= (int N);

	bitK operator [] (int N);

	bool operator == (BBV& obj);

	bool operator != (BBV& obj);

	operator char*();

	operator bool();
	
	operator std::string();

	friend std::istream& operator >> (std::istream& in, BBV& obj);
	friend std::ostream& operator << (std::ostream& out, BBV& obj);

protected:
	unsigned char* bits;
	int len;
	int size;
};