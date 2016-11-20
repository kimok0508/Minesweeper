#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

int width, height, level;
int cnt, all_cnt;
int map[111][111], info[111][111], visit[111][111];
int all_mine;
int cell_cnt;

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

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

void print_map() {
	system("cls");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf(".");
		}
		printf("\n");
	}
}

void DFS(int x, int y) {
	if (y < 0 || y >= height || x < 0 || x >= width) return;
	if (visit[y][x] == 1) return;
	else {
		cnt++;
		visit[y][x] = 1;
	}

	gotoxy(x, y);
	printf("%d", info[y][x]);

	if (info[y][x] == 0) {
		DFS(x - 1, y - 1);
		DFS(x, y - 1);
		DFS(x + 1, y - 1);
		DFS(x - 1, y);
		DFS(x + 1, y);
		DFS(x - 1, y + 1);
		DFS(x, y + 1);
		DFS(x + 1, y + 1);
	}
}

int main() {
	char input_key;
	int pos_x = 0, pos_y = 0;

	printf("******************************************************\n");
	printf("*               지  뢰  찾  기  게  임               *\n");
	printf("******************************************************\n");

	printf("맵의 가로 크기를 입력하세요(10~78) : ");
	while (1) {
		scanf("%d", &width);
		if (10 <= width && width <= 78) break;
		else printf("범위를 벗어났습니다! 다시 입력해주세요(10~78) : ");
	}

	printf("맵의 세로 크기를 입력하세요(10~23) : ");
	while (1) {
		scanf("%d", &height);
		if (10 <= height && height <= 23) break;
		else printf("범위를 벗어났습니다! 다시 입력해주세요(10~23) : ");
	}

	printf("난이도를 선택하세요(1:초급, 2:중급, 3:고급) : ");
	while (1) {
		scanf("%d", &level);
		if (1 <= level && level <= 3) break;
		else printf("범위를 벗어났습니다! 다시 입력해주세요(1~3) : ");
	}

	create_map();
	create_info();
	print_map();

	while (1) {
		gotoxy(pos_x, pos_y);
		input_key = getch();

		if (input_key == 224) input_key = getch();

		if (input_key == 72) pos_y--;
		else if (input_key == 80) pos_y++;
		else if (input_key == 75) pos_x--;
		else if (input_key == 77) pos_x++;
		else if (input_key == 'x') printf("*");
		else if (input_key == 'z') {
			if (info[pos_y][pos_x] == -1) {
				//gotoxy(20, 0);
				//printf("GAME OVER");
				//system("pause");
				//return 0;
			}
			else if(info[pos_y][pos_x] == 0){
				DFS(pos_x, pos_y);
			}else{
				if (visit[pos_y][pos_x] == 1) continue;
				printf("%d", info[pos_y][pos_x]);
				cnt++;
				visit[pos_y][pos_x] = 1;
			}

			if (cnt == all_cnt) {
				gotoxy(20, 0);
				printf("축하합니다!!!");
				system("pause");
				return 0;
			}
		}
	}
}