#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include<algorithm>
#include <queue>


using namespace std;

// Struct to store all necessary information for each process
struct Process {
    string identifier;
    int arrival_time;
    int service_time;
    int wait_time = 0;
    float response_ratio = 0;
    int level = 0;
};


vector<Process> vec; // Vector to hold the main copy of the input
int max_time = 0; // Variable to hold how much time will be needed to complete all services for fault tolerance


// ---- First Come First Serve ----
void fcfs(){
    vector<Process> fcfs_vec = vec; // Copy vector for editing
    
    ofstream fcfs_ptr("fcfs.out"); // Open output file
    if (!fcfs_ptr.is_open()){
        cerr << "Error opening file: " << endl;
    }
    
    
    for (auto& process : fcfs_vec) { // Iterate through vector and output until all completed (pre-sorted in main)
        fcfs_ptr << process.identifier << "\n";
        while (process.service_time > 10){
            fcfs_ptr << process.identifier << "\n";
            process.service_time = process.service_time - 10;
        }
    }
}

// ---- Round Robin (q=10) ----
void rrTen (){
    vector<Process> rr_vec = vec;
    queue<Process> rrque; // Queue for determining which is next
    
    ofstream rrTen_ptr("rr_10.out"); // Open output file
    if (!rrTen_ptr.is_open()){
        cerr << "Error opening file: " << endl;
    }
    
    int curr_time =  0;
    int quantum = 0; // variable to store how many times each process has run
    
    for (auto& process : rr_vec) { // Initialize queue for time=0
        if (process.arrival_time == curr_time){
            rrque.push(process);
        }
    }
    curr_time = curr_time + 10;
    
    while (!rrque.empty()){ 
        for (auto& process : rr_vec) { // Add all process arriving at current time to queue
            if (process.arrival_time == curr_time){
                rrque.push(process);
            }
        }
        
        if (quantum < 10){ // If current process has ran less than maximum quantum (10), output to file and decrease service time
            rrTen_ptr << rrque.front().identifier << "\n"; 
            rrque.front().service_time = rrque.front().service_time - 10;
            quantum = quantum + 10;
            
            if (rrque.front().service_time <= 0){ // If process complete, pop from queue
                rrque.pop();
                quantum = 0;
            }
            else if (quantum == 10){ // If process not complete in max quantum, pop and push to back
                Process temp = rrque.front();
                rrque.pop();
                rrque.push(temp);
                quantum = 0;
            }
            curr_time = curr_time + 10;
        }
    }
}

// ---- Round Robin (q=40) ---- 
void rrForty (){ // * see rrTen for comments
    vector<Process> rr_vec = vec;
    queue<Process> rrque;
    
    ofstream rrForty_ptr("rr_40.out");
    if (!rrForty_ptr.is_open()){
        cerr << "Error opening file: " << endl;
    }
    
    int curr_time =  0;
    int quantum = 0;
    
    for (auto& process : rr_vec) {
        if (process.arrival_time == curr_time){
            rrque.push(process);
        }
    }
    curr_time = curr_time + 10;
    
    while (curr_time <= max_time){
        for (auto& process : rr_vec) {
            if (process.arrival_time == curr_time){
                rrque.push(process);
            }
        }
        
        if (quantum < 40){ // If current process has ran less than maximum quantum (40), output to file and decrease service time
            rrForty_ptr << rrque.front().identifier << "\n"; 
            rrque.front().service_time = rrque.front().service_time - 10;
            quantum = quantum + 10;
            
            if (rrque.front().service_time <= 0){
    
                rrque.pop();
                quantum = 0;
            }
            curr_time = curr_time + 10; // only increase current time if quantum is increasing to prevent double add
        }
        else {
            Process temp = rrque.front();
            rrque.pop();
            rrque.push(temp);
            quantum = 0;
        }
    }
}

// ---- Shortest Process Next ----
void spn (){
    vector<Process> spn_vec = vec;
    vector<Process> spnque; // Queue for determining which is next (vector type for easy resorting)

    ofstream spn_ptr("spn.out");
    if (!spn_ptr.is_open()) {
        cerr << "Error opening file: " << endl;
        return;
    }

    int curr_time = 0;  
    for (auto& process : spn_vec) {
        if (process.arrival_time == curr_time) {
            spnque.push_back(process);
        }
    }
    curr_time = curr_time + 10;
    sort(spnque.begin(), spnque.end(), [](const Process& lhs, const Process& rhs) { return lhs.service_time < rhs.service_time; }); // sort initial queue for lowest service time

    while (!spnque.empty()) {
        for (auto& process : spn_vec) { // Add all process arriving at current time to queue
            if (process.arrival_time == curr_time) {
                spnque.push_back(process);
            }
        }

        if (spnque.begin()->service_time > 0) { // If the current front of the queue has not finished, write to file and decrease service time
            spn_ptr << spnque.begin()->identifier << "\n";
            spnque.begin()->service_time = spnque.begin()->service_time - 10;
            curr_time = curr_time + 10;
        } 
        else { // if current front of the queue is finsihed, sort queue based on service time and pop the front
            sort(spnque.begin(), spnque.end(), [](const Process& lhs, const Process& rhs) { return lhs.service_time < rhs.service_time; });
            spnque.erase(spnque.begin());
        }
    }
}

