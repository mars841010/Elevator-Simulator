/*
	電梯模擬器：
	操作方式：
		1：先輸入當前樓層、兩個電梯的初始樓層
		2：主介面-用鍵盤操作
			上下方向鍵：選擇電梯的方向按鈕
			Enter按鍵：確認所選的方向按鈕
			r,R		：隨機移動電梯
			f,F		：重新輸入使用者當前樓層
			q,Q		：結束程式
*/

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <iomanip>
#include <math.h>
#include <cstdlib> /* 亂數相關函數 */
#include <ctime>   /* 時間相關函數 */

#define UP '^'
#define DOWN 'v'
#define STOP " "
#define C_UP "<^>"
#define C_DOWN "<v>"
#define ENTER 13
#define CLEAR "                                                          "
#define BTN_UP 1
#define BTN_DOWN 2

using namespace std;
void get_init_conditions(HANDLE screen,int *floors);
void enviroment(HANDLE screen,int current_floor);	//顯示介面
int	 dirButton_select(HANDLE screen, int dir,int current_floor);
int  floorButton_select(HANDLE screen,int current_floor,int dirButton);

/*
重要位置：
	電梯方向示意符號：左2,14 右2,21
	電梯樓層示意符號：左3,13 右3,20
	上按鈕：6,17
	下按鈕：7,17
	環境下邊緣：14
*/

void placeCursor(HANDLE screen,int row, int col)
{
    COORD position;    // holds a pair of X and Y coordinates
    position.Y = row;
    position.X = col;
    SetConsoleCursorPosition(screen, position);
}

// 電梯類別
class Elevator {

public:
	//int	 id = 0;
	Elevator(int idx,int floor,int floor_row,int floor_col,int dir_row,int dir_col){
		id = idx;
		current_floor = floor;
		f_row=floor_row;
		f_col=floor_col;
		d_row=dir_row;
		d_col=dir_col;
	}
    int  id;
    void display_floor(HANDLE screen);	//顯示當前樓層 
    void move(HANDLE screen,int floor);	//移動 
	int  current_floor;	//當前樓層 
	int  f_row,f_col;		//樓層示意位置row,col
	int  d_row,d_col;		//方向示意位置row,col
};

// 顯示當前樓層 
void Elevator::display_floor(HANDLE screen) {
	//cout << "Elevator " << id << "'s current floor : " << current_floor << endl;
    placeCursor(screen,f_row,f_col);
	cout << setw(2) << current_floor;
	placeCursor(screen,14,0);
	return;
}

// 移動 
void Elevator::move(HANDLE screen,int floor){
	int step=(floor>current_floor)?1:-1;	//+/-
	char sign=(floor>current_floor)?UP:DOWN;
	
	//移動中，顯示方向示意符號，並提示正在移動，一秒一動
	while(1){
		placeCursor(screen,14,0);
		cout <<CLEAR;
		placeCursor(screen,14,0);
		cout <<"Elevator " << id << " moves from " << current_floor <<" to "<< floor <<endl;
		placeCursor(screen,d_row,d_col);
		cout<<sign;
		display_floor(screen);
		if(current_floor==floor) break;
		current_floor+=step;
		Sleep(1000);
	}
	//移動完，清除方向示意符號並提示
	placeCursor(screen,14,0);
	cout <<CLEAR;
	placeCursor(screen,d_row,d_col);
	cout<<' ';
	placeCursor(screen,14,0);
	cout << "The target floor "<< floor <<" has arrived. " << endl;
	Sleep(1000);
	placeCursor(screen,14,0);
	cout <<CLEAR;
	return ;
}

