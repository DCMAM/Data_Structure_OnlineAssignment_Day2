#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

/*
	error:
	1. belum random food
	2. enter_queue();
	3. update profit
*/

int profit = 0;
char curr_user[50];
int total_user = 0;
int total_foods = 0;
int total_queue = 0;

struct priority_queue{
	char name[32];
	int member;
	int time_;
	struct priority_queue *next;
	struct priority_queue *prev;
}*head = NULL, *tail = NULL;

void queue_pop_head(){
	struct priority_queue* curr = head;
	if(curr->time_ == 0){
		if(curr == tail){	//data is only 1
			curr = head = tail = NULL;
			free(curr);
		}
		else{
			head = head->next;
			free(curr);
			head->prev = NULL;
		}
	}
}

void priority_queue_push(char name[], int member, int time_){
	struct priority_queue *curr, *temp;
	curr = (struct priority_queue*)malloc(sizeof(struct priority_queue));
	
	strcpy(curr->name, name);
	curr->member = member;
	curr->time_ = time_;
	curr->next = NULL;
	
	total_queue++;
	
	if (head == NULL)
	{
		head = curr;
	}
	else if (head->member > member)
	{
		curr->next = head;
		head = curr;
	}
	else
	{
		temp = head;
		while(temp->next != NULL && member > temp->next->member)
		{
			temp = temp->next;
		}
		curr->next = temp->next;
		temp->next = curr;
	}
}

void queue_update(char name[]){
	struct priority_queue *curr = head;
	
	while(curr!=NULL) {
		if(strcmpi(curr->name, name) == 0) {
			curr->member = 2;
			return;
		}
		
		if(curr->next != NULL)
			curr = curr->next;
		else
			break;
	}
}


void view_queue(){
	struct priority_queue *curr = head;
	int i = 1;
	
	printf("+===========+==================================+==========+======+\n");
	printf("| Queue %-3s | %-32s | %-8s | %-4s |\n", "No.", "Name", "Member" , "Time");
	printf("+===========+==================================+==========+======+\n");
	while(curr){ // curr != NULL
		printf("| Queue %-3d | %-32s |", i, curr->name);
		if(curr->member == 1) printf(" %-8s", "vip");
		else if(curr->member == 2) printf(" %-8s", "standard");
		printf(" | %-4d |\n" , curr->time_);
		curr = curr->next;
		i++;
	}
	printf("+===========+==================================+==========+======+\n");	
}

struct user{
	char name[50];
	char password[50];
	int member;
	int time_;
	struct user *next;
	struct user *prev;
}*user_head = NULL, *user_tail = NULL;

void random_user(int index){
	struct user *curr = user_head;
	int i = 0;
	while(curr){ // curr != NULL
		if(i+1 == index) priority_queue_push(curr->name, curr->member, curr->time_);
		curr = curr->next;
		i++;
	}
}

void user_priority_push(char name[], char password[], int member, int time_){
	struct user *curr, *temp;
	curr = (struct user*)malloc(sizeof(struct user));
	
	strcpy(curr->name, name);
	strcpy(curr->password, password);
	curr->member = member;
	curr->time_ = time_;
	curr->next = NULL;
	
	if (user_head == NULL){
		user_head = curr;
	}
	else if (user_head->member > member){
		curr->next = user_head;
		user_head = curr;
	}
	else{
		temp = user_head;
		while(temp->next != NULL && member >= temp->next->member){
			temp = temp->next;
		}
		curr->next = temp->next;
		temp->next = curr;
	}
}

void update_user_member(char name[]){
	struct user *curr = user_head;
	
	while(curr!=NULL) {
		if(strcmpi(curr->name, name) == 0) {
			curr->member = 1;
			return;
		}
		
		if(curr->next != NULL)
			curr = curr->next;
		else
			break;
	}
}

void view_users(){
	struct user *curr = user_head;
	int i = 1;

	printf("+===========+==================================+==========+======+\n");
	printf("| Queue %-3s | %-32s | %-8s | %-4s |\n", "No.", "Name", "Member" , "Time");
	printf("+===========+==================================+==========+======+\n");
	while(curr){ // curr != NULL
		printf("| Queue %-3d | %-32s |", i, curr->name);
		if(curr->member == 1) printf(" %-8s", "vip");
		else if(curr->member == 2) printf(" %-8s", "standard");
		printf(" | %-4d |\n" , curr->time_);
		curr = curr->next;
		i++;
	}
	printf("+===========+==================================+==========+======+\n");	
}

