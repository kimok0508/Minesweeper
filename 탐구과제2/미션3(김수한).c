#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>
#include<conio.h>

#define SAVE_FILE_NAME "saveData.dat"
#define MAP_FILE_NAME "mapData.dat"

FILE *fhand;
int width, height, level;
int cnt, all_cnt;
int map[111][111], info[111][111], visit[111][111], point[111][111];
int all_mine;
int cell_cnt;
int point_cnt;

int game_cnt, win_cnt, win_per;
int log_line = 0;

void gotoxy(int x, int y);
void create_map();
int find_mine(int start_x, int end_x, int start_y, int end_y);
void create_info();
void print_map();
void ui_manager(int x,int y);
void clear_screen();
void reset_game(int x, int y);
void resume_game(int x, int y);
void setting(int x, int y);
void DFS(int x, int y);
void cheater();
void bigmap();
void store_data(int x, int y);
void load_data(int x, int y);
void init_history(int x, int y);
void init_data(int x, int y);

int main() {
	char input_key;	//Ű �Է� ���� ����
	int pos_x = 0, pos_y = 0;
	 
	fhand = fopen(SAVE_FILE_NAME, "r"); //���̺� ������ ���� �ε�
	if(fhand == NULL){	//���� ���� ó��
		fhand = fopen(SAVE_FILE_NAME, "w");
		game_cnt = win_cnt = 0;
	}else {
		fscanf(fhand, "%d %d", &game_cnt, &win_cnt);
	}
	fclose(fhand);

	printf("******************************************************\n");
	printf("*               ��  ��  ã  ��  ��  ��               *\n");
	printf("******************************************************\n");

	printf("���� ���� ũ�⸦ �Է��ϼ���(10~78) : ");
	while (1) {
		scanf("%d", &width);
		if (10 <= width && width <= 78) break;
		else printf("������ ������ϴ�! �ٽ� �Է����ּ���(10~78) : ");
	}

	printf("���� ���� ũ�⸦ �Է��ϼ���(10~23) : ");
	while (1) {
		scanf("%d", &height);
		if (10 <= height && height <= 23) break;
		else printf("������ ������ϴ�! �ٽ� �Է����ּ���(10~23) : ");
	}

	printf("���̵��� �����ϼ���(1:�ʱ�, 2:�߱�, 3:���) : ");
	while (1) {
		scanf("%d", &level);
		if (1 <= level && level <= 3) break;
		else printf("������ ������ϴ�! �ٽ� �Է����ּ���(1~3) : ");
	}

	create_map();
	create_info();
	print_map();

	while (1) {
		//Ŀ���� x, y���� ������ ������ÿ��� �ݴ����� Ŀ���� ��Ÿ������ ��
		if (pos_x >= width) pos_x = 0;
		if (pos_x < 0) pos_x = width - 1;
		if (pos_y >= height) pos_y = 0;
		if (pos_y < 0) pos_y = height - 1;

		ui_manager(pos_x, pos_y);

		gotoxy(pos_x, pos_y);
		input_key = getch();

		if (input_key == 224) input_key = getch();

		if (input_key == 72) pos_y--;
		else if (input_key == 80) pos_y++;
		else if (input_key == 75) pos_x--;
		else if (input_key == 77) pos_x++;
		else if (input_key == 'x') {	//���� ǥ��
			if (point[pos_y][pos_x] == 1) {
				if (visit[pos_y][pos_x] == 0) {
					printf(".");
					point[pos_y][pos_x] = 0;
					point_cnt--;
				}
			}
			else {
				if (visit[pos_y][pos_x] == 0) {
					printf("*");
					point[pos_y][pos_x] = 1;
					point_cnt++;
				}
			}
		}
		else if (input_key == 'z') {	//������
			if (point[pos_y][pos_x] == 1) continue;
			if (info[pos_y][pos_x] == -1) {
				system("cls");
				struct tm *t;
				time_t timer = time(NULL);
				t = localtime(&timer);

				gotoxy(20, 0);
				printf("GAME OVER\n");
				fhand = fopen(SAVE_FILE_NAME, "w");	//���� ��� ����
				fprintf(fhand, "%d %d", ++game_cnt, win_cnt);
				fclose(fhand);
				printf("���� Ƚ�� : %d\n��¥ : %d��-%d��-%d��-%d��-%d��-%d��\n�̱� ���� Ƚ�� : %d\n�·� : %.2f%%\n", game_cnt, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, win_cnt, (double)win_cnt / game_cnt * 100);
				system("pause");
				return 0;
			}
			else if (info[pos_y][pos_x] == 0) {
				DFS(pos_x, pos_y);
			}
			else {
				if (visit[pos_y][pos_x] == 1) continue;
				printf("%d", info[pos_y][pos_x]);
				cnt++;
				visit[pos_y][pos_x] = 1;
			}

			if (cnt == all_cnt) {
				system("cls");
				struct tm *t;
				time_t timer = time(NULL);
				t = localtime(&timer);

				gotoxy(20, 0);
				printf("�����մϴ�!!!\n");
				fhand = fopen(SAVE_FILE_NAME, "w");	//���� ��� ����
				fprintf(fhand, "%d %d", ++game_cnt, ++win_cnt);
				fclose(fhand);
				printf("���� Ƚ�� : %d\n��¥ : %d��-%d��-%d��-%d��-%d��-%d��\n�̱� ���� Ƚ�� : %d\n�·� : %.2f%%\n", game_cnt, t->tm_year + 1900, t->tm_mon +1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, win_cnt, (double)win_cnt / game_cnt * 100);
				system("pause");
				return 0;
			}
		}else if (input_key == 'r') {	//���� ����
			reset_game(pos_x, pos_y);
			pos_x = pos_y = 0;
			gotoxy(pos_x, pos_y);
		}else if(input_key == 's'){	//����
			setting(pos_x, pos_y);
		}
		else {
		
		}
	}
}

