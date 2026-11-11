#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <limits>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

// ============================================================
// INPUT HELPERS
// ============================================================

int readInt(const string &prompt)
{
    int x;
    cout << prompt;

    while (!(cin >> x))
    {
        if (cin.eof())
            exit(0);

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }

    return x;
}

double readDouble(const string &prompt)
{
    double x;
    cout << prompt;

    while (!(cin >> x))
    {
        if (cin.eof())
            exit(0);

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Enter a number: ";
    }

    return x;
}

string readLine(const string &prompt)
{
    string s;
    cout << prompt;
    getline(cin >> ws, s);
    return s;
}

// ============================================================
// FILE HANDLING HELPERS
// ============================================================

// Save strings safely, including spaces.
void saveString(ofstream &file, const string &value)
{
    file <<quoted(value);
}

// ============================================================
// MODULE 1: JOB MANAGEMENT
// Data structure: Singly Linked List + Quick Sort
// ============================================================

struct JobNode
{
    int id;
    string company, role, branch;
    double packageLPA, minCGPA;
    int maxBacklogs;
    JobNode *next;
};

class JobList
{
private:
    JobNode *head;
    JobNode *tail;

    static int partition(JobNode *arr[], int low, int high)
    {
        double pivot = arr[high]->packageLPA;
        int i = low - 1;

        for (int j = low; j < high; j++)
        {
            if (arr[j]->packageLPA < pivot)
            {
                swap(arr[++i], arr[j]);
            }
        }

        swap(arr[i + 1], arr[high]);
        return i + 1;
    }

    static void quickSort(JobNode *arr[], int low, int high)
    {
        if (low < high)
        {
            int pi = partition(arr, low, high);
            quickSort(arr, low, pi - 1);
            quickSort(arr, pi + 1, high);
        }
    }

    static void printJob(const JobNode *p)
    {
        cout << "\nJob ID: " << p->id
             << "\nCompany: " << p->company
             << "\nRole: " << p->role
             << "\nBranch: " << p->branch
             << "\nPackage: " << p->packageLPA << " LPA"
             << "\nMinimum CGPA: " << p->minCGPA
             << "\nMaximum Backlogs: " << p->maxBacklogs
             << "\n";
    }

public:
    JobList() : head(nullptr), tail(nullptr) {}

    ~JobList()
    {
        clear();
    }

    void clear()
    {
        while (head != nullptr)
        {
            JobNode *temp = head;
            head = head->next;
            delete temp;
        }

        tail = nullptr;
    }

    JobNode *findById(int id) const
    {
        for (JobNode *p = head; p != nullptr; p = p->next)
        {
            if (p->id == id)
                return p;
        }

        return nullptr;
    }

    void addJob()
    {
        int id = readInt("Enter Job ID: ");

        if (findById(id))
        {
            cout << "Job ID already exists!\n";
            return;
        }

        JobNode *p = new JobNode;

        p->id = id;
        p->company = readLine("Enter Company Name: ");
        p->role = readLine("Enter Job Role: ");
        p->branch = readLine("Enter Eligible Branch: ");
        p->packageLPA = readDouble("Enter Package (LPA): ");
        p->minCGPA = readDouble("Enter Minimum CGPA: ");
        p->maxBacklogs = readInt("Enter Maximum Backlogs: ");
        p->next = nullptr;

        if (head == nullptr)
            head = tail = p;
        else
        {
            tail->next = p;
            tail = p;
        }

        cout << "Job added successfully!\n";
    }

    void displayJobs() const
    {
        if (!head)
        {
            cout << "No jobs available.\n";
            return;
        }

        for (JobNode *p = head; p != nullptr; p = p->next)
            printJob(p);
    }

    void searchJob() const
    {
        int id = readInt("Enter Job ID: ");
        JobNode *p = findById(id);

        if (p)
            printJob(p);
        else
            cout << "Job not found.\n";
    }

    void searchCompany() const
    {
        string company = readLine("Enter Company Name: ");
        bool found = false;

        for (JobNode *p = head; p != nullptr; p = p->next)
        {
            if (p->company == company)
            {
                printJob(p);
                found = true;
            }
        }

        if (!found)
            cout << "Company not found.\n";
    }

    void deleteJob()
    {
        int id = readInt("Enter Job ID to delete: ");

        JobNode *current = head;
        JobNode *previous = nullptr;

        while (current && current->id != id)
        {
            previous = current;
            current = current->next;
        }

        if (!current)
        {
            cout << "Job not found.\n";
            return;
        }

        if (!previous)
            head = current->next;
        else
            previous->next = current->next;

        if (current == tail)
            tail = previous;

        delete current;
        cout << "Job deleted successfully.\n";
    }

