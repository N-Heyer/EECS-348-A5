#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

//struct to store the email with sender cat, subj, date, prior, and date
struct Email {
    string sender_category;  // cat like boss, sub, important
    string subject;          //email subj
    string date;             // date string
    int priority_value;      //numeric priority
    int numeric_date;        //date conversion

    //init email object
    Email(string sender, string subj, string dt) 
        : sender_category(sender), subject(subj), date(dt) {
        //map to assign prior to email based on sender cat
        static unordered_map<string, int> PRIORITY_MAP = {
            {"Boss", 5},
            {"Subordinate", 4},
            {"Peer", 3},
            {"ImportantPerson", 2},
            {"OtherPerson", 1}
        };
        priority_value = PRIORITY_MAP[sender]; //priority based on sender

        //convert date for ease
        numeric_date = stoi(dt.substr(6, 4) + dt.substr(0, 2) + dt.substr(3, 2));
    }
};

//comparing for prior Q
//emails sorted by sender then date
struct CompareEmails {
    bool operator()(const Email& a, const Email& b) {
        if (a.priority_value != b.priority_value)
            return a.priority_value < b.priority_value; //highest prior first
        return a.numeric_date < b.numeric_date; //if same date first
    }
};

int main() {
    // store emails with top prior at top
    priority_queue<Email, vector<Email>, CompareEmails> emailQueue;
    
    string file_name;
    cout << "Enter the file name containing emails and commands: ";
    cin >> file_name;

    ifstream file(file_name);
    if (!file) {
        cout << "\nFile not found.\n";
        return 1;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; //skip empty line

        istringstream iss(line);
        string command;
        iss >> command; //reads commands like read, next

        if (command == "EMAIL") {
            string sender, subject, date;

            //read sender cat
            iss >> ws;
            getline(iss, sender, ',');

            //read subj
            iss >> ws;
            getline(iss, subject, ',');

            //read date
            iss >> ws;
            getline(iss, date, ',');

            //add new email to Q
            emailQueue.emplace(sender, subject, date);
        } 
        else if (command == "COUNT") {
            //dsiplay unread emails
            cout << "\nThere are " << emailQueue.size() << " emails to read.\n";
        } 
        else if (command == "NEXT") {
            if (!emailQueue.empty()) {
                //get highest priopr without removal
                const Email& next_email = emailQueue.top();
                cout << "\nNext email:\n"
                     << "Sender: " << next_email.sender_category << "\n"
                     << "Subject: " << next_email.subject << "\n"
                     << "Date: " << next_email.date << "\n";
            } else {
                cout << "\nNo emails waiting.\n";
            }
        } 
        else if (command == "READ") {
            if (!emailQueue.empty()) {
                //remove highest prior from Q
                emailQueue.pop();
            } else {
                cout << "\nNo emails to read.\n";
            }
        } 
        else {
            //only handle valid commands
            cout << "\nInvalid command found in file: " << line << "\n";
        }
    }
    return 0;
}