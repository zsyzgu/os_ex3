#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define MAXN 10
#define MAXM 32  

struct Block {
	int start;
	int size;
};
int n, m;
struct Block fr[MAXN];
struct Block us[MAXN];

void init() {
	srand(time(0));
	
	fr[n].start = 0;
	fr[n].size = MAXM;
	n++;
}

void pmm_output() {
	int i, j;

	char str[MAXM + 1];
	str[MAXM] = '\0';
	for (i = 0; i < MAXM; i++) {
		str[i] = '*';
	}
	for (i = 0; i < n; i++) {
		for (j = 0; j < fr[i].size; j++) {
			str[fr[i].start + j] = '_';
		}
	}
	printf("%s\n", str);
}

void pmm_alloc(int size) {
	int i, j;
	int id = -1;
	
	for (i = 0; i < n; i++) {
		if (fr[i].size >= size) {
			if (id == -1 || fr[i].size < fr[id].size) {
				id = i;
			}
		}
	}
	
	if (id == -1) {
		printf("no space!\n");
		return;
	}

	us[m].start = fr[id].start;
	us[m].size = size;
	m++;

	if (fr[id].size == size) {
		for (i = id; i < n; i++) {
			fr[i] = fr[i + 1];
		}
		n--;
	} else {
		fr[id].start += size;
		fr[id].size -= size;
	}

	pmm_output();
}

void pmm_free(int id) {
	int i, j;

	int addr = us[id].start;
	int size=  us[id].size;
	
	for (i = id; i < m; i++) {
		us[i] = us[i + 1];
	}
	m--;

	for (i = 0; i < n; i++) {
		if (fr[i].start > addr) {
			break;
		}
	}
	for (j = n; j > i; j--) {
		fr[j] = fr[j - 1];
	}
	n++;
	fr[i].start = addr;
	fr[i].size = size;

	while (i - 1 >= 0 && fr[i - 1].start + fr[i - 1].size == fr[i].start) {
		fr[i - 1].size += fr[i].size;
		for (j = i; j < n; j++) {
			fr[j] = fr[j + 1];
		}
		n--;
		i--;
	}

	while (i + 1 < n && fr[i].start + fr[i].size == fr[i + 1].start) {
		fr[i].size += fr[i + 1].size;
		for (j = i + 1; j < n; j++) {
			fr[j] = fr[j + 1];
		}
		n--;
	}

	pmm_output();
}

int main() {
	init();

	int i;
	for (i = 0; i < MAXN; i++) {
		int mem = rand() % MAXM + 1;
		printf("[alloc] size = %d\n", mem);
		pmm_alloc(mem);

		if (m > 0 && (rand() & 1) == 0) {
			int id = rand() % m;
			printf("[free] addr = %d, size = %d\n", us[id].start, us[id].size);
			pmm_free(id);
		}
	}
	
	return 0;
}