    void sortByPackage()
    {
        if (!head)
        {
            cout << "No jobs available to sort.\n";
            return;
        }

        vector<JobNode *> arr;

        for (JobNode *p = head; p; p = p->next)
            arr.push_back(p);

        quickSort(arr.data(), 0, static_cast<int>(arr.size()) - 1);

        head = arr[0];

        for (size_t i = 0; i + 1 < arr.size(); i++)
            arr[i]->next = arr[i + 1];

        arr.back()->next = nullptr;
        tail = arr.back();

        cout << "Jobs sorted by package.\n";
    }

    void saveToFile()
    {
        ofstream file("jobs.txt");

        if (!file)
        {
            cout << "Could not save jobs.\n";
            return;
        }

        for (JobNode *p = head; p; p = p->next)
        {
            file << p->id << ' ';
            saveString(file, p->company);
            file << ' ';
            saveString(file, p->role);
            file << ' ';
            saveString(file, p->branch);
            file << ' ' << p->packageLPA
                 << ' ' << p->minCGPA
                 << ' ' << p->maxBacklogs << '\n';
        }
    }

    void loadFromFile()
    {
        ifstream file("jobs.txt");
        if (!file)
            return;

        clear();

        JobNode p;

        while (file >> p.id
                    >> quoted(p.company)
                    >> quoted(p.role)
                    >> quoted(p.branch)
                    >> p.packageLPA
                    >> p.minCGPA
                    >> p.maxBacklogs)
        {
            if (findById(p.id))
                continue;

            JobNode *node = new JobNode(p);
            node->next = nullptr;

            if (!head)
                head = tail = node;
            else
            {
                tail->next = node;
                tail = node;
            }
        }
    }
};

// ============================================================
// MODULE 2: STUDENT MANAGEMENT
// Data structure: Hash Map
// ============================================================

struct Student
{
    string name;
    double cgpa;
    int backlogs;
    string company;
};

class StudentManager
{
private:
    unordered_map<int, Student> students;

public:
    Student *find(int roll)
    {
        auto it = students.find(roll);

        if (it == students.end())
            return nullptr;

        return &it->second;
    }

    void addStudent()
    {
        int roll = readInt("Enter Student Roll Number: ");

        Student s;
        s.name = readLine("Enter Student Name: ");
        s.cgpa = readDouble("Enter CGPA: ");
        s.backlogs = readInt("Enter Backlogs: ");
        s.company = "-";

        auto it = students.find(roll);

        if (it != students.end())
        {
            s.company = it->second.company;
            cout << "Student details updated.\n";
        }
        else
        {
            cout << "Student registered successfully.\n";
        }

        students[roll] = s;
    }

    void searchStudent()
    {
        int roll = readInt("Enter Roll Number: ");
        Student *s = find(roll);

        if (!s)
        {
            cout << "Student not found.\n";
            return;
        }

        cout << "\nName: " << s->name
             << "\nCGPA: " << s->cgpa
             << "\nBacklogs: " << s->backlogs
             << "\nPlaced Company: " << s->company << '\n';
    }

    void displayStudents() const
    {
        if (students.empty())
        {
            cout << "No students registered.\n";
            return;
        }

        for (const auto &entry : students)
        {
            cout << "\nRoll Number: " << entry.first
                 << "\nName: " << entry.second.name
                 << "\nCGPA: " << entry.second.cgpa
                 << "\nBacklogs: " << entry.second.backlogs
                 << "\nCompany: " << entry.second.company << '\n';
        }
    }

    bool deleteStudent(int roll)
    {
        return students.erase(roll) > 0;
    }

    void deleteStudentMenu()
    {
        int roll = readInt("Enter Roll Number to Delete: ");

        if (deleteStudent(roll))
            cout << "Student deleted.\n";
        else
            cout << "Student not found.\n";
    }

    void saveToFile()
    {
        ofstream file("students.txt");

        if (!file)
        {
            cout << "Could not save students.\n";
            return;
        }

        for (const auto &entry : students)
        {
            file << entry.first << ' ';
            saveString(file, entry.second.name);
            file << ' ' << entry.second.cgpa
                 << ' ' << entry.second.backlogs << ' ';
            saveString(file, entry.second.company);
            file << '\n';
        }
    }