void read_user_csv(){
	char line[200][300];

	char email[100];
	char password[100];
	char member[100];
	int num;

	FILE *fp = fopen("users.csv", "r");
	
	int x=0;
    while (1) {
		if(feof(fp)) break;
		fscanf(fp, "%[^\n]", line[x]); 
		fgetc(fp);
		
		char *p = strtok(line[x], ";");
	
		int z=0;
	    while(p != NULL){
	    	if(z%4 == 0) strcpy(email, p);
	    	if(z%4 == 1) strcpy(password, p);
	    	if(z%4 == 2) strcpy(member, p);
	    	if(z%4 == 3) num = atoi(p);
	        p = strtok (NULL, ";");
	        z++;
		}
		
		int member2=0;
		
		if(strcmpi(member, "vip")==0) member2 = 1;
		if(strcmpi(member, "standard")==0) member2 = 2;
		
		if(member2 > 0) user_priority_push(email, password, member2, 0);
		total_user++;
		x++;
    }
}

int validate_email(char email[], char pass[]){
	struct user *curr = user_head;
	int i = 1;
	while(curr){
		if(strcmp(curr->name, email) == 0 && strcmp(curr->password, pass) == 0){
		return 1;
		}
		curr = curr->next;
		i++;
	}
	return 0;
}

struct food{
	char name[50];
	int price;
	int stock;
	int time;
	struct food *next;
	struct food *prev;
}*food_head = NULL, *food_tail = NULL;

void pop_head_food(){
	struct food* curr = food_head;
	if(curr ==  food_tail){	//data is only 1
		curr = food_head = food_tail = NULL;
		free(curr);
	}
	else{
		food_head = food_head->next;
		free(curr);
		food_head->prev = NULL;
	}
}

void food_priority_push(char name[], int price, int stock, int time){
	struct food *curr, *temp;
	curr = (struct food*)malloc(sizeof(struct food));
	
	strcpy(curr->name, name);
	curr->price = price;
	curr->stock = stock;
	curr->time = time;
	curr->next = NULL;
	
	total_foods++;
	
	if (food_head == NULL)
	{
		food_head = curr;
	}
	else if (strcmp(food_head->name, name) >= 0)
	{
		curr->next = food_head;
		food_head = curr;
	}
	else
	{
		temp = food_head;
		while(temp->next != NULL && strcmp(name, temp->next->name) >=0 )
		{
			temp = temp->next;
		}
		curr->next = temp->next;
		temp->next = curr;
	}
}

void update_food_stock(int index, int updated){
	struct food *curr = food_head;
	
	int i= 1;
	while(curr!=NULL) {
		if(i == index) {
			curr->stock = curr->stock + updated;
			return;
		}
		
		if(curr->next != NULL)
			curr = curr->next;
		else break;
		
		i++;
	}
}

void view_foods(){
	struct food *curr = food_head;
	int i = 1;
	
	printf("+=====+===============================================+=======+=======+======+\n");
	printf("| %-3s | %-45s | %-5s | %-5s | %-4s |\n", "No.", "Food Name", "Price", "Stock", "Time");
	printf("+=====+===============================================+=======+=======+======+\n");
	while(curr){ 
		printf("| %-3d | %-45s | %-5d | %-5d | %-4d |\n", i, curr->name, curr->price, curr->stock, curr->time);
		curr = curr->next;
		i++;
	}
	printf("+=====+===============================================+=======+=======+======+\n");	
}

void read_food_csv(){
	char line[200][300];

	char name[100];
	int price;
	int stock;
	int time;

	FILE *fp = fopen("foods.csv", "r");
	
	int x=0;
    while (1) {
		if(feof(fp)) break;
		fscanf(fp, "%[^\n]", line[x]); 
		fgetc(fp);
		
		char *p = strtok(line[x], ";");
	
		int z=0;
	    while(p != NULL){
	    	if(z%4 == 0) strcpy(name, p);
	    	if(z%4 == 1) price = atoi(p);
	    	if(z%4 == 2) stock = atoi(p);
	    	if(z%4 == 3) time = atoi(p);
	        p = strtok (NULL, ";");
	        z++;
		}
		
		food_priority_push(name, price, stock, time);
		
		x++;
    }
}

void cls(){
	for(int i=0; i<45; i++){
		printf("\n");
	}
}

void print_menu(){
	printf("SALT'O\n");
	printf("1. Login\n");
	printf("2. Register\n");
	printf("3. Exit\n");
	printf("Choose >> ");
}

void program_first_run(){
	read_food_csv();
	read_user_csv();
	random_user(rand() % total_user);
	random_user(rand() % total_user);
}

void update_stock(){
	int index=0, addition=0;
	do{
		printf("Choose food index to update [1..16]: ");
		scanf("%d", &index);	
		break;
	}while(1);
	printf("How many stock do you want to add: ");
	scanf("%d", &addition);
	update_food_stock(index, addition);
}

void view_profit(){
	cls();
	printf("Current profit : $%d\n", profit);
}

void admin_page(){
	int option=0;
	do{
		cls();
		view_foods();
		printf("\nWelcome Admin\n");
		printf("==============\n");
		printf("1. Update stock\n");
		printf("2. View profit\n");
		printf("3. Log out\n");
		printf("Choose >> ");
		scanf("%d",  &option);
		if(option==1) update_stock();
		else if(option == 2) view_profit();
		else if(option == 3) break;
		system("pause");
	}while(1);
}

