#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <fstream>
#include <ctime>
// #include <bits/stdc++.h>

using namespace std;

int generateUniqueID()
{

    return static_cast<int>(time(nullptr));
};

vector<string> readData(string fileName)
{
    map<int, vector<int>> threads;
    vector<string> data;
    ifstream inputFile(fileName);
    if (!inputFile.is_open())
    {
        cout << "can not open the file";
    }
    else
    {
        string line;
        while (getline(inputFile, line))
        {
            data.push_back(line);
        }
    }
    return data;
};

class Question
{
private:
public:
    int q_id;
    string question_text;
    string answer;
    int fromUserId;
    int toUserId;
    int HeadQ;

    // Question(int id ,string q_text, ) {};
    ~Question() {};
};

class QuestionManager
{

private:
public:
    vector<Question> Questions;

    void loadDatabase()
    {
        vector<string> lines = readData("Questions.txt");

        for (auto line : lines)
        {
            int index = 0;
            string in1_scannedID = "";
            string in0_q_id = "";
            string in2_toUserId = "";
            string in3_question_text = "";
            string in4_answer = "";
            string HeadQID = "";

            for (auto c : line)
            {
                if (c == ',')
                {
                    index++;
                }
                else
                {
                    switch (index)
                    {
                    case 0:
                        in0_q_id += c;
                        break;

                    case 1:
                        in1_scannedID += c;
                        break;
                    case 2:
                        in2_toUserId += c;
                        break;
                    case 3:
                        in3_question_text += c;
                        break;
                    case 4:
                        in4_answer += c;
                        break;
                    case 5:
                        HeadQID += c;
                        break;

                    default:
                        break;
                    }
                }
            }
            Question temp;

            temp.q_id = stoi(in0_q_id);
            temp.fromUserId = stoi(in1_scannedID);
            temp.toUserId = stoi(in2_toUserId);
            temp.question_text = in3_question_text;
            temp.answer = in4_answer;
            temp.HeadQ = stoi(HeadQID);

            Questions.push_back(temp);
        }
    };

    void updateLastQuestion()
    {
        Question q = Questions[Questions.size() - 1];
        // check if the database file exist and if not create it
        ofstream outputFile("Questions.txt", ios_base::app);
        if (!outputFile.is_open())
        {
            cout << "can not open the file";
            return;
        }
        else
        {

            outputFile
                << q.q_id << ','
                << q.fromUserId << ','
                << q.toUserId << ','
                << q.question_text << ','
                << q.answer << ','
                << q.HeadQ << '\n';
        }
        outputFile.close();
    }

    void feed()
    {
        for (auto question : Questions)
        {
            if (question.answer != "")
            {
                if (question.HeadQ == -1)
                {
                    int currentId = question.q_id;
                    /* code */
                    cout << "Parent Question : " << question.q_id << ','
                         << question.fromUserId << ','
                         << question.toUserId << ','
                         << question.question_text << ','
                         << question.answer << ','
                         << question.HeadQ << '\n';
                }
                else
                {
                    cout << "" << question.q_id << ','
                         << question.fromUserId << ','
                         << question.toUserId << ','
                         << question.question_text << ','
                         << question.answer << ','
                         << question.HeadQ << '\n';
                }
            }
        }
    }
};

void writeData(vector<Question> &line)
{
    // logging.open(FILENAME, std::ios_base::app);
    // check if the database file exist and if not create it
    ofstream outputFile("Questions.txt");
    if (!outputFile.is_open())
    {
        cout << "can not open the file";
        return;
    }
    else
    {
        for (auto q : line)
        {

            outputFile
                << q.q_id << ','
                << q.fromUserId << ','
                << q.toUserId << ','
                << q.question_text << ','
                << q.answer << ','
                << q.HeadQ << '\n';
        }
        outputFile.close();
    }
}

class User
{
private:
public:
    int id;
    string name;
    int password;
    vector<Question> user_questions;
    User() {};
    ~User() {};

    bool askQuestion(int toId, QuestionManager &Q)
    {

        Question temp;
        cout << "For Thread question Parent Question id else type -1";

        string headId;
        cin >> headId;
        if (stoi(headId) != -1)
        {
            while (stoi(headId) != -1)
            {
                for (auto &question : Q.Questions)
                {
                    // check if the Question is exist and can be head
                    if (question.toUserId == toId && stoi(headId) == question.q_id && question.HeadQ == -1)
                    {
                        temp.q_id = generateUniqueID();

                        string qtext;
                        cout << "Type your Question :";
                        getline(cin, qtext);
                        getline(cin, qtext);
                        qtext = qtext + " ?";
                        temp.question_text = qtext;
                        temp.fromUserId = id;
                        temp.toUserId = toId;
                        temp.HeadQ = stoi(headId);

                        Q.Questions.push_back(temp);

                        return true;
                    }
                }
                cout << " not exist try agian ";
                cin >> headId;
            }
        }

        temp.q_id = generateUniqueID();

        string qtext;
        cout << "Type your Normal Question :";
        getline(cin, qtext);
        getline(cin, qtext);
        qtext = qtext + " ?";
        temp.question_text = qtext;
        temp.fromUserId = id;
        temp.toUserId = toId;
        temp.HeadQ = -1;

        Q.Questions.push_back(temp);

        return true;
    }

