#include <iostream>
#include <winsock2.h>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include <vector>
#include <cstring> 
#include <windows.h>
#include "DataBase.h"

#define BASE_DIRECTORY "./DB/"

void DatabaseUpdateFile(const std::string& fileName, const std::string& newData) {
    // Открываем файл для записи
    std::ofstream file(BASE_DIRECTORY + fileName);
    if (!file.is_open()) {
        std::cout << "Failed to open file for writing: " << fileName << std::endl;
        return;
    }

    // Записываем новые данные в файл
    file << newData;

    // Закрываем файл
    file.close();

    std::cout << "File updated successfully: " << fileName << std::endl;
}

void DatabaseCreateFile(const std::string& fileName, const std::string& data) {
    // Открываем файл для записи
    std::ofstream file(BASE_DIRECTORY + fileName);
    if (!file.is_open()) {
        std::cout << "Failed to create file: " << fileName << std::endl;
        return;
    }

    // Записываем данные в файл
    file << data;

    // Закрываем файл
    file.close();

    std::cout << "File created successfully: " << fileName << std::endl;
}

void DatabaseDeleteFile(const std::string& fileName) {
    if (std::remove((BASE_DIRECTORY + fileName).c_str()) != 0) {
        std::cout << "Failed to delete file: " << fileName << std::endl;
        return;
    }

    std::cout << "File deleted successfully: " << fileName << std::endl;
}

std::vector<std::string> DatabaseListFiles(const std::string& directory) {
    std::vector<std::string> fileList;
    std::string searchPath = BASE_DIRECTORY + directory + "\\*.*"; // Строка для поиска всех файлов в указанной директории

    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &fileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (!(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                fileList.push_back(fileData.cFileName);
            }
        } while (FindNextFileA(hFind, &fileData) != 0);

        FindClose(hFind);
    }
    else {
        std::cerr << "Failed to find files in directory: " << directory << std::endl;
    }

    return fileList;
}

bool DatabaseCreateCollection(const std::string& collectionName) {
    if (CreateDirectoryA((BASE_DIRECTORY + collectionName).c_str(), nullptr) != 0) {
        std::cout << "Collection created successfully: " << collectionName << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to create collection: " << collectionName << std::endl;
        return false;
    }
}

bool DatabaseDeleteCollection(const std::string& collectionName) {
    if (RemoveDirectoryA((BASE_DIRECTORY + collectionName).c_str()) != 0) {
        std::cout << "Collection deleted successfully: " << collectionName << std::endl;
        return true;
    }
    else {
        std::cerr << "Failed to delete collection: " << collectionName << std::endl;
        return false;
    }
}

std::vector<std::string> DatabaseListCollections() {
    std::vector<std::string> collectionList;
    WIN32_FIND_DATAA fileData;
    HANDLE hFind = FindFirstFileA(BASE_DIRECTORY "*", &fileData);

    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            if (fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && strcmp(fileData.cFileName, ".") != 0 && strcmp(fileData.cFileName, "..") != 0) {
                collectionList.push_back(fileData.cFileName);
            }
        } while (FindNextFileA(hFind, &fileData) != 0);

        FindClose(hFind);
    }
    else {
        std::cerr << "Failed to find collections." << std::endl;
    }

    return collectionList;
}

std::string DatabaseGetFileContents(const std::string& fileName) {
    std::ifstream file(BASE_DIRECTORY + fileName);
    std::string fileContents;

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            fileContents += line + "\n";
        }
        file.close();
        std::cout << "File read successfully: " << fileName << std::endl;
    }
    else {
        std::cout << "Failed to open file for reading: " << fileName << std::endl;
    }

    return fileContents;
}
