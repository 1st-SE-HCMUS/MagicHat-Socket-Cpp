// Server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Server.h"
#include "afxsock.h"
#include <iostream>
#include <string>
#include <vector>
#include "conio.h"
#include <fstream>
using namespace std;

//Macro define
#define N 3
#define NAME_MAX_LENGTH 30
#define BUFF_MAX_SIZE 100
#define MAX_GUESS_TIME 5
#define DATA_FILE_PATH "crossword.txt"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Khai bao class Crossword
class Crossword
{
public:
	Crossword()
	{
		crossword = "";
		hint = "";
		guessLimit = 0;
	}

	Crossword(string _crossword, string _hint, int _guessLimit)
	{
		this->crossword = _crossword;
		this->hint = _hint;
		this->guessLimit = _guessLimit;
	}
	string crossword;
	string hint;
	int guessLimit;

	char* getCrossword()
	{
		const char* tempChar = new char[crossword.length()];

		tempChar = crossword.c_str();

		char* result = new char[crossword.length()];
		strcpy_s(result, sizeof(crossword), tempChar);

		return result;
	}

	char* getHint()
	{
		const char* tempChar = new char[hint.length()];

		tempChar = hint.c_str();

		char* result = new char[hint.length()];
		strcpy_s(result, sizeof(hint), tempChar);

		return result;
	}

	int getGuessLimit()
	{
		return this->guessLimit;
	}

	int getLength()
	{
		return crossword.length();
	}
};


vector<Crossword> crosswordFileInput(string _path)
{
	ifstream inFile;
	inFile.open("crossword.txt");
	vector<Crossword> _arr;
	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			string line;
			Crossword cw;
			if (getline(inFile, line))
			{
				cw.crossword = line;

				if (getline(inFile, line))
				{
					cw.hint = line;
				}

				if (getline(inFile, line))
				{
					cw.guessLimit = stoi(line);
				}

				_arr.push_back(cw);
			}
			else
				cout << "Loi: File du lieu sai format!" << endl;
		}

		return _arr;
		inFile.close();


	}

	cout << "Error opening file! " << endl;
	return{};
}

//Kiem tra ten dang nhap co hop le hay khong
bool isValidName(char* _name, vector<char*> _listClientsName)
{
	//Kiem tra ten co null hay khong, hay dat ten rong
	if (_name == NULL || strcmp(_name, " ") == 0 || strcmp(_name, " ") == 0)
	{
		return false;
	}

	//Duyet qua vector chua ten
	for (int i = 0; i < _listClientsName.size(); i++)
	{
		//Neu phat hien trung, tra ve false
		if (strcmp(_name, _listClientsName[i]) == 0)
		{
			return false;
		}
	}
	return true;
}

Crossword getCrossWord(vector<Crossword> _arr)
{
	if (_arr.size() == 0)
	{
		cout << "Khong tim thay o chu! Dang thu lay lai du lieu..." << endl;
		_arr = crosswordFileInput(DATA_FILE_PATH);

		if (_arr.size() == 0)
		{
			cout << "Lay lai du lieu khong thanh cong! Thoat game..." << endl;
			system("pause");
			exit(0);
		}
	}

	//Da co du lieu
	//Tien hanh random lay 1 de
	srand(time(NULL));
	int index = rand() % _arr.size();

	return _arr[index];
}

char* crosswordInfoConstruct(int _numberOfPlayer, int _turn ,int _crosswordLength, char* hint)
{
	string temp = to_string(_numberOfPlayer) + "/" + to_string(_turn) + "/" + to_string(_crosswordLength) + "/" + string(hint);

	const char* tempChar = new char[temp.length()];

	tempChar = temp.c_str();

	char* result = new char[temp.length()];
	strcpy_s(result, sizeof(temp), tempChar);

	return result;
}

bool checkWin(int* _guessed, int _crosswordLength)
{
	for (int i = 0; i < _crosswordLength; i++)
	{
		if (_guessed[i] == 0)
		{
			//Co tu con chua dc doan
			return false; 
		}
	}
	return true;
}

