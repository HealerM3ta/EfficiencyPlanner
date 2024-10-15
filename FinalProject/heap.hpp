#ifndef HEAPANDNODE
#define HEAPANDNODE

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <sstream>

class Node { // Node that includes the Name of a task, it's time of occurance, and how long the task took to complete.
public:
    Node(std::string Name, int Time) : Task(Name), TimeTaken(Time) { getCurrentTime(); } // Creating a new Node.
    Node(std::string Name, int Time, std::string TimeOccur) : Task(Name), TimeTaken(Time), TimeOfEnter(TimeOccur) {} // Appending an existing node.

    void setTask(const std::string& TaskName) { Task = TaskName; }
    void setTimeTaken(int Minutes) { TimeTaken = Minutes; }

    std::string getTask() const { return Task; }
    int getTimeTaken() const { return TimeTaken; }
    std::string getTimeOfEnter() const { return TimeOfEnter; } // Exact time of entry down to the second.
    std::string getDateOfEnter() const { return DateOfEnter; } // Calendar date of entry.

    bool isEqual(const std::string& taskName, int timeTaken, const std::string& timeOfEnter) const { 
        return Task == taskName && TimeTaken == timeTaken && TimeOfEnter == timeOfEnter;
    }

private:
    std::string Task;
    int TimeTaken;
    std::string TimeOfEnter;
    std::string DateOfEnter;

    void getCurrentTime() { // Gets the local time using chrono and ctime.
        auto now = std::chrono::system_clock::now();

        std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

        std::tm* localTime = std::localtime(&currentTime);

        int year = localTime->tm_year + 1900;
        int month = localTime->tm_mon + 1;
        int day = localTime->tm_mday;
        int hour = localTime->tm_hour;
        int minute = localTime->tm_min; 
        int second = localTime->tm_sec;  

        std::string yearStr = std::to_string(year);
        std::string monthStr = std::to_string(month);
        std::string dayStr = std::to_string(day);
        std::string hourStr = std::to_string(hour);
        std::string minuteStr = std::to_string(minute);
        std::string secondStr = std::to_string(second);

        TimeOfEnter = monthStr + "/" + dayStr + "/" + yearStr + " " + hourStr + ":" + minuteStr + ":" + secondStr;
        DateOfEnter = monthStr + "/" + dayStr + "/" + yearStr;
    }
};

// Below functions are passed as arguments to sort the heap in a variety of ways.

bool TaskName(const Node& a, const Node& b) {
    return a.getTask() > b.getTask();
}

bool ReverseTaskName(const Node& a, const Node& b) {
    return a.getTask() < b.getTask();
}

bool TimeTakenAsc(const Node& a, const Node& b) {
    return a.getTimeTaken() < b.getTimeTaken();
}

bool TimeTakenDesc(const Node& a, const Node& b) {
    return a.getTimeTaken() > b.getTimeTaken();
}

bool DateCompletedAsc(const Node& a, const Node&b) {
    return a.getTimeOfEnter() > b.getTimeOfEnter();
}

bool DateCompletedDesc(const Node& a, const Node&b) {
    return a.getTimeOfEnter() < b.getTimeOfEnter();
}


class Heap { // A heap of nodes using a vector.
public:
    Heap() {}

    void insert(const std::string& TaskName, int TimeTaken) {
        heap.push_back(Node(TaskName, TimeTaken));
    }

    void insertHistory(const std::string& TaskName, int TimeTaken, const std::string& TimeOfEnter) {
        heap.push_back(Node(TaskName, TimeTaken, TimeOfEnter));
    }

    void sort(bool (*comp)(const Node&, const Node&)) { // Sorts a heap by calling heapify and passing a bolean function as an argument to determine the sort order.
        for (int i = heap.size() / 2 - 1; i >= 0; --i) {
            heapify(i, heap.size(), comp);
        }
        for (int i = heap.size() - 1; i >= 1; --i) {
            std::swap(heap[0], heap[i]);
            heapify(0, i, comp);
        }
    }

