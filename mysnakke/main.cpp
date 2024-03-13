#include "snake.h"
int main() {
	srand((unsigned int)time(0));		 //生成随机数种子
	int end = 1, result;
	while (end) {
		result = Menu();			 //显示主菜单，并根据用户选择菜单选项决定游戏的执行
		switch (result) {
		case 1:					 //选择1表示，开始贪吃蛇游戏
			InitMap();			 //初始化地图、蛇和食物
			while (MoveSnake());  //如果返回0，则蛇停止移动；返回1，继续移动
			break;
		case 2:					  //选择2表示，显示帮助信息
			Help();
			break;
		case 3:					  //选择3表示，显示关于信息
			About();
			break;
		default:					  //选择0表示，表示结束程序
			end = 0;
			break;
		}
	}
	return 0;
}