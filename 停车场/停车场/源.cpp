#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2 // 停车场包含的车位数量
#define SIZE 1 // 每个车位的容量
#define RATE 2 // 每小时收费2元

// 定义汽车信息结构
typedef struct {
    char state[10];
    char CarID[10];
    int time;
} Car;

// 停车场栈结构
typedef struct {
    Car car[SIZE];
    int TOP;
} Garage;

// 便道队列链表节点
typedef struct street {
    Car car;
    struct street* next;
} Street;

// 函数声明
Garage* CreateGarage();
int isFull(Garage* garage);
int isEmpty(Garage* garage);
void Push(Car car, Garage* garage);
Car Pop(Garage* garage);
Street* Enqueue(Street* rear, Car car);
Street* Dequeue(Street* front);
int isCarIDPresent(Garage garages[], int numGarages, Street* front, char* CarID);
int ProcessArrival(Car car, Garage garages[], int numGarages, Street** front, Street** rear);
int ProcessDeparture(char* CarID, int time, Garage garages[], int numGarages, Street** front, Street** rear);


// 创建单个栈
Garage* CreateGarage() {
    Garage* g = (Garage*)malloc(sizeof(Garage));
    g->TOP = -1;
    return g;
}

// 检查栈是否满
int isFull(Garage* garage) {
    return (garage->TOP == SIZE - 1);
}

// 检查栈是否空
int isEmpty(Garage* garage) {
    return (garage->TOP == -1);
}

// 将车停入栈
void Push(Car car, Garage* garage) {
    garage->car[++garage->TOP] = car;
   // printf("车牌为%s的车已停入停车场，车位位置为%d\n", car.CarID, garage->TOP + 1);
}

// 将车从栈移出
Car Pop(Garage* garage) {
    if (isEmpty(garage)) {
        printf("错误：停车场为空\n");
        exit(1);
    }
    return garage->car[garage->TOP--];
}

// 将车加入便道队列
Street* Enqueue(Street* rear, Car car) {
    Street* newNode = (Street*)malloc(sizeof(Street));
    newNode->car = car;
    newNode->next = NULL;
    if (rear) rear->next = newNode;
    return newNode;
}

// 将车从便道队列移出
Street* Dequeue(Street* front) {
    if (!front) return NULL;
    Street* temp = front;
    front = front->next;
    printf("车牌为%s的车已从便道离开\n", temp->car.CarID);
    free(temp);
    return front;
}

// 检查停车场和便道中是否已有相同车牌
int isCarIDPresent(Garage garages[], int numGarages, Street* front, char* CarID) {
    // 检查所有栈
    for (int i = 0; i < numGarages; i++) {
        for (int j = 0; j <= garages[i].TOP; j++) {
            if (strcmp(garages[i].car[j].CarID, CarID) == 0) {
                return 1; // 找到相同车牌
            }
        }
    }
    // 检查便道队列
    Street* current = front;
    while (current) {
        if (strcmp(current->car.CarID, CarID) == 0) {
            return 1; // 找到相同车牌
        }
        current = current->next;
    }
    return 0; // 没有重复车牌
}


void DisplayStatus(Garage garages[], int numGarages, Street* front) {
    int emptySlots = 0; // 统计空车位数
    for (int i = 0; i < numGarages; i++) {
        emptySlots += (SIZE - (garages[i].TOP + 1));
    }

    int queueCount = 0; // 统计便道车辆数
    Street* current = front;
    while (current) {
        queueCount++;
        current = current->next;
    }

    printf("当前空车位数: %d\n", emptySlots);
    printf("当前便道车辆数: %d\n", queueCount);
}


// 处理汽车到达逻辑
int ProcessArrival(Car car, Garage garages[], int numGarages, Street** front, Street** rear) {
    for (int i = 0; i < numGarages; i++) {
        if (!isFull(&garages[i])) {
            Push(car, &garages[i]);
            printf("车牌为%s的车停入停车场的第%d个车位\n", car.CarID, i + 1);
            DisplayStatus(garages, numGarages, *front); // 显示状态
            return 1; // 成功停入停车场
        }
    }
    // 如果所有栈已满，加入便道
    *rear = Enqueue(*rear, car);
    if (*front == NULL) *front = *rear; // 队列为空时设置front指针
    printf("停车场已满，车牌为%s的车停入便道\n", car.CarID);
    DisplayStatus(garages, numGarages, *front); // 显示状态
    return 1; // 成功到达
}

