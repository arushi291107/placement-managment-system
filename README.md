Placement Management System
A menu-driven Placement Management System developed in C++ using
Object-Oriented Programming and data structures. The program helps
manage job opportunities, student details, placement applications, and
placement-cell records through a console-based interface.
Features
1. Job Management
- Add a job with its ID, company name, role, eligible branch, package
  (LPA), minimum CGPA, and maximum allowed backlogs.
- Display all available jobs.
- Search for a job using its ID.
- Search for jobs by company name.
- Delete a job using its ID.
- Sort jobs by package using Quick Sort.
2. Student Management
- Register a student with roll number, name, CGPA, and backlog count.
- Update an existing student's details using the same roll number.
- Search for a student by roll number.
- Display all registered students.
- Delete a student record.
3. Application Management
- Submit a placement application by entering the student's roll number
  and job ID.
- Check whether the student is registered and the job exists.
- Check eligibility using the job's minimum CGPA and maximum backlog
  criteria.
- Prevent the same student from applying more than once for the same
  job.
- Display pending applications and process the next application.
- Update an application to Shortlisted, Selected, or Rejected.
- Search applications by student roll number or company name.
- Display all applications.
4. Placement Cell Records
- Add a placement-cell record.
- Delete the latest record.
- View the latest record.
- Search for a record.
- Display records from newest to oldest.
5. File Handling
The program saves data to text files so that it can be loaded the next
time the program starts.

  File                      Data stored
  jobs.txt                Job details
students.txt            Student details
applications.txt        Placement applications and statuses
placement_records.txt   Placement-cell records

Choose Save All Data from the main menu to save manually. The
program also saves all data when you exit through the Exit menu option.
Data Structures and Algorithms Used
  Module                  Data structure /        Purpose
algorithm               
  Job Management          Singly Linked List      Stores and manages job
records

  Job Management          Quick Sort              Sorts jobs by package

  Student Management      unordered_map (Hash   Stores and retrieves
Map)                    students using roll
number

  Application Management  vector                Stores placement
applications

  Application Management  Queue                   Processes pending
applications in FIFO
order
Placement Cell Records  Stack behaviour using   Adds and removes the
vector                latest record using
LIFO order
Requirements
- A C++ compiler that supports C++14 or later.
- A terminal or command prompt.
The program uses standard C++ libraries; no external libraries are
required.
How to Compile and Run
Using GCC / G++
1. Open a terminal in the folder containing the source file.
2. Compile the program:
   g++ -std=c++14 "placement_management_system(1).cpp" -o placement_management_system
3. Run it:
   Windows:
   placement_management_system.exe
   Linux / macOS:
   ./placement_management_system
If you rename the source file, replace its name in the compile command.
Main Menu
When the program starts, the main menu provides these options:
1. Job Management
2. Student Management
3. Application Management
4. Placement Cell Records
5. Save All Data
6. Exit
Choose an option by entering its corresponding number.
Notes and Limitations
- This is a console-based project; it does not use a graphical user
  interface or a database.
- The data files are created in the program's current working
  directory.
- Job eligibility currently checks minimum CGPA and maximum backlogs.
  Although the eligible branch is stored with each job, the current
  application eligibility check does not compare the student's branch.
- Pending applications are processed in first-in, first-out order.
- Placement-cell records follow last-in, first-out behaviour and are
  limited to 100 records.
Project Purpose
This project demonstrates the practical use of C++ classes, structures,
pointers, linked lists, hashing, vectors, queues, stack operations,
sorting, and file handling in a placement-manage
