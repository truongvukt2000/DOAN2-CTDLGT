#pragma warning(disable:4996)
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include<bitset>
using namespace std;

struct NODE {
	string c;	// ky tu
	int	freq;	// so lan xuat hien
	bool used;	// danh dau node da xu ly chua 
	int	nLeft;	// chi so nut con nam ben trai
	int	nRight;	// chi so nut con nam ben phai
};

struct MABIT
{
	string bits;
	int	soBit;
};

const int MAX_NODE = 256 * 9;
const int MAX_BIT_LEN = 10000;
NODE	huffTree[MAX_NODE];
MABIT	bangMaBit[256];


void KhoiTao() 
{
	for (int i = 0; i < MAX_NODE; i++) 
	{
		huffTree[i].c = i;
		huffTree[i].freq = 0;
		huffTree[i].used = false;
		huffTree[i].nLeft = -1;
		huffTree[i].nRight = -1;
	}
}

void ThongKeTanSoXuatHien(ifstream &is) 
{
	char c[MAX_BIT_LEN];
	while (is.read(c,MAX_BIT_LEN)) 
	{
		for (int i = 0; i < MAX_BIT_LEN; i++)
		{
			int a = int(c[i]);
			if (a < 0) a += 256;
			huffTree[a].freq++;
		}
		/*is.get(c);
		if (is.eof()) 
		{
			break;
		}
		if (int(c) < 0)
		{
			c += 256;
		}
		huffTree[c].freq++;*/ 
	}
	for (int i = 0; i < is.gcount(); i++)
	{
		int a = int(c[i]);
		if (a < 0) a = a + 256;
		huffTree[a].freq++;
	}
}

bool Tim2PhanTuMin(int& i, int& j, int nNode) 
{
	i = -1;
	j = -1;

	// tim 2 phan tu co trong so nho nhat
	for (int k = 0; k < nNode; k++)
		if (huffTree[k].used == false && huffTree[k].freq > 0) //neu node k chua dc xu ly va node k co su dung
		{ 
			if (i == -1) {
				i = k;
			}
			else if (j == -1) {
				j = k;
			}
			else {
				if (huffTree[i].freq > huffTree[j].freq) {
					if (huffTree[k].freq < huffTree[i].freq) {
						i = k;
					}
				}
				else {
					if (huffTree[k].freq < huffTree[j].freq) {
						j = k;
					}
				}
			}
		}
	// sap xep lai 2 phan tu de co i: phan tu co trong so nho nhat, j: phan tu co trong so nho nhi	
	// co 2 truong hop can doi i,j:
			// huffTree[i].freq > huffTree[j].freq
			// huffTree[i].freq == huffTree[j].freq va (huffTree[i].c > huffTree[j].c)
	if (i != -1 && j != -1) {
		if ((huffTree[i].freq > huffTree[j].freq) || ((huffTree[i].freq > huffTree[j].freq) && (huffTree[i].c > huffTree[j].c))) {
			int t = i;
			i = j;
			j = t;
		}
		return true;
	}
	else {
		if (i != -1 || j != -1)
			return true;
		return false;
	}
}

int TaoCayHuffman()
{
	int nNode = 256;
	int i, j;
	bool timThay = false;
	while (true)
	{
		timThay = Tim2PhanTuMin(i, j, nNode);
		if (!timThay) 
		{
			break;
		}
		if (i != -1 && j == -1 || i == -1 && j != -1) //truong hop 1 ki tu
			return nNode - 1;
		//tao node cha
		huffTree[nNode].c = huffTree[i].c + huffTree[j].c;
		huffTree[nNode].freq = huffTree[i].freq + huffTree[j].freq;
		huffTree[nNode].nLeft = i;
		huffTree[nNode].nRight = j;

		//node da xu ly
		huffTree[i].used = true;
		huffTree[j].used = true;

		//node moi chua duoc xu ly
		huffTree[nNode].used = false;
		nNode++; //tao them node moi

	}
	return nNode - 1; //so node moi duoc tao
}

void DuyetCayHuffman(int node, char maBit[], int nMaBit) //node vi tri cua node, maBIt[node], nMaBit la so bit cua ki tu
{
	if (node == -1) //node ko ton tai
	{
		return;
	}
	if (huffTree[node].nLeft == -1 && huffTree[node].nRight == -1) //node la
	{		
		bangMaBit[node].soBit = nMaBit;
		for (int i = 0; i < nMaBit; i++) //gan mabit vao node la
		{
			bangMaBit[node].bits += maBit[i];
		}
		return;
	}
	else {
		//left 0
		maBit[nMaBit] = '0';
		DuyetCayHuffman(huffTree[node].nLeft, maBit, nMaBit + 1);

		//right 1
		maBit[nMaBit] = '1';
		DuyetCayHuffman(huffTree[node].nRight, maBit, nMaBit + 1);
	}
}

void PhatSinhMaBit(int nRoot) //vi tri root cua cay
{
	for (int i = 0; i < 256; i++) 
	{
		bangMaBit[i].soBit = 0;
		bangMaBit[i].bits = "";
	};
	char maBit[MAX_BIT_LEN];
	int nMaBit = 0;

	DuyetCayHuffman(nRoot, maBit, nMaBit);
}

