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
	char input_key;	//키 입력 받을 변수
	int pos_x = 0, pos_y = 0;
	 
	fhand = fopen(SAVE_FILE_NAME, "r"); //세이브 데이터 파일 로드
	if(fhand == NULL){	//파일 예외 처리
		fhand = fopen(SAVE_FILE_NAME, "w");
		game_cnt = win_cnt = 0;
	}else {
		fscanf(fhand, "%d %d", &game_cnt, &win_cnt);
	}
	fclose(fhand);

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
		//커서의 x, y값이 범위를 벗어났을시에는 반대편에서 커서가 나타나도록 함
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
		else if (input_key == 'x') {	//지뢰 표시
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
		else if (input_key == 'z') {	//뒤집기
			if (point[pos_y][pos_x] == 1) continue;
			if (info[pos_y][pos_x] == -1) {
				system("cls");
				struct tm *t;
				time_t timer = time(NULL);
				t = localtime(&timer);

				gotoxy(20, 0);
				printf("GAME OVER\n");
				fhand = fopen(SAVE_FILE_NAME, "w");	//게임 결과 갱신
				fprintf(fhand, "%d %d", ++game_cnt, win_cnt);
				fclose(fhand);
				printf("게임 횟수 : %d\n날짜 : %d년-%d월-%d일-%d시-%d분-%d초\n이긴 게임 횟수 : %d\n승률 : %.2f%%\n", game_cnt, t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, win_cnt, (double)win_cnt / game_cnt * 100);
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
				printf("축하합니다!!!\n");
				fhand = fopen(SAVE_FILE_NAME, "w");	//게임 결과 갱신
				fprintf(fhand, "%d %d", ++game_cnt, ++win_cnt);
				fclose(fhand);
				printf("게임 횟수 : %d\n날짜 : %d년-%d월-%d일-%d시-%d분-%d초\n이긴 게임 횟수 : %d\n승률 : %.2f%%\n", game_cnt, t->tm_year + 1900, t->tm_mon +1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, win_cnt, (double)win_cnt / game_cnt * 100);
				system("pause");
				return 0;
			}
		}else if (input_key == 'r') {	//게임 리셋
			reset_game(pos_x, pos_y);
			pos_x = pos_y = 0;
			gotoxy(pos_x, pos_y);
		}else if(input_key == 's'){	//설정
			setting(pos_x, pos_y);
		}
		else {
		
		}
	}
}

//gotoxy함수 정의
void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

//맵을 생성하는 코드. 난이도에 따라 지뢰의 위치를 램덤하게 지정.
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

//특정 좌표의 주변 부분의 지뢰 개수 반환
int find_mine(int start_x, int end_x, int start_y, int end_y) {
	int tmp_cnt = 0;

	for (int i = start_y; i <= end_y; i++) {
		for (int j = start_x; j <= end_x; j++) {
			if (map[i][j] == 1) tmp_cnt++;
		}
	}

	return tmp_cnt;
}

//지뢰위치와 주변지뢰 정보를 생성
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

				//맵을 벗어나지 않도록 좌표값을 보정
				if (start_x < 0) start_x = 0;
				if (start_y < 0) start_y = 0;
				if (end_x > width)  end_x = width;
				if (end_y > height) end_y = height;

				info[i][j] = find_mine(start_x, end_x, start_y, end_y);
			}
		}
	}
}

//맵 출력
void print_map() {
	system("cls");
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf(".");
		}
		printf("\n");
	}
}

//main함수에서 while루프를 돌며 유용한 정보를 표시해줌
void ui_manager(int x, int y) {
	gotoxy(0, height + 2);
	printf("남은 지뢰의 개수 : %d개\n", all_mine - point_cnt);

	gotoxy(x, y);
}

//화면 초기화
void clear_screen() {
	system("cls");
	print_map();
}

//게임을 재시작시킴. 지뢰의 위치가 재지정되고 관련 변수가 초기화됨
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

//게임 데이터를 다시 표시
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

//설정을 표시하고 선택에 따라 분기한 뒤에 다시 원래 상태로 복귀함
void setting(int x, int y) {
	int selection;

	gotoxy(0, height + 4);
	printf("****** 설정 ******\n");
	printf("1. 게임 기록 데이터 초기화\n");
	printf("2. 게임 데이터 임시 저장\n");
	printf("3. 임시 저장된 게임 불러오기\n");
	printf("4. 임시 저장된 게임 삭제\n");
	printf("5. 취소\n");
	printf("******************\n");
	printf("선택 : ");
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

//주변의 지뢰의 개수가 0인 좌표가 이어져있으면 전부 뒤집음
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

//정답을 보여줍니다. 디버깅이 힘들어서..
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

//광활한 맵을 생성합니다. 왜 만들었지..
void bigmap() {
	width = 100;
	height = 100;
	level = 3;
}

//맵데이터와 방문 데이터, 지뢰 표시 데이터등을 저장
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
		printf("저장 성공");
	}
	else {
		fclose(fhand);
		gotoxy(width + 25, ++log_line);
		printf("| 이미 저장된 데이터가 존재합니다.");
		setting(x, y);
		return;
	}
	fclose(fhand);
	gotoxy(x, y);
}

//맵데이터와 방문 데이터, 지뢰 표시 데이터등을 저장
void load_data(int x, int y) {
	int tmp_width, tmp_height, tmp_level;

	gotoxy(0, height + 4);

	fhand = fopen(MAP_FILE_NAME, "r");
	if (fhand == NULL) {
		gotoxy(width + 25, ++log_line);
		printf("| 저장된 데이터가 없습니다.");
		setting(x, y);
		return;
	}
	else {
		fscanf(fhand, "%d %d %d\n", &tmp_width, &tmp_height, &tmp_level);
		if (tmp_width != width || tmp_height != height || tmp_level != level) {
			fclose(fhand);
			gotoxy(width + 25, ++log_line);
			printf("| 게임을 종료 후 너비 : %d, 높이 : %d, 레벨 : %d로 설정한 뒤 다시 시도하세요.", tmp_width, tmp_height, tmp_level);
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

//게임 기록 데이터를 삭제
void init_history(int x, int y) {
	gotoxy(0, height + 4);

	char str_path[] = { ".\\saveData.dat" };
	if (remove(str_path) == 0) {
		printf("데이터 삭제 성공");
		game_cnt = win_cnt = 0;
	}
	else {
		gotoxy(width + 25, ++log_line);
		printf("| 데이터 삭제 실패");
		setting(x, y);
		return;
	}
	gotoxy(x, y);
}

//저장된 데이터를 삭제
void init_data(int x, int y) {
	gotoxy(0, height + 4);

	char str_path[] = { ".\\mapData.dat" };
	if (remove(str_path) == 0) printf("데이터 삭제 성공");
	else {
		gotoxy(width + 25, ++log_line);
		printf("| 데이터 삭제 실패");
		setting(x, y);
		return;
	}
	gotoxy(x, y);
}