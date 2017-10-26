/*main.cpp*/

/* iClicker Analysis program in modern C++.
Mika Cabudol
U. of Illinois, Chicago
CS 341: Fall 2017
Project 02 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <functional>
#include <map>

#include "student.h"
#include "session.h"

using namespace std;

/*FileExists: Returns true if the file exists, false if not. */
bool FileExists(string filename)
{
  ifstream file(filename);
  return file.good();
}

/* parses correct answer for the question */
string cans(string l)
{
  int canspos = l.find("cans=");
  string aftercanspos = l.substr(canspos + 6);
  canspos = aftercanspos.find("\" ");
  return aftercanspos.substr(0, canspos);
}

/* updates student in sudent vector updateStudent(stdline, clickers, correctAnswer, ssnobj);*/
void updateStudent(string l, vector<Student>& clkr, string correct, Session& sn)
{
  // get clicker id
  string clickerid = l.substr(l.find("id=") + 5, 8);
  auto sobj = std::find_if(clkr.begin(), clkr.end(),
        [=](const Student& s){
          if (s.getClickerID().compare(clickerid) == 0)
            return true;
          else
            return false;
      });

  // initialize student object if it is not in vector
  if (sobj == clkr.end())
  {
    Student snew(clickerid, 0, 0);
    clkr.push_back(snew);
    sobj = std::find_if(clkr.begin(), clkr.end(),
      [=](const Student& s){
        if (s.getClickerID().compare(clickerid) == 0)
          return true;
        else
          return false;
    });
    sn.setClickers();
  }

  // get answer of student and update student data in vector
  string sans = l.substr(l.find(" ans=") + 6, 1);
  if (sans.compare("\"") != 0)
  {
    sobj->setAnswered();
    sn.setAnswers();
    if (correct.find(sans) != -1)
    {
      sobj->setCorrect();
      sn.setCorrect();
    }
  }
}

int countItems(map<string, Session>& M)
{
  int items = 0;
  for (const auto& m : M)
  {
    Session ms = m.second;
    items += ms.getQuestions();
  }
  return items;
}

// flag: 0 for answered, 1 for correct
void printClassAnalysis(int flag, map<string, Session>& N)
{
  for(const auto& ns: N)
  {
    Session n = ns.second;
    int comp = ((flag == 0) ? n.getAnswers() : n.getCorrect());
    string cap = ((flag == 0) ? "" : " correct");
    double avg = (double(comp) / double(n.getClickers() * n.getQuestions())) * 100;
    cout << " " << ns.first << ": " << avg << "% (" << n.getQuestions() << " questions, " << n.getClickers() << " clickers, " << comp  << cap << " answers)" << endl;
  }
}

// print analysis for certain clicker
void printClickerAnalysis(string i, map<string, vector<Student>>& M, map<string, Session>& S)
{
  bool isThere = false;
  for(const auto& ma: M)
  {
    auto& Q = S.find(ma.first)->second;
    int getQ = Q.getQuestions();
    
    vector<Student> m = ma.second;
    auto mobj = std::find_if(m.begin(), m.end(),
      [=](const Student& s){
        if (s.getClickerID().compare(i) == 0)
          return true;
        else
          return false;
    });

    if(mobj != m.end())
    {
      isThere = true;
      //cout << "Test Purpose Q: " << Q->second.getAnswers() << "/" << getQ << endl;
      double p = (double(mobj->getCorrect()) / double(getQ)) * 100;
      cout << " " << ma.first << ": " << mobj->getAnswered() << " out of " << getQ << " answered, " << p << "% correctly." << endl;
    }
  }
  
  if(!isThere)
    cout << "** not found..." << endl;
}

int main()
{
  /* READ FILE */
  string fileline, stdline;
  
  if (!FileExists("files.txt"))
  {
    cout << "Error: files.txt not found. Exiting program..." << endl;
    return -1;
  }
  ifstream mainfile("files.txt");
  map<string, vector<Student>> sessions;
  map<string, Session> sessioninfo;
  int commentlines = 40;

  /* PARSE FILES */
  cout << "**Starting**" << endl;
  while(getline(mainfile, fileline))
  {
    vector<Student> clickers;

    cout << ">>Parsing \'" << fileline << "\'..." << endl;

    if (!FileExists(fileline))
    {
      cout << "Error: '" << fileline << "' not found." << endl;
      break;
    }
    ifstream file(fileline);

    // skips comments
    for (int i = 0; i <= commentlines; i++)
      getline(file, stdline);

    /* HEADER ANALYSIS */
    int namepos = stdline.find("ssnn=");
    string sessionname = stdline.substr(namepos + 5, 11);
    Session ssnobj(sessionname, 0, 0, 0, 0);
    string correctAnswer = "";

    /* PER QUESTIONS ANALYSIS */
    while(getline(file, stdline))
    {
      // new question
      if (stdline.find("<p") != -1)
      {
        ssnobj.setQuestions();
        correctAnswer = cans(stdline);
      }

      // new answer
      if (stdline.find("<v") != -1)
        updateStudent(stdline, clickers, correctAnswer, ssnobj);
    }
    
    // push clicker vector into sessions map
    sessioninfo.insert(pair<string, Session>(sessionname, ssnobj));
    sessions.insert(pair<string, vector<Student>>(sessionname, clickers));
  }

  /* OUTPUT GENERAL CLASS ANALYSIS */
  cout << endl;
  cout << "**Class Analysis**" << endl;
  cout << ">>Total sessions: " << sessions.size() << endl;
  cout << ">>Total questions: " << countItems(sessioninfo) << endl;
  cout << ">>Answered: " << endl;
  printClassAnalysis(0, sessioninfo);
  cout << ">>Correctly: " << endl;
  printClassAnalysis(1, sessioninfo);
  cout << endl;

  /* DYNAMIC STUDENT ANALYSIS */
  cout << "**Student Analysis**" << endl;
  // loop user input here
  string inputclicker = "";
  while(inputclicker.compare("#") != 0)
  {
    cout << ">> Enter a clicker id (# to quit): ";
    cin >> inputclicker;
    //cout << inputclicker << endl;
    cout << endl;
    if (inputclicker.compare("#") == 0)
      break;
    else
      printClickerAnalysis(inputclicker, sessions, sessioninfo);
    cout << endl;
  }

  cout << endl;
  cout << "**END**" << endl;
  return 0;
}