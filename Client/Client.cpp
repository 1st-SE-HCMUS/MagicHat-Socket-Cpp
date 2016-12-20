// Client.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include "Client.h"
#include "afxsock.h"
#include <iostream>
#include <vector>
#include "conio.h"
using namespace std;


#define NAME_MAX_LENGTH 30
#define BUFF_MAX_SIZE 100

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char* parseCrosswordInfo(char* info, int& _numberOfPlayer, int& _turn, int& _crosswordLength)
{
	_numberOfPlayer = atoi(strtok(info, "/"));
	_turn = atoi(strtok(NULL, "/"));
	_crosswordLength = atoi(strtok(NULL, "/"));
	return strtok(NULL, "/");
}

char* parseGuessWordPosition(char* appearance)
{
	char* result = new char[100];
	char playerName[30];
	int startIndex = 0;
	int len = strlen(appearance);

	for (int i = 2; i < len; i++)
	{
		if (appearance[i] == '/')
		{
			playerName[startIndex] = '\0';
			startIndex = i + 1;
			break;
		}
		playerName[startIndex] = appearance[i];
		startIndex++;
	}

	strcpy_s(result, sizeof(playerName), playerName);
	result = strcat(result, " vua doan ky tu '");
	result = strcat(result, &appearance[len - 1]);


	int sum = 0;
	result = strcat(result, "'. Vi tri:");

	//Bo 'A' va '/' va ky tu cuoi 
	for (int i = startIndex; i < len - 1; i++)
	{
		if (appearance[i] == '/')
		{
			char c_sum[10];

			_itoa_s(sum, c_sum, 10);
			result = strcat(result, " ");
			result = strcat(result, c_sum);
			sum = 0;
		}
		else
			sum = sum * 10 + appearance[i] - '0';
	}


	return result;

}

char* parseGuessString(char* appearance)
{
	//Duyet tu ten nguoi choi
	char name[10];
	int index = 0;
	int len = strlen(appearance);
	for (int i = 2; i < len; i++)
	{
		if (appearance[i] == '/')
		{
			name[index] = '\0';
			index = i + 1;
			break;
		}
		else
		{
			name[index] = appearance[i];
			index++;
		}
	}

	char* result = new char[100];
	strcpy_s(result, sizeof(name), name);
	result = strcat(result, " doan o chu: '");
	result = strcat(result, (appearance + index * sizeof(char)));
	result = strcat(result, "' nhung khong chinh xac!");
	return result;
}

