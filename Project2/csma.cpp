#include"csma.h"

int min(int a, int b) {
	if (a > b)
		return b;
	else return a;
}

int backoff(int collision_count) {
	int k = min(collision_count, 10);
	int m = pow(2, k) - 1;
	int randnum = rand() % m + 1;
	return randnum * SLOT;
};

