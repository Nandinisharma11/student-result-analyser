#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>

using namespace std;

/* ---------------- STUDENT CLASS ---------------- */

class Student {
public:
    int roll;
    string name;
    vector<int> marks;
    int total;
    float avg;
    char grade;
    string status;

    Student() {}

    Student(int r, string n, vector<int> m) {
        roll = r;
        name = n;
        marks = m;
        calcAll();
    }

    void calcAll() {
        total = 0;
        for (int x : marks) total += x;
        avg = (float) total / marks.size();

        if (avg >= 90) grade = 'A';
        else if (avg >= 75) grade = 'B';
        else if (avg >= 60) grade = 'C';
        else if (avg >= 40) grade = 'D';
        else grade = 'F';

        status = (avg >= 40) ? "PASS" : "FAIL";
    }
};

/* ---------------- MANAGER CLASS ---------------- */

class Manager {
public:
    unordered_map<int, Student> mp;

    void loadFromFile() {
        ifstream file("data.txt");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string value;
            vector<string> tokens;

            while (getline(ss, value, ',')) {
                tokens.push_back(value);
            }

            int roll = stoi(tokens[0]);
            string name = tokens[1];

            vector<int> marks;
            for (int i = 2; i < tokens.size(); i++) {
                marks.push_back(stoi(tokens[i]));
            }

            Student s(roll, name, marks);
            mp[roll] = s;
        }
        file.close();
    }

    void saveToFile() {
        ofstream file("data.txt");

        for (auto &p : mp) {
            Student s = p.second;

            file << s.roll << "," << s.name;
            for (int mark : s.marks)
                file << "," << mark;

            file << endl;
        }
        file.close();
    }
};

/* ---------------- MAIN FUNCTION ---------------- */

int main(int argc, char* argv[]) {

    Manager m;
    m.loadFromFile();

    if (argc < 2) {
        cout << "{\"error\":\"No command\"}";
        return 0;
    }

    string cmd = argv[1];

    // ADD STUDENT
    if (cmd == "add") {

        int roll = stoi(argv[2]);
        string name = argv[3];

        vector<int> marks;
        for (int i = 4; i < argc; i++)
            marks.push_back(stoi(argv[i]));

        Student s(roll, name, marks);
        m.mp[roll] = s;
        m.saveToFile();

        cout << "{\"message\":\"Student added\"}";
    }

    // SHOW ALL
    else if (cmd == "showall") {

        cout << "[";
        bool first = true;

        for (auto &p : m.mp) {
            if (!first) cout << ",";
            Student s = p.second;

            cout << "{";
            cout << "\"roll\":" << s.roll << ",";
            cout << "\"name\":\"" << s.name << "\",";
            cout << "\"total\":" << s.total << ",";
            cout << "\"avg\":" << s.avg << ",";
            cout << "\"grade\":\"" << s.grade << "\",";
            cout << "\"status\":\"" << s.status << "\"";
            cout << "}";

            first = false;
        }
        cout << "]";
    }

    // TOPPER
    else if (cmd == "topper") {

        if (m.mp.empty()) {
            cout << "{\"error\":\"No students\"}";
            return 0;
        }

        Student top;
        bool first = true;

        for (auto &p : m.mp) {
            if (first || p.second.total > top.total) {
                top = p.second;
                first = false;
            }
        }

        cout << "{";
        cout << "\"name\":\"" << top.name << "\",";
        cout << "\"total\":" << top.total;
        cout << "}";
    }

    return 0;
}