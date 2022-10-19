/*PRABHAT DINKAR CHAUDHARI
BT19CSE020
DSPD-II Homework assignment 2*/
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<math.h>

//STRUCTURES:


//node for the sensor for list of sensors
struct sensor_node{
	int sensor_ID;
	char sensor_type[15];
	char sensor_location[15];
	int duration;


	struct sensor_node *lchild;
	struct sensor_node *rchild;
	//for a binary search tree
};
//pointer pointing to root of the tree
struct sensor_node *root;

//________________________________________________________

//node for record that'll store date,time,etc
struct record{
	int sensor_ID;
	int year;
	int month;
	int day;
	double date;
	int hour;
	int min;
	int time;
	float data;
	struct record *lchild;
	struct record *rchild;
	struct sensor_node *pointer_to_sensor;//pointer to that perticular sensor in the list of all sensors
};
//pointer pointing to head of list of record
struct record *record_root=NULL;

//node for the sensor for list of sensors
struct AQI_node{
	int PM10;
	int PM2;
	int NO2;
	int SO2;
	int CO;
	int O;
	int year;
	int month;
	int day;
	int total;
	int flag;
	struct AQI_node *lchild;
	struct AQI_node *rchild;
	char status[40];
};
struct AQI_node *AQI_root;



//FUNCTIONS:
void display1(struct sensor_node* new_node)
{
	if(new_node!=NULL)
	{
		display1(new_node->lchild);
		printf("ID: %d ",new_node->sensor_ID);
		printf("of type: %s \n",new_node->sensor_type);
		display1(new_node->rchild);
	}

}

void display2(struct record* new_node)
{
	if(new_node!=NULL)
	{
		display2(new_node->lchild);
		printf("%d ",new_node->sensor_ID);
		printf("%d/%d/%d   \n",new_node->day,new_node->month,new_node->year);
		display2(new_node->rchild);
	}

}

//for displaying data of only one specific sensor
void display(struct sensor_node *p)
{
	printf("\n     sensor ID-[%d] retrieved\n",p->sensor_ID);
	printf("sensor type: %s\n",p->sensor_type);
	printf("sensor location: %s\n",p->sensor_location);
	printf("Duration of report : %d\n",p->duration);
}

//central repository function
void central_repository(struct record* new_record,struct sensor_node *p)
{
	struct record *new_entry,*r;//new_record is the new record added
	new_entry=(struct record*)malloc(sizeof(struct record));
	//getting basic data
	new_entry->sensor_ID=p->sensor_ID;
	printf("Enter the date (DD/MM/YYYY):\n");
	printf("DD: ");
	scanf("%d",&new_entry->day);
	printf("MM: ");
	scanf("%d",&new_entry->month);
	printf("YYYY: ");
	scanf("%d",&new_entry->year);

	//keeping whole date as one integer so that comparison become easy
	new_entry->date=new_entry->day*10000 + new_entry->month*100 + new_entry->year;

	printf("Enter time of recording data (HH:MM) :\n");
	printf("HH: ");
	scanf("%d",&new_entry->hour);
	printf("MM: ");
	scanf("%d",&new_entry->min);

	//keeping whole time as one integer so that comparison become easy
	new_entry->time= new_entry->hour*100 + new_entry->min;
	new_entry->pointer_to_sensor=p;//since we are using a pointer to point at that very specific node we don't have to traverse through the list of all sensors
	//for first node
	
	//if its the first root entry
	if(new_record==NULL)
	{
		new_entry->lchild=NULL;
		new_entry->rchild=NULL;
		record_root=new_entry;
	}
	//for other entries
	else
	{
		//assigning node to the binary search tree
		while(new_record!=NULL)
		{
			r=new_record;
			if(new_entry->pointer_to_sensor->sensor_ID < new_record->pointer_to_sensor->sensor_ID)
			{
				new_record=new_record->lchild;
			}
			else if(new_entry->pointer_to_sensor->sensor_ID > new_record->pointer_to_sensor->sensor_ID)
			{
				new_record=new_record->rchild;
			}
			else
			{
				return;
			}
		}
		new_entry->lchild=NULL;
		new_entry->rchild=NULL;

		if(new_entry->pointer_to_sensor->sensor_ID < r->pointer_to_sensor->sensor_ID)
		{
			r->lchild=new_entry;
		}
		else
		{
			r->rchild=new_entry;
		}
	}
}

