#include <iostream>
#include <cstdio>  
#include <cstring>  
#include <cstdlib>  

using namespace std;

// --- Struktur Data untuk Tugas ---
struct Task {
    char description[256];
    int priority;
    Task* next;
    Task* prev;
};

// --- Pointer Global ---
Task* head = nullptr;
Task* tail = nullptr;
const char* FILENAME = "todolist_no_constructor.dat"; // Nama file baru untuk versi ini

// --- Implementasi Fungsi ---

Task* buatNode(const char* desc_param, int prio_param) {
    Task* newNode = new Task; 
    if (desc_param != nullptr) { 
        strncpy(newNode->description, desc_param, sizeof(newNode->description) - 1);
        newNode->description[sizeof(newNode->description) - 1] = '\0'; 
    } else {
        newNode->description[0] = '\0'; 
    }
    newNode->priority = prio_param;
    newNode->next = nullptr; 
    newNode->prev = nullptr; 

    return newNode;
}

void sisipNodeSesuaiPrioritas(const char* desc, int prio) {
    Task* newNode = buatNode(desc, prio); 

    if (head == nullptr) { 
        head = tail = newNode;
    } else if (prio < head->priority) {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    } else{
        Task* current = head;
        while (current != nullptr && prio >= current->priority) {
            current = current->next;
        }

        if (current == nullptr) { 
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        } else { 
            newNode->next = current;
            newNode->prev = current->prev;

            if (current->prev != nullptr) { 
                current->prev->next = newNode;
            } else{
                head = newNode;
            }
            current->prev = newNode;
        }
    }
}