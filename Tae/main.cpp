#include"value.h"
#include"ServerGame.h"
#include"ClientUser.h"

void main(){

	int mode = 0;

	cout<< "==== Bingo Game ====" <<endl;
	cout<< "Select Mode [1. Server] [2. Client] [3. Exit] : ";
	cin>> mode;
	
	while(true){

		switch(mode){

		case MF_SERVER : 
			{

				CServerGame* game = new CServerGame;
				game->runGame();

				break;
			}
		case MF_CLIENT : 
			{ 

				CClientUser* user = new CClientUser;
				user->runUser();

				break;
			}
		case MF_EXIT : 
			{

				cout<< "Program will be down"<<endl;
				break;
			}
		default : 
			{ 

				cout<< "Inappropriate Input Try again"<<endl;
				break;
			}
		}

		if(mode < 0 || mode > 3)
			continue;
	}
}