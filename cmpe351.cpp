
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h> 

using namespace std;

struct Process {
  int id;
  int burstTime;
  int arrivalTime;
  int waitingTime;
  int turnAroundTime;
  int priority;
  int remainingTime;
  bool completed;
  Process *next;
};

void parseArguments(int argc, char *argv[], int &timeQuantum, string &inputFile,string &outputFile) {
  int option;
  while ((option = getopt(argc, argv, "t:f:o:")) != -1) {
    switch (option) {
    case 't':
      timeQuantum = atoi(optarg);
      break;
    case 'f':
      inputFile = optarg;
      break;
    case 'o':
      outputFile = optarg;
      break;
    default:
      cout << "Usage: " << argv[0]
           << " -t <timeQuantum> -f <inputFile> -o <outputFile>" << endl;
      exit(EXIT_FAILURE);
    }
  }
}

void readInputFile(const string &inputFile, Process *&head) {
  ifstream file(inputFile);
  if (!file.is_open()) {
    cout << "Error: Could not open the file " << inputFile << endl;
    exit(EXIT_FAILURE);
  }

  string line;
  Process *tail = nullptr;
  int id = 0;

  while (getline(file, line)) {
    size_t firstColon = line.find(':');
    size_t secondColon = line.rfind(':');

    if (firstColon == string::npos || secondColon == string::npos ||
        firstColon == secondColon) {
      cout << "Wrong format in input file. Ensure the format is "
              "'burstTime:arrivalTime:priority' for each process. Skipping "
              "line: "
           << line << endl;
      continue;
    }
    int burstTime = stoi(line.substr(0, firstColon));
    int arrivalTime =
        stoi(line.substr(firstColon + 1, secondColon - firstColon - 1));
    int priority = stoi(line.substr(secondColon + 1));

    Process *newProcess = new Process();
    newProcess->id = id++;
    newProcess->burstTime = burstTime;
    newProcess->arrivalTime = arrivalTime;
    newProcess->priority = priority;
    newProcess->waitingTime = 0;
    newProcess->turnAroundTime = 0;
    newProcess->next = nullptr;

    if (!head) {
      head = newProcess;
      tail = head;
    } else {
      tail->next = newProcess;
      tail = newProcess;
    }
  }

  file.close();
}
// Frees all dynamically allocated memory in the linked list and prevents memory leaks.
void freeProcesses(Process *&head) {
  while (head) {
    Process *temp = head;
    head = head->next;
    delete temp;
  }
}
void fcfs(Process *head, ofstream &outputFile) {
  if (!head) {
    cout<< "Error: No processes to schedule." << endl;
    return;
  }
  int currentTime = 0;
  int totalWaitingTime = 0;
  int processCount = 0; //Counts the number of processes.
  Process *current = head; //A pointer to iterate through the linked list.
  outputFile << "1";
  cout<<"1";
  while (current != nullptr) {
    current->waitingTime = max(0, currentTime - current->arrivalTime);
    current->turnAroundTime = current->waitingTime + current->burstTime;
    currentTime = max(currentTime, current->arrivalTime) + current->burstTime;
    totalWaitingTime += current->waitingTime;
    processCount++;
    cout<<":"<<current->waitingTime;
    outputFile <<":"<< current->waitingTime;
    current = current->next;
    
  }
// Static_cast to convert totalWaitingTime to float and ensuring precise floating point division.
  double avgWaitingTime = static_cast<double>(totalWaitingTime) / processCount;
  avgWaitingTime = static_cast<int>(avgWaitingTime * 10 + 0.5) / 10.0;// Round avgWaitingTime to one decimal place using math 
  outputFile << ":" << avgWaitingTime << endl;
  cout<<":"<<avgWaitingTime<<endl;
}

  void sjfNonPreemptive(Process *head, ofstream &outputFile)
{
    Process *SJF_Current = head;
    Process *SJF_Selected = nullptr;

    int current_Time = 0;
    int completed = 0;
    int totalWaitingTime = 0;
    int processCounter = 0;

    
    while (SJF_Current != nullptr)
    {
        processCounter++;
        SJF_Current = SJF_Current->next;
    }
    outputFile << "2";
    cout<<"2";

    while (completed < processCounter)
    {
        SJF_Current = head;//Pointer for traversing the process list.
        SJF_Selected = nullptr;// Pointer to the currently selected process with the shortest burst time.

        
        while (SJF_Current != nullptr)
        {
            if (SJF_Current->arrivalTime <= current_Time && SJF_Current->burstTime > 0)
            {
                if (SJF_Selected == nullptr || SJF_Current->burstTime < SJF_Selected->burstTime)
                {
                    SJF_Selected = SJF_Current;
                }
            }
            SJF_Current = SJF_Current->next;
        }

        if (SJF_Selected == nullptr)
        {
            current_Time++;
            continue;
        }
        SJF_Selected->waitingTime = current_Time - SJF_Selected->arrivalTime;
        current_Time = current_Time + SJF_Selected->burstTime;
        SJF_Selected->turnAroundTime = SJF_Selected->waitingTime + SJF_Selected->burstTime;
        totalWaitingTime += SJF_Selected->waitingTime;
        SJF_Selected->burstTime = 0;
        completed++;
    }
    SJF_Current = head;
    while (SJF_Current != nullptr)
    {
        outputFile << ":" << SJF_Current->waitingTime;
        cout<<":"<<SJF_Current->waitingTime;
        SJF_Current = SJF_Current->next;
    }

    double avgWaitingTime = (double)totalWaitingTime / processCounter;
    avgWaitingTime = static_cast<int>(avgWaitingTime * 10 + 0.5) / 10.0;// Round the avgWaitingTime to one decimal place
    outputFile << ":" << avgWaitingTime << endl;
    cout<<":"<<avgWaitingTime<<endl;
}

