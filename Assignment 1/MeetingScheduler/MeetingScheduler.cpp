/*
Meeting Scheduler for Assignment 1 of DSA! Yay! 
Meeting scheuler will take in a list of meetings and return true 
if there is a conflict OR if there is a problem with the list of meetings. 

It will return false if the list contains good data and 
there are no conflicts. 
*/

#include <vector> 
#include <string>
#include <regex>    
#include <iostream>
#include <algorithm>


//First implement the straightforward algorithm that checks pairs of meetings.

// strategy for checking conflicts:
// 1. go through the list of meetings and check if it overlaps with any other meeting

bool MeetingConflictDetectorBruteForce(std::vector<std::string> meeting_list){
    // we can use regex to determine if the meeting times are formatted correctly 
    std::regex pattern(R"(^([01]\d|2[0-3])[0-5]\d-([01]\d|2[0-3])[0-5]\d$)");
    // the regex limits us to military time: 
    // [01]\d|2[0-3] -> allows for 0/1 and then any digit 0-9 OR a 2 and a digit 0-3
    // [0-5]\d -> allows for any digit 0-5 and then any digit 0-9
    // then we have a dash and repeat 

    std::smatch match; //declare a match variable 

    if (meeting_list.empty()) {
            return false;
        } //check if empty 

    // now we iterate through all of our meetings 
    for (int i = 0; i < meeting_list.size(); i++){

        // check to make sure each meeting is properly formatted 
        if (!std::regex_search(meeting_list[i], match, pattern)) {
            std::cout << "Invalid Entry! \n";
            return true; // return true if there's bad data 
        }

        // if data is good, compare with all other meetings 
        for (int j = 0; j < meeting_list.size(); j++){
            if (i == j){
                continue; //skip if its the same meeting 
            }
            else{
                // we can do the actual comparison very easily with substrings 
                // since we are using strings and military time. All we need to do 
                // is just check which substring is greater than the other
                std::string meeting1 = meeting_list[i];
                std::string meeting2 = meeting_list[j];
                std::string start1 = meeting1.substr(0, 4);
                std::string end1 = meeting1.substr(5, 4);
                std::string start2 = meeting2.substr(0, 4);
                std::string end2 = meeting2.substr(5, 4);
                if (start1 < end2 && start2 < end1){
                    return true; // and then return true if there is a conflict 
                }
            }
        }
    }
    return false;
}




// Develop a second algorithm that sorts the meetings in a useful way so you can check for conflicts more easily. 
// You should not implement your own search function, but instead use your language’s built-in sort function.

// strategy for checking conflicts:
// 1. sort the meetings by their start time.
// 2. go through the sorted list and check if the end time of a meeting is after than the start time of next meeting

// this follows much of the same logic so i'll use less comments here 
bool MeetingConflictDetectorSort(std::vector<std::string> meeting_list){
    // we follow the same regex matching strategy
    std::regex pattern(R"(^([01]\d|2[0-3])[0-5]\d-([01]\d|2[0-3])[0-5]\d$)");
    std::smatch match;

    //check if list is empty or has bad data and do the proper returns 
    if (meeting_list.empty()) {
        return false;
    }

    for (int i = 0; i < meeting_list.size(); i++){
        if (!std::regex_search(meeting_list[i], match, pattern)) {
            std::cout << "Invalid Entry! \n";
            return true;
        }
    }
   
    // then we use the built in sort function, sorting ALL the meetings
    // again, since we use military time and strings, we don't need to 
    // modify any of our entires before we sort 
    std::sort(meeting_list.begin(), meeting_list.end());

    // and then lastly, we just need to compare if the end of the last meeting is AFTER 
    // the start of the next one. ends up being very easy 
    for (int i = 0; i < meeting_list.size()-1; i++){
        std::string meeting1 = meeting_list[i];
        std::string meeting2 = meeting_list[i + 1];
        std::string end1 = meeting1.substr(5, 4);
        std::string start2 = meeting2.substr(0, 4);
        if (end1 > start2){
            return true;
        }
    }

    return false;
}



////////////////////////////////////////

/*
And then we have everyone's fav, the unit tests! 
*/

//we can have a simple function that simply prints out the results of our tests 
void RunTest(const std::string& test_name, const std::vector<std::string>& meetings, bool expected) {
    bool result_brute_force = MeetingConflictDetectorBruteForce(meetings);
    bool result_sort = MeetingConflictDetectorSort(meetings);

    std::cout << test_name << ":\n";
    std::cout << "  Brute Force Result: " << result_brute_force << "\n";
    std::cout << "  Sort Result: " << result_sort << "\n";
    std::cout << "  Expected Result: " << expected << "\n";
    std::cout << "---------" << ":\n";

}

int main() {
    std::cout << "=== Running Meeting Conflict Detector Tests ===\n\n";

    // Test cases
    // standard conflict and no conflict tests
    std::vector<std::string> meetings_conflict = {"0410-0415", "1030-1230", "1200-1300"};
    std::vector<std::string> meetings_no_conflict = {"0910-0915", "1030-1130", "1200-1300"};
    
    // standard conflict and no conflict tests but the times are unordered
    std::vector<std::string> meetings_conflict_unordered = {"1030-1230", "0410-0415", "1200-1300"};
    std::vector<std::string> meetings_no_conflict_unordered = {"1030-1130", "1200-1300", "0910-0915"};

    // special unit tests
    std::vector<std::string> meetings_back_to_back = {"0910-0915", "1030-1200", "1200-1300"}; 
    std::vector<std::string> meetings_invalid_time = {"0910-0915", "2530-1130", "1200-1300"};   
    std::vector<std::string> meetings_invalid_string = {"hello", "2530-1130", "1200-1300"};   
    std::vector<std::string> meetings_empty = {};

    // and then we can use our runtest function to test them all! yay! 
    RunTest("Conflict Test", meetings_conflict, true);
    RunTest("No Conflict Test", meetings_no_conflict, false);

    RunTest("Conflict Unordered Test", meetings_conflict_unordered, true);
    RunTest("No Conflict Unordered Test", meetings_no_conflict_unordered, false);

    RunTest("Empty List Test", meetings_empty, false);
    RunTest("Back-to-Back Meetings Test", meetings_back_to_back, false);
    RunTest("Invalid Time Format Test", meetings_invalid_time, true);
    RunTest("Invalid String Format Test", meetings_invalid_string, true);
}

// and all the unit tests pass! hurrah! 

