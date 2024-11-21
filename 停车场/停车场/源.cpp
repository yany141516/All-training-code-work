#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 2 // ͣ���������ĳ�λ����
#define SIZE 1 // ÿ����λ������
#define RATE 2 // ÿСʱ�շ�2Ԫ

// ����������Ϣ�ṹ
typedef struct {
    char state[10];
    char CarID[10];
    int time;
} Car;

// ͣ����ջ�ṹ
typedef struct {
    Car car[SIZE];
    int TOP;
} Garage;

// �����������ڵ�
typedef struct street {
    Car car;
    struct street* next;
} Street;

// ��������
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


// ��������ջ
Garage* CreateGarage() {
    Garage* g = (Garage*)malloc(sizeof(Garage));
    g->TOP = -1;
    return g;
}

// ���ջ�Ƿ���
int isFull(Garage* garage) {
    return (garage->TOP == SIZE - 1);
}

// ���ջ�Ƿ��
int isEmpty(Garage* garage) {
    return (garage->TOP == -1);
}

// ����ͣ��ջ
void Push(Car car, Garage* garage) {
    garage->car[++garage->TOP] = car;
   // printf("����Ϊ%s�ĳ���ͣ��ͣ��������λλ��Ϊ%d\n", car.CarID, garage->TOP + 1);
}

// ������ջ�Ƴ�
Car Pop(Garage* garage) {
    if (isEmpty(garage)) {
        printf("����ͣ����Ϊ��\n");
        exit(1);
    }
    return garage->car[garage->TOP--];
}

// ��������������
Street* Enqueue(Street* rear, Car car) {
    Street* newNode = (Street*)malloc(sizeof(Street));
    newNode->car = car;
    newNode->next = NULL;
    if (rear) rear->next = newNode;
    return newNode;
}

// �����ӱ�������Ƴ�
Street* Dequeue(Street* front) {
    if (!front) return NULL;
    Street* temp = front;
    front = front->next;
    printf("����Ϊ%s�ĳ��Ѵӱ���뿪\n", temp->car.CarID);
    free(temp);
    return front;
}

// ���ͣ�����ͱ�����Ƿ�������ͬ����
int isCarIDPresent(Garage garages[], int numGarages, Street* front, char* CarID) {
    // �������ջ
    for (int i = 0; i < numGarages; i++) {
        for (int j = 0; j <= garages[i].TOP; j++) {
            if (strcmp(garages[i].car[j].CarID, CarID) == 0) {
                return 1; // �ҵ���ͬ����
            }
        }
    }
    // ���������
    Street* current = front;
    while (current) {
        if (strcmp(current->car.CarID, CarID) == 0) {
            return 1; // �ҵ���ͬ����
        }
        current = current->next;
    }
    return 0; // û���ظ�����
}


void DisplayStatus(Garage garages[], int numGarages, Street* front) {
    int emptySlots = 0; // ͳ�ƿճ�λ��
    for (int i = 0; i < numGarages; i++) {
        emptySlots += (SIZE - (garages[i].TOP + 1));
    }

    int queueCount = 0; // ͳ�Ʊ��������
    Street* current = front;
    while (current) {
        queueCount++;
        current = current->next;
    }

    printf("��ǰ�ճ�λ��: %d\n", emptySlots);
    printf("��ǰ���������: %d\n", queueCount);
}


// �������������߼�
int ProcessArrival(Car car, Garage garages[], int numGarages, Street** front, Street** rear) {
    for (int i = 0; i < numGarages; i++) {
        if (!isFull(&garages[i])) {
            Push(car, &garages[i]);
            printf("����Ϊ%s�ĳ�ͣ��ͣ�����ĵ�%d����λ\n", car.CarID, i + 1);
            DisplayStatus(garages, numGarages, *front); // ��ʾ״̬
            return 1; // �ɹ�ͣ��ͣ����
        }
    }
    // �������ջ������������
    *rear = Enqueue(*rear, car);
    if (*front == NULL) *front = *rear; // ����Ϊ��ʱ����frontָ��
    printf("ͣ��������������Ϊ%s�ĳ�ͣ����\n", car.CarID);
    DisplayStatus(garages, numGarages, *front); // ��ʾ״̬
    return 1; // �ɹ�����
}