// ---- Shortest Remaining Time ----
void srt (){

    vector<Process> srt_vec = vec; 
    vector<Process> srtque; 

    ofstream srt_ptr("srt.out");
    if (!srt_ptr.is_open()) {
        cerr << "Error opening file: " << endl;
        return;
    }

    int curr_time = 0;
    for (auto& process : srt_vec) {// Initialize queue for time 0
        if (process.arrival_time == curr_time) {
            srtque.push_back(process);
        }
    }
    curr_time = curr_time + 10;
    sort(srtque.begin(), srtque.end(), [](const Process& lhs, const Process& rhs) { return lhs.service_time < rhs.service_time; });

    while (!srtque.empty()) {
        for (auto& process : srt_vec) { // Add all process arriving at current time to queue
            if (process.arrival_time == curr_time) {
                srtque.push_back(process);
            }
        }
        
        srt_ptr << srtque.begin()->identifier << "\n"; // Output front process and decrease service time
        srtque.begin()->service_time = srtque.begin()->service_time - 10;
        sort(srtque.begin(), srtque.end(), [](const Process& lhs, const Process& rhs) { return lhs.service_time < rhs.service_time; }); // Resort entire queue
        curr_time = curr_time + 10;

        if (srtque.begin()->service_time == 0) { // Erase front if service time == 0
            srtque.erase(srtque.begin());
        }
    }
}

// ---- Highest Response Ratio Next ----
void hrrn() {
    vector<Process> hrrn_vec = vec;
    vector<Process> hrrnque;

    ofstream hrrn_ptr("hrrn.out");
    if (!hrrn_ptr.is_open()) {
        cerr << "Error opening file" << endl;
        return;
    }

    int curr_time = 0;
    
    for (auto& process : hrrn_vec) {
        if (process.arrival_time == curr_time) {
            process.wait_time = process.wait_time + 10;
            hrrnque.push_back(process);
        }
    }
    curr_time = curr_time + 10;

    while (!hrrnque.empty()) {
        for (auto& process : hrrn_vec) {
            if (process.arrival_time == curr_time) {
                hrrnque.push_back(process);
            }
        }

        if (hrrnque.begin()->service_time > 0) { // If serveice time is not 0, output and decrease service time
            hrrn_ptr << hrrnque.begin()->identifier << "\n";
            hrrnque.begin()->service_time = hrrnque.begin()->service_time - 10;
            
            for (auto& process : hrrnque) { 
                if (process.identifier != hrrnque.begin()->identifier) { // For every item in queue except the front, increase service time
                    process.wait_time = process.wait_time + 10;
                    if (process.service_time != 0 ){ // Prevention of dividing by 0
                        process.response_ratio = (process.service_time + process.wait_time) / process.service_time; // Calculate new response ratio
                    }
                }
            }
            curr_time = curr_time + 10;
        } 
        else { // If service time is 0, erase and sort by response ratio
            hrrnque.erase(hrrnque.begin());
            sort(hrrnque.begin(), hrrnque.end(), [](const Process& lhs, const Process& rhs) { return lhs.response_ratio > rhs.response_ratio; });
        }
    }
}

// ---- Feedack ----
void feedback (){
    vector<Process> feedback_vec = vec;
    vector<Process> feedbackque;

    ofstream feedback_ptr("feedback.out");
    if (!feedback_ptr.is_open()) {
        cerr << "Error opening file: " << endl;
        return;
    }

    int curr_time = 0;
    
    for (auto& process : feedback_vec) {
        if (process.arrival_time == curr_time) {
            feedbackque.push_back(process);
        }
    }
    curr_time = curr_time + 10;

    while (!feedbackque.empty()) {
        for (auto& process : feedback_vec) {
            if (process.arrival_time == curr_time) {
                feedbackque.push_back(process);
            }
        }
        
        feedback_ptr << feedbackque.begin()->identifier << "\n"; // Output front of queue to file and decrease service time by 10
        feedbackque.begin()->service_time = feedbackque.begin()->service_time - 10;
        curr_time = curr_time + 10;
        

        if (feedbackque.begin()->service_time > 0) { // If current process did not finish, increase queue level
            feedbackque.begin()->level = feedbackque.begin()->level + 1;
            Process temp = *feedbackque.begin(); // pop and repush into queue for proper sorting order for items arrived at the same time
            feedbackque.erase(feedbackque.begin());
            feedbackque.push_back(temp);
        } 
        else { // If current process completed, pop from queue
            feedbackque.erase(feedbackque.begin());
        }
        sort(feedbackque.begin(), feedbackque.end(), [](const Process& lhs, const Process& rhs) { return lhs.arrival_time < rhs.arrival_time; }); // sort by arrival time
        sort(feedbackque.begin(), feedbackque.end(), [](const Process& lhs, const Process& rhs) { return lhs.level < rhs.level; });// sort by queue level
    }
}


int main(int argc, char* argv[]){
    
    ifstream inp_ptr(argv[1]); // Get input file
    if (!inp_ptr.is_open()){
        cerr << "Error opening file: " << argv[1] << endl;
        return 1;
    }
    
    
    string line;
    while (getline(inp_ptr, line)){ // Read file line by line
        stringstream line_string(line);
        string word;
        int word_count = 0;
        Process curr_process;
        while (line_string >> word) { // Split line into words to create process
            if (word_count == 0){ // First word identifier
                curr_process.identifier = word;
            }
            else if (word_count == 1) { // Second word arrivial time
                curr_process.arrival_time = stoi(word);
            }
            else if (word_count == 2) { // Third word service time
                curr_process.service_time = stoi(word);
            }
            word_count ++;
            
        }
        vec.push_back(curr_process); // Push process into vector
    }
    
    sort(vec.begin(), vec.end(), [](const Process& lhs, const Process& rhs) {return lhs.arrival_time < rhs.arrival_time;}); // Sort vector using lambda expression (change to function?)
    
    for (const auto& process : vec) { // Calculate how much time is required to complete entire service
        max_time = max_time + process.service_time;
    }
    
    
    fcfs();
    rrTen();
    rrForty();
    spn();
    srt();
    hrrn();
    feedback();
    
    return 0;
}