//this functions append a sensor in the list
void create_sensor_list(struct sensor_node* new_node)//new_node is pointer for traversing through list
{
	struct sensor_node* new_sensor;//new_sensor is the new node creating
	struct sensor_node* r;
	
	new_sensor=(struct sensor_node*)malloc(sizeof(struct sensor_node));
	//getting basic data
	printf("Enter ID of the sensor: ");
	scanf("%d",&new_sensor->sensor_ID);
	printf("Enter Type of the sensor: ");
	scanf("%s",new_sensor->sensor_type);
	printf("Enter the location of the sensor: ");
	scanf("%s",new_sensor->sensor_location);
	printf("Enter sensing time interval of the sensor: ");
	scanf("%d",&new_sensor->duration);
	new_sensor->lchild=NULL;
	new_sensor->rchild=NULL;
	//root is null
	if(new_node==NULL)
	{
		root=new_sensor;
		central_repository(record_root,new_sensor);
		return;
	}
	
	//for condition other than root
	while(new_node!=NULL)
	{
		//r is a tail pointer that follows new node pointer
		r=new_node;
		if(new_sensor->sensor_ID < new_node->sensor_ID)
		{
			new_node=new_node->lchild;
		}
		else if(new_sensor->sensor_ID > new_node->sensor_ID)
		{
			new_node=new_node->rchild;
		}
		else
		{
			return;//if two sensor with same ID exist then do nothing
		}
	}
	if(new_sensor->sensor_ID < r->sensor_ID)
	{
		r->lchild=new_sensor;
	}
	else
	{
		r->rchild=new_sensor;
	}
	
	central_repository(record_root,new_sensor);
}

//for checking the type of sensor condition
int traverse_flag=0;
//in order traversal function
void traversal_for_installing(struct sensor_node* p,char str[])
{
   	if(p != NULL)
	{
	    traversal_for_installing(p->lchild,str);
	    if(strcmp(str,p->sensor_type)==0)
	    {
	    	traverse_flag=1;
		}
	    traversal_for_installing(p->rchild,str);
   	}
}

//function for installing new sensor in between two sensors
void install_new_sensor(struct sensor_node *new_node)
{
	char str[15];
	printf("Enter the type of sensor: ");
	scanf("%s",&str);
	//by in order traversal
	traversal_for_installing(root,str);
	if(traverse_flag==1)
	{
		return;
	}
	create_sensor_list(root);
	traverse_flag=0;
}

//finding minimum node of the sensor tree
struct sensor_node* min_node(struct sensor_node* node)
{
    while (node && node->lchild!= NULL)
    {
    	node = node->lchild;
	}
	return node;
}

struct sensor_node* delete_sensor(struct sensor_node* node,int ID)
{
	//if node gets null
    if (node == NULL)
        return node;

    if (ID < node->sensor_ID)
    {
    	node->lchild=delete_sensor(node->lchild,ID);
	}

    // key, then it lies in right subtree if greater than root
    else if (ID > node->sensor_ID)
    {
    	node->rchild= delete_sensor(node->rchild,ID);
	}

	//condition satisfies, ID same as node ID
    else {
        // node with only one child or no child
        if (node->lchild == NULL)
		{
            struct sensor_node* temp = node->rchild;
            free(node);
            return temp;
        }
        else if (node->rchild == NULL) {
            struct sensor_node* temp = node->rchild;
            free(node);
            return temp;
        }

        //smallest in the right subtree
        struct sensor_node* temp = min_node(node->rchild);
        node->sensor_ID=temp->sensor_ID;


		//deleting by inorder
        node->rchild = delete_sensor(node->rchild,temp->sensor_ID);
    }
    return node;
}

//finding idle sensors
void idle_sensors(struct record* p)
{
   	if(p != NULL)
	{
	    idle_sensors(p->lchild);
	    if(p->pointer_to_sensor->duration > 61)
	    {
			delete_sensor(root,p->pointer_to_sensor->sensor_ID);
		}
	    idle_sensors(p->rchild);
   	}
}


void traversal_for_retrieving(struct record* p,int date,char str[])
{
   	if(p != NULL)
	{
	    traversal_for_retrieving(p->lchild,date,str);
	    if(p->date<=date && strcmp(str,p->pointer_to_sensor->sensor_type)==0)
	    {
	    	display(p->pointer_to_sensor);
		}
	    traversal_for_retrieving(p->rchild,date,str);
   	}
}