void sjfPreemptive(Process *head, ofstream &outputFile)
{
    int currentTime = 0;
    int total_waitingTime = 0;
    int processCounter = 0;
    int completed = 0;


    Process *current = head;
    while (current != nullptr)
    {

        processCounter++;
        current->remainingTime = current->burstTime;
        current = current->next;
    }
    while(completed<processCounter)
    {
         Process *shortest = nullptr;
        current = head;
        

        while (current != nullptr)
        {

            if (current->arrivalTime <= currentTime && current->remainingTime > 0)
            {
                if (shortest == nullptr || current->remainingTime < shortest->remainingTime)
                {
                    shortest = current;
                }
            }
            current = current->next;
        }

        if (shortest == nullptr)
        {

            currentTime++;
            continue;
        }

        shortest->remainingTime--;
        currentTime++;

        if(shortest->remainingTime == 0)
        {
            completed++;
            shortest->turnAroundTime = currentTime - shortest->arrivalTime;
            shortest->waitingTime = shortest->turnAroundTime - shortest->burstTime;
            total_waitingTime += shortest->waitingTime;
        }

    }

    outputFile << "3";
    cout << "3";

    current = head;
     while(current!=nullptr)
     {
        cout<<":"<<current->waitingTime;
        outputFile<<":"<<current->waitingTime;
        current = current->next;
     }
    double avgWaitingTime = (double)total_waitingTime / processCounter;
    avgWaitingTime = static_cast<int>(avgWaitingTime * 10 + 0.5) / 10.0;
    outputFile << ":" << avgWaitingTime << endl;
    cout << ":" << avgWaitingTime << endl;
}

