//GAME #2

// should be able to flee/surrender mid-fight (?)
// re-implement score?
// currently no endscreen

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

typedef struct{
	int def;
	float hp, atk, hpo;
} plr;

//generate stats
void plrGen(plr a[], int n){
	srand((unsigned)time(NULL));
	for (int i=0; i<n; i++){
		a[i].hp = rand()/(RAND_MAX/600);
		a[i].atk = rand()/(RAND_MAX/100);
		a[i].def = rand()/(RAND_MAX/70);
		
		if (a[i].hp < 250)
			a[i].hp+=300;
		if (a[i].atk < 50)
			a[i].atk+=25;
		if (a[i].def < 30)
			a[i].def+=25;
			
		a[i].hpo = a[i].hp;
	}
}

int menu1(){
	printf("\n\n == MAIN MENU ==\n\n");
	printf(" 1 - New game\n 2 - How to play\n 3 - Quit (no save)\n");
	printf("\n <Input>: ");
	int ch;
	scanf("%d", &ch);
	switch(ch){
		case 1:
			
			return 1;
			break;
		case 2:
			printf("\n == HOW TO PLAY ==\n\n");
			printf(" 1. Choose the number of characters to participate.\n");
			printf("\n 2. Choose your character ID. IDs are automatically generated (1, 2, 3...)\n");
			printf("\n 3. You can choose to start a new battle, view your character's stats,\n    list opponents you haven't fought yet or exit the current game.\n");
			printf("\n 4. Stats include HP, ATK, DEF. You and the opponent will take turn to\n    attack, choose 1 of 4 moves available, your opponent can't block.\n");
			printf("\n 5. You can choose between the options again after each battle.\n");
			printf("\n 6. After your last battle, you'll return to the main menu.\n\n");
			return 2;
			break;
//		case 3:
//			changelog();
//			return 3;
//			break;
		default:
			printf("\n <Console>: Exited");
			return 0;
			break;
	}
}

int menu2(){
	printf("\n 1 - Battle\n 2 - Stats\n 3 - Opponents\n 4 - Quit (no save)\n");
	printf("\n <Input>: ");
	int ch;
	scanf("%d", &ch);
	switch(ch){
		case 1:
			return 1;
			break;
		case 2:
			return 2;
			break;
		case 3:
			return 3;
			break;
		default:
			printf("\n");
			return 0;
			break;
	}
}

void menu3(plr a){
	printf("\nMoves:\n");
	printf(" - Attack (A)     : %.0f damage\n", a.atk);
	printf(" - Crit Strike (C): %.0f damage (50%%)\n", a.atk * 1.5);
	printf(" - Block (B)      : Nullifies all dmg\n");
	printf(" - Heal (H)       : Heal %.0f hp\n\n\n", a.hpo * 0.1);
}

void viewstat(plr a){
	printf("\n   HP: %.0f\n  ATK: %.0f\n  DEF: %d\n", a.hp, a.atk, a.def);
}

void attack(plr a[], int x, int y){
	if (a[x-1].atk - a[y-1].def <= 0){
		a[y-1].hp -= 5;
		printf("\n\tDeals 5 damage.\n");
	}
	else{
		a[y-1].hp -= (a[x-1].atk - a[y-1].def);
		printf("\n\tDeals %.0f damage.\n", a[x-1].atk - a[y-1].def);
	}
}

void crit(plr a[], int x, int y){
	if (a[x-1].atk*1.1 - a[y-1].def <= 0){
		a[y-1].hp -= 5;
		printf("\n\tDeals 5 damage.\n");
	}
	else{
		a[y-1].hp -= (a[x-1].atk*1.5 - a[y-1].def);
		printf("\n\tDeals %.0f damage.\n", a[x-1].atk*1.5 - a[y-1].def);
	}
}

void heal(plr a[], int x){
	if (a[x-1].hp == a[x-1].hpo)
		printf("\n\tFull HP!\n");
	else if (a[x-1].hpo - a[x-1].hp < a[x-1].hpo*0.1){
		a[x-1].hp = a[x-1].hpo;
		printf("\n\tFully healed.\n");
	}
	else{
		a[x-1].hp += a[x-1].hpo * 0.1;
		printf("\n\tHealed %.0f hp.\n", a[x-1].hpo*0.1);
	}
}

//create a list of IDs
void plrid(char a[], int n){
	for (int i=0; i<n; i++){
		a[i] = i+1;
	}
}

