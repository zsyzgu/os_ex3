#include <iostream>
#include <cstdio>
#include <fstream>

using namespace std;

int memory[4096];
int disk[4096];

int main() {

	fstream fin;
	fin.open("memory.txt");

	for(int i = 0 ; i < 128 ; i++) {
		string str;
		fin >> str;
		fin >> str;
		for(int j = 0 ; j < 32 ; j++) {
			fin >> hex >> memory[i * 32 + j];
		}
	}

	fin.close();

	fin.open("disk.txt");

	for(int i = 0 ; i < 128 ; i++) {
		string str;
		fin >> str;
		fin >> str;
		for(int j = 0 ; j < 32 ; j++) {
			fin >> hex >> disk[i * 32 + j];
		}
	}

	fin.close();

	int virtual_address;
	printf("Virtual Address 0x");
	scanf("%x", &virtual_address);

	int PDE_index = (virtual_address & 0x7c00) >> 10;
	int PTE_index = (virtual_address & 0x03e0) >> 5;
	int page_offset = virtual_address & 0x001f;

	int PDE = memory[0xd80 + PDE_index];
	int pt = PDE & 0x7f;
	int PDE_valid = (PDE & 0x80) >> 7;
	printf("  --> pde index:0x%x  pde contents:(valid %d, pt 0x%x)\n", PDE_index, PDE_valid, pt);

	if(PDE_valid != 1) {
		printf("      --> Fault (page directory entry not valid)\n");
	}
	else {
		int PTE = memory[pt * 32 + PTE_index];
		int pfn = PTE & 0x7f;
		int PTE_valid = (PTE & 0x80) >> 7;
		printf("    --> pte index:0x%x  pte contents:(valid %d, pfn 0x%x)\n", PTE_index, PTE_valid, pfn);

		int physical_address = pfn * 32 + page_offset;
		int value = memory[physical_address];

		if(PTE_valid != 1) {
			printf("      --> To Disk Sector Address 0x%x --> Value: %02x\n", physical_address, disk[physical_address]);
		}
		else {
			printf("      --> To Physical Address 0x%x --> Value: %02x\n", physical_address, value);
		}
	}

	return 0;
}