/*
* Creston Getz
* 4/12/25
*
* This program will allow the user to load a csv file holding course data into a vector.
* The user can print a list of the courses and search for a specific course using a course number.
*/


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <limits>
#include <cctype>
using namespace std;


//Course class
class Course {
    public:
        std::string courseNumber;
        std::string title;
        std::vector<string> prerequisites;

        Course() {} //constructor

        //over ride < operator
        bool operator<(const Course& other) const {
            return this->courseNumber < other.courseNumber;
        }

};


//takes a string and splits(by coma) it into tokens
vector<string> getTokens(string &line) {
        std::vector<string> tokens;
        std::string token;
        std::istringstream ss(line); //treat the line string as a input stream called ss

        //splits the line by comas
        while (getline(ss, token, ',')) { 
            tokens.push_back(token);
        }   
        return tokens; // return a vector of string tokens 
}


// Takes csv file path and stores data in the vector data structure. Returns a vector full of course objects read from data
vector<Course> ReadFileAndStore(const string &fileName, vector<Course> &courses){

    std::string line; 

    std::ifstream file(fileName); //open file

    //check if file is open and return emtpy course vector if not
    if(!file.is_open()) {
        cerr << "Error: Cannot open file " << fileName << endl;
        return courses;
    }

    //If file is open read it line by line
    while (getline(file, line)) {
        
        std::vector<string> tokens = getTokens(line); //vector holding tokens for current line
        
        // if the line has less then two tokens output error. Each line should have a title and course number
        if (tokens.size() < 2) {
            cerr << "Error: Line incorrectly formatted" << endl;
            continue;
        }

        //create a new course object
        Course course;

        //Using tokens add data to course object
        course.courseNumber = tokens[0];
        course.title = tokens[1];

        //If there are more then 2 tokens there are prerequisites
        if (tokens.size() > 2) {
            //After course number and title the rest of the line is prerequisites
            //Start at index 2 and loop until the end of the line adding each token to course.prerequisites object vector
            for (size_t i = 2; i < tokens.size(); ++i) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        // add course object to coruses vector
        courses.push_back(course);
    }

    //close file
    file.close();

    return courses;
}

//print all courses in the vector
void printAll(const vector<Course> &courses) {
    for (const auto &course : courses) {
        cout << course.courseNumber << ", " << course.title;
        if (!course.prerequisites.empty()) {
            cout << ", ";
            for (const auto &prereq : course.prerequisites) {
                cout << prereq << ", ";
            }
        }
        cout << endl;
    }
}

//finds a course in the vector using a course number
void searchCourse(const vector<Course> &courses, std::string courseNumber) { 

    //loops over all couruses in vector looking for matching course number
    for (const auto &course : courses) {
        if (course.courseNumber == courseNumber) {
            cout << course.courseNumber << ", " << course.title << endl; //output course number and title

            //If the course has prerequisites
            if (!course.prerequisites.empty()) {
                cout << "Prerequisites: ";
                for (size_t i = 0; i < course.prerequisites.size(); ++i) { //loops the courses prerequisites
                    cout << course.prerequisites[i]; //print the prerequisite
                    if (i < course.prerequisites.size() - 1) { //if not the last prerequisite add a comma
                        cout << ", ";
                    } else {
                        cout << endl; //if the last prerequisite add a new line
                    }
                }
            } 
            std::cout << endl;
            return;
        }
    }
    cout << "Course Not found Try Another Course Number" << endl; //if the course is not found
    cout << "Enter course in caps" << endl; 
}

//sort and print the vector in alphanumeric order
void PrintVectorAlphanumeric(vector<Course> &courses) { 

    //sort the vector
    std::sort(courses.begin(), courses.end());
    
    cout << "Here is a sample schedule:" << endl << endl;
    //loop through vector and print sorted data
    for(const auto &course : courses) {
        cout << course.courseNumber << ", " << course.title << endl;
    }
    
} 

//Main function holding program loop and menu
int main() {
    std::string fileName;
    int userChoice = 0;
    std::string courseNumber; // string var for user to search for a course

    //fileName = "CS 300 ABCU_Advising_Program_Input.csv";

    //creates a course object vector
    std::vector<Course> courses;

    //create menu using case switch
    while (userChoice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data" << endl;
        cout << "  2. Display Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl << endl;
        cout << "What would you like to do? ";
        cin >> userChoice;

        // Validate input
        if (cin.fail()) {
            cin.clear(); // Clear error flag
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard input
            cout << "Please enter a number from the menu." << endl;
            continue; // Restart loop
        }

        switch(userChoice) { 

            //1. Load the file data into the data structure
            case 1:
                std::cout <<  "Enter file name: " << endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear the input buffer
                std::getline(cin, fileName); //get file name from user
                courses = ReadFileAndStore(fileName, courses);
                break;

            //2. Print an alphanumeric list of all the courses
            case 2:
                PrintVectorAlphanumeric(courses);
                break;

            //3. Print the course title and the prerequisites for any individual course
            case 3:
                //ask user for course number
                cout << "What course do you want to know about? ";
                cin >> courseNumber;
                searchCourse(courses, courseNumber);
                break;

            //9. Exit the program
            case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;

            //If the user enters invalid menu choice
            default:
                cout << userChoice << " is not a valid option." << endl;
        }
    }
    return 0;
}