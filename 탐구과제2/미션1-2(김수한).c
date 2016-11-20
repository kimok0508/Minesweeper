#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int width, height, level;
int cnt, all_cnt;
int map[111][111];
int all_mine;
int cell_cnt;

void create_map() {
	cell_cnt = width * height;
	srand(time(NULL));

	if (level == 1) all_mine = cell_cnt / 10;
	else if (level == 2) all_mine = cell_cnt / 8;
	else all_mine = cell_cnt / 5;

	all_cnt = width * height - all_mine;

	for (int i = 0; i < all_mine; i++) {
		int pos_x = rand() % width;
		int pos_y = rand() % height;

		if (map[pos_y][pos_x] == 0) map[pos_y][pos_x] = 1;
		else i--;
	}
}

//deprecated
//only for debug
void print_map() {
	system("cls");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (map[i][j] == 0) printf(".");
			else printf("*");
		}
		printf("\n");
	}
}

int main() {
	printf("******************************************************\n");
	printf("*               �� �� ã �� �� ��              *\n");
	printf("******************************************************\n");

	printf("���ǰ���ũ�⸦�Է��ϼ���(10~78) : ");
	while (1) {
		scanf("%d", &width);
		if (10 <= width && width <= 78) break;
		else printf("������������ϴ�! �ٽ��Է����ּ���(10~78) : ");
	}

	printf("���Ǽ���ũ�⸦�Է��ϼ���(10~23) : ");
	while (1) {
		scanf("%d", &height);
		if (10 <= height && height <= 23) break;
		else printf("������������ϴ�! �ٽ��Է����ּ���(10~23) : ");
	}

	printf("���̵��������ϼ���(1:�ʱ�, 2:�߱�, 3:���) : ");
	while (1) {
		scanf("%d", &level);
		if (1 <= level && level <= 3) break;
		else printf("������������ϴ�! �ٽ��Է����ּ���(1~3) : ");
	}

	create_map();
	print_map();

	system("pause");
	return 0;
}