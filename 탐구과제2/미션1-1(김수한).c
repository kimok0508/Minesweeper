#include<stdio.h>
#include<stdlib.h>

int width, height, level;

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

	system("pause");
	return 0;
}