void priorityNonPreemptive(Process *head, ofstream &outputFile) {
    if (!head) {
        outputFile << "4:0\n";
        return;
    }

    Process *current = head;
    Process *selected = nullptr;
    int currentTime = 0;
    int totalWaitingTime = 0;
    int completed = 0;
    int processCount = 0;
    while (current) {
        processCount++;
        current = current->next;
    }
    cout<<"4";
    outputFile << "4";
    current =head;

    while (completed <processCount) {
        current =head;
        selected =nullptr;
        while (current) {
            if (current->arrivalTime <= currentTime && !current->completed) {
                if (!selected || current->priority < selected->priority) {
                    selected = current;
                }
            }
            current = current->next;
        }

        if (!selected) {
            currentTime++;
            continue;
        }
        selected->waitingTime = currentTime - selected->arrivalTime;
        currentTime += selected->burstTime;
        selected->turnAroundTime = selected->waitingTime + selected->burstTime;
        selected->completed = true;
        totalWaitingTime += selected->waitingTime;
        completed++;
    }
    current = head;
    while (current) {
        outputFile << ":" << current->waitingTime;
        cout << ":" << current->waitingTime;
        current = current->next;
    }

    double avgWaitingTime = (double)totalWaitingTime / processCount;
    avgWaitingTime = static_cast<int>(avgWaitingTime * 10 + 0.5) / 10.0;
    outputFile << ":" << avgWaitingTime << "\n";
    cout << ":" << avgWaitingTime << endl;
}
void priorityPreemptive(Process *head, ofstream &outputFile) {
    if (!head) {
        outputFile << "5:0\n";
        return;
    }
    int currentTime =0;
    int totalWaitingTime =0;
    int completed = 0;
    int processCount =0;
    Process *current = head;
    while (current) {
        processCount++;
        current->remainingTime = current->burstTime;
        current = current->next;
    }
    cout << "5";
    outputFile << "5";
    current = head;
    while (completed < processCount) {
        Process *selected = nullptr;
        current = head;
        while (current) {
            if (current->arrivalTime <= currentTime && current->remainingTime > 0) {
                if (!selected || current->priority < selected->priority) {
                    selected = current;
                }
            }
            current = current->next;
        }

        if (!selected) {
            currentTime++;
            continue;
        }
        selected->remainingTime--;
        currentTime++;

        if (selected->remainingTime == 0) {
            completed++;
            selected->turnAroundTime = currentTime - selected->arrivalTime;
            selected->waitingTime = selected->turnAroundTime - selected->burstTime;
            totalWaitingTime += selected->waitingTime;
        }
    }
    current = head;
    while (current) {
        outputFile << ":" << current->waitingTime;
        cout << ":" << current->waitingTime;
        current = current->next;
    }
    double avgWaitingTime = (double)totalWaitingTime / processCount;
    avgWaitingTime = static_cast<int>(avgWaitingTime * 10 + 0.5) / 10.0;
    outputFile << ":" << avgWaitingTime << "\n";
    cout << ":" << avgWaitingTime << endl;
}
void roundRobin(Process *head, int timeQuantum, ofstream &outputFile) {
    if (!head || timeQuantum <= 0) {
        outputFile << "6:0\n";
        return;
    }
    int currentTime = 0;
    int totalWaitingTime = 0;
    int processCount = 0;
    int completed = 0;
    Process *current = head;
    while (current) {
        processCount++;
        current->remainingTime = current->burstTime;
        current = current->next;
    }
    cout<<"6";
    outputFile << "6";
    current = head;

    while (completed < processCount) {
        bool processExecuted = false;
        Process *round = current;

        while (round) {
            if (round->arrivalTime <= currentTime && round->remainingTime > 0) {
                processExecuted = true;
                int executeTime = min(timeQuantum, round->remainingTime);
                currentTime += executeTime;
                round->remainingTime -= executeTime;

                if (round->remainingTime == 0) {
                    completed++;
                    round->turnAroundTime = currentTime - round->arrivalTime;
                    round->waitingTime = round->turnAroundTime - round->burstTime;
                    totalWaitingTime += round->waitingTime;
                }
            }
            round = round->next;
        }

        if (!processExecuted) {
            currentTime++;
        }
    }
    current = head;
    while (current) {
        outputFile << ":" << current->waitingTime;
        cout << ":" << current->waitingTime;
        current = current->next;
    }
    double avgWaitingTime = static_cast<double>(totalWaitingTime) / processCount;
    avgWaitingTime = static_cast<int>(avgWaitingTime * 10 + 0.5) / 10.0;
    outputFile << ":" << avgWaitingTime << "\n";
    cout << ":" << avgWaitingTime << endl;
}

int main(int argc, char *argv[]) {
  int timeQuantum = 0;
  string inputFile, outputFile;
  
  parseArguments(argc, argv, timeQuantum, inputFile, outputFile);
  Process *head = nullptr;
  readInputFile(inputFile, head);
  ofstream output(outputFile);
  if (!output.is_open()) {
    cout << "Error: Could not open the output file " << outputFile << endl;
    return EXIT_FAILURE;
  }

  fcfs(head, output);
  freeProcesses(head);

  Process *head1 = nullptr;
  readInputFile(inputFile, head1);
  sjfNonPreemptive(head1, output);
  freeProcesses(head1);

  Process *head2 = nullptr;
  readInputFile(inputFile, head2);
  sjfPreemptive(head2, output);
  freeProcesses(head2);

  Process *head3 = nullptr;
  readInputFile(inputFile, head3);
  priorityNonPreemptive(head3, output);
  freeProcesses(head3);

  Process *head4 = nullptr;
  readInputFile(inputFile, head4);
  priorityPreemptive(head4, output);
  freeProcesses(head4);

  Process *head5 = nullptr;
  readInputFile(inputFile, head5);
  roundRobin(head5, timeQuantum, output);
  freeProcesses(head5);

  output.close();
  return 0;
}