    void printQuestionsFromMe(QuestionManager &Q)
    {
        for (auto question : Q.Questions)
        {
            if (question.fromUserId == id)
            {
                /* code */
                cout << question.q_id << ','
                     << question.fromUserId << ','
                     << question.toUserId << ','
                     << question.question_text << ','
                     << question.answer << ','
                     << question.HeadQ << '\n';
            }
        }
    }

    void printQuestionsToMe(QuestionManager &Q)
    {

        for (auto question : Q.Questions)
        {
            if (question.toUserId == id && question.HeadQ == -1)
            {
                int currentId = question.q_id;
                /* code */
                cout << "Parent Question : " << question.q_id << ','
                     << question.fromUserId << ','
                     << question.toUserId << ','
                     << question.question_text << ','
                     << question.answer << ','
                     << question.HeadQ << '\n';
                for (auto question : Q.Questions)
                {
                    if (question.HeadQ == currentId)
                    {
                        /* code */
                        cout << "   Thread : " << question.q_id << ','
                             << question.fromUserId << ','
                             << question.toUserId << ','
                             << question.question_text << ','
                             << question.answer << ','
                             << question.HeadQ << '\n';
                    }
                }
            }
        }
    }

    void answerQuestion(QuestionManager &Q)
    {
        string qID;

        while (true)
        {
            /* code */
            cout << "\nquestion id to be answered : ";
            cin >> qID;
            for (auto &question : Q.Questions)
            {

                if (stoi(qID) == question.q_id && id == question.toUserId)
                {
                    string answer;
                    cout << "Your Answer : ";
                    getline(cin, answer);
                    getline(cin, answer);

                    question.answer = answer;
                    writeData(Q.Questions);
                    return;
                }
            }
            cout << "question id not found or the question is not for you ... try another id";
        }
    }

    void deleteQuestion(QuestionManager &Q)
    {
        // Note User just can delete to me Question
        string delId;

        cout << "Enter a Question id : ";
        cin >> delId;
        // int i = 0;
        // int index = -1;
        bool found = false;
        for (int i = 0; i < Q.Questions.size(); i++)
        {
            if (Q.Questions[i].q_id == stoi(delId) && Q.Questions[i].toUserId == id && Q.Questions[i].HeadQ != -1)
            {
                Q.Questions.erase(Q.Questions.begin() + i);
                writeData(Q.Questions);
                cout << "Deleted Successfully";
                // found = true;
                break;
            }
            else if (Q.Questions[i].q_id == stoi(delId) && Q.Questions[i].toUserId == id && Q.Questions[i].HeadQ == -1)
            {
                int currentId = stoi(delId);
                for (int i = 0; i < Q.Questions.size(); i++)
                {
                    if (currentId == Q.Questions[i].HeadQ)
                    {
                        Q.Questions.erase(Q.Questions.begin() + i);
                    }
                }

                Q.Questions.erase(Q.Questions.begin() + i);
                writeData(Q.Questions);
                break;
            }
        }
    }
    //     if (found)
    //     {

    //         Q.Questions.erase(Q.Questions.begin() + index);
    //         writeData(Q.Questions);
    //         cout << "Deleted Successfully";
    //     }
    //     else
    //     {
    //         cout << "Oops Error !";
    //     }
    // }
};

void writeData(vector<User> line)
{
    // logging.open(FILENAME, std::ios_base::app);
    // check if the database file exist and if not create it
    ofstream outputFile("database.txt");
    if (!outputFile.is_open())
    {
        cout << "can not open the file";
        return;
    }
    else
    {
        for (auto pice : line)
        {

            outputFile << pice.id << ',' << pice.name << ',' << pice.password << '\n';
        }
        outputFile.close();
    }
}

class UserManager
{
private:
public:
    vector<User> user_vec;

    void loadData()
    {
        vector<string> line = readData("database.txt");

        for (auto eachLine : line)
        {
            int index = 0;
            User temp;
            string id;
            string name;
            string password;
            for (auto c : eachLine)
            {
                if (c == ',')
                {
                    index++;
                }
                else
                {
                    switch (index)
                    {

                    case 0:
                        id += c;
                        break;

                    case 1:
                        name += c;
                        break;
                    case 2:
                        password += c;
                        break;

                    default:
                        break;
                    }
                }
            }
            temp.id = stoi(id);
            temp.name = name;
            temp.password = stoi(password);
            user_vec.push_back(temp);
        }
    };

