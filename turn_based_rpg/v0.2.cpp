//Game #1

//Notes:
//Can choose removed opponents

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define ver "0.2"

typedef struct character{
	float atk, agi, hp;
	int battles, score;
} chr;

//generate stats
void plrgen(chr a[], int n){
	srand((unsigned)time(NULL));
	for (int i=0; i<n; i++){
		a[i].atk = (float)rand()/(float)(RAND_MAX/100);
		a[i].agi = (float)rand()/(float)(RAND_MAX/100);
		a[i].hp = (float)rand()/(float)(RAND_MAX/100);
	}
}

void changelog(){
	printf("\nChangelog:\n");
	
	printf("\n v0.2:   Added Changelog, can no longer choose opponents that you've fought\n       to exploit stats increase, improved visuals.\n");
	printf("\n v0.1.4: Added How to play option, fix various display issues, improved visuals.\n");
	printf("\n v0.1:   Initial release.\n\n");
}

int menu1(){
	printf("MAIN MENU\n\n");
	printf(" 1 - New game\n 2 - How to play\n 3 - Changelog\n 4 - Quit (no save)\n");
	printf("\n <Input>: ");
	int ch;
	scanf("%d", &ch);
	switch(ch){
		case 1:
			
			return 1;
			break;
		case 2:
			printf("\nHOW TO PLAY\n\n");
			printf(" 1. Choose the number of characters to participate.\n");
			printf("\n 2. Choose your character ID. IDs are automatically generated (1, 2, 3...)\n");
			printf("\n 3. You can choose to start a new battle, view your character's stats,\n    list opponents you haven't fought yet or exit the current game.\n");
			printf("\n 4. Stats include HP, ATK, AGI. If the sum of your stats is greater than\n    your opponent's, you win and get an increase of 2%% for each stat.\n    Otherwise, you lose and get nothing.\n");
			printf("\n 5. You can choose between the options again after each battle.\n");
			printf("\n 6. After your last battle, you'll be shown your results and will return\n    to the main menu.\n\n");
			return 2;
			break;
		case 3:
			changelog();
			return 3;
			break;
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

void viewstat(chr a, int x){
	printf("HP: %.2f\tATK: %.2f\tAGI: %.2f\n", a.hp, a.atk, a.agi);
}

//create a list of IDs
void plrid(char a[], int n){
	for (int i=0; i<n; i++){
		a[i] = i+1;
	}
}

//show list of IDs
void plrid_list(char a[], int n){
	printf("\nOpponents: ");
	for (int i=0; i<n; i++){
		printf("%d ", a[i]);
	}
	printf("\n");
}

//add to a checklist
void blacklist(char a[], int x){
}

//remove plr ID from list
void plr(char a[], int *n, int x){
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

void battle(chr a[], char b[], int *n, int x, int n1){
	int y, z=0;
	printf("\nChoose your opponent: ");
	scanf("%d", &y);
	
	do{
		if (checkid(b, *n, y) == 1) break;
		else{
			printf(" (!) Unavailable.\n\n");
			printf("Choose your opponent: ");
			scanf("%d", &y);
			z = checkid(b, *n, y);
		}
	} while (z != 1);
			
	plr(b, n, y);
	
	printf("\nBattling...\n\n");
	sleep(2);
	
	float c = a[x-1].atk + a[x-1].agi + a[x-1].hp;
	float d = a[y-1].atk + a[y-1].agi + a[y-1].hp;
	
	printf("Your score:       %.2f\nOpponent's score: %.2f\n\n", c, d);
	printf("Opponent's stats:\n");
	viewstat(a[y-1], y);
	a[x-1].battles++;
	if (c > d){
		printf("\n\tYou win!\n\tAll stats +2%%\n");
		a[x-1].atk *= 1.02;
		a[x-1].hp *= 1.02;
		a[x-1].agi *= 1.02;
		
		a[x-1].score++;
	}
	else if (c < d) printf("\n\tYou lose!\n");
}

int main(){
	chr cha[100];
	char list[100];
	int n, x, n1;
	printf("GAME v%s\n\n", ver);
	int ch=1;
	fflush(stdin);
	ch = menu1();
	do{
		do{
			if (ch != 1) break;
			do{
				printf("\nNumber of characters?  ");
				scanf("%d", &n);
				if (n<1) printf(" (!) Invalid.\n");
				else if (n==1) printf(" (!) Need at least 2 characters.\n");
			} while (n < 2);
			
			n1 = n;
			printf("\nGenerating characters...\n");
			plrgen(cha, n);
			plrid(list, n);	
			
			sleep(1);
			printf("Done!\n");
		
			printf("\nChoose your character ID: ");
			scanf("%d", &x);
			while (x < 1 || x > n){
				printf(" (!) Unavailable.\n");
				printf("\nChoose your character ID: ");
				scanf("%d", &x);
			}
			plr(list, &n, x);
			cha[x-1].score = 0;
			cha[x-1].battles = 0;
		
			do{
				ch = menu2();
				if (ch == 0) break;
				else if (ch == 3) plrid_list(list, n);
				else if (ch == 2){
					printf("\nYour stats (ID: %d):\n", x);
					viewstat(cha[x-1], x);
				}
				else if (ch == 1) battle(cha, list, &n, x, n1);
				
				if (n == 0){
					printf("\nResult:\n");
					printf("Battles fought: %d\nScore: %d\n\n", cha[x-1].battles, cha[x-1].score);
					
					printf("Win ratio: %.1f%%\n\n", ((float)cha[x-1].score / (float)cha[x-1].battles)*100);
					break;
				}
			} while (ch != 0);
			
			if (n == 0) break;
			ch = menu1();
		} while (ch != 0);
		
		if (ch == 0) break;
		ch = menu1();
	} while (ch != 0);
}
