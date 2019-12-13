#include "Folder.h"


vector<string> get_folders_in(const string& link)
{
	std::vector<string> r;
	for (auto& p : filesys::recursive_directory_iterator(link))
		if (p.status().type() == filesys::file_type::directory)
			r.push_back(p.path().string());
	return r;
}
vector<string> get_files_in(string folder)
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}
void creatFolder(string folder)
{
	int check;
	string dirname = folder;
	check = _mkdir(dirname.c_str());  //creat folder
}

void setFolder(string link, vector<folder_c>& ds)
{
	folder_c temp;
	string name1, foldercha1, link1;
	int i = link.size() - 1;
	for (i; i >= 0; i--)
	{
		if (link[i] == 92) break;
		else
		{
			name1 += link[i];
		}
	}
	reverse(name1.begin(), name1.end()); //dao chuoi
	temp.name = name1;
	link = link.substr(0, i--); //lay i-- phan tu bat dau tu 0 cua link
	for (i; i >= 0; i--)
	{
		if (link[i] == 92 || link[i] == 58) break;
		else
		{
			foldercha1 += link[i];
		}
	}
	reverse(foldercha1.begin(), foldercha1.end());
	temp.folderCha = foldercha1;
	if (foldercha1.size() == 0) //truong hop nam ngoai o dia~
	{
		temp.link = link + name1 + '2'; //tao duong dan
		creatFolder(temp.link);
	}
	else
	{
		if (ds.size() == 0) //truong hop folder nen khong nao ngoai o dia~
		{
			temp.link = link + '\\' + temp.name + '2';
			creatFolder(temp.link);
		}
		else
		{
			int dem;
			for (int i = 0; i < ds.size(); i++) //folder con
			{
				if (temp.folderCha == ds[i].name)
				{
					dem = i;
				}
			}
			string asd = ds[dem].link + '\\' + temp.name + '2';
			temp.link = asd;
			creatFolder(temp.link);
		}
	}
	ds.push_back(temp);
}
void compressFolder(string link)
{
	vector<string> folders = get_folders_in(link);
	vector<folder_c> ds; //ds thong tin cac folder
	setFolder(link, ds); //add folder can nen
	for (int i = 0; i < folders.size(); i++) //add folder vao ds
	{
		setFolder(folders[i], ds);
	}
	vector<string> files = get_files_in(link);
	for (int i = 0; i < files.size(); i++) //add files nen cua folder cha vao folder nen 
	{
		string in = link + '\\' + files[i];
		string out = ds[0].link + '\\' + files[i];
		Compress(in, out);
	}
	for (int i = 0; i < folders.size(); i++)  //xu ly tung files cua tung folder
	{
		vector<string> files = get_files_in(folders[i]);
		for (int j = 0; j < files.size(); j++)
		{
			string in = folders[i] + '\\' + files[j];
			string out = ds[i + 1].link + '\\' + files[j];
			Compress(in, out);
		}
	}
}
void extractFolder(string link) //tuong tu nen
{
	vector<string> folders = get_folders_in(link);
	vector<folder_c> ds;
	setFolder(link, ds);
	for (int i = 0; i < folders.size(); i++)
	{
		setFolder(folders[i], ds);
	}
	vector<string> files = get_files_in(link);
	for (int i = 0; i < files.size(); i++)
	{
		string in = link + '\\' + files[i];
		string out = ds[0].link + '\\' + files[i];
		extract(in, out);
	}
	for (int i = 0; i < folders.size(); i++)
	{
		vector<string> files = get_files_in(folders[i]);
		for (int j = 0; j < files.size(); j++)
		{
			string in = folders[i] + '\\' + files[j];
			string out = ds[i + 1].link + '\\' + files[j];
			extract(in, out);
		}
	}
}