void traversal_for_retrieving_2(struct record* p,int date1,int date2,char str[])
{
   	if(p != NULL)
	{
	    traversal_for_retrieving_2(p->lchild,date1,date2,str);
	    if(date1<=p->date && p->date<=date2 && strcmp(str,p->pointer_to_sensor->sensor_type)==0)
	    {
	    	display(p->pointer_to_sensor);
		}
	    traversal_for_retrieving_2(p->rchild,date1,date2,str);
   	}
}

//function to retrieve information
void retrieve_info(int choice,struct record* node)
{
	int D,M,Y;
	int hr,min,time1,time2;
	double date1,date2;
	int option;
	char type[15];
	printf("enter the type of sensor: ");
	scanf("%s",type);
	switch(choice){
		case 1:
			printf("enter the date till which you want to retrieve data (in form DD/MM/YYYY): ");
			scanf("%d%d%d",&D,&M,&Y);
			double date;
			date=Y*10000 + M*100 +D;
			traversal_for_retrieving(record_root,date,type);
			break;

		case 2://for specific date for specific sensor type
			printf("enter the first date (DD/MM/YY):\n ");
			scanf("%d%d%d",&D,&M,&Y);
			date1=Y*10000 + M*100 +D;
			printf("enter the second date (DD/MM/YY):\n ");
			scanf("%d%d%d",&D,&M,&Y);
			date2=Y*10000 + M*100 +D;

			traversal_for_retrieving_2(record_root,date1,date2,type);
			break;


		default:
			printf("Didn't enter valid option!!!");
			break;

	}
}

//using in-order traversal method
void sensors_in_between(int ID1, int ID2, struct sensor_node* p)
{
   	if(p != NULL)
	{
	    sensors_in_between(ID1,ID2,p->lchild);
	    if(p->sensor_ID>ID1 && p->sensor_ID<ID2)
	    {
	    	display(p);
		}
		if(p->sensor_ID<ID1 && p->sensor_ID>ID2)
		{
			display(p);
		}
		//considered if statement for both the conditions i,e if ID1 is bigger or ID2 is bigger
	    sensors_in_between(ID1,ID2,p->rchild);
	}
}




//FUNCTIONS for AQI problems
void create_AQI(struct AQI_node *last_node)
{
	struct AQI_node *new_node,*r;//r is a tail pointer that follows last_node ptr
	new_node=(struct AQI_node *)malloc(sizeof(struct AQI_node ));
	printf("Enter amount of PM 10: ");
	scanf("%d",&new_node->PM10);
	printf("Enter amount of Nitrogen Dioxide: ");
	scanf("%d",&new_node->NO2);
	printf("Enter amount of sulphur dioxide: ");
	scanf("%d",&new_node->SO2);
	printf("Enter amount of carbon monoxide ");
	scanf("%d",&new_node->CO);
	printf("Enter amount of ground level ozone: ");
	scanf("%d",&new_node->O);

	int temp;
	printf("Enter data of PM2.5:\n");
	printf("Enter amount of PM 2.5 in first 8 hours: ");
	scanf("%d",&temp);
	new_node->PM2= new_node->PM2 +temp;
	printf("Enter amount of PM 2.5 in next 8 hours: ");
	scanf("%d",&temp);
	new_node->PM2= new_node->PM2 +temp;
	printf("Enter amount of PM 2.5 in first 8 hours: ");
	scanf("%d",&temp);
	new_node->PM2= new_node->PM2 +temp;

	new_node->PM2= (new_node->PM2)/3;

	printf("Enter the date of report(DD/MM/YYYY): \n");
	printf("Enter DD: ");
	scanf("%d",&new_node->day);
	printf("Enter MM: ");
	scanf("%d",&new_node->month);
	printf("Enter YYYY: ");
	scanf("%d",&new_node->year);

	new_node->total=(new_node->PM10 + new_node->PM2 + new_node->NO2 + new_node->SO2 + new_node->CO + new_node->O)/6;
	new_node->flag=1;
	//for noting status
	if(new_node->total>0 && new_node->total<=50)
	{
		strcpy(new_node->status,"good");
	}
	if(new_node->total>50 && new_node->total<=100)
	{
		strcpy(new_node->status,"satisfactory");
	}
	if(new_node->total>100 && new_node->total<=200)
	{
		strcpy(new_node->status,"moderately_polluted");
	}
	if(new_node->total>200 && new_node->total<=300)
	{
		strcpy(new_node->status,"poor");
	}
	if(new_node->total>300 && new_node->total<=400)
	{
		strcpy(new_node->status,"may_cause_respiratory_illness");
	}
	if(new_node->total>400 && new_node->total<=500)
	{
		strcpy(new_node->status,"severe");
	}
	if(new_node->total>501)
	{
		strcpy(new_node->status,"hazardous");
	}


	//for the root node
	if(AQI_root==NULL)
	{
		new_node->lchild=NULL;
		new_node->rchild=NULL;
		AQI_root=new_node;
		return;
	}
	while(last_node!=NULL)
	{
		r=last_node;
		if(new_node->total < last_node->total)
		{
			last_node=last_node->lchild;
		}
		else if(new_node->total > last_node->total)
		{
			last_node=last_node->rchild;
		}
		else
		{
			return;
		}
	}
	new_node->lchild=NULL;
	new_node->rchild=NULL;
	if(new_node->total < r->total)
	{
		r->lchild=new_node;
	}
	else
	{
		r->lchild=new_node;
	}
}