// 程式執行的 main()
int main(void) {
    // 宣告並建立兩個電梯物件 
    
    int chosen_elevator_id;	//選定的電梯id
	int current_floor;		//使用者的當前樓層
	int chosen_dirButton;		//選定的方向按鈕
	int desired_floor;	//選定的樓層
	int conditions[3]={-1,-1,-1}; //[當前樓層，電梯1初始樓層，電梯2初始樓層]
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	srand( time(NULL) );

	//輸入當前樓層和設定電梯初始樓層:
	get_init_conditions(screen,conditions);
	Elevator elevator0(1,conditions[1],3,13,2,14),elevator1(2,conditions[2],3,20,2,21);
    Elevator *chosen_elevator=&elevator0;
	current_floor=conditions[0];
	
	while(1){
		bool done=false;	//回合結束
		system("CLS");
		enviroment(screen,current_floor);
		elevator0.display_floor(screen);
		elevator1.display_floor(screen);
		chosen_dirButton=-1;	//選定的方向按鈕
		int new_current_floor;	//用於修改當前樓層
		while(!done){
			placeCursor(screen,0,37);
			cout<<"Current floor:"<<current_floor;
			placeCursor(screen,14,0);
			switch(getch()){
				//方向鍵
				case 0:
				case 224:
					//方向鍵：選擇上下按鈕
					chosen_dirButton=dirButton_select(screen,getch(),current_floor);
					break;
				//確認輸入
				case ENTER:
					//處理沒選就按ENTER的狀況
					if(chosen_dirButton==-1) break;
					// 判斷較佳電梯
					abs(current_floor-elevator0.current_floor)>abs(current_floor-elevator1.current_floor)
					?chosen_elevator=&elevator1:chosen_elevator=&elevator0;
					//前往使用者當前樓層
					chosen_elevator->move(screen,current_floor);
					//輸入前往樓層
					desired_floor=floorButton_select(screen,current_floor,chosen_dirButton);
					placeCursor(screen,14,0);
					//提示要前往的樓層
					cout<<" Desired floor : "<<desired_floor;
					Sleep(1000);
					placeCursor(screen,14,0);
					chosen_elevator->move(screen,desired_floor);
					current_floor=desired_floor;
					done = true;	//回合結束
					break;
				//隨機移動電梯
				case 'R':
				case 'r':
					placeCursor(screen,14,0);
					cout<<"Move elevators to random floor..."<<endl;
					Sleep(1000);
					elevator0.move(screen,(rand()%10+1));
					elevator1.move(screen,(rand()%10+1));
					placeCursor(screen,14,0);
					cout<<CLEAR;
					placeCursor(screen,14,0);
					break;
				//重新設定當前樓層
				case 'f':
				case 'F':
					while(1){
						placeCursor(screen,14,0);
						cout<<"Enter new current floor: ";
						cin>>new_current_floor;
						//檢測值是否合理
						if(new_current_floor<1||new_current_floor>10){
							placeCursor(screen,15,0);
							cout<<"Invalid floor.";
							Sleep(1000);
							placeCursor(screen,14,0);
							cout<<CLEAR<<endl<<CLEAR;
						}
						else break;	
					}
					//提示
					placeCursor(screen,14,0);
					cout<<CLEAR;
					placeCursor(screen,14,0);
					cout<<"New current floor: "<<new_current_floor;
					current_floor=new_current_floor;
					Sleep(1000);
					system("CLS");
					enviroment(screen,current_floor);
					elevator0.display_floor(screen);
					elevator1.display_floor(screen);
					placeCursor(screen,14,0);
					break;
				//關閉程式
				case 'q':
				case 'Q':
					system("PAUSE");
					return 0; 
					break;
			}	
		}	
	}

}

//顯示環境(包含是否隱藏特定電梯方向按鈕)
void enviroment(HANDLE screen,int current_floor){
	// 上按鈕：6,17
	// 下按鈕：7,17
	cout
	<<"**********               **********"<<endl
	<<"*        *  ****   ****  *        *"<<endl
	<<"*    o   *  *  *   *  *  *  oooo  *"<<endl
	<<"*   oo   *  *  *   *  *  *     o  *"<<endl
	<<"*    o   *  ****   ****  *  oooo  *"<<endl
	<<"*    o   *               *  o     *"<<endl
	<<"*   ooo  *       ^       *  oooo  *"<<endl
	<<"*        *       v       *        *"<<endl
	<<"*        *               *        *"<<endl
	<<"*        *               *        *"<<endl
	<<"*        *               *        *"<<endl
	<<"*        *               *        *"<<endl
	<<"*        *               *        *"<<endl
	<<"**********               **********"<<endl;

	// 刪除1樓10樓的無效按鈕
	switch(current_floor){
		case 1:
			placeCursor(screen,7,16);
			cout<<"   ";
			break;
		case 10:
			placeCursor(screen,6,16);
			cout<<"   ";
			break;
	}

	placeCursor(screen,14,0);
	return;

}

