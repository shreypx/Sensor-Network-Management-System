/*PRABHAT DINKAR CHAUDHARI
BT19CSE020
DSPD-II Homework assignment 1*/
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
	int x;
	int y;

	struct sensor_node *next;
	struct sensor_node *prev;
	//for doubly linked lists
};
//pointer pointing to head of list of sensors
struct sensor_node *first_sensor;

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
	struct record *next;
	struct record *prev;
	struct sensor_node *pointer_to_sensor;//pointer to that perticular sensor in the list of all sensors
};
//pointer pointing to head of list of record
struct record *first_record=NULL;

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
	struct AQI_node *next;
	struct AQI_node *prev;
	char status[40];
};
struct AQI_node *first_AQI;



//FUNCTIONS:
void display1(struct sensor_node* new_node)
{
	while(new_node!=NULL)
	{
		printf("ID: %d ",new_node->sensor_ID);
		printf("of type: %s \n",new_node->sensor_type);
		new_node=new_node->next;
	}
}
void display2(struct record* new_node)
{
	while(new_node!=NULL)
	{
		printf("%d ",new_node->sensor_ID);
		printf("%d/%d/%d   \n",new_node->day,new_node->month,new_node->year);
		new_node=new_node->next;
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


float calc_distance(struct sensor_node *p,struct sensor_node *q)
{
	float res;

	//distance equation
	//distance= sqrt((x1-x2)^2 + (y1-y1)^2)

	res=((p->x - q->x)*(p->x - q->x))+((p->y - q->y)*(p->y - q->y));
	return sqrt(res);
}


void central_repository(struct record* new_record,struct sensor_node *p)
{
	struct record* new_entry;//new_record is the new record added
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
	if(new_record==NULL)
	{
		new_entry->prev=NULL;
		new_entry->next=NULL;
		first_record=new_entry;
	}
	else
	{
		while(new_record->next!=NULL)
		{
			new_record=new_record->next;
		}
		new_entry->prev=new_record;
		new_record->next=new_entry;
		new_entry->next=NULL;
	}
}

void create_sensor_list(struct sensor_node* new_node)//new_node is pointer for traversing through list
{
	struct sensor_node* new_sensor;//new_sensor is the new node creating
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

	//for first node
	if(new_node==NULL)
	{
		new_sensor->prev=NULL;
		new_sensor->next=NULL;
		first_sensor=new_sensor;
		new_sensor->x=0;
		new_sensor->y=0;
	}
	else
	{
		printf("Enter the co-ordinaes of the sensor(x,y): \n");
		scanf("%d%d",&new_sensor->x,&new_sensor->y);
		while(new_node->next!=NULL)
		{
			new_node=new_node->next;
		}
		new_sensor->prev=new_node;
		new_node->next=new_sensor;
		new_sensor->next=NULL;
	}
	central_repository(first_record,new_sensor);
}

//function for installing new sensor in between two sensors
void install_new_sensor(struct sensor_node *new_node)
{
	int ID1,ID2;
	float distance=0;
	char type[20];
	//pointers for traversing and marking 2 sensors
	struct sensor_node *p,*q,*temp;
	p=first_sensor;
	q=p;
	int position_p=0,position_q=0;


	//getting data
	printf("Installing a new sensor between two sensors.....");
	printf("enter ID of first sensor: ");
	scanf("%d",&ID1);
	printf("enter ID of second sensor: ");
	scanf("%d",&ID2);
	printf("What is the type of sensor?");
	scanf("%s",type);

	//bringing p and q at positions of the two sensors
	while(p->sensor_ID!=ID1)
	{
		p=p->next;
		position_p=position_p+1;
	}
	while(q->sensor_ID!=ID2)
	{
		q=q->next;
		position_q=position_q+1;
	}

	//for considering station p is before q, we should swap q and p if user entered q before p
	//it'll make aou work easy ahead
	if(position_p>position_q)
	{
		temp=p;
		p=q;
		q=temp;
	}

	//finding distance between two stations from the distance list for checking distance condition
	distance=calc_distance(p,q);

	//checking the sensor_type condition
	int flag=1;
	while(p->next!=q)
	{
		p=p->next;
		if(strcmp(type,p->sensor_type)==0)
		{
			flag=0;
		}

	}

	//checking distance condtion
	if(distance>=10 && flag==1)
	{

		create_sensor_list(first_sensor);
		//now since this function will put the new sensor at last, we needd to rearrange the sensors system
		while(new_node->next!=NULL)
		{
			new_node=new_node->next;
		}
		new_node->prev->next=NULL;
		p->next=new_node;
		new_node->prev=p;
		new_node->next=q;
		q->prev=new_node;
	}
	else
	{
		int choice;
		printf("cannot add  sensor:\neither distance between thode two stations are less than 10KM\nOR\nthe sensor type already exist in between two entered sensors\n\nDo you want to re-enter data?\n1: yes\n2:NO");
		printf("distance= %f",distance);
		scanf("%d",&choice);
		if(choice==1)
		{
			install_new_sensor(first_sensor);
		}
	}



}

//function for removing node that has duration of more than 2 months
void delete_record(struct record* p)
{
	struct record *q;
	q=p;
	//two pointers for traversing through the list
	while(p!=NULL)
	{
		q=p;
		//61 days for 2 months
		if(p->pointer_to_sensor->duration>=61)
		{
			//deleting node of the sensor list
			if(p->pointer_to_sensor->prev==NULL)
			{
				p->pointer_to_sensor->next->prev=NULL;
				first_sensor=p->pointer_to_sensor->next;
				free(p->pointer_to_sensor);
			}
			else if(p->pointer_to_sensor->next==NULL)
			{
				p->pointer_to_sensor->prev->next=NULL;
				free(p->pointer_to_sensor);
			}
			else
			{
				p->pointer_to_sensor->prev->next=p->pointer_to_sensor->next;
				p->pointer_to_sensor->next->prev=p->pointer_to_sensor->prev;
				free(p->pointer_to_sensor);
			}


			//deleting node of the record list
			if(p->prev==NULL)
			{
				p=p->next;
				p->prev=NULL;
				first_record=p;
				free(q);
				q=p;
			}
			else
			{
				p=p->next;
				p->prev=q->prev;
				q->prev->next=p;
				free(q);
			}
		}
		p=p->next;
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
	printf("enter the typr of sensor: ");
	scanf("%s",type);
	switch(choice){
		case 1:
			printf("enter the date till which you want to retrieve data (in form DD/MM/YYYY): ");
			scanf("%d%d%d",&D,&M,&Y);
			double date;
			date=Y*10000 + M*100 +D;
			while(node!=NULL)
			{
				if(strcmp(node->pointer_to_sensor->sensor_type,type)==0)
				{
					//for checking till date condition
					if(node->data < date)
					{
						display(node->pointer_to_sensor);
					}
				}
				node=node->next;
			}
			break;

		case 2://for specific date for specific sensor type
			printf("enter the date till which you want to retrieve data (in form DD/MM/YYYY): ");
			scanf("%d%d%d",&D,&M,&Y);
			while(node!=NULL)
			{
				if(strcmp(node->pointer_to_sensor->sensor_type,type)==0 && Y==node->year && M==node->month && D==node->day)
				{
					display(node->pointer_to_sensor);
				}
				node=node->next;
			}
			break;
			
		case 3:
			printf("enter the time intervals (HH:MM): )");
			printf("time 1: ");
			scanf("%d%d",&hr,&min);
			time1=(hr*100)+min;//since it will be of form HHMM it'll be eaqsy for comparison
			printf("time 2: ");
			scanf("%d%d",&hr,&min);
			time2=(hr*100)+min;
			
			printf("how would you like to retrieve?\n");
			printf("1: for specified date (single day)\n");
			printf("2: for specified date range (multiple dates)\n");
			scanf("%d",&option);
			switch(option)
			{
				case 1:
					
					printf("enter the date till which you want to retrieve data (in form DD/MM/YYYY): ");
					scanf("%d%d%d",&D,&M,&Y);
					while(node!=NULL)
					{
						if(Y==node->year && M==node->month && D==node->day)
						{
							if(node->time>time1 && node->time<time2)
							{
								display(node->pointer_to_sensor);
							}
						}
						node=node->next;
					}
					break;

				case 2:
					printf("enter the first date (DD/MM/YY):\n ");
					scanf("%d%d%d",&D,&M,&Y);
					date1=Y*10000 + M*100 +D;
					
					printf("enter the second date (DD/MM/YY):\n ");
					scanf("%d%d%d",&D,&M,&Y);
					date2=Y*10000 + M*100 +D;
					
					while(node!=NULL)
					{
						if(node->date > date1 && node->date < date2)
						{
							if(node->time >time1 && node->time >time2)
							{
								display(node->pointer_to_sensor);
							}
						}
						node=node->next;
					}
					break;
					
				default:
					printf("Didn't enter valid option!!!");
					break;
			}
			break;
			
		default:
			printf("Didn't enter valid option!!!");
			break;
			
	}
}

//function to find communicative neighbours
void find_communicating_neighbours(int ID,struct sensor_node *current_sensor)
{
	//since we have ID, traversing to the node that contain this ID
	struct sensor_node *p;
	p=current_sensor;
	while(p->sensor_ID!=ID)
	{
		p=p->next;
	}
	float distance;
	printf("ID's of all communicating sensors are:\n( ");
	//traversing through whole list to find neighbours
	while(current_sensor!=NULL)
	{
		//checking distance conditions
		distance=calc_distance(current_sensor,p);
		if(distance<10 && distance!=0)
		{
			//printing the ID if distance condition matches
			printf("%d, ",current_sensor->sensor_ID);
		}
		current_sensor=current_sensor->next;
	}
	printf(")");
}



//FUNCTIONS for AQI problems
void create_AQI(struct AQI_node *last_node)
{
	struct AQI_node *new_node;
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
	
	
	//for first node
	if(last_node==NULL)
	{
		new_node->prev=NULL;
		new_node->next=NULL;
		first_AQI=new_node;
	}
	else
	{
		while(last_node->next!=NULL)
		{
			last_node=last_node->next;
		}
		new_node->prev=last_node;
		last_node->next=new_node;
		new_node->next=NULL;
	}
}

//6.a finding max AQI for all years
void max_AQI(struct AQI_node *p)
{
	int max=0;
	struct AQI_node *q;
	while(p!=NULL)
	{
		if(p->flag==1)
		{
			max=p->total;
			q=p;
			while(q!=NULL)
			{
				if(q->year==p->year)
				{
					q->flag=0;
					if(max < q->total)
					{
						max=q->total;
					}
				}
				q=q->next;
			}
			printf("Maximum AQI in the year %d is %d\n",p->year,max);
		}
		p=p->next;
	}
}

//for case 2
void get_date_AQI(struct AQI_node *p,char status[])
{
	printf("date on which %s conditiopn was recorded were:\n");
	//checking condition of health status by traversing through list
	while(p!=NULL)
	{
		if(strcmp(status,p->status)==0)
		{
			printf("%d/%d/%d\n",p->day,p->month,p->year);
		}
		p=p->next;
	}
}

void displayaqi(struct AQI_node *p)
{
	while(p!=NULL)
	{
		printf("%d %d/%d/%d\n",p->total,p->day,p->month,p->year);
		p=p->next;
	}
}

//MAIN FUNCTION
int main()
{
	printf("\t\t\t\tPrabhat Dinkar Chaudhari\n\t\t\t\tBT19CSE020\n\t\t\t\tASSIGNMENT 3 -CPL\n\t\t\t\t__________________________________\n\n");
	
	//for creating sensors and the repository
	printf("!Firstly we need to create the system of sensor stations!\n\n");
	printf("               _____________Creating the list of sensors__________\n\n");
	int flag;
	do
	{
		create_sensor_list(first_sensor);
		printf("Enter 0 to stop OR any number to enter another sensor: ");
		scanf("%d",&flag);
	}while(flag!=0);
	display1(first_sensor);
	printf("\n");
	display2(first_record);
	printf("\n");
	
	printf("- - - - - - - - - - - - - - - - - - - - - - - - - - ");
	flag=1;
	int choice;
	int choice_ret;
	int ID;
	do
	{
		printf("what do you want to perform??\n");
		printf("1: install new sensor\n");
		printf("2: delete idle sensors\n");
		printf("3: retrieve information of a sensor\n");
		printf("4: find communicating neighbours for a sensor");
		printf("5: exit!");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				install_new_sensor(first_sensor);
				display1(first_sensor);
				break;
			case 2:
				delete_record(first_record);
				display2(first_record);
				break;
			case 3:
				printf("Enter how you want to retrieve information of the sensor:\n");
				printf("1: Depending on sensor type (retrieves till date data)\n");
				printf("2: Depending on specified date for specific sensor type\n");
				printf("3: Depending on specific time interval for specific sensor type\n");
				printf("enter choice: ");
				scanf("%d",&choice_ret);
				retrieve_info(choice_ret,first_record);
				break;
			case 4:
				printf("Enter ID: ");
				scanf("%d",&ID);
				find_communicating_neighbours(ID,first_sensor);
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
		create_AQI(first_AQI);
		printf("Enter:");
		scanf("%d",&n);
	}while(n!=0);
	printf("\n");
	displayaqi(first_AQI);
	printf("For AQI information:\n");
	printf("1: display the month during which maximum AQI is reported for all years for all stations\n");
	printf("2: find out the date on which particular health status is recorded\n");
	printf("3: display the dates on which hazardous health status is recorded for all stations\n");

	printf("Enter option: ");
	scanf("%d",&n);
	char status[40];
	switch(n){
		case 1:
			max_AQI(first_AQI);
			break;
		case 2:
			printf("Enter ther health status(use only small letters and _ instead of space): ");
			scanf("%s",status);
			get_date_AQI(first_AQI,status);
			break;
		case 3:
			//since we made get_date_AQI function, we need to only pass the string "hazardous"
			get_date_AQI(first_AQI,"hazardous");
			break;
		case 4:
			printf("Wrong option entered");
	}
}