//6.a finding max AQI for all years
void max_AQI(struct AQI_node *current)
{
    while (current->rchild != NULL)
    {
    	current = current->rchild;
	}
    printf("\nThe max AQI reported is %d\n",current->total);
}

//for case 2
void get_date_AQI(struct AQI_node *p)
{
	printf("date on which %s conditiopn was recorded were:\n");
	//checking condition of hazardous health status by traversing through list
	while(p!=NULL)
	{
		get_date_AQI(p->lchild);
		if(strcmp("hazardous",p->status)==0)
		{
			printf("%d/%d/%d\n",p->day,p->month,p->year);
		}
		get_date_AQI(p->rchild);
	}

}


void displayaqi(struct AQI_node *p)
{
	while(p!=NULL)
	{
		displayaqi(p->lchild);
		printf("%d %d/%d/%d\n",p->total,p->day,p->month,p->year);
		displayaqi(p->rchild);
	}
}

//MAIN FUNCTION
int main()
{
	printf("\t\t\t\tPrabhat Dinkar Chaudhari\n\t\t\t\tBT19CSE020\n\t\t\t\tHW ASSIGNMENT 2 -DSPD\n\t\t\t\t__________________________________\n\n");

	//for creating sensors and the repository
	printf("!Firstly we need to create the system of sensor stations!\n\n");
	printf("               _____________Creating the list of sensors__________\n\n");
	int flag;
	do
	{
		create_sensor_list(root);
		printf("Enter 0 to stop OR any number to enter another sensor: ");
		scanf("%d",&flag);
	}while(flag!=0);
	
	display1(root);
	printf("\n");
	display2(record_root);
	printf("\n");

	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - ");
	flag=1;
	int choice;
	int choice_ret;
	int ID1,ID2;
	do
	{
		printf("\n\nwhat do you want to perform??\n");
		printf("1: install new sensor\n");
		printf("2: delete idle sensors\n");
		printf("3: retrieve information of a sensor\n");
		printf("4: find communicating neighbours for a sensor\n");
		printf("5: exit!");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				install_new_sensor(root);
				display1(root);
				break;
			case 2:
				idle_sensors(record_root);
				display2(record_root);
				break;
			case 3:
				printf("Enter how you want to retrieve information of the sensor:\n");
				printf("1: retrieves till date data for a specific type\n");
				printf("2: Depending on date range for specific sensor type\n");

				printf("enter choice: ");
				scanf("%d",&choice_ret);
				retrieve_info(choice_ret,record_root);
				break;
			case 4:
				printf("Enter the range of IDs\n ");
				printf("Enter ID-1: ");
				scanf("%d",&ID1);
				printf("Enter ID-2: ");
				scanf("%d",&ID2);
				sensors_in_between(ID1,ID2,root);
				break;
			case 5:
				flag=0;
				break;
			default:
				printf("Enter correct option\n");
				break;
		}
	}while(flag!=0);



	printf("\n--------------------------------------------------------------\n\n");
	int n;
	do{
		create_AQI(AQI_root);
		printf("Enter:");
		scanf("%d",&n);
	}while(n!=0);
	printf("\n");
	displayaqi(AQI_root);
	printf("For AQI information:\n");
	printf("1: display the month during which maximum AQI is reported for all years for all stations\n");
	printf("2: display the dates on which hazardous health status is recorded for all stations\n");

	printf("Enter option: ");
	scanf("%d",&n);
	char status[40];
	switch(n){
		case 1:
			max_AQI(AQI_root);
			break;

		case 2:
			//since we made get_date_AQI function, we need to only pass the string "hazardous"
			get_date_AQI(AQI_root);
			break;
		default:
			printf("Wrong option entered");
			break;
	}
}