int ProcessDeparture(char* CarID, int time, Garage garages[], int numGarages, Street** front, Street** rear) {
    
    int leave_time = 0;

    // 检查所有栈
    for (int i = 0; i < numGarages; i++) {
        for (int j = 0; j <= garages[i].TOP; j++) {
            if (strcmp(garages[i].car[j].CarID, CarID) == 0) {
                Car departingCar = garages[i].car[j];
                int duration = time - departingCar.time;

                if (duration < 0) {
                    printf("错误：车牌为%s的车离开时间早于到达时间，停车时长不能小于0\n", CarID);
                    return 0; // 时间错误，重新输入
                }

                leave_time = time;//存储离去时间

                int fee = duration * RATE;
                printf("车牌为%s的车从停车场第%d个车位离开，停留时间为%d小时，应缴费%d元\n", CarID, i + 1, duration, fee);

                // 调整栈
                for (int k = j; k < garages[i].TOP; k++) {
                    garages[i].car[k] = garages[i].car[k + 1];
                }
                garages[i].TOP--;

                // 如果便道有车，将队首车辆移入停车场
                if (*front) {
                    (*front)->car.time = leave_time;
                    Car movedCar = (*front)->car;
                    *front = Dequeue(*front);
                    if (!*front) *rear = NULL; // 队列空时调整rear指针
                    
                    ProcessArrival(movedCar, garages, numGarages, front, rear);
                }

                DisplayStatus(garages, numGarages, *front); // 显示状态
                return 1; // 成功离开
            }
        }
    }

    // 检查便道
    Street* current = *front;
    while (current) {
        if (strcmp(current->car.CarID, CarID) == 0) {
            printf("车牌为%s的车在便道中，停留时间不收费\n", CarID);
            Street* prev = NULL;
            while (current && strcmp(current->car.CarID, CarID) != 0) {
                prev = current;
                current = current->next;
            }
            if (current) {
                if (prev) prev->next = current->next;
                if (current == *front) *front = current->next;
                free(current);
            }
            DisplayStatus(garages, numGarages, *front); // 显示状态
            return 1; // 离开处理完成
        }
        current = current->next;
    }
    printf("未找到车牌为%s的车\n", CarID);
    return 1; // 未找到，离开处理完成
}


int main() {
    printf("停车场车位数:%d 停车场停车每小时%d元\n", MAX, RATE);
    int num = 0;
    printf("请输入操作次数: ");
    scanf("%d", &num);

    Garage garages[MAX];
    for (int i = 0; i < MAX; i++) {
        garages[i].TOP = -1; // 初始化每个栈
    }
    Street* front = NULL, * rear = NULL;

    for (int i = 0; i < num; i++) {
        char state[10] = { 0 }, CarID[10] = { 0 };
        int time;
        Car newCar;

        while (1) { // 循环输入，直到输入正确
            printf("请输入车辆状态（arrive/leave）、车牌号和时间:\n ");
            scanf("%s %s %d", state, CarID, &time);

            strcpy(newCar.state, state);
            strcpy(newCar.CarID, CarID);
            newCar.time = time;

            if (strcmp(state, "arrive") == 0) {
                if (isCarIDPresent(garages, MAX, front, CarID)) {
                    printf("错误：车牌为%s的车已在停车场或便道中，请重新输入\n", CarID);
                }
                else {
                    ProcessArrival(newCar, garages, MAX, &front, &rear);
                    break;
                }
            }
            else if (strcmp(state, "leave") == 0) {
                if (ProcessDeparture(CarID, time, garages, MAX, &front, &rear)) {
                    break;
                }
            }
            
            else {
                printf("无效的状态输入，请重新输入\n");
            }
        }
    }

    // 释放资源
    while (front) front = Dequeue(front);

    return 0;
}