    void loadFromFile()
    {
        ifstream file("students.txt");
        if (!file)
            return;

        students.clear();

        int roll;
        Student s;

        while (file >> roll
                    >> quoted(s.name)
                    >> s.cgpa
                    >> s.backlogs
                    >> quoted(s.company))
        {
            students[roll] = s;
        }
    }
};

// ============================================================
// MODULE 3: APPLICATION MANAGEMENT
// Data structures: Vector + Queue
// ============================================================

struct Application
{
    int studentId;
    int companyId;
    string status;
};

class ApplicationManager
{
private:
    vector<Application> applications;
    queue<int> pending;

    JobList &jobs;
    StudentManager &students;

    bool exists(int sid, int cid) const
    {
        for (const Application &a : applications)
        {
            if (a.studentId == sid && a.companyId == cid)
                return true;
        }

        return false;
    }

    void rebuildPending()
    {
        queue<int> empty;
        swap(pending, empty);

        for (size_t i = 0; i < applications.size(); i++)
        {
            if (applications[i].status == "Applied")
                pending.push(static_cast<int>(i));
        }
    }

public:
    ApplicationManager(JobList &j, StudentManager &s)
        : jobs(j), students(s) {}

    void apply()
    {
        int sid = readInt("Enter Student Roll Number: ");
        int cid = readInt("Enter Job ID: ");

        Student *student = students.find(sid);
        JobNode *job = jobs.findById(cid);

        if (!student)
        {
            cout << "Student not registered.\n";
            return;
        }

        if (!job)
        {
            cout << "Job not found.\n";
            return;
        }

        if (student->cgpa < job->minCGPA ||
            student->backlogs > job->maxBacklogs)
        {
            cout << "Student is not eligible for this job.\n";
            cout << "Required CGPA: " << job->minCGPA
                 << ", Maximum backlogs: " << job->maxBacklogs << '\n';
            return;
        }

        if (exists(sid, cid))
        {
            cout << "Student has already applied for this job.\n";
            return;
        }

        applications.push_back({sid, cid, "Applied"});
        pending.push(static_cast<int>(applications.size()) - 1);

        cout << "Application submitted successfully.\n";
    }

    void displayPending() const
    {
        queue<int> temp = pending;

        if (temp.empty())
        {
            cout << "No pending applications.\n";
            return;
        }

        while (!temp.empty())
        {
            const Application &a = applications[temp.front()];
            temp.pop();

            JobNode *job = jobs.findById(a.companyId);
            Student *student = students.find(a.studentId);

            cout << "\nStudent: "
                 << (student ? student->name : "Missing student")
                 << " (Roll " << a.studentId << ")"
                 << "\nCompany: "
                 << (job ? job->company : "Missing job")
                 << "\nJob ID: " << a.companyId
                 << "\nStatus: " << a.status << '\n';
        }
    }

    void processNext()
    {
        if (pending.empty())
        {
            cout << "No pending applications.\n";
            return;
        }

        int i = pending.front();
        pending.pop();

        applications[i].status = "UnderReview";

        cout << "Application moved to UnderReview.\n";
        displayApplication(applications[i]);
    }

    void displayApplication(const Application &a) const
    {
        Student *student = students.find(a.studentId);
        JobNode *job = jobs.findById(a.companyId);

        cout << "\nStudent ID: " << a.studentId
             << "\nStudent Name: "
             << (student ? student->name : "Missing student")
             << "\nJob ID: " << a.companyId
             << "\nCompany: "
             << (job ? job->company : "Missing job")
             << "\nRole: "
             << (job ? job->role : "Unavailable")
             << "\nPackage: ";

        if (job)
            cout << job->packageLPA << " LPA";
        else
            cout << "Unavailable";

        cout << "\nStatus: " << a.status << '\n';
    }

    void updateStatus()
    {
        int sid = readInt("Enter Student Roll Number: ");
        int cid = readInt("Enter Job ID: ");

        for (Application &a : applications)
        {
            if (a.studentId == sid && a.companyId == cid)
            {
                cout << "1. Shortlisted\n"
                     << "2. Selected\n"
                     << "3. Rejected\n";

                int choice = readInt("Choose new status: ");

                if (choice == 1)
                    a.status = "Shortlisted";
                else if (choice == 2)
                    a.status = "Selected";
                else if (choice == 3)
                    a.status = "Rejected";
                else
                {
                    cout << "Invalid choice.\n";
                    return;
                }

                // Update the student's placement information.
                Student *student = students.find(sid);
                JobNode *job = jobs.findById(cid);

                if (student && choice == 2 && job)
                    student->company = job->company;

                // Keep queue synchronized with application status.
                rebuildPending();

                cout << "Application status updated.\n";
                displayApplication(a);
                return;
            }
        }

        cout << "Application not found.\n";
    }

