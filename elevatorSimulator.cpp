/*
	�q��������G
	�ާ@�覡�G
		1�G����J��e�Ӽh�B��ӹq�誺��l�Ӽh
		2�G�D����-����L�ާ@
			�W�U��V��G��ܹq�誺��V���s
			Enter����G�T�{�ҿ諸��V���s
			r,R		�G�H�����ʹq��
			f,F		�G���s��J�ϥΪ̷�e�Ӽh
			q,Q		�G�����{��
*/

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <string>
#include <iomanip>
#include <math.h>
#include <cstdlib> /* �üƬ������ */
#include <ctime>   /* �ɶ�������� */

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
void enviroment(HANDLE screen,int current_floor);	//��ܤ���
int	 dirButton_select(HANDLE screen, int dir,int current_floor);
int  floorButton_select(HANDLE screen,int current_floor,int dirButton);

/*
���n��m�G
	�q���V�ܷN�Ÿ��G��2,14 �k2,21
	�q��Ӽh�ܷN�Ÿ��G��3,13 �k3,20
	�W���s�G6,17
	�U���s�G7,17
	���ҤU��t�G14
*/

void placeCursor(HANDLE screen,int row, int col)
{
    COORD position;    // holds a pair of X and Y coordinates
    position.Y = row;
    position.X = col;
    SetConsoleCursorPosition(screen, position);
}

// �q�����O
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
    void display_floor(HANDLE screen);	//��ܷ�e�Ӽh 
    void move(HANDLE screen,int floor);	//���� 
	int  current_floor;	//��e�Ӽh 
	int  f_row,f_col;		//�Ӽh�ܷN��mrow,col
	int  d_row,d_col;		//��V�ܷN��mrow,col
};

// ��ܷ�e�Ӽh 
void Elevator::display_floor(HANDLE screen) {
	//cout << "Elevator " << id << "'s current floor : " << current_floor << endl;
    placeCursor(screen,f_row,f_col);
	cout << setw(2) << current_floor;
	placeCursor(screen,14,0);
	return;
}

