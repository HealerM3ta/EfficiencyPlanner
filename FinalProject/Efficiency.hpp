#ifndef EfficiencyRater
#define EfficiencyRater
#include <cmath>
#include <vector>
#include <string>
#include "heap.hpp"

double averageTimeTaken(const Heap& heap) { // Calculates the average time a task from the heap takes.
    int temp, average, n;
    for (const auto& Node : heap.getHeap()) { // Adds all the times together and devides by the number of nodes in the heap.
        temp = Node.getTimeTaken();
        n++;
        average += temp;
    }
    return average/n; // Returns the average.
}

double standardDeviation(const Heap& heap) { // Calculates the standard deviation of tasks in the heap.
    double average = averageTimeTaken(heap), n, sum = 0;
    for (const auto& Node : heap.getHeap()) { // Acts as the Sigma for the equation for standard deviation.
        sum += (Node.getTimeTaken() - average) * (Node.getTimeTaken() - average);
        n++;
    }
    return sqrt(sum / n); // Returns the standard deviation.
}

int percentImprovement(const Heap& heap, const std::string& name) { // Calcualtes the percent improvement of a specific task over time.
    double temp1 = 0, temp2 = 0, improvement = 0, n = 0;
    for (const auto& Node : heap.getHeap()) {
        if (Node.getTask() == name && temp1 == 0) { // Reassigns the value of temp 1 for the first occurance of a node with a given name.
            temp1 = Node.getTimeTaken();
        } else if (Node.getTask() == name) { // For every other occurance of a name, the function finds the percent improvement of that node and the prior node.
            temp2 = temp1;
            temp1 = Node.getTimeTaken();
            improvement += (temp2 - temp1) / temp2;
            n++;
        }
    }
    if (temp2 == 0) { // Exits the function if there is one or fewer nodes of a given name.
        std::cout << "Task \"" << name << "\" not found in the task list." << std::endl << std::endl;
        return INT_MAX;
    }
    return (improvement / n) * 100; // Returns the average of all the improvements over time.
}

//Clears cin
void clearInputBuffer() {
    std::cin.clear(); // Clear any error flags
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard remaining characters up to newline
}

void printSort() { // Standard print statement.
    std::cout << "\nHow do you want to view the tasks?" << std::endl;
    std::cout << "1. Alphabetical (A-Z)" << std::endl;
    std::cout << "2. Least time taken (ascending)" << std::endl;
    std::cout << "3. Most time taken (descending)" << std::endl;
    std::cout << "4. Date entered (ascending)" << std::endl;
    std::cout << "5. Date entered (descending)" << std::endl;
    std::cout << "6. Don't want to view" << std::endl;
    std::cout << "Enter your choice (1-6): ";
}

void printInitial() { // Standard print statement.
        std::cout << "Hello! What would you like to do today?"  << std::endl;
        std::cout << "1. Add to my tasks"<< std::endl;
        std::cout << "2. Delete Task" << std::endl;
        std::cout << "3. View my current session task list" << std::endl;
        std::cout << "4. View all my tasks" << std::endl;
        std::cout << "5. Calculate my progress" << std::endl;
        std::cout << "6. Delete History" << std::endl;
        std::cout << "7. Exit Planner" << std::endl;
        std::cout << "Enter your choice (1-7): ";
}

#endif