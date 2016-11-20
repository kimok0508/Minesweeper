#include<stdio.h>
#include<stdlib.h>

int width, height, level;

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

	system("pause");
	return 0;
}