bool checkGuessLimitExceeded(int _guess, int _maxTimes)
{
	if (_guess >= _maxTimes)
	{
		return true;
	}
	return false;
}

char* printScoreTable(int* _array, int n, vector<char*> _listClientsName)
{
	char* winner = new char[NAME_MAX_LENGTH];
	int _count = 0;
	while (_count < n)
	{
		int _max = _array[0];
		int _maxIndex = 0;
		for (int i = 1; i < n; i++)
		{
			if (_array[i] > _max)
			{
				_max = _array[i];
				_maxIndex = i;
			}
		}
		//Tim ra duoc max
		_count++;
		if (_count == 1)
		{
			strcpy_s(winner, sizeof(_listClientsName[_maxIndex]), _listClientsName[_maxIndex]);
		}
		cout << _count << " . " << _listClientsName[_maxIndex] << ": " << _max << " diem." << endl;

		//Gan max = 0
		_array[_maxIndex] = 0;
	}

	return winner;
}

void printRoundInfomation(char* _crossword, int _numberOfPlayer, int _length, char* _hint, int _maxGuessLimit)
{
	cout << endl;
	cout << "\t\t\t\t\t     THONG TIN CUA VONG CHOI" << endl;
	cout << "  - O chu: ";
	puts(_crossword);
	cout << "  - So nguoi choi: " << _numberOfPlayer << endl;
	cout << "  - Do dai cua o chu: " << _length << endl;
	cout << "  - Goi y: ";
	puts(_hint);
	cout << "  - So luot doan toi da: " << _maxGuessLimit << endl;
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

			cout << "\t\t\t\t\t       BO MON MANG MAY TINH" << endl;
			cout << "\t\t\t\t\t ----- DO AN LAP TRINH SOCKET ----- " << endl;
			cout << "\t\t\t\t\t ----- GAME CHIEC NON KY DIEU ----- " << endl << endl;
			cout << "Thong tin nhom: " << endl;
			cout << "  - Doan Hieu Tam: 1412477" << endl;
			cout << "  - Nguyen Dinh Son: 1412465" << endl << endl;

			//////////////////////////////////////////////////////////////////////////
			//Khai bao
			//////////////////////////////////////////////////////////////////////////

			//Khai bao cac bien socket
			CSocket server;
			CSocket* listClient;
			//Khai bao danh sach cac o chu
			vector<Crossword> listCrossword;
			//Khai bao bien
			int numberOfPlayer = N;
			int tmpLen;
			int* guessed;
			bool isWin = false;
			bool isExceedGuessLimit = false;
			int maxGuessTimes = MAX_GUESS_TIME;
			int numberOfGuessedTimes = 0;
			//Khai bao bien o chu
			Crossword crossword;
			char* c_Crossword;
			char temp[BUFF_MAX_SIZE]; //Bien tam de gui va nhan thong tin trong chuong trinh
			int* listClientPoint; //Bien chua diem cua cac client

			//char** listClientsName = new char*[numberOfPlayer];


			//////////////////////////////////////////////////////////////////////////
			//Khoi tao
			//////////////////////////////////////////////////////////////////////////

			//Lay so luong nguoi choi
			cout << "  --> Nhap so luong nguoi choi: ";
			cin >> numberOfPlayer;

			//Khoi tao Windows Socket voi tham so lpwsaData bang NULL
			AfxSocketInit(NULL);
			//Tao socket voi PORT bang 7765
			server.Create(7765); //mac dinh nSocketType = 1 = SOCK_STREAM = TCP
			
			cout << "  - Server: Khoi tao server thanh cong! Dang lang nghe ket noi tu Clients..." << endl;
			server.Listen(10); //backlog = 10, chap nhan 10 ket noi dang cho ma chua dc accept
			
			listClient = new CSocket[numberOfPlayer];
			vector<char*> listClientsName(numberOfPlayer, "na");
			

			//Cho va chap nhan ket noi tu client, du N nguoi choi moi bat dau choi
			for (int i = 0; i < numberOfPlayer; i++)
			{
				//Chap nhan ket noi tu client thu [i], kieu CAsyncSocket&
				server.Accept(listClient[i]);
				
				//Kiem tra den khi nao nhan duoc ten hop le (khong trung)
				while (true)
				{
					//Nhan ten tu client gui ve
					tmpLen = listClient[i].Receive(temp, NAME_MAX_LENGTH); //Default flag = 0
					temp[tmpLen] = '\0';

					if (isValidName(temp, listClientsName))
					{
						//Ten hop le
						listClientsName[i] = new char[strlen(temp)];
						strcpy_s(listClientsName[i], sizeof(temp), temp);
						
						cout << "  - Server: Nguoi choi " << listClientsName[i] << " vua ket noi!" << endl;

						//Chep so thu tu vao bien tam de gui cho client
						//Vua de xac nhan viec dang ky thanh cong, vua de xac dinh thu tu luot choi cua client
						_itoa_s(i + 1, temp, 10);
						listClient[i].Send(temp, strlen(temp) + 1);
						break;
					}
					else
					{
						//Ten khong hop le, gui tin hieu phan hoi ve client
						//strcpy_s(temp, sizeof("0"), "0");
						listClient[i].Send("0", strlen("0")); 
						continue;
					}
				}
			}

			//Khoi tao diem va gui cho nguoi choi
			for (int i = 0; i < numberOfPlayer; i++)
			{

				char ack[BUFF_MAX_SIZE];
				listClient[i].Send("0", strlen("0")); //Khoi tao diem
				tmpLen = listClient[i].Receive(ack, BUFF_MAX_SIZE);
				ack[tmpLen] = '\0';

				if (strcmp(ack, "ok") != 0)
				{
					cout << "  ***CLIENT ERROR*** Client " << i << " nhan that bai!" << endl;
				}


			}


			//Lay o chu tu file
			listCrossword = crosswordFileInput(DATA_FILE_PATH);
			crossword = getCrossWord(listCrossword);
			//Gan cho bien chuoi o chu
			c_Crossword = crossword.getCrossword();
			
			//In thong tin cua vong choi
			printRoundInfomation(c_Crossword,numberOfPlayer,crossword.getLength(),crossword.getHint(),crossword.getGuessLimit());
			
			//ghep thong tin o chu va gui cho cac client
			for (int i = 0; i < numberOfPlayer; i++)
			{
				char ack[BUFF_MAX_SIZE];
				char* info = crosswordInfoConstruct(numberOfPlayer, i + 1, crossword.getLength(), crossword.getHint());
				//cout << info << " " << strlen(info) << endl;
				listClient[i].Send(info, strlen(info));
				tmpLen = listClient[i].Receive(ack, BUFF_MAX_SIZE);
				ack[tmpLen] = '\0';

				if (strcmp(ack, "ok") != 0)
				{
					cout << "  ***CLIENT ERROR*** Client " << i << " nhan that bai!" << endl;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			//Bat dau tro choi
			//////////////////////////////////////////////////////////////////////////
			cout << endl;
			cout << "\t\t\t\t\t     GAME START!" << endl;

			//Khoi tao gia tri
			numberOfGuessedTimes = 0;
			isExceedGuessLimit = false;
			isWin = false;
			guessed = new int[strlen(c_Crossword)];
			for (int index = 0; index < strlen(c_Crossword); index++)
			{
				guessed[index] = 0;
			}


			//////////////////////////////////////////////////////////////////////////
			//Game loop
			//////////////////////////////////////////////////////////////////////////
			while (true)
			{
				//Duyet lan luot qua tung nguoi choi
				for (int i = 0; i < numberOfPlayer; i++)
				{
					//Kiem tra o chu da duoc giai het chua, hoac da ai doan dung o chu chua
					if (checkWin(guessed, strlen(c_Crossword)))
					{
						//O chu da duoc lat het
						cout << "  - Server: O chu da duoc lat het! Dap an: " << c_Crossword << endl;

						for (int m = 0; m < numberOfPlayer; m++)
						{
							listClient[m].Send("WIN", strlen("WIN"));
						}
						
						//Goto
						isWin = true;
						goto EndGame;
					}
					else if (checkGuessLimitExceeded(numberOfGuessedTimes, maxGuessTimes)) //Kiem ra co vuot qua so lan doan chua
					{
						//Gui thong bao toi cac client la da het luot choi, endgame
						for (int m = 0; m < numberOfPlayer; m++)
						{
							listClient[m].Send("LIMIT", strlen("LIMIT"));
						}
						
						//Goto
						isExceedGuessLimit = true;
						goto EndGame;
					}
					
					//Khoi tao bien chua diem
					char rollPoint[10];
					
					//////////////////////////////////////////////////////////////////////////
					//Gui luot choi cho client
					//////////////////////////////////////////////////////////////////////////
					listClient[i].Send("turn",strlen("turn"));

					//Nhan thong tin tra ve la diem ma nguoi choi quay duoc
					tmpLen =listClient[i].Receive(rollPoint, 10);
					rollPoint[tmpLen] = '\0';

					cout << "  - Server: Nguoi choi " << listClientsName[i] << " quay duoc " << rollPoint << endl;

					//Neu diem so bang 0 thi mat luot
					if (atoi(rollPoint) == 0)
					{
						//Mat luot
						for (int j = 0; j < numberOfPlayer; j++)
						{
							//j index
							//Gui thong tin nguoi choi bi mat luot cho cac client khac
							char lossTurn[BUFF_MAX_SIZE];
							strcpy_s(lossTurn, sizeof("L/"), "L/");
							strcat(lossTurn, listClientsName[i]);

							listClient[j].Send(lossTurn, strlen(lossTurn)); //Dang "L/name"

							char ack[BUFF_MAX_SIZE];
							tmpLen = listClient[j].Receive(ack, BUFF_MAX_SIZE);
							ack[tmpLen] = '\0';

							if (strcmp(ack, "ok") != 0)
							{
								cout << "  ***CLIENT ERROR*** Client " << j << " nhan that bai!" << endl;
							}
						}

						//Thong bao xong. Bo qua luot.
						//Khong tinh vao so lan doan 
						continue;
					}
					
					//////////////////////////////////////////////////////////////////////////
					//Gui tin hieu cho phep nguoi choi doan chu
					//////////////////////////////////////////////////////////////////////////
					listClient[i].Send("guess", strlen("guess"));
					char guess[BUFF_MAX_SIZE];

					//Tang so luot doan len 1
					numberOfGuessedTimes += 1;

					//Nhan ky tu (hoac o chu) ma nguoi choi gui ve
					tmpLen = listClient[i].Receive(guess, BUFF_MAX_SIZE);
					guess[tmpLen] = '\0';

					//put cai guess ra console kiem tra
					//puts(guess);

					//Kiem tra xem nguoi choi doan ky tu hay doan o chu
					if (strlen(guess) > 1)
					{
						//Doan o chu
						if (strcmp(guess, c_Crossword) == 0)
						{
							//Doan dung o chu
							cout << "  - Server: Nguoi choi doan dungo chu! Tro choi ket thuc!" << endl;

							//Gui tin hieu doan dung o chu cho tat ca nguoi choi, tro choi ket thuc
							for (int m = 0; m < numberOfPlayer; m++)
							{
								listClient[m].Send("WIN", strlen("WIN"));
							}

							//Goto
							isWin = true;
							goto EndGame;
						}
						else
						{
							//Doan sai o chu
							//Gep chuoi thong bao nguoi choi doan sai
							//Chuoi dang "W/name/guess_cross_word"
							char guessResponse[BUFF_MAX_SIZE];
							strcpy_s(guessResponse, sizeof("W"), "W");
							strcat(guessResponse, "/");
							strcat(guessResponse, listClientsName[i]);
							strcat(guessResponse, "/");
							strcat(guessResponse, guess);
							puts(guessResponse);

							//Gui thong tin cho tat ca cac client
							for (int m = 0; m < numberOfPlayer; m++)
							{
								listClient[m].Send(guessResponse, strlen(guessResponse));

								char ack[BUFF_MAX_SIZE];
								tmpLen = listClient[m].Receive(ack, BUFF_MAX_SIZE);
								ack[tmpLen] = '\0';

								if (strcmp(ack, "ok") != 0)
								{
									cout << "  ***CLIENT ERROR*** Client " << m << " nhan that bai!" << endl;
								}
							}
							
						}
					}
					else
					{
						//Doan ky tu
						//Khai bao bien flag da xuat hien, va bien chua chuoi phan hoi ve client
						bool isAppear = false;
						char* appearSend = new char[BUFF_MAX_SIZE];

						//Copy
						strcpy(appearSend, "D");

						//Chen ten nguoi choi
						appearSend = strcat(appearSend, "/");
						appearSend = strcat(appearSend, listClientsName[i]);

						//Duyet qua o chu, tim xem co xuat hien hay khong
						for (int j = 0; j < strlen(c_Crossword); j++)
						{
							//Xuat hien
							if (guess[0] == c_Crossword[j])
							{
								guessed[j] = 1; //Da doan
								char pos[10];
								_itoa_s(j + 1, pos, 10); //Vi tri  bang index + 1
								appearSend = strcat(appearSend, "/");
								appearSend = strcat(appearSend, pos);
								
								isAppear = true;
							}
						}

						//Gui cho cac ng choi khac
						appearSend = strcat(appearSend, "/");
						appearSend = strcat(appearSend, guess);

						//Sua lai prefix
						if (isAppear)
						{
							appearSend[0] = 'A';
						}
						else
							appearSend[0] = 'D';

						//Gui cho tat ca client
						for (int m = 0; m < numberOfPlayer; m++)
						{
							//put chuoi gui di de kiem tra
							//puts(appearSend);
							listClient[m].Send(appearSend, strlen(appearSend));

							char ack[BUFF_MAX_SIZE];
							tmpLen = listClient[m].Receive(ack, BUFF_MAX_SIZE);
							ack[tmpLen] = '\0';

							if (strcmp(ack, "ok") != 0)
							{
								cout << "  ***CLIENT ERROR*** Client " << m << " nhan that bai!" << endl;
							}

						}
					}

				}

				//Den day se lap lai nguoi choi dau tien
			}

			//Lable endgame
		EndGame:
			if (isWin)
			{
				cout << "  - Server: O chu da duoc giai. Tro choi ket thuc!" << endl;
			}
			else if (isExceedGuessLimit)
			{
				cout << "  - Server: Vuot qua so lan doan! Tro choi ket thuc!" << endl;
			}


			listClientPoint = new int[numberOfPlayer];

			//Gui yeu cau lay diem, sau do nhan diem tu client
			for (int i = 0; i < numberOfPlayer; i++)
			{
				char c_point[10];
				listClient[i].Send("POINT", strlen("POINT")); //GUI YEU CAU
				tmpLen = listClient[i].Receive(c_point, BUFF_MAX_SIZE);
				c_point[tmpLen] = '\0';

				listClientPoint[i] = atoi(c_point);
			}

			//In bang xep hang
			cout << endl << "\t\t\t\t\t     BANG XEP HANG" << endl;
			char* winner = printScoreTable(listClientPoint,numberOfPlayer,listClientsName);
			cout << "  - Server: Chuc mung chien thang cua ";
			puts(winner);

			//Dong tat ca cac client va server
			for (int i = 0; i < numberOfPlayer; i++)
			{
				listClient[i].Close();
			}
			server.Close();

			system("pause");

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

