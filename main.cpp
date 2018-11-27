#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

//Hằng số
#define BOARD_SIZE 12 // Kích thức ma trận bàn cờ
#define LEFT 3 // Tọa độ trái màn hình bàn cờ
#define TOP 1 // Tọa độ trên màn hình bàn cờ
// Khai báo kiểu dữ liệu
struct _POINT { int x, y, c; }; // x: tọa độ dòng, y: tọa độ cột, c: đánh dấu
_POINT _A[BOARD_SIZE][BOARD_SIZE]; //Ma trận bàn cờ
bool _TURN; //true là lượt người thứ nhất và false là lượt người thứ hai
int _COMMAND; // Biến nhận giá trị phím người dùng nhập
int _X, _Y; //Tọa độ hiện hành trên màn hình bàn cờ
#define max 100
int test = 0;


void FixConsoleWindow();
void GotoXY(int x, int y);
void textcolor(int x);
void veHinh(int x, int y, char s[], int color);
void ResetData();
void DrawBoard(int pSize);
void XO();
void xuLyGiaoDien(int &win1, int &win0, int &x, int &o);
void StartGame();
void GabageCollect();
void ExitGame();
int AskContinue();
int ProcessFinish(int pWhoWin);
int kiemTraHoa(_POINT _A[][BOARD_SIZE]);
int kiemTraHag(_POINT _A[][BOARD_SIZE]);
int kiemTraCot(_POINT _A[][BOARD_SIZE]);
int kiemTraCheo(_POINT _A[][BOARD_SIZE]);
int kiemTraCheoPhu(_POINT _A[][BOARD_SIZE]);
int kiemTraWin(_POINT _A[][BOARD_SIZE]);
int TestBoard();
int CheckBoard(int pX, int pY);
void MoveRight();
void MoveLeft();
void MoveDown();   
void MoveUp();
void saveGame(_POINT _A[][BOARD_SIZE], int win1, int win0);
void loadGame(_POINT _A[][BOARD_SIZE], bool &_TURN, int &win1, int &win0, int&demX, int&demO);
void huongDan();
void menu();
void caro(int i);
void vaogame();
void demSo();

