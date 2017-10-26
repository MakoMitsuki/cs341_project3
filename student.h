/*student.h*/

#pragma once
#include <string>

using namespace std;

class Student
{
	private:
		string ClickerID;
		int Answered;
		int Correct;

	public:
	Student(string clickerid, int answered, int correct)
	: ClickerID(clickerid), Answered(answered), Correct(correct)
	{}

	void setClickerID(string cid)
	{
		ClickerID = cid;
	}

	void setAnswered()
	{
		Answered++;
	}

	void setCorrect()
	{
		Correct++;
	}

	string getClickerID() const
	{
		return ClickerID;
	}

	int getAnswered() const
	{
		return Answered;
	}

	int getCorrect() const
	{
		return Correct;
	}  
};