//選擇電梯方向按鈕
int dirButton_select(HANDLE screen,int dir,int current_floor){

	//上按鈕：6,17
	//下按鈕：7,17
	//環境下邊緣：14
	if(current_floor==1){
		placeCursor(screen,6,16);
		cout<<C_UP<<endl;
		placeCursor(screen,14,0);
		return BTN_UP;
	}
	else if(current_floor==10){
		placeCursor(screen,7,16);
		cout<<C_DOWN<<endl;
		placeCursor(screen,14,0);
		return BTN_DOWN;
	}
	switch(dir){
		case 72:	//上
			placeCursor(screen,7,16);
			cout<<" "<<DOWN<<" "<<endl;
			placeCursor(screen,6,16);
			cout<<C_UP<<endl;
			placeCursor(screen,14,0);
			return BTN_UP;	
			break;
		case 80:	//下
			placeCursor(screen,6,16);
			cout<<" "<<UP<<" "<<endl;
			placeCursor(screen,7,16);
			cout<<C_DOWN<<endl;
			placeCursor(screen,14,0);
			return BTN_DOWN;	
			break;
	}
	return 0;
}

//輸入樓層
int  floorButton_select(HANDLE screen,int current_floor,int dirButton){
	int range[2]={1,10};	//[min,max]
	int desired_floor=0;
	
	//判斷上下限
	switch(dirButton){
		case BTN_UP:
			range[0]=current_floor+1;
			break;
		case BTN_DOWN:
			range[1]=current_floor-1;
			break;
	}

	//檢查有效值
	while(1){
		placeCursor(screen,14,0);
		cout<<CLEAR<<endl<<CLEAR;
		placeCursor(screen,14,0);
		cout<<"please Enter your desired floor("<<range[0]<<"~"<<range[1]<<"): ";
		cin>>desired_floor;
		if(desired_floor<range[0]||desired_floor>range[1]){
			placeCursor(screen,15,0);
			cout<<"Invalid floor";
			Sleep(1000);
		}
		else break;
	}

	//回傳結果
	placeCursor(screen,14,0);
	cout<<CLEAR<<endl<<CLEAR;
	return desired_floor;
}

//初始畫面：要求輸入模擬器的初始條件 1、使用者當前樓層 2、左電梯預設樓層 3、右電梯預設樓層
void get_init_conditions(HANDLE screen,int info[]){
	
	//info[3] = [當前樓層，電梯1初始樓層，電梯2初始樓層]
	bool flag[3]={false,false,false};
	string parameter_name[3]={"current floor","elevator 1's initial floor","elevator 2's initial floor"};

	//顯示表單
	placeCursor(screen,1,1);
	cout<<"*************************"<<endl;
	placeCursor(screen,2,1);
	cout<<"@Your current floor:     "<<endl;
	placeCursor(screen,3,1);
	cout<<"@Elevators' initial floor:"<<endl;
	placeCursor(screen,4,1);
	cout<<"@ 1:          2:          "<<endl;
	placeCursor(screen,5,1);
	cout<<"*************************"<<endl;
	

	//輸入資訊
	//************待修：輸入字串會錯誤
	placeCursor(screen,2,22);
	cin>>info[0];
	cin.ignore();
	placeCursor(screen,4,6);
	cin>>info[1];
	cin.ignore();
	placeCursor(screen,4,18);
	cin>>info[2];
	cin.ignore();

	//檢查無效值
	for(int i=0;i<=2;i++){
		if(info[i]<=0||info[i]>10) flag[i]=true;
	}

	//要求重新輸入
	while(flag[0]||flag[1]||flag[2]){
		
		//提示
		system("CLS");
		placeCursor(screen,1,1);
		cout<<"Invalid Value:";
		for(int i=0;i<=2;i++){
			if(flag[i]) cout<<"\n  "<<parameter_name[i];
		}

		//隔開
		cout<<"\n\n";

		//重新輸入
		for(int i=0;i<=2;i++){
			if(flag[i]){
				cout<<"  "<<parameter_name[i]<<": ";
				cin>>info[i];
				cin.ignore();
			}
		}

		//重新檢查
		for(int i=0;i<=2;i++){
			if(info[i]<=10&&info[i]>0) flag[i]=false;
		}
	}
	
	return;
}