vector<int> findchar(int nRoot) 
{
	vector<int> a;
	for (int i = 0; i < 256; i++) //0-255 la cac node con
	{
		if (huffTree[i].used == true)
			a.push_back(i);
	}
	return a;
}

string compressCode(string infile, int nRoot)
{
	string kq = "";
	vector<int> a = findchar(nRoot);
	for (int i = 0; i < infile.length(); i++)
	{
		for (int j = 0; j < a.size(); j++)
		{
			int temp = a[j];
			if (infile[i] == huffTree[temp].c[0])
			{
				kq += bangMaBit[temp].bits;
			}
		}
	}
	return kq;
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

char binaryToChar(string n)	
{
	int he10 = 0;
	int heso = 1;
	for (int i = n.length() - 1; i >= 0; i--) //chuyen sang he 10
	{
		if (n[i] == '1')
			he10 += heso;
		heso = heso * 2;
	}
	return char(he10);
}

string ConvertToChar(string compressed)	//compressedFile
{
	string kq = "";
	int i = 0;
	while (i <= compressed.length())
	{
		string des = compressed.substr(i, 8); //lay 8 bit
		kq += binaryToChar(des);			  //chuyen thanh char
		i += 8;
	}
	return kq;
}

void Compress(char* inFile, char* outFile)
{
	ifstream is(inFile, ios::binary);

	KhoiTao();
	ThongKeTanSoXuatHien(is);
	int nRoot = TaoCayHuffman();
	PhatSinhMaBit(nRoot);
	//XuatBangMaBit();
	vector<int> temp = findchar(nRoot);
	char buffer[MAX_BIT_LEN];
	is.clear();
	is.seekg(0, ios::beg);

	//outfile
	ofstream outfile(outFile, ios::binary);
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
		for (int j = 0; j < MAX_BIT_LEN; j++)
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
		//string temp1 = bit_buffer.substr(0, bit_buffer.length() - bit_buffer.length() % 8);
		//outfile << ConvertToChar(temp1);
		for (int i = 0; i < bit_buffer.length() - bit_buffer.length() % 8; i += 8)
		{
			int dec_value = 0;
			int data = 1;
			for (int j = i + 7; j >= i; j--)
			{
				if (bit_buffer[j] == '1') dec_value += data;
				data = data * 2;

			}
			outfile << char(dec_value);
		}	//out compressedfile
		bit_buffer = bit_buffer.substr(bit_buffer.length() - bit_buffer.length() % 8, bit_buffer.length());
	}

	for (int j = 0; j < is.gcount(); j++)
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
	//outfile << ConvertToChar(bit_buffer);
	for (int i = 0; i < bit_buffer.length(); i += 8)
	{
		int dec_value = 0;
		int data = 1;
		for (int j = i + 7; j >= i; j--)
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
	bitset<8> x(s);
	return x.to_string();
}
string ConvertStringToBit(char* index, int _char, int bit_end)
{
	string kq = "";
	for (int i = 0; i < _char; i++)
	{
		kq += convert8bit(index[i]);
	}
	return kq.substr(0, _char * 8 - bit_end);
}

string extracted(string bit, int nRoot)
{
	string kq = "";
	int temp1 = nRoot;
	for (int i = 0; i < bit.length(); i++)
	{
		if (bit[i] == '0') temp1 = huffTree[temp1].nLeft;
		else
			temp1 = huffTree[temp1].nRight;
		if (huffTree[temp1].nLeft == -1 && huffTree[temp1].nRight == -1)
		{
			kq += huffTree[temp1].c;
			temp1 = nRoot;
		}
	}
	return kq;
}

void extract(char* inFile, char* outFile)
{
	ifstream inF(inFile, ios::binary);
	KhoiTao();
	int bit_end;	// dem bit_end
	inF >> bit_end;
	int capacity;
	inF >> capacity;

	// doc vao struct Char
	char* des = new char;
	inF.read(des, 1);
	for (int i = 0; i < capacity; i++)
	{
		char des2;
		inF.read(&des2, 1);
		int value = int(des2);
		if (value < 0) value += 256;
		huffTree[value].c = des2;

		inF >> huffTree[value].freq;

		inF.read(des, 1);
	}

	int nRoot = TaoCayHuffman();
	PhatSinhMaBit(nRoot);

	ofstream output(outFile, ios::binary);

	char getter[MAX_BIT_LEN];
	string bit = "";
	while (inF.read(getter, MAX_BIT_LEN))
	{
		for (int i = 0; i < MAX_BIT_LEN; i++)
		{
			bit += convert8bit(getter[i]);
		}
		output << extracted(bit, nRoot);
		bit = "";
	}
	for (int i = 0; i < inF.gcount(); i++)
	{
		bit += convert8bit(getter[i]);
	}
	bit = bit.substr(0, bit.length() - bit_end);
	output << extracted(bit, nRoot);

	inF.close();
	output.close();
}

int main() 
{
	// nen
	//Compress((char*)"corpus-title.txt", (char*)"out.txt");
	extract((char*)"out.txt", (char*)"test1.txt");

	return 0;
}