    const std::vector<Node>& getHeap() const { // Gets heap.
        return heap;
    }

    void printTasks() { // Print and formats the data in the heap.
        int i = 0;
        std::cout << std::right << std::setw(14) << "Task Name:" << std::setw(28) << "Time Taken (min):" << std::setw(21) << "TimeCompleted:" << std::endl;
        for (const auto& node : getHeap()) {
            i++;
            std::cout << std::right << std::setw(2) << i << std::left << ". " << std::setw(20) << node.getTask() << " " << std::setw(23) << node.getTimeTaken() << " " << node.getTimeOfEnter() << std::endl;
        }
    }

    void clearHistory(const char* filename) { // Clears the history.csv file.
        std::remove(filename);
    }

    void deleteNode(int nodeIndex) { // Deletes a Node of a given index.
        deleteLine("history.csv", nodeIndex);
        heap.erase(heap.begin() + nodeIndex);
    }

    void deleteLine(const std::string& filename, int lineNumber) { // Deletes a given line in the history.csv file.
        // Open the input file
        std::ifstream inputFile(filename);
        std::vector<std::string> lines;
        std::string line;
        int currentLine = 0;

        while (std::getline(inputFile, line)) {
            currentLine++;
            if (currentLine != lineNumber) {
                lines.push_back(line);
            }
        }
        inputFile.close();

        // Open the output file to write the modified content
        std::ofstream outputFile(filename);
        // Write the modified content back to the file
        for (const std::string& line : lines) {
            outputFile << line << std::endl;
        }
        outputFile.close();
    }

    bool inHistory(const std::string& taskName, int timeTaken, const std::string& timeOfEnter) {
        // Read historical records into a temporary heap
        Heap temp;
        temp.readHistory("history.csv", temp);

        // Check if the given task details are in the temporary heap
        for (const auto& node : temp.getHeap()) {
            if (node.isEqual(taskName, timeTaken, timeOfEnter)) {
                return true; // In History
            }
        }
        return false; // Not in History
    }

    void appendCSV(const std::string& filename) {
        std::ofstream file(filename, std::ios_base::app); // Open file in append mode
        if (!file.is_open()) {
            //std::cerr << "Error opening file for appending." << std::endl;
            return;
        }
        // Write data to the file
        for (const auto& node : heap) {
            if (inHistory(node.getTask(),node.getTimeTaken(), node.getTimeOfEnter())){
                continue;
            }
            file << node.getTask() << "," << node.getTimeTaken() << "," << node.getTimeOfEnter() << std::endl;
        }
        file.close();
    }

    void readHistory(const std::string& filename, Heap& heap) { // Reads the history.csv file and adds it to the heap.
        std::ifstream file(filename);
        if (!file.is_open()) {
        std::cerr << "Error opening file for reading." << std::endl;
        return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()){
                continue;
            }
            // Process the line
            std::istringstream tuMa(line);
            std::string taskName, TimeOfEnter;
            int timeTaken;

            if (std::getline(tuMa, taskName, ',') &&
                (tuMa >> timeTaken) &&
                (tuMa.ignore() && std::getline(tuMa, TimeOfEnter))) {
                heap.insertHistory(taskName, timeTaken, TimeOfEnter);
            } else {
                std::cerr << "Error reading line from file: " << line << std::endl;
            }
        }   
        file.close();
    }

private:
    std::vector<Node> heap; // Heap.

    void heapify(int index, int heapSize, bool (*comp)(const Node&, const Node&)) { // A heapify function that heapifies the heap.
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heapSize && comp(heap[left], heap[largest])) {
            largest = left;
        }

        if (right < heapSize && comp(heap[right], heap[largest])) {
            largest = right;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapify(largest, heapSize, comp);
        }
    }
};

#endif