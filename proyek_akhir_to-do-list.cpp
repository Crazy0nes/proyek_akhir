#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

// --- Struktur Data untuk Tugas ---
struct Task
{
    char description[256];
    int priority;
    Task *next;
    Task *prev;
};

void bersihkanList();

// --- Pointer Global ---
Task *head = nullptr;
Task *tail = nullptr;
const char *FILENAME = "to_d_olist.dat"; // Nama file 

// --- Implementasi Fungsi ---

Task *buatNode(const char *desc_param, int prio_param)
{
    Task *newNode = new Task;
    if (desc_param != nullptr)
    {
        strncpy(newNode->description, desc_param, sizeof(newNode->description) - 1);
        newNode->description[sizeof(newNode->description) - 1] = '\0';
    }
    else
    {
        newNode->description[0] = '\0';
    }
    newNode->priority = prio_param;
    newNode->next = nullptr;
    newNode->prev = nullptr;

    return newNode;
}

void sisipNodeSesuaiPrioritas(const char *desc, int prio)
{
    Task *newNode = buatNode(desc, prio);

    if (head == nullptr)
    {
        head = tail = newNode;
    }
    else if (prio < head->priority)
    {
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    }
    else
    {
        Task *current = head;
        while (current != nullptr && prio >= current->priority)
        {
            current = current->next;
        }

        if (current == nullptr)
        {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        else
        {
            newNode->next = current;
            newNode->prev = current->prev;

            if (current->prev != nullptr)
            {
                current->prev->next = newNode;
            }
            else
            {
                head = newNode;
            }
            current->prev = newNode;
        }
    }
}

void tampilkanTugas()
{
    if (head == nullptr)
    {
        cout << "Tidak ada tugas dalam daftar.\n";
        return;
    }
    cout << "\n--- Daftar Tugas (Terurut Berdasarkan Prioritas) ---\n";
    Task *current = head;
    int count = 1;
    while (current != nullptr)
    {
        cout << count << ". Deskripsi: " << current->description
             << " (Prioritas: " << current->priority << ")\n";
        current = current->next;
        count++;
    }
    cout << "--------------------------------------------------\n";
}

void cariTugas()
{
    if (head == nullptr)
    {
        cout << "Daftar tugas kosong. Tidak ada yang bisa dicari.\n";
        return;
    }
    char keyword[100];
    cout << "Masukkan kata kunci pencarian: ";
    cin.ignore(1024, '\n');
    cin.getline(keyword, sizeof(keyword));

    Task *current = head;
    bool found = false;
    cout << "\n--- Hasil Pencarian untuk \"" << keyword << "\" ---\n";
    int count = 1;
    while (current != nullptr)
    {
        if (strstr(current->description, keyword) != nullptr)
        {
            cout << count << ". Deskripsi: " << current->description
                 << " (Prioritas: " << current->priority << ")\n";
            found = true;
            count++;
        }
        current = current->next;
    }
    if (!found)
    {
        cout << "Tidak ada tugas yang cocok dengan kata kunci \"" << keyword << "\".\n";
    }
    cout << "----------------------------------------------\n";
}

void hapusTugasBerdasarkanDeskripsi(const char *descKey)
{
    if (head == nullptr)
    {
        cout << "List Kosong, tidak ada yang bisa dihapus.\n";
        return;
    }
    Task *current = head;
    while (current != nullptr && strcmp(current->description, descKey) != 0)
    {
        current = current->next;
    }

    if (current == nullptr)
    {
        cout << "Tugas dengan deskripsi \"" << descKey << "\" tidak ditemukan.\n";
        return;
    }

    if (current == head)
    {
        head = current->next;
    }
    if (current->prev != nullptr)
    {
        current->prev->next = current->next;
    }
    if (current->next != nullptr)
    {
        current->next->prev = current->prev;
    }
    if (current == tail)
    {
        tail = current->prev;
    }
    if (head == nullptr)
    {
        tail = nullptr;
    }

    delete current;
    cout << "Tugas \"" << descKey << "\" berhasil dihapus.\n";
}


void simpanKeFile()
{
    FILE *file = fopen(FILENAME, "wb");
    if (file == nullptr)
    {
        cerr << "Error: Tidak dapat membuka file " << FILENAME << " untuk ditulis.\n";
        return;
    }

    int taskCount = 0;
    Task *tempCount = head;
    while (tempCount != nullptr)
    {
        taskCount++;
        tempCount = tempCount->next;
    }
    fwrite(&taskCount, sizeof(int), 1, file);

    Task *current = head;
    while (current != nullptr)
    {
        int descLength = strlen(current->description);
        fwrite(&descLength, sizeof(int), 1, file);
        fwrite(current->description, sizeof(char), descLength, file);
        fwrite(&current->priority, sizeof(int), 1, file);
        current = current->next;
    }
    fclose(file);
}

void muatDariFile()
{
    FILE *file = fopen(FILENAME, "rb");
    if (file == nullptr)
    {
        return;
    }
    bersihkanList();

    int taskCount = 0;
    if (fread(&taskCount, sizeof(int), 1, file) != 1)
    {
        fclose(file);
        return;
    }

    for (int i = 0; i < taskCount; ++i)
    {
        int descLength;
        char descBuffer[256];
        int priority;

        if (fread(&descLength, sizeof(int), 1, file) != 1)
            break;

        if (descLength >= 0 && descLength < (int)sizeof(descBuffer))
        {
            if (fread(descBuffer, sizeof(char), descLength, file) != (size_t)descLength)
                break;
            descBuffer[descLength] = '\0';
        }
        else
        {
            cerr << "Peringatan: Panjang deskripsi tidak valid atau terlalu besar di file untuk task ke-" << i + 1 << ". Task dilewati." << endl;
            if (fread(&priority, sizeof(int), 1, file) != 1)
                break;
            continue;
        }

        if (fread(&priority, sizeof(int), 1, file) != 1)
            break;

        sisipNodeSesuaiPrioritas(descBuffer, priority);
    }
    fclose(file);
}

void bersihkanList()
{
    Task *current = head;
    Task *nextNode = nullptr;
    while (current != nullptr)
    {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = tail = nullptr;
}