// ���� 
void Elevator::move(HANDLE screen,int floor){
	int step=(floor>current_floor)?1:-1;	//+/-
	char sign=(floor>current_floor)?UP:DOWN;
	
	//���ʤ��A��ܤ�V�ܷN�Ÿ��A�ô��ܥ��b���ʡA�@��@��
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
	//���ʧ��A�M����V�ܷN�Ÿ��ô���
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

// �{�����檺 main()
int main(void) {
    // �ŧi�ëإߨ�ӹq�誫�� 
    
    int chosen_elevator_id;	//��w���q��id
	int current_floor;		//�ϥΪ̪���e�Ӽh
	int chosen_dirButton;		//��w����V���s
	int desired_floor;	//��w���Ӽh
	int conditions[3]={-1,-1,-1}; //[��e�Ӽh�A�q��1��l�Ӽh�A�q��2��l�Ӽh]
	HANDLE screen = GetStdHandle(STD_OUTPUT_HANDLE);
	srand( time(NULL) );

	//��J��e�Ӽh�M�]�w�q���l�Ӽh:
	get_init_conditions(screen,conditions);
	Elevator elevator0(1,conditions[1],3,13,2,14),elevator1(2,conditions[2],3,20,2,21);
    Elevator *chosen_elevator=&elevator0;
	current_floor=conditions[0];
	
	while(1){
		bool done=false;	//�^�X����
		system("CLS");
		enviroment(screen,current_floor);
		elevator0.display_floor(screen);
		elevator1.display_floor(screen);
		chosen_dirButton=-1;	//��w����V���s
		int new_current_floor;	//�Ω�ק��e�Ӽh
		while(!done){
			placeCursor(screen,0,37);
			cout<<"Current floor:"<<current_floor;
			placeCursor(screen,14,0);
			switch(getch()){
				//��V��
				case 0:
				case 224:
					//��V��G��ܤW�U���s
					chosen_dirButton=dirButton_select(screen,getch(),current_floor);
					break;
				//�T�{��J
				case ENTER:
					//�B�z�S��N��ENTER�����p
					if(chosen_dirButton==-1) break;
					// �P�_���ιq��
					abs(current_floor-elevator0.current_floor)>abs(current_floor-elevator1.current_floor)
					?chosen_elevator=&elevator1:chosen_elevator=&elevator0;
					//�e���ϥΪ̷�e�Ӽh
					chosen_elevator->move(screen,current_floor);
					//��J�e���Ӽh
					desired_floor=floorButton_select(screen,current_floor,chosen_dirButton);
					placeCursor(screen,14,0);
					//���ܭn�e�����Ӽh
					cout<<" Desired floor : "<<desired_floor;
					Sleep(1000);
					placeCursor(screen,14,0);
					chosen_elevator->move(screen,desired_floor);
					current_floor=desired_floor;
					done = true;	//�^�X����
					break;
				//�H�����ʹq��
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
				//���s�]�w��e�Ӽh
				case 'f':
				case 'F':
					while(1){
						placeCursor(screen,14,0);
						cout<<"Enter new current floor: ";
						cin>>new_current_floor;
						//�˴��ȬO�_�X�z
						if(new_current_floor<1||new_current_floor>10){
							placeCursor(screen,15,0);
							cout<<"Invalid floor.";
							Sleep(1000);
							placeCursor(screen,14,0);
							cout<<CLEAR<<endl<<CLEAR;
						}
						else break;	
					}
					//����
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
				//�����{��
				case 'q':
				case 'Q':
					system("PAUSE");
					return 0; 
					break;
			}	
		}	
	}

}

//�������(�]�t�O�_���ïS�w�q���V���s)
void enviroment(HANDLE screen,int current_floor){
	// �W���s�G6,17
	// �U���s�G7,17
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

	// �R��1��10�Ӫ��L�ī��s
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

//��ܹq���V���s
int dirButton_select(HANDLE screen,int dir,int current_floor){

	//�W���s�G6,17
	//�U���s�G7,17
	//���ҤU��t�G14
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
		case 72:	//�W
			placeCursor(screen,7,16);
			cout<<" "<<DOWN<<" "<<endl;
			placeCursor(screen,6,16);
			cout<<C_UP<<endl;
			placeCursor(screen,14,0);
			return BTN_UP;	
			break;
		case 80:	//�U
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

//��J�Ӽh
int  floorButton_select(HANDLE screen,int current_floor,int dirButton){
	int range[2]={1,10};	//[min,max]
	int desired_floor=0;
	
	//�P�_�W�U��
	switch(dirButton){
		case BTN_UP:
			range[0]=current_floor+1;
			break;
		case BTN_DOWN:
			range[1]=current_floor-1;
			break;
	}

	//�ˬd���ĭ�
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

	//�^�ǵ��G
	placeCursor(screen,14,0);
	cout<<CLEAR<<endl<<CLEAR;
	return desired_floor;
}

//��l�e���G�n�D��J����������l���� 1�B�ϥΪ̷�e�Ӽh 2�B���q��w�]�Ӽh 3�B�k�q��w�]�Ӽh
void get_init_conditions(HANDLE screen,int info[]){
	
	//info[3] = [��e�Ӽh�A�q��1��l�Ӽh�A�q��2��l�Ӽh]
	bool flag[3]={false,false,false};
	string parameter_name[3]={"current floor","elevator 1's initial floor","elevator 2's initial floor"};

	//��ܪ��
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
	

	//��J��T
	//************�ݭסG��J�r��|���~
	placeCursor(screen,2,22);
	cin>>info[0];
	cin.ignore();
	placeCursor(screen,4,6);
	cin>>info[1];
	cin.ignore();
	placeCursor(screen,4,18);
	cin>>info[2];
	cin.ignore();

	//�ˬd�L�ĭ�
	for(int i=0;i<=2;i++){
		if(info[i]<=0||info[i]>10) flag[i]=true;
	}

	//�n�D���s��J
	while(flag[0]||flag[1]||flag[2]){
		
		//����
		system("CLS");
		placeCursor(screen,1,1);
		cout<<"Invalid Value:";
		for(int i=0;i<=2;i++){
			if(flag[i]) cout<<"\n  "<<parameter_name[i];
		}

		//�j�}
		cout<<"\n\n";

		//���s��J
		for(int i=0;i<=2;i++){
			if(flag[i]){
				cout<<"  "<<parameter_name[i]<<": ";
				cin>>info[i];
				cin.ignore();
			}
		}

		//���s�ˬd
		for(int i=0;i<=2;i++){
			if(info[i]<=10&&info[i]>0) flag[i]=false;
		}
	}
	
	return;
}