void FixConsoleWindow() {
	HWND consoleWindow = GetConsoleWindow();
	LONG style = GetWindowLong(consoleWindow, GWL_STYLE);
	style = style & ~(WS_MAXIMIZEBOX)& ~(WS_THICKFRAME);
	SetWindowLong(consoleWindow, GWL_STYLE, style);

}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void textcolor(int x)
{
	HANDLE mau;
	mau = GetStdHandle
		(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}
void veHinh(int x, int y, char s[], int color)
{
	GotoXY(x, y);
	textcolor(color);
	printf("%s", s);
	textcolor(7);
}

//Hàm khởi tạo dữ liệu mặc định ban đầu cho ma trận bàn cờ (hàm nhóm Model)
void ResetData() {
	for (int i = 0; i < BOARD_SIZE; i++){
		for (int j = 0; j < BOARD_SIZE; j++){
			_A[i][j].x = 4 * j + LEFT + 2; // Trùng với hoành độ màn hình bàn cờ
			_A[i][j].y = 2 * i + TOP + 1; // Trùng với tung độ màn hình bàn cờ
			_A[i][j].c = 0; // 0 nghĩa là chưa ai đánh dấu, nếu đánh dấu phải theo quy
			//định như sau: -1 là lượt true đánh, 1 là lượt false đánh

		}

	}
	_TURN = true; _COMMAND = -1; // Gán lượt và phím mặc định
	_X = _A[0][0].x; _Y = _A[0][0].y; // Thiết lập lại tọa độ hiện hành ban đầu

}

void DrawBoard(int pSize) {
	for (int i = 3; i < LEFT + 4 * pSize - 1;){
		for (int j = 1; j < TOP + 2 * pSize - 1;){
			GotoXY(i, j);

			printf("%c%c%c%c%c", 197, 196, 196, 196, 197);
			GotoXY(i, j + 1);
			printf("%c   %c", 179, 179);
			GotoXY(i, j + 2);
			printf("%c%c%c%c%c", 197, 196, 196, 196, 197);
			j = j + 2;

		}
		i = i + 4;
	}
	for (int i = 3; i <= LEFT + 4 * pSize;)
	{
		GotoXY(i, 1);
		printf("%c", 194);
		i = i + 4;
	}
	for (int i = 3; i <= LEFT + 4 * pSize;)
	{
		GotoXY(i, TOP + 2 * pSize);
		printf("%c", 193);
		i = i + 4;
	}
	for (int i = 3; i <TOP + 2 * pSize - 1;)
	{
		GotoXY(3, i);
		printf("%c", 195);
		i = i + 2;
	}
	for (int i = 3; i <TOP + 2 * pSize - 1;)
	{
		GotoXY(LEFT + 4 * pSize, i);
		printf("%c", 180);
		i = i + 2;
	}
	GotoXY(3, 1);
	printf("%c", 218);
	GotoXY(LEFT + 4 * pSize, TOP + 2 * pSize);
	printf("%c", 217);
	GotoXY(3, TOP + 2 * pSize);
	printf("%c", 192);
	GotoXY(LEFT + 4 * pSize, 1);
	printf("%c", 191);

}
void XO()
{
	veHinh(75, 13, "       ||*****@@             @@*****||         ", 14);
	veHinh(75, 14, "      ****     @@           @@     ****     ", 14);
	veHinh(75, 15, "    **    **    @@         @@    **    **   ", 14);
	veHinh(75, 16, "    **    **     @@       @@     **    **   ", 14);
	veHinh(75, 17, "    **    **      @@     @@      **    **  ", 14);
	veHinh(75, 18, "      ****         @@@@@@@         ****    ", 14);
	veHinh(75, 19, "                   @@@@@@@            ", 14);
	veHinh(75, 20, "                  @@     @@         ", 14);
	veHinh(75, 21, "                 @@       @@         ", 14);
	veHinh(75, 22, "                @@         @@        ", 14);
	veHinh(75, 23, "               @@          @@       ", 14);
	veHinh(75, 24, "               @@          @@        ", 14);
	veHinh(75, 25, "               @@          @@        ", 14);
	veHinh(75, 26, "              @@@         @@@       ", 14);

	veHinh(75, 14, "      @@@@", 11);
	veHinh(110, 14, "@@@@", 11);
	veHinh(75, 15, "    @@    @@", 11);
	veHinh(108, 15, "@@    @@   ", 11);
	veHinh(75, 16, "    @@    @@", 11);
	veHinh(108, 16, "@@    @@   ", 11);
	veHinh(75, 17, "    @@    @@", 11);
	veHinh(108, 17, "@@    @@   ", 11);
	veHinh(75, 18, "      @@@@", 11);
	veHinh(110, 18, "@@@@", 11);

	

}

void xuLyGiaoDien(int &win1, int &win0, int &x, int &o)
{
	int dem = 0;
	textcolor(15);
	GotoXY(55, 1);
	printf("\t\t\tTY SO");
	GotoXY(55, 3);
	printf("Nguoi choi 1\t\t    Nguoi choi 0");
	GotoXY(56, 5);
	textcolor(188);
	printf("  %d  \t\t----\t\t  %d  ", win1, win0);
	textcolor(15);
	GotoXY(55, 7);
	printf("\t\t    SO BUOC DA DANH");
	GotoXY(58, 9);
	printf("%d\t\t ---   \t\t  %d", x, o);
	GotoXY(65, 11);
	if (x <= o)
	{
		textcolor(14);
		printf("Den luot nguoi choi 1");
		textcolor(7);
	}
	else
	{
		textcolor(11);
		printf("Den luot nguoi choi 0");
		textcolor(7);
	}
	GotoXY(55, 13);
	printf("Cac phim tat: ");
	GotoXY(55, 15);
	printf("\"N\": choi lai tu dau.");
	GotoXY(55, 17);
	printf("\"L\": luu game.");
	GotoXY(55, 19);
	printf("\"T\": tai game.");
	GotoXY(55, 21);
	printf("\"Esc\": thoat game.");
	XO();
	textcolor(7);
		
}
void StartGame() {

	  system("cls");
	  ResetData(); // Khởi tạo dữ liệu gốc
	  DrawBoard(BOARD_SIZE); // Vẽ màn hình game
	  GotoXY(_A[0][0].x, _A[0][0].y);
}
//Hàm dọn dẹp tài nguyên (hàm nhóm Model)
 void GabageCollect()
 {
	  // Dọn dẹp tài nguyên nếu có khai báo con trỏ
}
 //Hàm thoát game (hàm nhóm Control)
 void ExitGame() {
	 system("cls");
	  GabageCollect();
	  //Có thể lưu game trước khi exit
		
}
 //Hàm xử lý khi người chơi thua
 int AskContinue()
 {
	 fflush(stdin);
	 GotoXY(40+18, 14);
	 textcolor(188);
	 printf("\tBan co muon tiep tuc choi? ");
	 GotoXY(40+18, 15);
	 printf("\tYES(Y)\t\tNO(N)");
	 textcolor(7);
	 char s;
	 do {
		 s = toupper(_getch());
	 } while (s != 'Y' && s != 'N');
	 return int(s);
 }
  int ProcessFinish(int pWhoWin) {
	  GotoXY(0, _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y + 2); // Nhảy tới vị trí
	 // thích hợp để in chuỗi thắng/thua/hòa
	  switch (pWhoWin){
	  case -1:
	  {
				 char s[] = { "CHUC MUNG\nNguoi choi 1 da thang va nguoi choi 0 da thua" };
				 for (int i = 10; i < 18; i++)
				 {
					 veHinh(34+18, i, "|||||||||||||||||||||||||||||||||||||||||||||||||||||||", 187);
				 }
				
				 GotoXY(57+18, 12);
				 textcolor(15);
				 for (int i = 0; i < 55; i++)
				 {					 
					 if (s[i] != '\n')
					 {
						 printf("%c", s[i]);
						 Sleep(100);
					 }
					 else GotoXY(40+18, 13);
					 if (i==54)
					 {
						 Sleep(500);
						 
					 }

				 }
				 while (1)
				 {

					 textcolor(8 + rand() % 8);
					 GotoXY(57+18, 12);
					 printf("CHUC MUNG");
					 GotoXY(40+18, 13);
					 printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", true, false);
					 Sleep(300);
					 
					 if (_kbhit())
					 {
						
						 break;
						 AskContinue();
					 }
					

				 }

				 break;
	  }
	  case 1:
	  {
				char s[] = { "CHUC MUNG\nNguoi choi 0 da thang va nguoi choi 1 da thua" };
				for (int i = 10; i < 18; i++)
				{
					veHinh(34+18, i, "|||||||||||||||||||||||||||||||||||||||||||||||||||||||", 187);
				}
				GotoXY(57+18, 12);
				textcolor(15);
				for (int i = 0; i < 55; i++)
				{
					
					if (s[i] != '\n')
					{
						printf("%c", s[i]);
						Sleep(100);
					}
					else GotoXY(40+18, 13);
					if (i == 54)
					{
						Sleep(500);

					}
				}
				while (1)
				{

					textcolor(8 + rand() % 8);
					GotoXY(57+18, 12);
					printf("CHUC MUNG");
					GotoXY(40+18, 13);
					printf("Nguoi choi %d da thang va nguoi choi %d da thua\n", false, true);
					
					Sleep(300);
					if (_kbhit())
					{
						break;
						AskContinue();
					}

				}
				break;
	  }
	  case 0:
	  {
				char s[] = { "CHUC MUNG\nNguoi choi 0 da hoa nguoi choi 1" };
				for (int i = 10; i < 18; i++)
				{
					veHinh(34+18, i, "|||||||||||||||||||||||||||||||||||||||||||||||||||||||", 187);
				}
				GotoXY(55+18, 12);
				textcolor(15);
				for (int i = 0; i < 43; i++)
				{
					
					if (s[i] != '\n')
					{
						printf("%c", s[i]);
						Sleep(100);
					}
					else GotoXY(43+18, 13);
					if (i == 42)
					{
						Sleep(500);

					}
				}
				while (1)
				{

					textcolor(8 + rand() % 8);
					GotoXY(55+18, 12);
					printf("CHUC MUNG");
					GotoXY(43+18, 13);
					printf("Nguoi choi %d da hoa nguoi choi %d\n", false, true);
					
					Sleep(300);
					if (_kbhit())
					{
						break;
						AskContinue();
					}

				}

				break;
	  }
					   case 2:
						   _TURN = !_TURN; // Đổi lượt nếu không có gì xảy ra
						 
	 }
	  GotoXY(_X, _Y); // Trả về vị trí hiện hành của con trỏ màn hình bàn cờ
	  return pWhoWin;
	 
 }
  
  int kiemTraHoa(_POINT _A[][BOARD_SIZE])
  {
	  for (int i = 0; i < BOARD_SIZE;i++)
	  for (int j = 0; j < BOARD_SIZE;j++)
	  if (_A[i][j].c == 0) return 0;
	  return 1;
  }
  int kiemTraHag(_POINT _A[][BOARD_SIZE])
  {
	  for (int i = 0; i < BOARD_SIZE;i++)
	  for (int j = 0; j < BOARD_SIZE-4;j++)
	  if (_A[i][j].c == _A[i][j + 1].c&&_A[i][j].c == _A[i][j + 2].c&&_A[i][j].c == _A[i][j + 3].c&&_A[i][j].c == _A[i][j + 4].c && _A[i][j].c != 0)
	  {
		  if (j == 0 || j == BOARD_SIZE - 5) return 1;
		  else
		  {
			  if ((_A[i][j - 1].c != _A[i][j].c * (-1)) || (_A[i][j].c != _A[i][j + 5].c*(-1))) return 1;
		  }
	  }
	  return 0;
  }
  int kiemTraCot(_POINT _A[][BOARD_SIZE])
  {
	  for (int i = 0; i < BOARD_SIZE-4;i++)
	  for (int j = 0; j < BOARD_SIZE;j++)
	  if (_A[i][j].c != 0 && _A[i][j].c == _A[i + 1][j].c&&_A[i + 2][j].c == _A[i][j].c&&_A[i + 3][j].c == _A[i][j].c&&_A[i + 4][j].c==_A[i][j].c)
	  {
		  if (i == 0 || i == BOARD_SIZE - 5) return 1;
		  else
		  {
			  if (((_A[i][j].c) != (_A[i - 1][j].c * (-1))) || ((_A[i][j].c) != (_A[i + 5][j].c*(-1)))) return 1;
		  }
	  }
	  return 0;
  }
  int kiemTraCheo(_POINT _A[][BOARD_SIZE])
  {
	  for (int i = 0; i < BOARD_SIZE - 4;i++)
	  for (int j = 0; j < BOARD_SIZE - 4;j++)
	  if (_A[i][j].c != 0 && _A[i][j].c == _A[i + 1][j + 1].c && _A[i][j].c == _A[i + 2][j + 2].c && _A[i][j].c == _A[i + 3][j + 3].c && _A[i][j].c == _A[i + 4][j + 4].c)
	  {
		  if (i == 0 || j == 0 || i == BOARD_SIZE - 5 || j == BOARD_SIZE - 5) return 1;
		  else
		  {
			  if (_A[i][j].c != _A[i - 1][j - 1].c * (-1) || _A[i][j].c != _A[i + 5][j + 5].c*(-1)) return 1;
		  }
	  }
	  return 0;
  }
  int kiemTraCheoPhu(_POINT _A[][BOARD_SIZE])
  {
	  int a = BOARD_SIZE-1;
	  for (int i = a; i > 3;i--)
	  for (int j = 0; j < BOARD_SIZE-4; j++)
	  if (_A[i][j].c != 0 && _A[i][j].c == _A[i - 1][j + 1].c && _A[i][j].c == _A[i - 2][j + 2].c && _A[i][j].c == _A[i - 3][j + 3].c && _A[i][j].c == _A[i - 4][j + 4].c)
	  {
		  if ((i == (BOARD_SIZE - 1)) || i == 4 || (j == (BOARD_SIZE - 5)) || j == 0) return 1;
		  else
		  {
			  if (_A[i][j].c != _A[i + 1][j - 1].c*(-1) || _A[i][j].c != _A[i - 5][j + 5].c*(-1)) return 1;
		  }
	  }
	  return 0;
  }
  int kiemTraWin(_POINT _A[][BOARD_SIZE])
  {
	  if (kiemTraCheo(_A)) return 1;
	  if (kiemTraCheoPhu(_A))  return 1;
	  if (kiemTraCot(_A)) return 1;
	  if (kiemTraHag(_A)) return 1;
	  return 0;
  }
  int TestBoard()
  {
	  if (kiemTraHoa(_A)) return 0; // Hòa
	  else {
		  if (kiemTraWin(_A))
			  return (_TURN == true ? -1 : 1); // -1 nghĩa là lượt ‘true’ thắng

		  else
			  return 2; // 2 nghĩa là chưa ai thắng

	  }
  }

  int CheckBoard(int pX, int pY){
	  for (int i = 0; i < BOARD_SIZE; i++){
		  for (int j = 0; j < BOARD_SIZE; j++){
			  if (_A[i][j].x == pX && _A[i][j].y == pY && _A[i][j].c == 0){
				  if (_TURN == true) _A[i][j].c = -1; // Nếu lượt hiện hành là true thì c = -1
				  else _A[i][j].c = 1; // Nếu lượt hiện hành là false thì c = 1
				  return _A[i][j].c;

			  }

		  }

	  }
	  return 0;

  }
  void MoveRight() {
	   if (_X < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].x)
		   {
		   _X += 4;
			   GotoXY(_X, _Y);
		   }
	  
  }
   void MoveLeft() {
	   if (_X > _A[0][0].x) {
		  _X -= 4;
		   GotoXY(_X, _Y);
		  
	  }
	  
  }
   void MoveDown() {
	   if (_Y < _A[BOARD_SIZE - 1][BOARD_SIZE - 1].y)
		   {
		   _Y += 2;
		   GotoXY(_X, _Y);
		   }
   }
    void MoveUp() {
	    if (_Y > _A[0][0].y) {
		    _Y -= 2;
		    GotoXY(_X, _Y);
		   
	   }
	   
   }
	void saveGame(_POINT _A[][BOARD_SIZE],int win1,int win0)
	{
		char s[max];
		int demX = 0, demO = 0;
		for (int i = 10; i < 18; i++)
		{

			veHinh(34, i, "||||||||||||||||||||||||||||||||||||||||||", 187);
		}
		
		veHinh(40, 13, "Nhap tap tin luu: ", 188);
		textcolor(188);
		fflush(stdin);
		gets(s);
		textcolor(7);
		FILE *f;
		f = fopen(s, "w");
		for (int i = 0; i < BOARD_SIZE;i++)
		for (int j = 0; j < BOARD_SIZE; j++)
		fprintf(f, "%d ", _A[i][j].c);
		fprintf(f, " %d %d", win1, win0);
		fclose(f);
		f = fopen(s, "r");
		StartGame();
		for (int i = 0; i < BOARD_SIZE; i++)
		for (int j = 0; j < BOARD_SIZE; j++)
			fscanf(f, "%d", &_A[i][j].c);
		fscanf(f, "%d %d", &win1, &win0);
		fclose(f);
		for (int i = 0; i < BOARD_SIZE; i++)
		{
			for (int j = 0; j < BOARD_SIZE; j++)
			{
				GotoXY(_A[i][j].x, _A[i][j].y);
				if (_A[i][j].c == 1)
				{
					demO++;
					textcolor(11);
					printf("O");
					textcolor(7);
				}

				if (_A[i][j].c == -1)
				{
					textcolor(14);
					printf("X");
					textcolor(7);
					demX++;
				}
			}
		}
		xuLyGiaoDien(win1, win0, demX, demO);
		GotoXY(_X, _Y);
		if (demX > demO) _TURN = false;
		else _TURN = true;

	}
	void loadGame(_POINT _A[][BOARD_SIZE], bool &_TURN,int &win1,int &win0,int&demX,int&demO)
	{
		//int demX = 0, demO = 0, win1, win0;
		demX = 0;
		demO = 0;
		char s[max];
		for (int i = 10; i < 18; i++)
		{

			veHinh(34, i, "||||||||||||||||||||||||||||||||||||||||||", 187);
		}
		
		veHinh(40, 12, "Nhap tap tin muon tai: ", 188);
		textcolor(188);
		fflush(stdin);
		gets(s);
		textcolor(7);
		FILE *f;
		f = fopen(s, "r");
		if (f == NULL)
		{
			veHinh(40, 13, "Khong mo duoc tap tin!", 188);
			veHinh(40, 14, "Nhan phim 'T' de tai lai.", 188);
			veHinh(40, 15, "Nhan phim 'N' de bat dau game moi.", 188);
		}
		else
		{
			StartGame();
			for (int i = 0; i < BOARD_SIZE; i++)
			for (int j = 0; j < BOARD_SIZE; j++)
				fscanf(f, "%d", &_A[i][j].c);
			fscanf(f, "%d %d", &win1, &win0);
			for (int i = 0; i < BOARD_SIZE; i++)
			{
				for (int j = 0; j < BOARD_SIZE; j++)
				{
					GotoXY(_A[i][j].x, _A[i][j].y);
					if (_A[i][j].c == 1)
					{
						demO++;
						textcolor(11);
						printf("O");
						textcolor(7);
					}

					if (_A[i][j].c == -1)
					{
						textcolor(14);
						printf("X");
						textcolor(7);
						demX++;
					}
				}
			}
			xuLyGiaoDien(win1, win0, demX, demO);
			GotoXY(_X, _Y);
			if (demX > demO) _TURN = false;
			else _TURN = true;
			fclose(f);
		}
	}
	void huongDan()
	{
		system("cls");
		textcolor(15);
		printf("Tro choi su dung ban co 12x12 o vuong, la tro choi doi khang truc tiep 2 nguoi choi.\nMoi nguoi choi se co gang xep du 5 ky hieu cua minh thanh mot duong thang thi se thang.");
		printf("\nLuat choi:\nTren ban co 12x12 o vuong.Mot nguoi di X, mot nguoi di O.\nKhi den luot minh, nguoi choi phai tich vao mot o tren ban co.");
		printf("\n - Nhan phim 'A' de di chuyen sang trai.");
		printf("\n - Nhan phim 'D' de di chuyen sang phai.");
		printf("\n - Nhan phim 'W' de di chuyen len.");
		printf("\n - Nhan phim 'S' de di chuyen xuong.");
		printf("\n - Nhan phim 'Enter' de danh vao bang.");

		printf("\nNguoi choi phai tim cach tich du 5 o theo chieu doc hoac chieu ngang hoac duong cheo ma khong bi chan 2 dau thi se thang");
		printf("Neu cac o trong bang deu duoc danh dau nhung chua tim duoc nguoi thang thi se hoa.\n");
		
		textcolor(10);
		printf("\t\t\t\t\tBAN DA HIEU RO LUAT CHOI?");
		printf("\n\t\t\t\t    Nhan phim bat ki de bat dau choi.");
		_getch();


	}
	
	void menu()
	{
		for (int i = 10; i < 19; i++)
		{

			veHinh(34, i, "|||||||||||||||||||||||||||||||||||||||||||||||||||||||", 187);
			
		}
		veHinh(55, 11, "MENU", 188);
		veHinh(50, 12, "1 - New Game", 188);
		veHinh(50, 13, "2 - Load Game", 188);
		veHinh(50, 14, "3 - How to play", 188);
		veHinh(50, 15, "4 - Exit", 188);
		veHinh(50, 16, "Lua chon: ", 188);
		textcolor(7);
		

	}
	void caro(int i)
	{
		veHinh(20, 3, "       ****             ***                       **********            ****           ", i);
		veHinh(20, 4, "     ***              **   **                     ***    ***         ***    ***    ", i);
		veHinh(20, 5, "    ***              **********                   ***    ***        ***      ***      ", i);
		veHinh(20, 6, "    ***             ***      ***                  **********        ***      ***           ", i);
		veHinh(20, 7, "    ***            ***        ***                 ***     ***        ***    ***           ", i);
		veHinh(20, 8, "      ****        ***          ***                ***      ***         ******           ", i);


	}
	void vaogame()
	{
		veHinh(9, 15, "      @@             @@", 14);
		veHinh(9, 16, "       @@           @@    ", 14);
		veHinh(9, 17, "        @@         @@ ", 14);
		veHinh(9, 18, "         @@       @@  ", 14);
		veHinh(9, 19, "           @@@@@@@   ", 14);
		veHinh(9, 20, "           @@   @@    ", 14);
		veHinh(9, 21, "          @@     @@  ", 14);
		veHinh(9, 22, "         @@       @@  ", 14);
		veHinh(9, 23, "        @@         @@  ", 14);


		veHinh(21, 11, "@@@@@", 11);
		veHinh(20, 12, "@     @", 11);
		veHinh(19, 13, "@       @", 11);
		veHinh(18, 14, "@         @", 11);
		veHinh(22, 14, "X", 14);
		veHinh(25, 14, "O", 11);
		veHinh(18, 15, "@         @", 11);
		veHinh(19, 16, "@       @", 11);
		veHinh(20, 17, "@     @", 11);
		veHinh(21, 18, "@@@@@", 11);
		
		while (1)
		{
			for (int i = 10; i < 19; i++)
			{

				veHinh(34, i, "\t\t\t\t\t\t\t\t", 176);
				
			}
			
				
			caro(8 + rand() % 8);
			textcolor(8 + rand() % 8);
			GotoXY(40, 13);
			printf("CHAO MUNG CAC BAN DEN VOI GAME CARO TREN CONSOLE!!!");
			GotoXY(50, 14);
			printf("Nhan phim bat ki de vao game.");
			if (_kbhit())
			{
				char c = _getch();
				system("cls");
				break;
			}
			

			textcolor(7);
			Sleep(400);



		}
	}
	void demSo()
	{
		textcolor(15);
		system("cls");
		GotoXY(50, 11);
		printf("%c", 205);
		printf("%c", 187);
		GotoXY(50, 12);
		printf("%c%c", 205, 185);
		GotoXY(50, 13);
		printf("%c%c", 205, 188);
		Sleep(1000);
		system("cls");
		textcolor(13);
		GotoXY(50, 11);
		printf("%c%c", 205, 205);
		printf("%c", 187);
		GotoXY(50, 12);
		printf("%c%c%c", 201, 205, 188);
		GotoXY(50, 13);
		printf("%c%c%c", 200, 205, 205);
		Sleep(1000);
		system("cls");
		textcolor(14);
		GotoXY(50, 11);
		printf("%c", 187);
		GotoXY(50, 12);
		printf("%c", 186);
		GotoXY(50, 13);
		printf("%c", 202);
		Sleep(1000);
		system("cls");
		textcolor(12);
		GotoXY(50, 11);
		printf("%c%c%c", 201, 205, 187);
		GotoXY(50, 12);
		printf("%c", 186);
		GotoXY(50, 13);
		printf("%c%c%c", 200, 205, 188);
		GotoXY(52, 12);
		printf("%c", 203);
		GotoXY(53, 11);
		printf("%c%c%c", 201, 205, 187);
		GotoXY(53, 12);
		printf("%c %c", 186, 186);
		GotoXY(53, 13);
		printf("%c%c%c", 200, 205, 188);
		Sleep(1000);
		system("cls");
		textcolor(7);
	}
	
	int win1 = 0, win0 = 0, x = 0, o = 0;
	int z;

	void main()
	{
		srand(time(NULL));
		FixConsoleWindow();
		vaogame();
		do{		
			menu();
			textcolor(188);
			scanf("%d", &z);
			textcolor(7);
		} while (z>=5||z<=0);
		switch (z)
		{
		case 4:
			ExitGame();
			break;
		case 3:
			huongDan();
		case 1:
			demSo();
			StartGame();
			xuLyGiaoDien(win1, win0, x, o);
			GotoXY(_X, _Y);
			test++;
		case 2:
			if (test == 0)
			{
				loadGame(_A, _TURN,win1,win0,x,o);
			}
			bool validEnter = true;
			while (1)
			{
				_COMMAND = toupper(_getch());
				if (_COMMAND == 27)
				{
					ExitGame();
					return;
				}
				else {
					if (_COMMAND == 'A') MoveLeft();
					else if (_COMMAND == 'W') MoveUp();
					else if (_COMMAND == 'S') MoveDown();
					else if (_COMMAND == 'D') MoveRight();
					else if (_COMMAND == 'L') saveGame(_A,win1,win0);
					else if (_COMMAND == 'T') loadGame(_A, _TURN,win1,win0,x,o);
					else if (_COMMAND == 'N') {
						demSo();
						win1 = 0, win0 = 0, x = 0, o = 0;
						StartGame();
						xuLyGiaoDien(win1, win0, x, o);
						GotoXY(_X, _Y);
					}
					else if (_COMMAND == 13){// Người dùng đánh dấu trên màn hình bàn cờ					
						switch (CheckBoard(_X, _Y)){
						case -1:							
							textcolor(14);
							printf("X");
							printf("\a");
							textcolor(7);
							x++;
							xuLyGiaoDien(win1, win0, x, o);
							GotoXY(_X, _Y);
							break;
						case 1:
							textcolor(11);
							printf("O");
							printf("\a");
							textcolor(7);
							o++;
							xuLyGiaoDien(win1, win0, x, o);
							GotoXY(_X, _Y);
							break;
						case 0: validEnter = false; // Khi đánh vào ô đã đánh rồi

						}
						// Tiếp theo là kiểm tra và xử lý thắng/thua/hòa/tiếp tục
						if (validEnter == true){
							switch (ProcessFinish(TestBoard())){
							case -1: 
							case 1:
							case 0:
								if (AskContinue() != 'Y'){
									ExitGame(); return;

								}
								else
								{
									if (TestBoard() == -1)win1++;
									if (TestBoard() == 1) win0++;
									x = 0;
									o = 0;
									StartGame();
									xuLyGiaoDien(win1, win0, x, o);
									GotoXY(_X, _Y);
								}
							

							}

						}
						validEnter = true; // Mở khóa

					}

				}


			}
		
		}
		

	}
	