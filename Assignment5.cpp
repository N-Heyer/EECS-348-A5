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

    Email(string sender, string subj, string dt) 
        : sender_category(sender), subject(subj), date(dt) {
        unordered_map<string, int>& PRIORITY_MAP = getPriorityMap();
        int category_priority = PRIORITY_MAP[sender];  // Get sender priority
        priority_value = category_priority; // Priority base value
    }
};

// **Sorting Function - Higher Priority & Newest Date First**
bool compareEmails(const Email &a, const Email &b) {
    if (a.priority_value != b.priority_value)
        return a.priority_value > b.priority_value; // Higher category first
    return a.date > b.date; // Newest date first (YYYY-MM-DD sorting works lexicographically)
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

            // Convert date to YYYYMMDD format for correct sorting
            string formatted_date = date.substr(6,4) + date.substr(0,2) + date.substr(3,2);
            emailQueue.emplace_back(sender, subject, formatted_date);
            sort(emailQueue.begin(), emailQueue.end(), compareEmails);
        } 
        else if (command == "COUNT") {
            cout << "\nThere are " << emailQueue.size() << " emails to read.\n";
        } 
        else if (command == "NEXT") {
            if (!emailQueue.empty()) {
                const Email &next_email = emailQueue.front();
                cout << "\nNext email:\n"
                     << "Sender: " << next_email.sender_category << "\n"
                     << "Subject: " << next_email.subject << "\n"
                     << "Date: " << next_email.date.substr(4,2) << "-" << next_email.date.substr(6,2) << "-" << next_email.date.substr(0,4) << "\n";
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