//gotoxy�Լ� ����
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//���� �����ϴ� �ڵ�. ���̵��� ���� ������ ��ġ�� �����ϰ� ����.
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

//Ư�� ��ǥ�� �ֺ� �κ��� ���� ���� ��ȯ
int find_mine(int start_x, int end_x, int start_y, int end_y) {
	int tmp_cnt = 0;

	for (int i = start_y; i <= end_y; i++) {
		for (int j = start_x; j <= end_x; j++) {
			if (map[i][j] == 1) tmp_cnt++;
		}
	}

	return tmp_cnt;
}

//������ġ�� �ֺ����� ������ ����
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

				//���� ����� �ʵ��� ��ǥ���� ����
				if (start_x < 0) start_x = 0;
				if (start_y < 0) start_y = 0;
				if (end_x > width)  end_x = width;
				if (end_y > height) end_y = height;

				info[i][j] = find_mine(start_x, end_x, start_y, end_y);
			}
		}
	}
}

//�� ���
void print_map() {
	system("cls");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf(".");
		}
		printf("\n");
	}
}

//main�Լ����� while������ ���� ������ ������ ǥ������
void ui_manager(int x, int y) {
	gotoxy(0, height + 2);
	printf("���� ������ ���� : %d��\n", all_mine - point_cnt);

	gotoxy(x, y);
}

//ȭ�� �ʱ�ȭ
void clear_screen() {
	system("cls");
	print_map();
}

//������ ����۽�Ŵ. ������ ��ġ�� �������ǰ� ���� ������ �ʱ�ȭ��
void reset_game(int x, int y) {
	system("cls");

	cnt = point_cnt = 0;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			map[i][j] = info[i][j] = visit[i][j] = point[i][j] = 0;
		}
	}

	create_map();
	create_info();
	print_map();

	ui_manager(x, y);
}

//���� �����͸� �ٽ� ǥ��
void resume_game(int x, int y) {
	system("cls");

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (point[i][j] == 1) printf("*");
			else if (visit[i][j] == 1) printf("%d", info[i][j]);
			else printf(".");
		}
		printf("\n");
	}

	ui_manager(x, y);
}

//������ ǥ���ϰ� ���ÿ� ���� �б��� �ڿ� �ٽ� ���� ���·� ������
void setting(int x, int y) {
	int selection;

	gotoxy(0, height + 4);
	printf("****** ���� ******\n");
	printf("1. ���� ��� ������ �ʱ�ȭ\n");
	printf("2. ���� ������ �ӽ� ����\n");
	printf("3. �ӽ� ����� ���� �ҷ�����\n");
	printf("4. �ӽ� ����� ���� ����\n");
	printf("5. ���\n");
	printf("******************\n");
	printf("���� : ");
	scanf("%d", &selection);

	if (selection == 1) {
		init_history(x, y);
	}
	else if (selection == 2) {
		store_data(x, y);
	}
	else if (selection == 3) {
		load_data(x, y);
	}
	else if (selection == 4) {
		init_data(x, y);
	}

	log_line = 0;
	resume_game(x, y);

	gotoxy(x, y);
}