void search_and_push(char email[]){
	struct user *curr = user_head;
	int i = 0;
	while(curr){ // curr != NULL
		if(strcmpi(email, curr->name) == 0) priority_queue_push(curr->name, curr->member, curr->time_);
		curr = curr->next;
		i++;
	}
}

int check_user(char email[]){
	struct priority_queue *curr = head;
	if(strcmp(curr->name, email) == 0) return 1;
	return 0;
}

void queue_update_time(){
	struct priority_queue *curr = head;
	curr->time_ = curr->time_ - 1;
}

void queue_update_time_1(int i){
	struct priority_queue *curr = head;
	curr->time_ = i;
}

int return_food_time(int i){
	struct food *curr = food_head;
	
	int x=1;
	while(curr){ 
		if(i == x) {
			return curr->time;
		}
		x++;
		curr = curr->next;	
	}
	return 0;
}

int check(){
	struct food *curr = food_head;
	if(curr->time <= 0 )return 1;
	return 0;
}

void queue(char email[]){
	int time=0;
	do{
		if(check_user(email) == 0){
			int random = rand() % total_foods;
			time = return_food_time(random);
			queue_update_time_1(time);
			
			do{
				view_queue();
				queue_update_time();
				if(check() == 1) break;
				queue_pop_head();
				Sleep(1000);
			}while(1);
		}
		else if(check_user(email) == 1){
			view_foods();
			int index=0;
			do{
				printf("\nWhat do you want to order [1..%d]: ", total_foods);
				scanf("%d", &index);
			}while(index < 1 || index > total_foods);
			time = return_food_time(index);
			queue_update_time_1(time);
			do{
				view_queue();
				queue_update_time();
				if(check() == 1) break;
				queue_pop_head();
				Sleep(1000);
			}while(1);
			break;
		}
		
	}while(1);
}

void enter_queue(char email[]){
	char option[10];
	do{
		printf("Are you sure want to queue ? [yes/no]: ");	
		scanf("%s", option); getchar();
		if(strcmp(option, "yes") == 0) break;
		else if(strcmp(option, "no") == 0) return;
	}while(1);
	
	search_and_push(email);
	
	queue(email);
}

void user_page(char email[]){
	int option=0;
	do{
		cls();
		view_queue();
		printf("\nWelcome %s\n", email);
		printf("==============\n");
		printf("1. Enter Queue\n");
		printf("2. Upgrade to VIP\n");
		printf("3. Log out\n");
		printf("Choose >> ");
		scanf("%d",  &option); getchar();
		if(option==1) enter_queue(email);
		else if(option == 2) update_user_member(email);
		else if(option == 3) break;
		system("pause");
	}while(1);
}

void login(){
	char email[50], password[32];
	do{
		cls();
		printf("Input 'exit' to cancel\n");
		printf("Input email: ");
		scanf("%[^\n]", email); getchar();
		if(strcmpi(password, "cancel") == 0) return;
	}while(strlen(email) < 1);
	do{
		cls();
		printf("Input 'exit' to cancel\n");
		printf("Input password: ");
		scanf("%[^\n]", password); getchar();
		if(strcmpi(password, "cancel") == 0) return;
	}while(strlen(password) < 1);
	if(strcmpi(email, "admin") == 0 && strcmpi(password, "admin") == 0) admin_page();
	else if(validate_email(email, password) == 1){
		strcpy(curr_user, email);
		user_page(email);
	} 
	else printf("Wrond credentials\n");
}

void register_page(){
	char email[50], password[32];
	int age;
	do{
		cls();
		printf("Input 'exit' to cancel\n");
		printf("Input email [name@gmail.com]: ");
		scanf("%[^\n]", email); getchar();
		if(strcmpi(password, "cancel") == 0) return;
	}while(strlen(email) < 1);
	do{
		cls();
		printf("Input 'exit' to cancel\n");
		printf("Input your password [must be at least 8 characters]: ");
		scanf("%[^\n]", password); getchar();
		if(strcmpi(password, "cancel") == 0) return;
	}while(strlen(password) < 1);
	do{
		cls();
		printf("Input 'exit' to cancel\n");
		printf("Input your age [must be greater than 0]: ");
		scanf("%d", &age); getchar();
		if(strcmpi(password, "cancel") == 0) return;
	}while(age < 1);
	user_priority_push(email, password, 2, 0);
	printf("Your account has been registered . . .\n");
}

int main(){
	srand(time(NULL));
	
	int menu=0;
	program_first_run();
//	view_foods();
//	view_queue();
	do{
//		view_users();
		cls();
		print_menu();
		scanf("%d", &menu);  getchar();
		if(menu==1) login();
		else if(menu==2) register_page();
		else if(menu==3) break;
		system("pause");
	}while(1);
	
	return 0;
}
