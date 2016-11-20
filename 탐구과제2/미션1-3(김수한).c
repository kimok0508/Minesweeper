#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int width, height, level;
int cnt, all_cnt;
int map[111][111], info[111][111];
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

int find_mine(int start_x, int end_x, int start_y, int end_y) {
	int tmp_cnt = 0;

	for (int i = start_y; i <= end_y; i++) {
		for (int j = start_x; j <= end_x; j++) {
			if (map[i][j] == 1) tmp_cnt++;
		}
	}

	return tmp_cnt;
}

void create_info() {
	int start_x;
	int start_y;
	int end_x;
	int end_y;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (map[i][j] == 1) info[i][j] = -1;
			else {
				start_x = j - 1;
				start_y = i - 1;
				end_x = j + 1;
				end_y = i + 1;

				if (start_x < 0) start_x = 0;
				if (start_y < 0) start_y = 0;
				if (end_x > width)  end_x = width;
				if (end_y > height) end_y = height;

				info[i][j] = find_mine(start_x, end_x, start_y, end_y);
			}
		}
	}
}

//deprecated
//only for debug
void print_map() {
	system("cls");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (info[i][j] == -1) printf("*");
			else printf("%d", info[i][j]);
		}
		printf("\n");
	}
}

int main() {
	printf("******************************************************\n");
	printf("*               지 뢰 찾 기 게 임              *\n");
	printf("******************************************************\n");

	printf("맵의가로크기를입력하세요(10~78) : ");
	while (1) {
		scanf("%d", &width);
		if (10 <= width && width <= 78) break;
		else printf("범위를벗어났습니다! 다시입력해주세요(10~78) : ");
	}

	printf("맵의세로크기를입력하세요(10~23) : ");
	while (1) {
		scanf("%d", &height);
		if (10 <= height && height <= 23) break;
		else printf("범위를벗어났습니다! 다시입력해주세요(10~23) : ");
	}

	printf("난이도를선택하세요(1:초급, 2:중급, 3:고급) : ");
	while (1) {
		scanf("%d", &level);
		if (1 <= level && level <= 3) break;
		else printf("범위를벗어났습니다! 다시입력해주세요(1~3) : ");
	}

	create_map();
	create_info();
	print_map();

	system("pause");
	return 0;
}