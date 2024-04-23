//Game #1

//Notes:
//Can choose removed opponents

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define ver "0.1.4"

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

int menu1(){
	printf("Main menu\n");
	printf("1 - New game\n2 - How to play\n3 - Quit (no save)\n");
	printf("Your action: ");
	int ch;
	scanf("%d", &ch);
	switch(ch){
		case 1:
			
			return 1;
			break;
		case 2:
			printf("\nHOW TO PLAY\n\n");
			printf("1. Choose the number of characters to participate.\n");
			printf("2. Choose your character ID. IDs are automatically generated (1, 2, 3...)\n");
			printf("3. You can choose to start a new battle, view your character's stats,\n   opponents you haven't fought yet or exit the current game.\n");
			printf("4. Stats include HP, ATK, AGI. If the sum of your stats is greater than\n   your opponent's, you win and get an increase of 2%% for each stat.\n   Otherwise, you lose and get nothing.\n");
			printf("5. You can choose between the options again after each battle.\n");
			printf("6. After your last battle, you'll be shown your results and will\n   return to the main menu.\n\n");
			return 2;
			break;
		default:
			printf("\nExited");
			return 0;
			break;
	}
}

int menu2(){
	printf("\n1 - Battle\n2 - Stats\n3 - Opponents\n4 - Quit (no save)\n");
	printf("Your action: ");
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
	printf("HP: %.2f\tATK: %.2f\tAGI: %.2f\n\n", a.hp, a.atk, a.agi);
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
	for (int k=0; k<*n; k++){
		if (a[k] == x){
			for (int j=k; j<*n; j++) a[j] = a[j+1];
			--*n;
		}
	}
}

int checkid(char c[], int n, int y){
	int count;
	for (int i=0; i<n; i++)
		if (y == c[i]) count++;
	return count;
}

void battle(chr a[], char b[], int *n, int x, int n1){
	int y;
	printf("Choose your opponent: ");
	scanf("%d", &y);
	while (y < 1 ||/* checkid(b, y) == 1 || */y == x || y > n1){
			printf("Unavailable.\n");
			scanf("%d", &y);
		}
	plr(b, n, y);
	
	printf("Battling...\n\n");
	sleep(2);
	
	float c = a[x-1].atk + a[x-1].agi + a[x-1].hp;
	float d = a[y-1].atk + a[y-1].agi + a[y-1].hp;
	
	printf("Your score: %.2f\nOpponent's score: %.2f\n\n", c, d);
	printf("Opponent's stats:\n");
	viewstat(a[y-1], y);
	a[x-1].battles++;
	if (c > d){
		printf("You win!\nAll stats +2%%\n");
		a[x-1].atk *= 1.02;
		a[x-1].hp *= 1.02;
		a[x-1].agi *= 1.02;
		
		a[x-1].score++;
	}
	else if (c < d) printf("You lose!\n\n");
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
				printf("\nNumber of characters?\n");
				scanf("%d", &n);
				if (n<1) printf("Invalid.\n");
				else if (n==1) printf("Need at least 2 characters.\n");
			} while (n < 2);
			
			n1 = n;
			printf("Generating characters...\n");
			plrgen(cha, n);
			plrid(list, n);	
			
			sleep(1);
			printf("Done!\n");
		
			printf("\nChoose your character ID: ");
			scanf("%d", &x);
			while (x < 1 || x > n){
				printf("Unavailable.\n");
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
		
		if (ch != 1) break;
		ch = menu1();
	} while (ch != 0);
}
