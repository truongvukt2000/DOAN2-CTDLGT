#include "File.h"

vector<int> findchar(int nRoot) //tra ve cac phan tu da duoc xu ly
{
	vector<int> a;
	for (int i = 0; i < 256; i++) //0-255 la cac node con
	{
		if (huffTree[i].used == true)
			a.push_back(i);
	}
	return a;
}
string fix_bit_end(string compressed, int& count)
{
	string fixer = "";
	count = 8 - (compressed.length() % 8);
	if (count == 8) count = 0;
	for (int i = 0; i < count; i++) //them bit 0 vao cho bit con thieu cho du 8.
	{
		fixer += '0';
	}
	return compressed + fixer;
}

void Compress(string inFile, string outFile)
{
	ifstream is(inFile, ios::binary);
	if (is.fail())
		cout << "fail";
	KhoiTao();
	ThongKeTanSoXuatHien(is);
	int nRoot = TaoCayHuffman();
	PhatSinhMaBit(nRoot);
	vector<int> temp = findchar(nRoot);
	char buffer[MAX_BIT_LEN];
	is.clear(); //reset con tro
	is.seekg(0, ios::beg); //dua con tro ve 0

	//outfile
	ofstream outfile(outFile, ios::binary);
	if (outfile.fail())
		cout << "fail";
	outfile << " " << endl << temp.size() << endl;
	for (int i = 0; i < temp.size(); i++)
	{
		outfile << huffTree[temp[i]].c;
		outfile << huffTree[temp[i]].freq;
		outfile << "\n";
	}

	//char s;
	string bit_buffer = "";
	while (is.read(buffer, MAX_BIT_LEN))
	{
		for (int j = 0; j < MAX_BIT_LEN; j++) //convert to binary_code
		{
			for (int i = 0; i < temp.size(); i++)
			{
				if (buffer[j] == huffTree[temp[i]].c[0])
				{
					bit_buffer += bangMaBit[temp[i]].bits;
					break;
				}
			}
		}

		for (int i = 0; i < bit_buffer.length() - bit_buffer.length() % 8; i += 8) //convert to char
		{
			int dec_value = 0;
			int data = 1;
			for (int j = i + 7; j >= i; j--) //binary to char
			{
				if (bit_buffer[j] == '1') dec_value += data;
				data = data * 2;

			}
			outfile << char(dec_value);
		}
		bit_buffer = bit_buffer.substr(bit_buffer.length() - bit_buffer.length() % 8, bit_buffer.length());
	}

	for (int j = 0; j < is.gcount(); j++) //convert binary_code
	{
		for (int i = 0; i < temp.size(); i++)
		{
			if (buffer[j] == huffTree[temp[i]].c[0])
			{
				bit_buffer += bangMaBit[temp[i]].bits;
				break;
			}
		}
	}
	int end_bit;
	bit_buffer = fix_bit_end(bit_buffer, end_bit);

	for (int i = 0; i < bit_buffer.length(); i += 8) //convert to char
	{
		int dec_value = 0;
		int data = 1;
		for (int j = i + 7; j >= i; j--) //binary to char
		{
			if (bit_buffer[j] == '1') dec_value += data;
			data = data * 2;
		}
		outfile << char(dec_value);
	}

	outfile.clear();
	outfile.seekp(0, ios::beg);
	outfile << end_bit;
	is.close();
	outfile.close();
}

string convert8bit(char s)
{
	bitset<8> x(s); //set 8 bit -> string
	return x.to_string();
}
void extract(string inFile, string outFile)
{
	ifstream inF(inFile, ios::binary);
	KhoiTao();
	int bit_end;	// dem bit_end
	inF >> bit_end;
	int capacity;
	inF >> capacity;

	// doc vao struct Char
	char* des = new char;
	inF.read(des, 1);////////
	for (int i = 0; i < capacity; i++)
	{
		char des2;
		inF.read(&des2, 1); /////////
		int value = int(des2);
		if (value < 0) value += 256;
		huffTree[value].c = des2;

		inF >> huffTree[value].freq;

		inF.read(des, 1); //////
	}

	int nRoot = TaoCayHuffman();
	PhatSinhMaBit(nRoot);

	ofstream output(outFile, ios::binary);

	char getter[MAX_BIT_LEN];
	string bit = "";
	int temp = nRoot;
	while (inF.read(getter, MAX_BIT_LEN))
	{
		for (int i = 0; i < MAX_BIT_LEN; i++) //convert string to bit
		{
			bit += convert8bit(getter[i]);
		}

		for (int i = 0; i < bit.length(); i++) //extract
		{
			if (bit[i] == '0')
				temp = huffTree[temp].nLeft;
			else
				temp = huffTree[temp].nRight;
			if (huffTree[temp].nLeft == -1 && huffTree[temp].nRight == -1)
			{
				output << huffTree[temp].c;
				temp = nRoot;
			}
		}
		bit = "";
	}
	for (int i = 0; i < inF.gcount(); i++) //convert string to bit
	{
		bit += convert8bit(getter[i]);
	}
	bit = bit.substr(0, bit.length() - bit_end);

	for (int i = 0; i < bit.length(); i++) //extract
	{
		if (bit[i] == '0')
			temp = huffTree[temp].nLeft;
		else
			temp = huffTree[temp].nRight;
		if (huffTree[temp].nLeft == -1 && huffTree[temp].nRight == -1)
		{
			output << huffTree[temp].c;
			temp = nRoot;
		}
	}

	inF.close();
	output.close();
}