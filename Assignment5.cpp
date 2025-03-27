#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Email {
    static unordered_map<string, int>& getPriorityMap() {
        static unordered_map<string, int> PRIORITY_MAP = {
            {"Boss", 5},
            {"Subordinate", 4},
            {"Peer", 3},
            {"ImportantPerson", 2},
            {"OtherPerson", 1}
        };
        return PRIORITY_MAP;
    }

    string sender_category;
    string subject;
    string date;
    int priority_value;
    int numeric_date; // Stores the date as an integer for sorting

    Email(string sender, string subj, string dt) 
        : sender_category(sender), subject(subj), date(dt) {
        unordered_map<string, int>& PRIORITY_MAP = getPriorityMap();
        priority_value = PRIORITY_MAP[sender];

        // Convert date format from MM-DD-YYYY to an integer YYYYMMDD
        numeric_date = stoi(dt.substr(6, 4) + dt.substr(0, 2) + dt.substr(3, 2));
    }
};

// **Sorting Function - Ensures correct prioritization**
bool compareEmails(const Email &a, const Email &b) {
    unordered_map<string, int>& PRIORITY_MAP = Email::getPriorityMap();
    int priorityA = PRIORITY_MAP[a.sender_category];
    int priorityB = PRIORITY_MAP[b.sender_category];
    
    if (priorityA != priorityB)
        return priorityA > priorityB; // Higher category first
    
    return a.numeric_date > b.numeric_date; // Newest date first
}

int main() {
    vector<Email> emailQueue;
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
        if (line.empty()) continue;

        istringstream iss(line);
        string command;
        iss >> command;
        
        if (command == "EMAIL") {
            string email_data;
            getline(iss, email_data);
            
            stringstream ss(email_data);
            string sender, subject, date;
            
            if (!getline(ss, sender, ',') || !getline(ss, subject, ',') || !getline(ss, date, ',')) {
                cout << "\nInvalid EMAIL format: " << line << "\n";
                continue;
            }

            emailQueue.emplace_back(sender, subject, date);
            sort(emailQueue.begin(), emailQueue.end(), compareEmails);
        } 
        else if (command == "COUNT") {
            cout << "\nThere are " << emailQueue.size() << " emails to read.\n";
        } 
        else if (command == "NEXT") {
            if (!emailQueue.empty()) {
                const Email &next_email = emailQueue.front();
                cout << "\nNext email:\n"
                     << "\tSender: " << next_email.sender_category << "\n"
                     << "\tSubject: " << next_email.subject << "\n"
                     << "\tDate: " << next_email.date << "\n";
            } else {
                cout << "\nNo emails waiting.\n";
            }
        } 
        else if (command == "READ") {
            if (!emailQueue.empty()) {
                emailQueue.erase(emailQueue.begin());
            } else {
                cout << "\nNo emails to read.\n";
            }
        } 
        else {
            cout << "\nInvalid command found in file: " << line << "\n";
        }
    }
    return 0;
}
