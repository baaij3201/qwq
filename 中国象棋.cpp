#include<iostream>
#include<windows.h>
#include<algorithm>
#include<conio.h>
#include<cstring>
#include<string>
#include<tchar.h>
#include<cstdlib>
#include<cwchar>
#include<cmath>
#include<vector>
//#define en //若中文显示乱码,去掉,前面的注释再编译
#ifndef COMMON_LVB_UNDERSCORE
#define COMMON_LVB_UNDERSCORE 32768
#endif
std::wstring string_to_wstring(const std::string& str) {
	std::setlocale(LC_ALL, "");  // 设置本地化
	const char* cstr = str.c_str();
	size_t requiredSize = 0;

	// 使用 mbstowcs_s 获取所需缓冲区大小
	errno_t err = mbstowcs_s(&requiredSize, nullptr, 0, cstr, 0);

	if (err != 0 || requiredSize == 0) {
		return L"";
	}

	std::vector<wchar_t> buffer(requiredSize);

	// 使用 mbstowcs_s 进行转换
	err = mbstowcs_s(&requiredSize, buffer.data(), buffer.size(), cstr, str.size());

	if (err != 0) {
		return L"";
	}

	// 注意：mbstowcs_s 会在字符串末尾添加 null 终止符
	// 所以我们需要去掉最后一个字符（如果是空字符）
	if (!buffer.empty() && buffer.back() == L'\0') {
		buffer.pop_back();
	}

	return std::wstring(buffer.data(), buffer.size());
}
void color(int x, int y) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	int t = 0;
	switch (x) {
	case 1:t= FOREGROUND_INTENSITY | FOREGROUND_RED;break;	//红
	case 2:t= FOREGROUND_INTENSITY | FOREGROUND_BLUE;break;	//蓝
	case 3:t= FOREGROUND_INTENSITY | FOREGROUND_GREEN;break;	//绿
	case 4:t= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE;break;	//红+蓝=紫
	case 5:t= FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN;break;	//红+绿=黄
	case 6:t= FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN;break;	//蓝+绿=青
	case 7:t= FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;break;	//红+蓝+绿=白
	case 8:t= FOREGROUND_RED;break;	//红
	case 9:t= FOREGROUND_BLUE;break;	//蓝
	case 10:t= FOREGROUND_GREEN;break;	//绿
	case 11:t= FOREGROUND_RED | FOREGROUND_BLUE;break;	//红+蓝=紫
	case 12:t= FOREGROUND_RED | FOREGROUND_GREEN;break;	//红+绿=黄
	case 13:t= FOREGROUND_BLUE | FOREGROUND_GREEN;break;	//蓝+绿=青
	case 14:t= FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;break;	//红+蓝+绿=白
	case 15:t= FOREGROUND_INTENSITY;//灰色
	default:break;
	}
	switch (y) {
	case 1:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED|t);break;	//红
	case 2:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_BLUE|t);break;	//蓝
	case 3:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_GREEN|t);break;	//绿
	case 4:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE|t);break;	//红+蓝=紫
	case 5:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN|t);break;	//红+绿=黄
	case 6:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_GREEN|t);break;	//蓝+绿=青
	case 7:SetConsoleTextAttribute(handle, BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED|t);break;	//红+蓝+绿=白
	case 8:SetConsoleTextAttribute(handle, BACKGROUND_RED | t);break;	//红
	case 9:SetConsoleTextAttribute(handle, BACKGROUND_BLUE | t);break;	//蓝
	case 10:SetConsoleTextAttribute(handle, BACKGROUND_GREEN | t);break;	//绿
	case 11:SetConsoleTextAttribute(handle, BACKGROUND_RED | BACKGROUND_BLUE | t);break;	//红+蓝=紫
	case 12:SetConsoleTextAttribute(handle, BACKGROUND_RED | BACKGROUND_GREEN | t);break;	//红+绿=黄
	case 13:SetConsoleTextAttribute(handle, BACKGROUND_BLUE | BACKGROUND_GREEN | t);break;	//蓝+绿=青
	case 14:SetConsoleTextAttribute(handle, BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED | t);break;	//红+蓝+绿=白
	default:break;
	}
	return;
}
int mgb(const wchar_t* a, const wchar_t* b, unsigned int c) {
	return MessageBox(NULL, a, b, c);
}
struct po {
	int v;//棋子类型,-1为空 ,1为将帅，2为士、仕 ，3为象、相，4为马，5为车，6为炮、砲 ，7为兵、卒 
	int s;//阵营，1为红 2为黑，0为空 
};
int chebyshev(int x1, int y1, int x2, int y2) {
	int x = abs(x1- x2);
	int y = abs(y1-y2);
	if (x > y) return x;
	return y;
}
int manhattan(int x1, int y1, int x2, int y2) {
	int x = abs(x1 - x2);
	int y = abs(y1 - y2);
	return x + y;
}
po map[10][9];
void init() {
	for (int i = 0;i < 10;i++) {//先清空 
		for (int j = 0;j < 9;j++) {
			map[i][j] = { -1,0 };
		}
	}
	map[0][0] = map[0][8] = { 5,1 };//红车
	map[0][1] = map[0][7] = { 4,1 };//马
	map[0][2] = map[0][6] = { 3,1 };//相
	map[0][3] = map[0][5] = { 2,1 };//仕
	map[0][4] = { 1,1 };//帅
	map[2][1] = map[2][7] = { 6,1 };//炮
	map[3][0] = map[3][2] = map[3][4] = map[3][6] = map[3][8] = { 7,1 };//兵

	//-------------------------------------------------------------------
	map[9][0] = map[9][8] = { 5,2 };//黑车
	map[9][1] = map[9][7] = { 4,2 };//马
	map[9][2] = map[9][6] = { 3,2 };//象
	map[9][3] = map[9][5] = { 2,2 };//士
	map[9][4] = { 1,2 };//将
	map[7][1] = map[7][7] = { 6,2 };//砲
	map[6][0] = map[6][2] = map[6][4] = map[6][6] = map[6][8] = { 7,2 };//卒 
}
int gameover(bool hh){
	bool k1 = 0, k2 = 0;
	int k1x = 0, k2x = 0, k1y = 0, k2y = 0;
	for (int i = 0;i < 10;i++) {//先清空 
		for (int j = 0;j < 9;j++) {
			if (map[i][j].v == 1 && map[i][j].s == 1) {
				k1x = j;
				k1y = i;
				k1 = 1;
			}
			if (map[i][j].v == 1 && map[i][j].s == 2) {
				k2x = j;
				k2y = i;
				k2 = 1;
			}
		}
	}
	if (k1 && !k2) return 1;//红方胜利
	if (!k1 && k2) return 2;//黑方胜利
	if (k1x == k2x) {
		int cnt = 0;
		int s,e;
		if (k1y < k2y) s = k1y, e = k2y;
		else s = k2y, e = k1y;//理论上不可能发生
		for (int i = s+1;i < e;i++) {
			if (map[i][k1x].s != 0) return 0;
		}
		if (hh == 1) return 2;
		else return 1;
	}
	return 0;
}
int judge(int ox, int oy, int nx, int ny) {//-1：违反所有棋子的基本规则 -2，移动超出范围（仅对于将帅士仕生效）-3：不符合除九宫限制外的普通走法 -4：违反特殊走法（蹩马腿，塞象眼，吃子炮需炮架）-5：取消选择
	po o, n;
	o = map[oy][ox];
	n = map[ny][nx];
	if (nx == ox && ny == oy) return -5;//停住
	if (n.s == o.s) return -1;//不能吃己方棋子
	if (o.v == 1) {
		if (o.s == 1) {//九宫格-帅的Y轴限制
			if (ny >= 3) {
				return -2;//帅超出限制
			}
		}
		if (o.s == 2) {//九宫格-将的Y轴限制
			if (ny <= 6) {
				return -2;//将超出限制
			}
		}
		if (ox < 3 || ox>5) return -2;//将/帅超出X轴限制
		if (manhattan(ox, oy, nx, ny) > 1) return -3;//将帅的移动等价于曼哈顿距离为1的点(以下同理）
	}
	if (o.v == 2) {
		if (o.s == 1) {//九宫格-仕的Y轴限制
			if (ny > 3) {
				return -2;
			}
		}
		if (o.s == 2) {//九宫格-士的Y轴限制
			if (ny < 6) {
				return -2;
			}
		}
		if (ox < 3 || ox>5) return -2;//超出X轴限制
		if (manhattan(ox, oy, nx, ny) != 2 || chebyshev(ox, oy, nx, ny) != 1) return -3;
	}
	if (o.v == 3) {
		if (manhattan(ox, oy, nx, ny) != 4 || chebyshev(ox, oy, nx, ny) != 2) return -3;
		int xx = nx - ox;
		int yy = ny - oy;
		xx /= 2;
		yy /= 2;
		if (map[oy + yy][ox + xx].s != 0) return -4;//塞象眼
		if (o.s == 1 && ny > 4) return -4;//过河
		if (o.s == 2 && ny < 5) return -4;
	}
	if (o.v == 4) {
		if (manhattan(ox, oy, nx, ny) != 3 || chebyshev(ox, oy, nx, ny) != 2) return -3;
		int xx = nx - ox;
		int yy = ny - oy;
		if (xx == 2 || xx == -2) {//蹩马腿
			if (map[oy][ox + xx / 2].s != 0) return -4;
		}
		else {
			if (map[oy + yy/2][ox].s != 0) return -4;
		}
	}
	if (o.v == 5) {
		if (manhattan(ox, oy, nx, ny) != chebyshev(ox, oy, nx, ny)) return -3;
		if (ox == nx) {
			int mxy, mny;
			if (oy < ny) mxy = ny, mny = oy;
			else mxy = oy, mny = ny;
			for (int i = mny + 1;i < mxy;i++) {
				if (map[i][ox].s != 0) return -3;
			}
		}
		else {
			int mxx, mnx;
			if (ox < nx) mxx = nx, mnx = ox;
			else mxx = ox, mnx = nx;
			for (int i = mnx + 1;i < mxx;i++) {
				if (map[oy][i].s != 0) return -6;
			}
		}
	}
	if (o.v == 6) {
		if (manhattan(ox, oy, nx, ny) != chebyshev(ox, oy, nx, ny)) return -3;
		if (n.s==0) {//若未吃子
			if (ox == nx) {
				int mxy, mny;
				if (oy < ny) mxy = ny, mny = oy;
				else mxy = oy, mny = ny;
				for (int i = mny+1;i < mxy;i++) {
					if (map[i][ox].s != 0) return -4;
				}
			}
			else {
				int mxx, mnx;
				if (ox < nx) mxx = nx, mnx = ox;
				else mxx = ox, mnx = nx;
				for (int i = mnx+1;i < mxx;i++) {
					if (map[oy][i].s != 0) return -4;
				}
			}
		}
		else {
			if (ox == nx) {
				int cnt = 0;//中间棋子的数量
				int mxy, mny;
				if (oy < ny) mxy = ny, mny = oy;
				else mxy = oy, mny = ny;
				for (int i = mny+1;i < mxy;i++) {
					if (map[i][ox].s != 0) cnt++;
				}
				if (cnt != 1) return -4;//若中间的棋子数不符合规定
			}
			else {
				int cnt = 0;
				int mxx, mnx;
				if (ox < nx) mxx = nx, mnx = ox;
				else mxx = ox, mnx = nx;
				for (int i = mnx+1;i < mxx;i++) {
					if (map[oy][i].s != 0) cnt++;
				}
				if (cnt != 1) return -4;
			}
		}
	}
	if (o.v == 7) {
		if (o.s == 1) {
			if (ny < oy) return -3;//兵不能后退
			if (oy > 4) {
				if (manhattan(ox, oy, nx, ny) > 1) return -3;
			}
			else {
				if (manhattan(ox, oy, nx, ny) > 1 || nx != ox) return -3;
			}
		}
		if (o.s == 2) {
			if (ny > oy) return -3;//卒不能后退
			if (oy < 5) {
				if (manhattan(ox, oy, nx, ny) > 1) return -3;
			}
			else {
				if (manhattan(ox, oy, nx, ny) > 1 || nx != ox) return -3;
			}
		}
	}
	return 1;
}
int main() {
	system("chcp 65001 > nul");
	system("cls");
	init();
	int x = 0, y = 0,oldx=0,oldy=0;
	bool hh = 0, scted=0;//hh1为红方0为黑方
	while (1) {
		for (int i = 0;i < 10;i++) {
			for (int j = 0;j < 9;j++) {
				color(0, 7);
				if (i == y && j == x) color(3, 7);
				po t = map[i][j];
				if (t.s == 0) std::cout << " +";
				if (t.s == 1) {
					color(1, 7);
					#ifdef en
					if (i == y && j == x) color(3, 7);
					if (t.v == 1) std::cout << "k1";//取自king
					if (t.v == 2) std::cout << "se";//取自sever
					if (t.v == 3) std::cout << "mi";//取自minister
					if (t.v == 4) std::cout << "ho";//取自horse
					if (t.v == 5) std::cout << "ca";//取自car
					if (t.v == 6) std::cout << "co";//取自cannon
					if (t.v == 7) std::cout << "s1";//取自soldier
					#endif
					#ifndef en
					if (i == y && j == x) color(3, 7);
					if (t.v == 1) std::cout << "帥";
					if (t.v == 2) std::cout << "仕";
					if (t.v == 3) std::cout << "相";
					if (t.v == 4) std::cout << "馬";
					if (t.v == 5) std::cout << "車";
					if (t.v == 6) std::cout << "炮";
					if (t.v == 7) std::cout << "兵";
					#endif
				}
				if (t.s == 2) {
					#ifdef en
					if (i == y && j == x) color(3, 7);
					if (t.v == 1) std::cout << "k2";//取自king
					if (t.v == 2) std::cout << "gu";//取自guard
					if (t.v == 3) std::cout << "el";//取自elephant
					if (t.v == 4) std::cout << "ho";//取自horse
					if (t.v == 5) std::cout << "ca";//取自car
					if (t.v == 6) std::cout << "ct";//取自catapult
					if (t.v == 7) std::cout << "s2";//取自soldier
					#endif
					#ifndef en
					color(0, 7);
					if (i == y && j == x) color(3, 7);
					if (t.v == 1) std::cout << "將";
					if (t.v == 2) std::cout << "士";
					if (t.v == 3) std::cout << "象";
					if (t.v == 4) std::cout << "馬";
					if (t.v == 5) std::cout << "車";
					if (t.v == 6) std::cout << "砲";
					if (t.v == 7) std::cout << "卒";
					#endif
				}
			}
			std::cout << '\n';
		}
		color(0, 7);
		if (!hh) {
			color(1,7);
			std::cout << "红";
			color(0,7);
		}
		else {
			std::cout << "黑";
		}
		std::cout << "方回合,wasd调整位置，enter抬子/落子,绿色为当前选择项,当前";
		if (scted) {
			color(3, 7);
			std::cout << "已选择";
			color(0, 7);
		}
		else {
			std::cout << "未选择";
		}
		int e;
		if (hh == 0) e = 1;
		else e = 2;
		char c=_getch();
		if (!scted) {
			switch (c) {
				case 'w':
				case 'W':
					if (y != 0) y--;
					oldy = y;
					break;
				case 'a':
				case 'A':
					if (x != 0) x--;
					oldx = x;
					break;
				case 's':
				case 'S':
					if (y < 9) y++;
					oldy = y;
					break;
				case 'd':
				case 'D':
					if (x < 8) x++;
					oldy = y;
					break;
				case '\n':
				case '\r':
					if (map[y][x].s != e) {
						mgb(_T("该位置为对方棋子或空"), _T("错误"), MB_ICONWARNING | MB_OK);
						break;
					}
					oldx = x;
					oldy = y;
					scted = !scted;
			}
		}
		else {
			switch (c) {
				case 'w':
				case 'W':
					if (y != 0) y--;
					break;
				case 'a':
				case 'A':
					if (x != 0) x--;
					break;
				case 's':
				case 'S':
					if (y < 9) y++;
					break;
				case 'd':
				case 'D':
					if (x < 8) x++;
					break;
				case '\n':
				case '\r':
					if (judge(oldx,oldy,x,y)<0&& judge(oldx, oldy, x, y)!=-5) {
						std::cout << "不合法的走子位置,错误码为:（ - 1：违反所有棋子的基本规则 - 2，移动超出范围（仅对于将帅士仕生效） - 3：不符合除九宫限制外的普通走法 - 4：违反特殊走法（蹩马腿，塞象眼，吃子炮需炮架））"<< judge(oldx, oldy, x, y);
						_getch();
						system("cls");
						break;
					}
					else if(judge(oldx, oldy, x, y) == -5){
						scted = !scted;
					}
					else {
						map[y][x] = map[oldy][oldx];
						map[oldy][oldx] = { -1,0 };
						scted = !scted;
						hh = !hh;
					}
			}
		}
		system("cls");
		if (gameover(hh)) {
			if (gameover(hh) == 1) {
				mgb(_T("红方胜利"), _T("提示"), MB_OK);
				return 0;
			}
			else {
				mgb(_T("黑方胜利"), _T("提示"), MB_OK);
				return 0;
			}
		}
	}
}
// 使用visual 2022 编译，OS: >= Windows10 1809注意：设置正确的编码