    int findName(string name)
    {
        int state = 0;
        for (auto u : user_vec)
        {
            if (u.name == name)
            {

                state = u.id;
                break;
            }
        }
        return state;
    }

    bool findId(int id)
    {
        bool state = false;
        for (auto u : user_vec)
        {
            if (u.id == id)
            {

                state = true;
                break;
            }
        }
        return state;
    }

    bool verifyPass(string name, int pass)
    {
        bool state = false;
        for (auto u : user_vec)
        {
            if (u.name == name)
            {
                if (u.password == pass)
                {
                    state = true;
                }
            }
        }
        return state;
    };

    void listUsers()
    {
        for (auto u : user_vec)
        {

            cout << "ID: " << u.id << "                 NAME: " << u.name << endl;
        }
    }
};

class AskSystem
{
private:
    /* data */
public:
    void run()
    {
        bool loggedIn = false;
        UserManager uManager;
        QuestionManager qManager;
        qManager.loadDatabase();
        uManager.loadData();

        User onlineUser;

        int login_selection = showLoginPage();

        if (login_selection == 1)
        {
            loggedIn = login(uManager, onlineUser);
            if (loggedIn)
            {
                // onlineUser.loadMyQuestions();
            }
        }
        else if (login_selection == 2)
        {
            signUp(uManager);
        }

        while (loggedIn)
        {
            int selection = showMainPage();

            switch (selection)
            {
            case 1:
                onlineUser.printQuestionsToMe(qManager);
                break;
            case 2:
                onlineUser.printQuestionsFromMe(qManager);
                break;
            case 3:
                onlineUser.answerQuestion(qManager);
                break;
            case 4:
                onlineUser.deleteQuestion(qManager);
                break;
            case 5:

                int toWhoID;
                cout << "but user ID That you want to ask: ";
                cin >> toWhoID;

                if (uManager.findId(toWhoID))
                {
                    bool done;
                    done = onlineUser.askQuestion(toWhoID, qManager);
                    if (done)
                    {
                        cout << "Your question was added successfully  ";
                        cout << endl;

                        qManager.updateLastQuestion();
                    }
                }
                else
                {
                    cout << "User does not exist\n";
                }

                break;
            case 6:

                uManager.listUsers();
                break;
            case 7:
                qManager.feed();
                break;
            case 8:
                loggedIn = false;
                break;

            default:
                //  selection = showMainPage();

                break;
            }
        }

        run();
    };

    bool signUp(UserManager uManager)
    {
        string userName;
        int password;
        cout << "your user name with no spaces : ";
        cin >> userName;
        cout << "\nyour password : ";
        cin >> password;
        User temp;
        temp.id = generateUniqueID();
        temp.name = userName;
        temp.password = password;

        uManager.user_vec.push_back(temp);
        writeData(uManager.user_vec);
        return true;
    };

    bool login(UserManager uManager, User &onlineUser)
    {
        string name;
        int password;

        cout << "your name : ";
        cin >> name;
        if (uManager.findName(name))
        {
            cout << "your pass : ";
            cin >> password;
            if (uManager.verifyPass(name, password))
            {
                int tempID = uManager.findName(name);
                onlineUser.id = tempID;
                onlineUser.name = name;
                onlineUser.password = password;

                cout
                    << endl
                    << endl
                    << "Welcome " << onlineUser.name << endl
                    << endl
                    << endl;

                return true;
            }
        }
        else
        {
            cout << "Error \n"
                 << endl;
        }
        return false;
    };

    int showLoginPage()
    {
        string num;
        while (true)
        {
            cout << "1 for login" << endl
                 << "2 for signup" << endl;
            cin >> num;
            if (num == "1" || num == "2")
            {
                return stoi(num);
            }
            else
            {
                cout << "error" << endl;
            }
        }
    };

    int showMainPage()
    {

        string num;
        while (true)
        {
            cout << "\n1- Print Questions To Me\n ";
            cout << "\n2 -Print Questions From Me \n ";
            cout << "\n3 - Answer Question\n";
            cout << "\n4 - Delete Question\n";
            cout << "\n5 - Ask Question\n";
            cout << "\n6 - List System Users\n";
            cout << "\n7 - Feed\n";
            cout << "\n8 - Logout\n ";

            cin >> num;
            if (num == "1" || num == "2" || num == "3" || num == "4" || num == "5" || num == "6" || num == "7" || num == "8")
            {
                return stoi(num);
            }
            else
            {
                cout << "error" << endl;
            }
        }
    }

    void showUserPage()
    {
    }
};

int main(int argc, char const *argv[])
{

    AskSystem service;
    service.run();
    return 0;
}
