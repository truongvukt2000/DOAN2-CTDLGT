#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
	string bits; //ma bit 0101010101....
	int	soBit;
};
const int MAX_NODE = 256 * 9;
const int MAX_BIT_LEN = 10000;
extern NODE huffTree[MAX_NODE];
extern MABIT bangMaBit[256];

void KhoiTao();
void ThongKeTanSoXuatHien(ifstream& is);
bool Tim2PhanTuMin(int& i, int& j, int nNode);
int TaoCayHuffman();
void DuyetCayHuffman(int node, char maBit[], int nMaBit);
void PhatSinhMaBit(int nRoot);