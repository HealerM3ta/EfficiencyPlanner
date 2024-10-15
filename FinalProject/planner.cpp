#include <iostream>
#include <fstream>
#include <climits>
#include "heap.hpp"
#include "Efficiency.hpp"


int main() {
    Heap heap;
    int cont = 1;
    int initial_choice = 0;
    
    while (cont) { // lets the user pick what they want to do. This prompt loops until the user wants to leave.
        std::cout << "Hello! What would you like to do today?"  << std::endl;
        std::cout << "1. Add to my tasks"<< std::endl;
        std::cout << "2. Delete Task" << std::endl;
        std::cout << "3. View my current session task list" << std::endl;
        std::cout << "4. View all my tasks" << std::endl;
        std::cout << "5. Calculate my progress" << std::endl;
        std::cout << "6. Delete History" << std::endl;
        std::cout << "7. Exit Planner" << std::endl;
        std::cout << "Enter your choice (1-7): ";
        
        std::cin >> initial_choice; // holds the inital choice of what the user wants to do in the planner.
        std::cin.ignore(INT_MAX, '\n'); //ignores the \n from the previous prompt
        
        if (initial_choice == 1) { // adds new tasks to the planner.
            int more;
            do {
                std::string name;
                int timeTaken;
                std::cout << "Enter Task Name (Max 20 characters): ";
                getline(std::cin, name);
                
                //check if name is valid(within character limit)
                while(name.size() > 20) {
                    clearInputBuffer();
                    std::cout<<"Max character length exceeded!\nPlease keep task name within 20 characters.\nRe-enter your taks name: ";
                    getline(std::cin, name);
                }
                
                std::cout << "Enter Time Taken for Task (min): ";
                
                //check is time taken is valid(is not a word or exceeds the maximum minutes in a day)
                while(!(std::cin >> timeTaken) || timeTaken < 0 || timeTaken > 1440) {
                    clearInputBuffer();
                    std::cout << "Invalid Entry.\nEither is not a number, or exceeds max time (1440min).\nRe-enter the time taken:";
                }
                
                heap.insert(name, timeTaken); //stores task in the heap
                //task will be saves to a csv file at the end of the session.

                name.clear(); //clears variables for next iteration(s)
                timeTaken = 0;
                clearInputBuffer();
                
                std::cout << "Do you want to add more tasks? \n('1' for Yes and '0' for No): ";
                std::cin >> more;
                while (std::cin.fail() || (more != 1 && more != 0)){
                    clearInputBuffer();
                    std::cout << "Invalid Input, please enter '1' or '0'.\nDo you want to add more tasks? \n('1' for Yes and '0' for No): " << std::endl;
                    std::cin >> more;
                }
                clearInputBuffer();
                heap.appendCSV("history.csv");
            }
            while (more);
            continue;
        }
        
        if (initial_choice == 2) { // allows the user to delete a task.
            int chosenNode;
            Heap all;
            all.readHistory("history.csv", all);
            all.printTasks();
            std::cout << "Which task do you want to delete: ";
            std::cin >> chosenNode;
            all.deleteNode(chosenNode);
            std::cout << std::endl;
            continue;
        }
        
        if (initial_choice == 3) { // options for the user to choose how they want to view their current session tasks.
            int choice;
            printSort(); // promts the user to with differnt task viewing options
            std::cin >> choice;
            
            while(std::cin.fail() || choice < 1 || choice > 6){ // validates input
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                std::cout << "Invalid Input, Enter your choice of how you want to view your tasks (1-6):  " << std::endl;
                std::cin >> choice;
            }
            
            if (choice == 6) { //when user chooses to not view
                std::cout << "Stay productive. Goodbye." << std::endl;
                continue;
            }
            
            switch (choice) {// different heap sorts for the task list 
                case 1:
                    heap.sort(TaskName);
                    break;
                case 2:
                    heap.sort(TimeTakenAsc);
                    break;
                case 3:
                    heap.sort(TimeTakenDesc);
                    break;
                case 4:
                    heap.sort(DateCompletedAsc);
                    break;
                case 5:
                    heap.sort(DateCompletedDesc);
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
                    continue;
            }
            
           std:: cout << "\nSorted Tasks:\n";
            heap.printTasks(); // prints the list of tasks stored in the heap.
            std::cout<<"\n\nThe tasks have been printed!"<< std::endl;
            std::cout<<"Returning you to the main menu......\n"<< std::endl;
            heap.appendCSV("history.csv");
            continue;
        }
        
        if (initial_choice == 4) { // options for the user to choose how they want to view all their current and previous tasks.
            int choice;
            Heap all;
            printSort();
            std::cin >> choice;
            
            while(std::cin.fail() || choice < 1 || choice > 6){
                std::cin.clear();
                std::cin.ignore(INT_MAX, '\n');
                std::cout << "Invalid Input, Enter your choice of how you want to view your tasks (1-6):  " << std::endl;
                std::cin >> choice;
            }
            
            heap.appendCSV("history.csv"); //adds the current session to the history
            all.readHistory("history.csv", all); // adds the entire history to a heap to itterate through
            
            if (choice == 6) {
                std::cout << "Stay productive. Goodbye." <<std:: endl;
                continue;
            }
            
            switch (choice) {
                case 1:
                    all.sort(TaskName);
                    break;
                case 2:
                    all.sort(TimeTakenAsc);
                    break;
                case 3:
                    all.sort(TimeTakenDesc);
                    break;
                 case 4:
                    all.sort(DateCompletedAsc);
                    break;
                case 5:
                    all.sort(DateCompletedDesc);
                    break;
                default:
                    std::cout << "Invalid choice!" << std::endl;
                    continue;
            }
            
            std::cout << "\nSorted Tasks:\n";
            all.printTasks(); // prints the list of tasks stored in the heap.
            
            std::cout<<"\n\nThe tasks have been printed!"<< std::endl;
            std::cout<<"Returning you to the main menu......\n"<< std::endl;
            continue;
        }
        
        if (initial_choice == 5) { //calculate user progress using average time spent on a task, standard deviaiton and percent improvement.
            int choice;
            heap.readHistory("history.csv", heap);
            std::cout << "\nHow do you want to view the tasks?" << std::endl;
            std::cout << "1. Average Time" << std::endl;
            std::cout << "2. Standard Deviation" << std::endl;
            std::cout << "3. Percent improvement" << std::endl;
            std::cout << "4. Return to menu" << std::endl;
            std::cout << "Enter your choice (1-4): ";
            std::cin >> choice;
            std::cout << std::endl;


            bool redo = true;
            switch(choice) {
                case 1:
                    std::cout << "Average time to complete tasks: " << averageTimeTaken(heap) << std::endl << std::endl;
                    break;
                case 2:
                    std::cout << "Standard deviation: " << standardDeviation(heap) << std::endl << std::endl;
                    break;
                case 3:
                    clearInputBuffer();
                    while (redo) {
                        int improvement = 0;
                        std::string line;
                        std::cout << "Name of task: ";
                        getline(std::cin, line);
                        heap.sort(DateCompletedAsc);
                        improvement = percentImprovement(heap, line);
                        if (improvement == INT_MAX) {
                            continue;
                        }
                        std::cout << "Percent improvement: " << improvement << "%" << std::endl << std::endl;
                        redo = false;
                    }
                    break;
                default:
                    continue;
            }
            continue;
        }
        
        if (initial_choice == 6) { // clears the history of the user.
            heap.clearHistory("history.csv");
            std::cout << "History Cleared" << std::endl << std::endl;
        }
        else {// exits the planner after saving the session tasks to the history.
            heap.appendCSV("history.csv");
            return 0;
        }
    }
}