bool isValidName(char* _name)
{
	if (strlen(_name) == 0)
	{
		return false;
	}

	for (int i = 0; i < strlen(_name); i++)
	{
		if (!((_name[i] >= 'a' && _name[i] <= 'z') || (_name[i] >= 'A' && _name[i] <= 'Z') || (_name[i] >= '0' && _name[i] <= '9')))
		{
			//Ten chi duoc cau thanh tu nhung chu cai 'a' ... 'z' hoac 'A' ... 'Z' hoac '0' ... '9'
			return false;
		}
	}

	return true;
}

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			//////////////////////////////////////////////////////////////////////////
			//Credits
			//////////////////////////////////////////////////////////////////////////
			cout << "\t\t\t\t\t      GAME CHIEC NON KY DIEU" << endl << endl;


			//////////////////////////////////////////////////////////////////////////
			//Khai bao
			//////////////////////////////////////////////////////////////////////////
			//Khoi tao bien socket cua client
			CSocket client;
			char* clientName;
			int turn = 0;
			int numberOfPlayer = 0;
			int crosswordLength = 0;
			int point = 0;
			int tmpLen;
			char* hint;
			char temp[BUFF_MAX_SIZE];


			//////////////////////////////////////////////////////////////////////////
			//Khoi tao
			//////////////////////////////////////////////////////////////////////////
			AfxSocketInit(NULL);
			client.Create();
			
			srand(time(NULL)); //Srand som de lay time khac nhau giua cac client


			//////////////////////////////////////////////////////////////////////////
			//Connect toi server
			//////////////////////////////////////////////////////////////////////////
			if (client.Connect(_T("127.0.0.1"), 7765))
			{
				//Nhap den khi nao hop le
				do 
				{
					cout << " --> Nhap ten: ";
					gets_s(temp);
					
					while (!isValidName(temp))
					{
						cout << "  - MagicHat: Khong hop le! Ten chi gom cac ky tu 'a' .. 'z', 'A' .. 'Z', '0' .. '9'. Nhap lai: ";
						gets_s(temp);
					}
					
					int nameLength = strlen(temp);
					temp[nameLength] = 0;

					client.Send(temp, nameLength, 0);
					
					tmpLen = client.Receive(temp, 10);
					temp[tmpLen] = '\0';

					if (strcmp(temp, "0") != 0)
					{
						break;
					}
					cout << "  - MagicHat: Ten nguoi choi da ton tai!" << endl;
				} 
				while (true);
				
				//Dang ky voi server thanh cong
				turn = atoi(temp);

				cout << "  - MagicHat: Ket noi den server thanh cong!" << endl;

				//Khoi tao diem
				client.Receive(temp, 10);
				point = atoi(temp);
				if (point == 0)
				{
					client.Send("ok", sizeof("ok"));
					cout << "  - MagicHat: Server khoi tao diem: " << point << endl;
				}
				else
					client.Send("error", sizeof("error"));
				

				//Nhan thong tin o chu
				tmpLen = client.Receive(temp, BUFF_MAX_SIZE,0);
				temp[tmpLen] = '\0';
				
				if (temp != NULL && tmpLen > 0)
				{
					client.Send("ok", sizeof("ok"));
				}
				else
					client.Send("error", sizeof("error"));

				//Parse ket qua ra
				hint = parseCrosswordInfo(temp, numberOfPlayer, turn, crosswordLength);

				//In thong tin vong choi
				cout << endl;
				cout << "\t\t\t\t\t     THONG TIN CUA VONG CHOI NAY" << endl;
				cout << "  - So nguoi choi: " << numberOfPlayer << endl;
				cout << "  - Luot choi cua ban: " << turn << endl;
				cout << "  - Do dai cua o chu: " << crosswordLength << endl;
				cout << "  - Goi y: ";
				puts(hint);

				//////////////////////////////////////////////////////////////////////////
				//Bat dau game
				//////////////////////////////////////////////////////////////////////////
				cout << "\t\t\t\t\t     GAME START!" << endl;
				
				int rollPoint = 0; //Bien chua diem D
				while (true)
				{
					//Nhan thong tin tu server xem minh phai lam gi
					tmpLen = client.Receive(temp, BUFF_MAX_SIZE);
					temp[tmpLen] = '\0';

					//////////////////////////////////////////////////////////////////////////
					//cout << "Bat dau lai vong while: ";
					//puts(temp);
					
					cout << endl;
					
					//Neu den luot choi
					if (strcmp(temp, "turn") == 0)
					{
						//Quay so random
						rollPoint = rand() % 101;

						cout << "  - MagicHat: Ban quay duoc: " << rollPoint << " diem." << endl;

						char c_rollPoint[10];
						
						//Convert sang char*
						_itoa_s(rollPoint, c_rollPoint, 10); 
						//Gui ve server
						client.Send(c_rollPoint, strlen(c_rollPoint) + 1); //Vua them +1 o day

						continue;
					}
					else if (strcmp(temp, "guess") == 0) //Den luot doan
					{
						char guess[BUFF_MAX_SIZE];
						do 
						{
							cout << " --> Nhap ky tu muon doan (hoac ca o chu): ";
							gets_s(guess);
						} while (strlen(guess) == 0 || guess == NULL);

						cout << endl;

						//Gui ky tu (hoac o chu) vua doan ve server
						client.Send(guess, strlen(guess));

						//Cho nhan thong tin tra ve, khoi tao bien guessResponse
						char guessResponse[BUFF_MAX_SIZE];
						tmpLen = client.Receive(guessResponse, BUFF_MAX_SIZE);
						guessResponse[tmpLen] = '\0';

						//Ky tu ton tai trong o chu
						if (guessResponse[0] == 'A')
						{
							//Phan tich doan A/pos/pos.../char ra de lay thong tin
							
							//Tang diem
							point += rollPoint;

							cout << "  - MagicHat: Ban doan dung! So diem hien tai: " << point << endl;
							client.Send("ok", sizeof("ok"));
							continue;
						}
						else if (strcmp(guessResponse, "WIN") != 0 && (guessResponse[0] == 'D' || guessResponse[0] == 'W'))
						{
							//Doan sai (sai ky tu hoac sai o chu)
							cout << "  - MagicHat: Ban doan sai." << endl;
							client.Send("ok", sizeof("ok"));
							continue;
						}
						else if (strcmp(guessResponse, "WIN") == 0) //Doan dung o chu
						{
							//Win x3 so diem da quay
							point += rollPoint * 3;
							cout << "  - MagicHat: Ban da doan thanh cong o chu. So diem hien tai: " << point << endl;
							goto EndGame;
						}
						else
							cout << "  ***SERVER ERROR*** Gui guess ma chua nhan duoc phan hoi!" << endl;


					}
					else if (temp[0] == 'A')
					{
						//Nguoi choi khac doan, va vi tri xuat hien cua ky tu trong o chu
						//Phan tich doan A/pos/pos.../char ra de lay thong tin
						cout << "  - MagicHat: ";
						puts(parseGuessWordPosition(temp));
						client.Send("ok", sizeof("ok"));
						continue;
					}
					else if (temp[0] == 'D')
					{
						//Doan sai ky tu
						cout << "  - MagicHat: Nguoi choi doan ky tu " << temp[tmpLen - 1] << ". Nhung khong xuat hien trong o chu!" << endl;
						client.Send("ok", sizeof("ok"));
						continue;
					}
					else if (strcmp(temp, "WIN") != 0 && temp[0] == 'W')
					{
						//Doan sai o chu
						cout << "  - MagicHat: Nguoi choi ";
						puts(parseGuessString(temp));
						client.Send("ok", sizeof("ok")); // gui thong bao da nhan
					}
					else if (temp == NULL && tmpLen <= 0)
					{
						client.Send("error", sizeof("error"));
						continue;
					}
					else if (strcmp(temp,"WIN") == 0)
					{
						//O chu da duoc 1 ng choi khac doan. Ket thuc game
						cout << "  - MagicHat: O chu da duoc giai. Tro choi ket thuc!" << endl;
						goto EndGame;
					}
					else if (strcmp(temp, "LIMIT") == 0)
					{
						//Da het luot doan
						cout << "  - MagicHat: So luot doan vuot qua gioi han! Tro choi ket thuc!" << endl;
						goto EndGame;

					}
					else if (temp[0] == 'L') //Mat luot
					{
						cout << "  - MagicHat: " << (temp + sizeof(char)*2) << " bi mat luot! " << endl;
						//////////////////////////////////////////////////////////////////////////
						client.Send("ok", sizeof("ok"));
						continue;
					}

				}

				//Label endgame
			EndGame:
				tmpLen = client.Receive(temp, 10);
				temp[tmpLen] = '\0';

				//Neu server yeu cau gui diem
				if (strcmp(temp,"POINT") == 0)
				{
					char c_Point[10];
					_itoa_s(point, c_Point, 10);
					client.Send(c_Point, strlen(c_Point));
				}

				//Tong ket diem
				cout << "  - MagicHat: So diem cua ban: " << point << endl << endl;

				//Dong client
				client.Close();
				system("pause");
			}
			else
			{
				cout << "  - MagicHat: Khong the ket noi den server!";
				system("pause");
			}
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
