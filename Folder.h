#pragma once
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include "File.h"
#include <direct.h>
#include <experimental/filesystem>
#include <Windows.h>
namespace filesys = std::experimental::filesystem;

struct folder_c //dung de nen folder
{
	string name;
	string folderCha;
	string link;
};
vector<string> get_folders_in(const string& link);
vector<string> get_files_in(string folder);
void creatFolder(string folder);
void setFolder(string link, vector<folder_c>& ds);
void compressFolder(string link);
void extractFolder(string link);