int ProcessDeparture(char* CarID, int time, Garage garages[], int numGarages, Street** front, Street** rear) {
    
    int leave_time = 0;

    // �������ջ
    for (int i = 0; i < numGarages; i++) {
        for (int j = 0; j <= garages[i].TOP; j++) {
            if (strcmp(garages[i].car[j].CarID, CarID) == 0) {
                Car departingCar = garages[i].car[j];
                int duration = time - departingCar.time;

                if (duration < 0) {
                    printf("���󣺳���Ϊ%s�ĳ��뿪ʱ�����ڵ���ʱ�䣬ͣ��ʱ������С��0\n", CarID);
                    return 0; // ʱ�������������
                }

                leave_time = time;//�洢��ȥʱ��

                int fee = duration * RATE;
                printf("����Ϊ%s�ĳ���ͣ������%d����λ�뿪��ͣ��ʱ��Ϊ%dСʱ��Ӧ�ɷ�%dԪ\n", CarID, i + 1, duration, fee);

                // ����ջ
                for (int k = j; k < garages[i].TOP; k++) {
                    garages[i].car[k] = garages[i].car[k + 1];
                }
                garages[i].TOP--;

                // �������г��������׳�������ͣ����
                if (*front) {
                    (*front)->car.time = leave_time;
                    Car movedCar = (*front)->car;
                    *front = Dequeue(*front);
                    if (!*front) *rear = NULL; // ���п�ʱ����rearָ��
                    
                    ProcessArrival(movedCar, garages, numGarages, front, rear);
                }

                DisplayStatus(garages, numGarages, *front); // ��ʾ״̬
                return 1; // �ɹ��뿪
            }
        }
    }

    // �����
    Street* current = *front;
    while (current) {
        if (strcmp(current->car.CarID, CarID) == 0) {
            printf("����Ϊ%s�ĳ��ڱ���У�ͣ��ʱ�䲻�շ�\n", CarID);
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
            DisplayStatus(garages, numGarages, *front); // ��ʾ״̬
            return 1; // �뿪�������
        }
        current = current->next;
    }
    printf("δ�ҵ�����Ϊ%s�ĳ�\n", CarID);
    return 1; // δ�ҵ����뿪�������
}


int main() {
    printf("ͣ������λ��:%d ͣ����ͣ��ÿСʱ%dԪ\n", MAX, RATE);
    int num = 0;
    printf("�������������: ");
    scanf("%d", &num);

    Garage garages[MAX];
    for (int i = 0; i < MAX; i++) {
        garages[i].TOP = -1; // ��ʼ��ÿ��ջ
    }
    Street* front = NULL, * rear = NULL;

    for (int i = 0; i < num; i++) {
        char state[10] = { 0 }, CarID[10] = { 0 };
        int time;
        Car newCar;

        while (1) { // ѭ�����룬ֱ��������ȷ
            printf("�����복��״̬��arrive/leave�������ƺź�ʱ��:\n ");
            scanf("%s %s %d", state, CarID, &time);

            strcpy(newCar.state, state);
            strcpy(newCar.CarID, CarID);
            newCar.time = time;

            if (strcmp(state, "arrive") == 0) {
                if (isCarIDPresent(garages, MAX, front, CarID)) {
                    printf("���󣺳���Ϊ%s�ĳ�����ͣ���������У�����������\n", CarID);
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
                printf("��Ч��״̬���룬����������\n");
            }
        }
    }

    // �ͷ���Դ
    while (front) front = Dequeue(front);

    return 0;
}