//show list of IDs
void plrid_list(char a[], int n){
	printf("\n Opponents: ");
	for (int i=0; i<n; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}

//remove plr ID from list
void Plr(char a[], int *n, int x){
	for (int k=0; k<*n; k++)
		if (a[k] == x){
			for (int j=k; j<*n; j++) a[j] = a[j+1];
			--*n;
		}
}

//check for used id
int checkid(char b[], int n, int y){
	int count=0;
	for (int i=0; i<n; i++){
		if (y == b[i]) {
			return 1;
			break;
		}
		
		else if (y != b[i]){
			count++;
			if (count == n) return 0;
		}
	}
}

void battle(plr a[], char b[], int *n, int x, int n1){
	int y, z=0;
	char c;
	int c1, c2=0, c3=0, c4=0, c5=0;
	printf("\n (?) Choose your opponent: ");
	scanf("%d", &y);
	
	do{
		if (checkid(b, *n, y) == 1) break;
		else{
			printf(" (!) Unavailable.\n\n");
			printf(" (?) Choose your opponent: ");
			scanf("%d", &y);
			z = checkid(b, *n, y);
		}
	} while (z != 1);
			
	Plr(b, n, y);
	
	printf("\n <Console>: Begin the battle");
	
	sleep(1);
	
	printf("\n\n\n\n\n");
	
	
	do{
		menu3(a[x-1]);
		printf("Opponent:\n");
		viewstat(a[y-1]);
		printf("\n");
		printf("You:\n");
		viewstat(a[x-1]);
		printf("\n\n\n\n");
			
		do{
			c1 = 0;
			fflush(stdin);
			printf(" <Input>: ");
			scanf(" %c", &c);
			printf("\n\n\n (!) Your move:");
			switch (c){
				case 'A':
					attack(a, x, y);
					break;
				case 'C':
					c3 = rand()/(RAND_MAX/100);
					if (c3 <= 50)
						crit(a, x, y);
					else{
						printf("\n\tCrit failed.\n");
						break;
					}
					break;
				case 'B':
					printf("\n\tYou used Block.\n");
					break;
				case 'H':		
					c4 = rand()/(RAND_MAX/100);
					if (c3 <= 75)
						heal(a, x);
					else{
						printf("\n\tHeal failed.\n");
						break;
					}
					break;
				default:
					printf(" (!) Unavailable.\n");
					c1 = 1;
					break;
			}
		} while (c1 == 1);
		
		if (a[y-1].hp <= 0){
			printf("\n\tYou won!\n\tHealed x2\n");
			heal(a, x);
			heal(a, x);
			break;
		}
		
		printf("\n (!) Opponent's move:");
		
		do{
			c2 = rand() % 4;
			switch (c2){
				case 1:
					if (c == 'B'){
						printf("\n\tDeals 0 damage.\n");
						break;
					}
					else
						attack(a, y, x);
					break;
				case 2:
					if (c == 'B'){
						printf("\n\tDeals 0 damage.\n");
						break;
					}
					else
						crit(a, y, x);
					break;
				case 3:
					c5 = rand()/(RAND_MAX/100);
					if (c3 <= 30)
						heal(a, y);
					else{
						printf("\n\tHeal failed.\n");
						break;
					}
					break;
			}
			
		*n--;
		} while (c2 != 1 && c2 != 2 && c2 != 3);
		
	} while (a[x-1].hp > 0);
}

int main(){
	plr a[100];
	char list[100];
	int n, x, n1;
//	printf(" GAME v%s\n\n", ver);
	int ch=1;
	fflush(stdin);
	ch = menu1();
	do{
		do{
			if (ch != 1) break;
			do{
				printf("\n (?) Number of characters?  ");
				scanf("%d", &n);
				if (n<1) printf(" (!) Invalid.\n");
				else if (n==1) printf(" (!) Need at least 2 characters.\n");
			} while (n < 2);
			
			n1 = n;
			printf("\n <Console>: Generating characters");
			plrGen(a, n);
			plrid(list, n);	
			
			sleep(1); 
			printf(".");
			sleep(1); 
			printf(".");
			sleep(1); 
			printf(".");
			
			sleep(1);
			printf("  Done!\n");
		
			printf("\n (?) Choose your character ID: ");
			scanf("%d", &x);
			while (x < 1 || x > n){
				printf(" (!) Unavailable.\n");
				printf("\n (?) Choose your character ID: ");
				scanf("%d", &x);
			}
			Plr(list, &n, x);
		
			do{
				ch = menu2();
				if (ch == 0) break;
				else if (ch == 3) plrid_list(list, n);
				else if (ch == 2){
					printf("\n Your stats (ID: %d):\n", x);
					viewstat(a[x-1]);
				}
				else if (ch == 1) battle(a, list, &n, x, n1);
				
				if (a[x-1].hp <= 0){
					printf("\n\t! ! YOU DIED ! !\n\n\n");
					break;
				}
				
				if (n == 0) break;
			} while (ch != 0);
			
			if (n == 0) break;
			ch = menu1();
		} while (ch != 0);
		
		if (ch == 0) break;
		ch = menu1();
	} while (ch != 0);
}