//�ֺ��� ������ ������ 0�� ��ǥ�� �̾��������� ���� ������
void DFS(int x, int y) {
	if (y < 0 || y >= height || x < 0 || x >= width) return;
	if (info[y][x] == -1) return;
	if (visit[y][x] == 1 || point[y][x] == 1) return;
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

//������ �����ݴϴ�. ������� ����..
void cheater() {
	gotoxy(0, height + 4);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (info[i][j] == -1) printf("*");
			else printf("%d", info[i][j]);
		}
		printf("\n");
	}
}

//��Ȱ�� ���� �����մϴ�. �� �������..
void bigmap() {
	width = 100;
	height = 100;
	level = 3;
}

//�ʵ����Ϳ� �湮 ������, ���� ǥ�� �����͵��� ����
void store_data(int x, int y) {
	gotoxy(0, height + 4);

	fhand = fopen(MAP_FILE_NAME, "r");
	if (fhand == NULL) {
		fhand = fopen(MAP_FILE_NAME, "w");

		fprintf(fhand, "%d %d %d\n", width, height, level);
		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				fprintf(fhand, "%d %d %d ", info[i][j], visit[i][j], point[i][j]);
			}
			fprintf(fhand, "\n");
		}
		fprintf(fhand, "%d %d", cnt, point_cnt);
		fclose(fhand);
		printf("���� ����");
	}
	else {
		fclose(fhand);
		gotoxy(width + 25, ++log_line);
		printf("| �̹� ����� �����Ͱ� �����մϴ�.");
		setting(x, y);
		return;
	}
	fclose(fhand);
	gotoxy(x, y);
}

//�ʵ����Ϳ� �湮 ������, ���� ǥ�� �����͵��� ����
void load_data(int x, int y) {
	int tmp_width, tmp_height, tmp_level;

	gotoxy(0, height + 4);

	fhand = fopen(MAP_FILE_NAME, "r");
	if (fhand == NULL) {
		gotoxy(width + 25, ++log_line);
		printf("| ����� �����Ͱ� �����ϴ�.");
		setting(x, y);
		return;
	}
	else {
		fscanf(fhand, "%d %d %d\n", &tmp_width, &tmp_height, &tmp_level);
		if (tmp_width != width || tmp_height != height || tmp_level != level) {
			fclose(fhand);
			gotoxy(width + 25, ++log_line);
			printf("| ������ ���� �� �ʺ� : %d, ���� : %d, ���� : %d�� ������ �� �ٽ� �õ��ϼ���.", tmp_width, tmp_height, tmp_level);
			setting(x, y);
			return;
		}

		for (int i = 0; i < height; i++) {
			for (int j = 0; j < width; j++) {
				fscanf(fhand, "%d %d %d ", &info[i][j], &visit[i][j], &point[i][j]);
			}
			fscanf(fhand, "\n");
		}
		fscanf(fhand, "%d %d", &cnt, &point_cnt);

		fclose(fhand);
	}
	gotoxy(x, y);
}

//���� ��� �����͸� ����
void init_history(int x, int y) {
	gotoxy(0, height + 4);

	char str_path[] = { ".\\saveData.dat" };
	if (remove(str_path) == 0) {
		printf("������ ���� ����");
		game_cnt = win_cnt = 0;
	}
	else {
		gotoxy(width + 25, ++log_line);
		printf("| ������ ���� ����");
		setting(x, y);
		return;
	}
	gotoxy(x, y);
}

//����� �����͸� ����
void init_data(int x, int y) {
	gotoxy(0, height + 4);

	char str_path[] = { ".\\mapData.dat" };
	if (remove(str_path) == 0) printf("������ ���� ����");
	else {
		gotoxy(width + 25, ++log_line);
		printf("| ������ ���� ����");
		setting(x, y);
		return;
	}
	gotoxy(x, y);
}