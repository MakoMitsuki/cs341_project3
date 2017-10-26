/*session.h*/

#pragma once
#include <string>

using namespace std;

class Session
{
	private:
		string SessionName;
		int Questions;
		int Clickers;
		int Answers;
		int Correct;

	public:
	Session(string sessionname, int questions, int clickers, int answers, int correct)
	: SessionName(sessionname), Questions(questions), Clickers(clickers), Answers(answers), Correct(correct)
	{}

	void setSessionName(string sid)
	{
		SessionName = sid;
	}

	void setQuestions()
	{
		Questions++;
	}

	void setClickers()
	{
		Clickers++;
	}

	void setAnswers()
	{
		Answers++;
	}

	void setCorrect()
	{
		Correct++;
	}

	string getSessionName() const
	{
		return SessionName;
	}

	int getQuestions() const
	{
		return Questions;
	}

	int getClickers() const
	{
		return Clickers;
	} 

	int getAnswers() const
	{
		return Answers;
	} 

	int getCorrect() const
	{
		return Correct;
	}
};