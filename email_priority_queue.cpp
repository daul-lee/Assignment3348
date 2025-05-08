/*
EECS 348 Assignment 8
Program: AI-Generated code that sorts out emails in a priority queue
Input: Test file
Output: The queue
Collaborators: Myself, ChatGPT
Other sources of the code: ChatGPT
Full Name: Daul Paul Lee
Creation Date: 5/8/2025, 3:00 AM
*/

#include <iostream> //input/output stream
#include <vector> //vectors for
#include <string> //strings for all things string-related
#include <sstream> //enables us to read strings as streams
#include <algorithm> //allows us to sort
#include <iomanip> // provides manipulators
#include <map> //provides the map class
#include <ctime> //provides thigns to work with dates/times
#include <fstream> //A include the fstream library to make the file compileable

using namespace std; //we don't have to type out std::thing every time

int arrivalCounter = 0; //initialize a counter that will be used to put arrival priority

// Convert MM-DD-YYYY to time_t for easy comparison (ChatGPT comment)
time_t parseDate(const string& dateStr) { //initialize a time
    struct tm tm{}; //form a basic structure
    sscanf(dateStr.c_str(), "%d-%d-%d", &tm.tm_mon, &tm.tm_mday, &tm.tm_year); //make the time as a structure of month, day, year
    tm.tm_mon -= 1;           // struct tm months start from 0 (ChatGPT comment)
    tm.tm_year -= 1900;       // struct tm years are since 1900 (ChatGPT comment)
    return mktime(&tm); //return the pointer to the time
}

// Map sender category to priority (ChatGPT comment)
map<string, int> senderPriority = { //assigns priorities to the following strings
    {"Boss", 5}, //"Boss gets a 5" (most important )
    {"Subordinate", 4}, //subordinate gets a 4 (second-most)
    {"Peer", 3}, //peer gets a 3 (middle)
    {"ImportantPerson", 2}, //importantperson is a 2 (second-last)
    {"OtherPerson", 1} //everyone else is last
};

class Email { //email class (ChatGPT comment)
public: //public methods
    string senderCategory; //initialize a string for the category
    string subject; //initialize a string for the subject
    string dateStr; //initialize a string for the date
    time_t date; //initialize a time object
    int arrivalOrder; //initialize the arrival order

    Email(string sender, string subj, string date_string) { //email object initialized
        senderCategory = sender; //senderCategory string will be the sender here
        subject = subj; //subject string to subj
        dateStr = date_string; //dateStr becomes a date string
        date = parseDate(date_string); //date will be the parsed date_string
        arrivalOrder = arrivalCounter++; //arrival counter is incremented from the arrivalOrder
    }

    // Compare by priority (ChatGPT comment)
    bool operator>(const Email& other) const {//operator function
        if (senderPriority[senderCategory] != senderPriority[other.senderCategory]) //if the two senders aren't the same
            return senderPriority[senderCategory] > senderPriority[other.senderCategory];//check to see which one has higher priority
        if (date != other.date) // dates are not the same
            return date > other.date; // check which date is later
        return arrivalOrder < other.arrivalOrder; // newer (lower arrivalOrder) is higher priority (ChatGPT comment)
    }

    void display() const { //display the following
        cout << "Sender: " << senderCategory << endl; //display the sender and the senderCategory
        cout << "Subject: " << subject << endl; //display the subject and the subject
        cout << "Date: " << dateStr << endl; //display the date
    }
};

class MaxHeap { //maxheap class
private: //private methods
    vector<Email> heap; //initialize heap

    void heapifyUp(int idx) { //heapifyUp function
        while (idx > 0 && heap[idx] > heap[(idx - 1) / 2]) { //when the index is greater than 0 and the heap is greater than the parent
            swap(heap[idx], heap[(idx - 1) / 2]); //swap the two things
            idx = (idx - 1) / 2; //change the index and repeat
        }
    }

