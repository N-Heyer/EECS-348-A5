#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Email {
    string sender_category;
    string subject;
    string date;
    int priority_value;
    int numeric_date; // Stores the date as an integer for sorting

    Email(string sender, string subj, string dt) 
        : sender_category(sender), subject(subj), date(dt) {
        static unordered_map<string, int> PRIORITY_MAP = {
            {"Boss", 5},
            {"Subordinate", 4},
            {"Peer", 3},
            {"ImportantPerson", 2},
            {"OtherPerson", 1}
        };
        priority_value = PRIORITY_MAP[sender];

        // Convert date format from MM-DD-YYYY to an integer YYYYMMDD
        numeric_date = stoi(dt.substr(6, 4) + dt.substr(0, 2) + dt.substr(3, 2));
    }
};

// **Sorting Function - Ensures correct prioritization**
bool compareEmails(const Email &a, const Email &b) {
    if (a.priority_value != b.priority_value)
        return a.priority_value > b.priority_value; // Higher priority first
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
            sort(emailQueue.begin(), emailQueue.end(), compareEmails); // Keep queue sorted
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
                     << "Date: " << next_email.date << "\n";
            } else {
                cout << "\nNo emails waiting.\n";
            }
        } 
        else if (command == "READ") {
            if (!emailQueue.empty()) {
                emailQueue.erase(emailQueue.begin());  // Remove the top email
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