    void searchByStudent()
    {
        int sid = readInt("Enter Student Roll Number: ");
        bool found = false;

        for (const Application &a : applications)
        {
            if (a.studentId == sid)
            {
                displayApplication(a);
                found = true;
            }
        }

        if (!found)
            cout << "No applications found for this student.\n";
    }

    void searchByCompany()
    {
        string name = readLine("Enter Company Name: ");
        bool found = false;

        for (const Application &a : applications)
        {
            JobNode *job = jobs.findById(a.companyId);

            if (job && job->company == name)
            {
                displayApplication(a);
                found = true;
            }
        }

        if (!found)
            cout << "No applications found for this company.\n";
    }

    void displayAll() const
    {
        if (applications.empty())
        {
            cout << "No applications found.\n";
            return;
        }

        for (const Application &a : applications)
            displayApplication(a);
    }

    // Preserve application history if a student is deleted.
    void removePendingForStudent(int sid)
    {
        for (Application &a : applications)
        {
            if (a.studentId == sid &&
                (a.status == "Applied" || a.status == "UnderReview"))
            {
                a.status = "StudentRecordDeleted";
            }
        }

        rebuildPending();
    }

    void saveToFile()
    {
        ofstream file("applications.txt");

        if (!file)
        {
            cout << "Could not save applications.\n";
            return;
        }

        for (const Application &a : applications)
        {
            file << a.studentId << ' '
                 << a.companyId << ' ';
            saveString(file, a.status);
            file << '\n';
        }
    }

    void loadFromFile()
    {
        ifstream file("applications.txt");
        if (!file)
            return;

        applications.clear();

        Application a;

        while (file >> a.studentId
                    >> a.companyId
                    >> quoted(a.status))
        {
            applications.push_back(a);
        }

        rebuildPending();
    }
};

// ============================================================
// MODULE 4: PLACEMENT CELL RECORDS
// Data structure: Stack (LIFO)
// ============================================================

class RecordStack
{
private:
    vector<string> records;
    static const size_t MAX_RECORDS = 100;

public:
    void addRecord(const string &record)
    {
        if (records.size() >= MAX_RECORDS)
        {
            cout << "Placement history is full.\n";
            return;
        }

        records.push_back(record);
        cout << "Record added successfully.\n";
    }

    void deleteRecord()
    {
        if (records.empty())
        {
            cout << "No record to delete.\n";
            return;
        }

        cout << "Deleted: " << records.back() << '\n';
        records.pop_back();
    }

    void peekRecord() const
    {
        if (records.empty())
            cout << "No records available.\n";
        else
            cout << "Latest record: " << records.back() << '\n';
    }

    void searchRecord(const string &value) const
    {
        for (auto it = records.rbegin(); it != records.rend(); ++it)
        {
            if (*it == value)
            {
                cout << "Record found: " << *it << '\n';
                return;
            }
        }

        cout << "Record not found.\n";
    }

    void displayRecords() const
    {
        if (records.empty())
        {
            cout << "No placement records.\n";
            return;
        }

        cout << "\nPlacement History (Newest First):\n";

        for (auto it = records.rbegin(); it != records.rend(); ++it)
            cout << *it << '\n';
    }

    void saveToFile()
    {
        ofstream file("placement_records.txt");

        if (!file)
        {
            cout << "Could not save placement records.\n";
            return;
        }

        for (const string &record : records)
            file << quoted(record) << '\n';
    }

    void loadFromFile()
    {
        ifstream file("placement_records.txt");
        if (!file)
            return;

        records.clear();
        string record;

        while (file >> quoted(record))
        {
            if (records.size() < MAX_RECORDS)
                records.push_back(record);
        }
    }
};

// ============================================================
// MENUS
// ============================================================

