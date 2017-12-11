
#include <windows.h>
#include <conio.h>
#include <tlhelp32.h>
#include <iostream>
#include <locale>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
/* Program for Windows*/
#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
using namespace std;


int Sockets(int pid)
{
	PMIB_TCPTABLE2 pTcpTable;
	ULONG ulSize = 0;
	DWORD dwRetVal = 0;

	char szLocalAddr[128];
	char szRemoteAddr[128];

	struct in_addr IpAddr;

	int i;

	pTcpTable = (MIB_TCPTABLE2 *)MALLOC(sizeof(MIB_TCPTABLE2));
	if (pTcpTable == NULL) {
		printf("Ошибка выделения памяти\n");
		return 1;
	}

	ulSize = sizeof(MIB_TCPTABLE);
	if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(pTcpTable);
		pTcpTable = (MIB_TCPTABLE2 *)MALLOC(ulSize);
		if (pTcpTable == NULL) {
			printf("Ошибка выделения памяти\n");
			return 1;
		}
	}
	if ((dwRetVal = GetTcpTable2(pTcpTable, &ulSize, TRUE)) == NO_ERROR) {
		for (i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
			if (pTcpTable->table[i].dwOwningPid == pid)
			{
				printf("\n\tTCP Состояние: ");
				switch (pTcpTable->table[i].dwState) {
				case MIB_TCP_STATE_CLOSED:
					printf("CLOSED(*Закрыт. Сокет не используется.)\n");
					break;
				case MIB_TCP_STATE_LISTEN:
					printf("LISTEN(Ожидает входящих соединений).\n");
					break;
				case MIB_TCP_STATE_SYN_SENT:
					printf("SYN-SENT(сокет активно пытается установить соединение с дистанционным узлом)\n");
					break;
				case MIB_TCP_STATE_SYN_RCVD:
					printf("SYN-RECEIVED(соединение устанавливается)\n");
					break;
				case MIB_TCP_STATE_ESTAB:
					printf("ESTABLISHED(соединение установлено)\n");
					break;
				case MIB_TCP_STATE_FIN_WAIT1:
					printf("FIN-WAIT-1(сокет закрылся и ждет разрыва соединения)\n");
					break;
				case MIB_TCP_STATE_FIN_WAIT2:
					printf("FIN-WAIT-2(соединение закрылось; сокет ожидает закрытия со стороны дистанционного узла) \n");
					break;
				case MIB_TCP_STATE_CLOSE_WAIT:
					printf("CLOSE-WAIT(дистанционный сокет закрыл соединение, локальный сокет тоже ожидает закрытия)\n");
					break;
				case MIB_TCP_STATE_CLOSING:
					printf("CLOSING(оба сокета закрыли соединение, но не все отправленные данные получены)\n");
					break;
				case MIB_TCP_STATE_LAST_ACK:
					printf("LAST-ACK(дистанционный сокет завершил соединение, локальный сокет ожидает подтверждения)\n");
					break;
				case MIB_TCP_STATE_TIME_WAIT:
					printf("TIME-WAIT(Сокет закрыт, но ожидает пакеты, ещё находящиеся в сети для обработки)\n");
					break;
				case MIB_TCP_STATE_DELETE_TCB:
					printf("DELETE-TCB\n");
					break;
				default:
					printf("Неизвестное состояние\n");
					break;
				}
				IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwLocalAddr;
				strcpy_s(szLocalAddr, sizeof(szLocalAddr), inet_ntoa(IpAddr));
				printf("\tTCP Локальный адрес:   %s\n", szLocalAddr);

				printf("\tTCP Порт:  %d \n", ntohs((u_short)pTcpTable->table[i].dwLocalPort));

				IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwRemoteAddr;
				strcpy_s(szRemoteAddr, sizeof(szRemoteAddr), inet_ntoa(IpAddr));
				printf("\tTCP Удаленный адрес:    %s\n", szRemoteAddr);
				printf("\tTCP Порт:    %d\n", ntohs((u_short)pTcpTable->table[i].dwRemotePort));

				printf("\tTCP PID:    %d\n", pTcpTable->table[i].dwOwningPid);
				printf("\tTCP Состояние: ");
				switch (pTcpTable->table[i].dwOffloadState) {
				case TcpConnectionOffloadStateInHost:
					printf("      Владеет сетевым стеком, но не выгружается\n\n\n");
					break;
				case TcpConnectionOffloadStateOffloading:
					printf("     В процессе выгрузки\n\n\n");
					break;
				case TcpConnectionOffloadStateOffloaded:
					printf("      Выгружается в управление сетевым интерфейсом\n\n\n");
					break;
				case TcpConnectionOffloadStateUploading:
					printf("     В процессе загрузки в сетевой стек \n\n\n");
					break;
				default:
					printf("      Неизвестное значение состояния разгрузки\n\n\n");
					break;
				}
			}
		}
	}
	else {
		printf("\tGetTcpTable2 произошла ошибка с %d\n", dwRetVal);
		FREE(pTcpTable);
		return 1;
	}

	if (pTcpTable != NULL) {
		FREE(pTcpTable);
		pTcpTable = NULL;
	}
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Russian");
	int m_count = 0, number;
	char name[200];
	int ch;
	bool flag = false;
	while (true)
	{
		cout << "-----Меню-----" << endl;
		cout << "1.Вывести список всех открытых сокетов, указанного процесса." << endl;
		cout << "2.Справка" << endl;
		cout << "Для выхода нажмите Esc." << endl;
		ch = getch();
		if (ch == 27)
			break;
		cin >> number;
		switch (number)
		{
		case 1:
			while (true)
			{
				system("cls");
				cout << "Введите название процесса: ";
				cin >> name;

				HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
				if (snapshot != INVALID_HANDLE_VALUE)
				{
					PROCESSENTRY32 process;
					process.dwSize = sizeof(PROCESSENTRY32);
					if (Process32First(snapshot, &process))
					{
						do {
							m_count++;
							if (strcmp(process.szExeFile, name) == 0)
							{
								flag = true;
								if (!flag)
									cout << "Список всех открытых сокетов\n";
								Sockets(process.th32ProcessID);
							}
						} while (Process32Next(snapshot, &process));
					}
					else
					{
						cout << "Возникла ошибка, попробуйте снова\n" << endl;
					}
				}
				else
				{
				}
				if (flag == false)
				{
					cout << "Неверное имя процесса\n";
				}
				CloseHandle(snapshot);
				cout << "Для выхода в меню нажмите Esc.\nДля того чтобы продолжить - любую другую клавишу\n";
				ch = getch();
				if (ch == 27)
					break;
			}
			system("cls");
			break;
		case 2:
			system("cls");
			cout << "Выполнил: Константинов Семен студент группы БИс-41\n\nПрограмма предназначена для получения всех откртых сокетов указанного процесса." << endl << endl;
			break;
		default:
			system("cls");
			cout << "Неверно указан пункт меню! Нажмите 1 или 2\n" << endl;
			break;
		}
	}
	return 0;
}