    void heapifyDown(int idx) { //heap down
        int size = heap.size(); //initialize size as being the size of the heap
        while (true) { //while the conditions are true
            int largest = idx; //largest integer is the index
            int left = 2 * idx + 1; //left child
            int right = 2 * idx + 2; //right child

            if (left < size && heap[left] > heap[largest]) largest = left; //if the left is larger than the parents, set parent to the left
            if (right < size && heap[right] > heap[largest]) largest = right; //if the right is larger than the parents, set parent to the right

            if (largest != idx) { //if largest is not the index
                swap(heap[idx], heap[largest]); //swap the two
                idx = largest; //index is the new parent
            } else { //otherwise
                break; //break
            }
        }
    }

public: //public methods
    void push(const Email& email) {  //push method
        heap.push_back(email); //push back the email
        heapifyUp(heap.size() - 1); //heapify it 
    }

    Email peek() { //peek method
        if (!heap.empty()) return heap[0]; //if the heap is empty
        throw runtime_error("No emails to read."); //run the error message "No emails to read"
    }

    void pop() { //pop method
        if (heap.empty()) return; //if the heap is empty, end the function
        heap[0] = heap.back(); //grab the index
        heap.pop_back(); //pop the index
        if (!heap.empty()) heapifyDown(0); //if the heap isn't empty, shift things around
    }

    int size() const { //size method
        return heap.size(); //return the size of the heap
    }

    bool empty() const { //empty method
        return heap.empty(); //check if the heap is empty or not
    }
};

class EmailManager { //email manager class
private: //private methods
    MaxHeap heap; //invokes the maxheap from earlier and creates such an object
    bool hasCurrent = false; //initializes a has current email line with false
    Email currentEmail = Email("", "", "01-01-2000"); //creates a blank email

public: //public methods
    void processLine(const string& line) { //processline function that takes a string as an input
        if (line.rfind("EMAIL ", 0) == 0) { //if the line starts with email
            string rest = line.substr(6); //create a substring rest to eliminate the first 6 characters
            stringstream ss(rest); //initializes a stringstream based on rest
            string sender, subject, date; //create sender, subject, date strings

            getline(ss, sender, ','); //get the sender line
            getline(ss, subject, ','); //get the subject line
            getline(ss, date, ','); //get the date line

            Email email(sender, subject, date); //initialize email
            heap.push(email); //push it into the stack
        } else if (line == "COUNT") { //if the line is count
            cout << "There are " << heap.size() << " emails to read.\n" << endl; //read how many emails there are
        } else if (line == "NEXT") { //if it's next
            if (!hasCurrent) { //if true
                if (!heap.empty()) { //if the heap isn't empty
                    currentEmail = heap.peek(); //peek at it
                    hasCurrent = true; //set hasCurrent to true
                }
            }
            if (hasCurrent) { //now that hascurrent is true
                cout << "Next email:" << endl; //display the phrase "next email"
                currentEmail.display(); //get the next email on screen
                cout << endl; //end it
            } else { //otherwise
                cout << "No emails to read.\n" << endl; //no emails to read
            }
        } else if (line == "READ") { //if it says read
            if (hasCurrent) { //hascurrent is true
                heap.pop(); //pop
                hasCurrent = false; //set hascurrent to false
            } else if (!heap.empty()) { //if the heap isn't empty
                heap.pop(); //pop the thing
            }
        }
    }
};

//runfromfile function will initialize and open the file and process the line if it is not empty
// Read from a file (ChatGPT comment)
void runFromFile(const string& filename) { //function name that takes in a filename
    ifstream infile(filename); //initialize file objected
    string line; //initialize string line
    EmailManager manager; //initialize emailmanager object

    while (getline(infile, line)) { //get and read the line
        if (!line.empty()) {//if the line is not empty
            manager.processLine(line); //the manager object created will process the line
        }
    }
}
 
//main function calls the runfromFile function
int main() { //main function
    // Test using provided test file (ChatGPT comment)
    // D runFromFile("test.txt");
    runFromFile("Assignment8_Test_File.txt"); //A changed the name of the file
    return 0; //finishes the thing
}
