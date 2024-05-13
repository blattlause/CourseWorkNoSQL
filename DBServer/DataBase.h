#pragma once

#include <string>
#include <vector>

void DatabaseUpdateFile(const std::string& fileName, const std::string& newData);
void DatabaseCreateFile(const std::string& fileName, const std::string& data);
void DatabaseDeleteFile(const std::string& fileName);
std::vector<std::string> DatabaseListFiles(const std::string& directory);
bool DatabaseCreateCollection(const std::string& collectionName);
bool DatabaseDeleteCollection(const std::string& collectionName);
std::vector<std::string> DatabaseListCollections();
std::string DatabaseGetFileContents(const std::string& fileName);
