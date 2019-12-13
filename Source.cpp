#include "Folder.h"
NODE huffTree[MAX_NODE];
MABIT bangMaBit[256];

int main(int argc, char* argv[])
{
	if (argc != 4 && argc != 5)
	{
		cout << endl << "<namefile.exe>.exe -c/-e file/folder infile (outfile)" << endl;
		cout << "-c: compress " << endl << "-e: extract." << endl;
		cout << "<file> need <outfile>." << endl;
		cout << "Example: project.exe -c file D:\\test1\\test.txt D:\\abc\\out.txt" << endl;
		cout << "<folder> needn't <outfile>." << endl;
		cout << "Example: project.exe -c folder D:\\test" << endl;
		system("pause");
		return 0;
	}
	if (argc == 5)
	{
		string kitu = argv[1];
		string choose = argv[2];
		string inFile = argv[3];
		string outFile = argv[4];
		ifstream in(inFile);
		ofstream out(outFile);
		if (in.fail() || out.fail())
		{
			cout << "Can't open file." << endl;
			return 0;
		}
		if (kitu == "-c")
		{
			Compress(inFile, outFile);
		}
		if (kitu == "-e")
		{
			extract(inFile, outFile);
		}
	}
	else if (argc == 4)
	{
		string kitu = argv[1];
		string choose = argv[2];
		string inFile = argv[3];
		if (kitu == "-c")
		{
			compressFolder(inFile);
		}
		if (kitu == "-e")
		{
			extractFolder(inFile);
		}
	}
	return 0;
}