void jobMenu(JobList &jobs)
{
    int choice;

    do
    {
        cout << "\n--- JOB MANAGEMENT ---\n"
             << "1. Add Job\n"
             << "2. Display Jobs\n"
             << "3. Search Job by ID\n"
             << "4. Search Jobs by Company\n"
             << "5. Delete Job\n"
             << "6. Sort by Package\n"
             << "7. Back\n";

        choice = readInt("Enter choice: ");

        switch (choice)
        {
        case 1: jobs.addJob(); break;
        case 2: jobs.displayJobs(); break;
        case 3: jobs.searchJob(); break;
        case 4: jobs.searchCompany(); break;
        case 5: jobs.deleteJob(); break;
        case 6: jobs.sortByPackage(); break;
        case 7: break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
}

void studentMenu(StudentManager &students,
                 ApplicationManager &applications)
{
    int choice;

    do
    {
        cout << "\n--- STUDENT MANAGEMENT ---\n"
             << "1. Add Student\n"
             << "2. Search Student\n"
             << "3. Display Students\n"
             << "4. Delete Student\n"
             << "5. Back\n";

        choice = readInt("Enter choice: ");

        switch (choice)
        {
        case 1:
            students.addStudent();
            break;

        case 2:
            students.searchStudent();
            break;

        case 3:
            students.displayStudents();
            break;

        case 4:
        {
            int roll = readInt("Enter Roll Number to Delete: ");

            if (students.find(roll))
            {
                applications.removePendingForStudent(roll);
                students.deleteStudent(roll);
                cout << "Student deleted; application history preserved.\n";
            }
            else
                cout << "Student not found.\n";

            break;
        }

        case 5:
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 5);
}

void applicationMenu(ApplicationManager &applications)
{
    int choice;

    do
    {
        cout << "\n--- APPLICATION MANAGEMENT ---\n"
             << "1. Apply for Placement\n"
             << "2. Display Pending Applications\n"
             << "3. Process Next Application\n"
             << "4. Update Application Status\n"
             << "5. Display All Applications\n"
             << "6. Search by Student\n"
             << "7. Search by Company\n"
             << "8. Back\n";

        choice = readInt("Enter choice: ");

        switch (choice)
        {
        case 1: applications.apply(); break;
        case 2: applications.displayPending(); break;
        case 3: applications.processNext(); break;
        case 4: applications.updateStatus(); break;
        case 5: applications.displayAll(); break;
        case 6: applications.searchByStudent(); break;
        case 7: applications.searchByCompany(); break;
        case 8: break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 8);
}

void recordMenu(RecordStack &records)
{
    int choice;

    do
    {
        cout << "\n--- PLACEMENT CELL RECORDS ---\n"
             << "1. Add Record\n"
             << "2. Delete Latest Record\n"
             << "3. Peek Latest Record\n"
             << "4. Search Record\n"
             << "5. Display Records\n"
             << "6. Back\n";

        choice = readInt("Enter choice: ");

        switch (choice)
        {
        case 1:
            records.addRecord(readLine("Enter Record: "));
            break;

        case 2:
            records.deleteRecord();
            break;

        case 3:
            records.peekRecord();
            break;

        case 4:
            records.searchRecord(readLine("Enter Record to Search: "));
            break;

        case 5:
            records.displayRecords();
            break;

        case 6:
            break;

        default:
            cout << "Invalid choice.\n";
        }
    } while (choice != 6);
}

// ============================================================
// MAIN
// ============================================================

int main()
{
    JobList jobs;
    StudentManager students;
    ApplicationManager applications(jobs, students);
    RecordStack records;

    // Load saved information when the program starts.
    jobs.loadFromFile();
    students.loadFromFile();
    applications.loadFromFile();
    records.loadFromFile();

    cout << "Placement Management System Started.\n";
    cout << "Saved data loaded successfully where available.\n";

    int choice;

    do
    {
        cout << "\n===== PLACEMENT MANAGEMENT SYSTEM =====\n"
             << "1. Job Management\n"
             << "2. Student Management\n"
             << "3. Application Management\n"
             << "4. Placement Cell Records\n"
             << "5. Save All Data\n"
             << "6. Exit\n";

        choice = readInt("Enter choice: ");

        switch (choice)
        {
        case 1:
            jobMenu(jobs);
            break;

        case 2:
            studentMenu(students, applications);
            break;

        case 3:
            applicationMenu(applications);
            break;

        case 4:
            recordMenu(records);
            break;

        case 5:
            jobs.saveToFile();
            students.saveToFile();
            applications.saveToFile();
            records.saveToFile();
            cout << "All data saved successfully.\n";
            break;

        case 6:
            jobs.saveToFile();
            students.saveToFile();
            applications.saveToFile();
            records.saveToFile();
            cout << "All data saved. Exiting program.\n";
            break;

        default:
            cout << "Invalid choice.\n";
        }

    } while (choice != 6);

    return 0;
}
