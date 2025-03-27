#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Email {
    static unordered_map<string, int> PRIORITY_MAP; // Declare but do not initialize here

    string sender_category;
    string subject;
    string date;
    int arrival_order;
    int priority;

    Email(string sender, string subj, string dt, int order) 
        : sender_category(sender), subject(subj), date(dt), arrival_order(order) {
        priority = (PRIORITY_MAP.count(sender) ? PRIORITY_MAP[sender] : 0) * 10000 - arrival_order;
    }
};

// **Define the static variable outside the struct**
unordered_map<string, int> Email::PRIORITY_MAP = {
    {"Boss", 5},
    {"Subordinate", 4},
    {"Peer", 3},
    {"ImportantPerson", 2},
    {"OtherPerson", 1}
};

// Comparator for sorting emails by priority
bool compareEmails(const Email &a, const Email &b) {
    return a.priority > b.priority;  // Higher priority emails first
}

int main() {
    vector<Email> emailQueue;
    int arrival_counter = 1;

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

            emailQueue.emplace_back(sender, subject, date, arrival_counter++);
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
                emailQueue.erase(emailQueue.begin());  // Remove highest priority email
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
