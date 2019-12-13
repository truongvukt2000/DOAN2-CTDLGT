#include "HuffmanTree.h"


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
void ThongKeTanSoXuatHien(ifstream& is)
{
	char c[MAX_BIT_LEN];
	while (is.read(c, MAX_BIT_LEN))
	{
		for (int i = 0; i < MAX_BIT_LEN; i++)
		{
			int a = int(c[i]);
			if (a < 0) a += 256;
			huffTree[a].freq++;
		}
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