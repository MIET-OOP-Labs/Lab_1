/*
3) Записи о занятиях: номер записи, название предмета, тип занятия (лекция,
практика, лабораторная, экзамен), аудитория. Сортировка по аудитории.
Вывести все занятия-экзамены в аудитории 1205.
*/

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdio.h>

using namespace std;

typedef enum lessonTypesEnum {none, Lection, Practice, Lab, Exam};  //enumeration of types of lessons

typedef struct {
    int num;
    string subjectName;
    lessonTypesEnum lessonType; //structure that stores info about each record of lessons
    int classroom;
} recordStruct;

vector<string> split(const string& str, char delim) {
    vector<string> tokens;
    if (!str.empty()) {
        size_t start = 0;
        size_t end = 0;
        do {
            end = str.find(delim, start);
            tokens.push_back(str.substr(start, (end - start)));
            start = end + 1;
        } while (end != string::npos);
    }

    return tokens;
}

lessonTypesEnum getLesson(string lessonStr){
    lessonTypesEnum lesson = none;
    if (!lessonStr.compare("Lection")) {
        lesson = Lection;
    } else if (!lessonStr.compare("Practice")) {
        lesson = Practice;
    } else if (!lessonStr.compare("Lab")) {
        lesson = Lab;
    } else if (!lessonStr.compare("Exam")) {
        lesson = Exam;
    }
    return lesson;
}

string getLesson(lessonTypesEnum lesson){       //functions to convert enum type of lessons to string and vice-versa
    switch (lesson) {
    case Lection:   return "Lection";
    case Practice:   return "Practice";
    case Lab:   return "Lab";
    case Exam:   return "Exam";
    default: return "None";
    }
}

bool getRecord(vector<string> str, recordStruct *record){
    if (str.size() != 4) {
        return false;
    }
    record->num = stoi(str[0]);
    record->subjectName = str[1];
    record->lessonType = getLesson(str[2]);                 //function for filling record struct from string-line, taken from .csv file
    record->classroom = stoi(str[3]);
    return true;
}

void printRecord(vector<recordStruct> lessonsFileRecords){
    for (recordStruct record : lessonsFileRecords) {            //function for printing all records in recordStruct vector
        printf("%2d %15s %10s %5d\n", record.num, record.subjectName.c_str(), getLesson(record.lessonType).c_str(), record.classroom);
    }
}

bool sortFunc (recordStruct i,recordStruct j) { return (i.classroom < j.classroom);} //comparison function object for sorting records by classroom

void filterRecords(vector<recordStruct> lessonsFileRecords, recordStruct filter){   //filter all records by filter struct. It will be also checked, if some parametr in filter isn't used
    for (recordStruct record : lessonsFileRecords) {
        bool subjectEqual = (record.subjectName.compare(filter.subjectName)) || (!filter.subjectName.length()); //check if subjects are equal or subject name in filter is empty, which means this parametr isn't used
        bool lessonTypeEqual = (record.lessonType == filter.lessonType) || (!filter.lessonType);                //check if lesson types are equal or lesson type in filter is none (none in lessonTypesEnum = 0)
        bool classroomEqual = (record.classroom == filter.classroom) || (!filter.classroom);                    //check if classrooms are equal or classroom in filter is null
        if (subjectEqual && lessonTypeEqual && classroomEqual) {
            printf("%2d %15s %10s %5d\n", record.num, record.subjectName.c_str(), getLesson(record.lessonType).c_str(), record.classroom);
        }
    }
}

void printSortedRecordsInFile(vector<recordStruct> lessonsFileRecords, string fname){
    ofstream sortedFile("sorted_" + fname);
    for (recordStruct record : lessonsFileRecords) {//function for printing all records in recordStruct vector
        char * buff;
        sprintf(buff, "%d;%s;%s;%d", record.num, record.subjectName.c_str(), getLesson(record.lessonType).c_str(), record.classroom);
        string foo = string(buff);
        sortedFile << foo << endl;       // Write to the file
    }
        sortedFile.close();
}

int main() {
    vector<recordStruct> lessonsFileRecords;    //vector that stores all lesson records
    vector<string> row;                         //vector that stores splitted line from .csv file
    string line;

    ifstream  file;
    string fname;
    do {
        cout<<"Enter the file name: ";
        cin>>fname;
        file.open(fname);
        if(!file.is_open()) {
            cout<<"Could not open the file\n";
        }
    } while (!file.is_open());

    while(getline(file, line)) {
        row.clear();
        row = split(line, ';');
        recordStruct currRecord;
        if (!getRecord(row, &currRecord)) {
            cout<<"File is invalid in line" << line;
            return 0;
        }
        lessonsFileRecords.push_back(currRecord);
    }
    printRecord(lessonsFileRecords);

    cout << endl << "Sorted by classroom records" << endl;
    sort (lessonsFileRecords.begin(), lessonsFileRecords.end(), sortFunc);
    printRecord(lessonsFileRecords);

    printSortedRecordsInFile(lessonsFileRecords, fname);


    cout << endl << "Exams in 1205 classroom" << endl;
    recordStruct filter = {-1,"",Exam, 1205};
    filterRecords(lessonsFileRecords,filter);

    return 0;
}
