#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
	unsigned char day;
	unsigned char month;
	unsigned int year;
}DATE;

typedef struct
{
	unsigned long long int id;
	char* name;
	char* family;
	DATE date_of_birthday;
	unsigned long long int partnerId;
	unsigned long long int motherId;
	unsigned long long int FatherId;
	unsigned char num_of_childern;
	unsigned long long int* children_ptr;
}PERSON;

typedef struct
{
	PERSON* person;
	int num_of_person;
	int actual_num_of_person;
}DB_MGR;


//func
void print_person(const PERSON* person); //print person details
void init_db(DB_MGR* p);//This function get input from the user and allocate the first number of persons array in DB_MGR
char menu(); //DataBase System Menu
void add_person(DB_MGR* residents); //add person to data base
void realloc_add_1_space(DB_MGR* residents); //add 1 more space to PERSON array
void setName(char** name); //set name to person
void setDateOfBirth(DATE* person); //set date of birth
unsigned long long int setFamilyId(); //set partner/father/mother id
void setChildren(PERSON* person); //set children id
void order_by_id(DB_MGR* p); // order by id from small to big
PERSON* search_id(const DB_MGR* p, unsigned long long int search_id);// Search Id
long long int check_id(); // check for all function if id input is correct and send it to the right variable
void search_person(const DB_MGR* p); // search for existing person
void search_parents(const DB_MGR* residents);//search for person parents, if found print parents details
void delete_person(DB_MGR* residents);//delete person from database
void delete_person_for_parents(PERSON* person, unsigned long long int kid_id);// help for delete_person function. delete the person from parents
void realloc_delete_1_space(DB_MGR* residents, unsigned long long int id);// delete 1 space from person array
void get_gen(const DB_MGR* residents); // next generation counter
void print_db(const DB_MGR* residents); //print data base
void search_by_name(const DB_MGR* residents);// search person by name
void quitAndClearMemory(DB_MGR* residents); //clear all memory and quit program

// main function get from menu function returning char and operate the match function
void main()
{
	char get_func;
	DB_MGR residents = { NULL, 0 };
	init_db(&residents);
	do
	{
		get_func = menu();
		if (get_func == 1)
			add_person(&residents);
		if (get_func == 2)
			search_person(&residents);
		if (get_func == 3)
			search_parents(&residents);
		if (get_func == 4)
			delete_person(&residents);
		if (get_func == 5)
			get_gen(&residents);
		if (get_func == 6)
			print_db(&residents);
		if (get_func == 7)
			search_by_name(&residents);

	} while (get_func != 8);

	quitAndClearMemory(&residents);
}

//print person details
//This function gets pointer to a specific person and print his details
//check if he have family members, if he have print their details
void print_person(const PERSON* person)
{
	printf("\nId: %llu, first name: '%s', last name: '%s', date of birth: %.2d-%.2d-%.4d",
		person->id, person->name, person->family, person->date_of_birthday.day, person->date_of_birthday.month,
		person->date_of_birthday.year);

	if (person->partnerId > 0)
	{
		printf("\nHis partner Id: %llu", person->partnerId);
	}
	if (person->motherId > 0)
	{
		printf("\nHis mother Id: %llu", person->motherId);
	}
	if (person->FatherId > 0)
	{
		printf("\nHis father Id: %llu", person->FatherId);
	}
	if (person->num_of_childern > 0)
	{
		printf("\nHe have %d kids", person->num_of_childern);
		for (int i = 0; i < person->num_of_childern; i++)
		{
			printf("\nkid %d Id: %llu", i + 1, person->children_ptr[i]);
		}
	}
}

//This function get input from the user and allocate the first number of persons array in DB_MGR
void init_db(DB_MGR* residents)
{
	do
	{
		if (residents->num_of_person < 0)
		{
			printf("\nInvalid input, try again");
		}

		printf("Please enter number of residents: ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &residents->num_of_person);
	} while (residents->num_of_person <= 0);

	residents->actual_num_of_person = 0;
	residents->person = (PERSON*)calloc(residents->num_of_person, sizeof(PERSON));
	if (!residents->person)
	{
		printf("Allocation failed\n");
		exit(1);
	}
}

//DataBase System Menu
//this function display the menu to the user and wait for his input
//the function return the char the user choose and send it to the main function
char menu()
{
	int x = 0;

	do
	{
		printf("\n\nDatabase System Menu:\n");
		printf("1. Add person\n");
		printf("2. Search a person\n");
		printf("3. Search Parents\n");
		printf("4. Delete a person\n");
		printf("5. Get generation\n");
		printf("6. Print database\n");
		printf("7. Search by name\n");
		printf("8. Quit\n");
		printf("Enter your choice: ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &x);

		if (x < 1 || x > 8)
		{
			printf("\nYou entered wrong value, try again\n");
		}

	} while (x < 1 || x > 8);
	char user_choice = (char)x;

	return user_choice;
}

//add person to the database
//This function execute option number 2 in the menu
void add_person(DB_MGR* residents)
{
	int i = 0;

	if (residents->person[0].id != 0)
	{
		realloc_add_1_space(residents);      //check if the id at index 0 = 0, if yes the array is full and its call
		i = residents->num_of_person - 1;    //to realloc_add_1_space to add 1 more space to the array before it can add a new person
	}
	else
	{
		for (i = 0; i < residents->num_of_person; i++)
		{
			if (residents->person[i].id == 0)           //if found and index with id 0 there is a free space in the array
			{                                           //break out of the loop and the person will be add at the array 
				break;
			}
		}
	}

	printf("\nHello, pls enter details of the person you want to add\n");

	//set person id
	printf("Enter person ID: ");
	residents->person[i].id = check_id();

	//set person first name
	printf("Enter person first name: ");
	setName(&residents->person[i].name);

	//set person last name
	printf("Enter person last name: ");
	setName(&residents->person[i].family);

	//Enter person birth of date
	setDateOfBirth(&residents->person[i].date_of_birthday);

	//Set partner id
	printf("This person have a partner? 1-yes 2-no  : ");
	residents->person[i].partnerId = setFamilyId();

	//set mother id
	printf("This person have a mother? 1-yes 2-no  : ");
	residents->person[i].motherId = setFamilyId();

	//set father id
	printf("This person have a father? 1-yes 2-no  : ");
	residents->person[i].FatherId = setFamilyId();

	//set children
	printf("How many kids he have: ");
	setChildren(&residents->person[i]);     

	order_by_id(residents); //order database by id number ascending
	residents->actual_num_of_person++; //enlarge actual num of person ampunt after add 1 more person to data base

}

//add 1 more space to PERSON array
//this function called by addPerson function when the person array is full
//the function is copy the person array to a temp array
//allocate again the person array with 1 more space and copy back temp to the original person array
//after this process addPerson function can add another person to the array
void realloc_add_1_space(DB_MGR* residents)
{
	DB_MGR tmp;
	residents->num_of_person++;
	tmp.person = (PERSON*)calloc(residents->num_of_person, sizeof(PERSON));
	if (!tmp.person)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	for (int i = 0; i < residents->num_of_person - 1; i++)
	{
		tmp.person[i] = residents->person[i];
	}
	residents->person = (PERSON*)calloc(residents->num_of_person, sizeof(PERSON));
	if (!residents->person)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	for (int i = 0; i < residents->num_of_person - 1; i++)
	{
		residents->person[i] = tmp.person[i];
	}

	free(tmp.person);

}

//set first or last name to AddPerson function
//this function get pointer to person first/last name 
//recive the name from the user and save it to temp string
//then copy the temp string to the person first/last name
void setName(char** name)
{
	char tempName[80];
	int nameSize = 0;

	fseek(stdin, 0, SEEK_END);
	gets(tempName);

	nameSize = strlen(tempName) + 1;

	*(name) = (char*)malloc(nameSize);
	if (!name)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	strcpy(*(name), tempName);
}

//set date of birth to AddPerson function
//this function get pointer to the birth of date of person
//recive the date from user and check if the date is valid
//and set the date to the person
void setDateOfBirth(DATE* date)
{
	int day = 0, month = 0;

	while (date->year > 9999 || date->year <= 1800)
	{
		printf("Enter person year of birth: ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &date->year);

		if (date->year > 9999 || date->year <= 1800)
		{
			printf("Wrong value, pls try again\n");
		}
	}

	while (month > 12 || month < 1)
	{
		printf("Enter person month of birth: ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &month);

		if (month > 12 || month < 1)
		{
			printf("Wrong value, pls try again\n");
		}
	}
	date->month = (unsigned char)month;

	while (day > 30 || day < 1)
	{
		printf("Enter person day of birth: ");
		fseek(stdin, 0, SEEK_END);
		scanf("%d", &day);

		if (day > 30 || day < 1)
		{
			printf("Wrong value, pls try again\n");
		}
	}
	date->day = (unsigned char)day;
}

//set partner/mother/father id to AddPerson function
//this function wait for the user choice 1-yes 2-no
//if the user choose 1 the function scan from the user the id
//if the id is valid the family member get this id
unsigned long long int setFamilyId()
{
	unsigned long long int tmp_id = 0;
	int temp = 0;

	while (temp != 1 && temp != 2)
	{
		scanf("%d", &temp);
		if (temp != 1 && temp != 2)
		{
			printf("\nTry again 1-yes 2-no : ");
		}
	}
	if (temp == 1)
	{
		printf("Enter his ID: ");
		tmp_id = check_id();

	}
	else
	{
		tmp_id = 0;
	}

	return tmp_id;
}

//set childrens id to AddPerson function
//the function get pointer to specific person
//then scan from the user the id for the children of the person
void setChildren(PERSON* person)
{
	int i = 0;

	do
	{
		scanf("%d", &i);

		if (i < 0)
		{
			printf("\nInvalid value, try again: ");
		}

	} while (i < 0);

	if (i > 0)
	{//allocate children_ptr of specific person by the input the user enter if the number is greater than 0
		person->num_of_childern = (unsigned char)i;
		person->children_ptr = (unsigned long long int*)malloc
		(person->num_of_childern * sizeof(unsigned long long int));
		if (!person->children_ptr)    //allocate children_ptr of specific person by the input the user enter (how many children)
		{
			printf("\nAllocation failed!");
			exit(1);
		}
	}
	else
	{
		person->num_of_childern = 0;
	}

	for (int i = 0; i < person->num_of_childern; i++)
	{
		printf("Enter id for children number %d: ", i + 1);
		person->children_ptr[i] = check_id();
	}
}

//order person array in data base by id ascending order - called by add person
//this function get pointer to specific data base
//after each peron that was add to the data base this function sort the person array by the id
void order_by_id(DB_MGR* p)
{
	PERSON tmp;

	for (int j = 0; j < p->num_of_person; j++)
	{
		for (int i = 0; i < p->num_of_person; i++)
		{
			if (p->person[i].id > p->person[i + 1].id)
			{
				tmp = p->person[i + 1];
				p->person[i + 1] = p->person[i];
				p->person[i] = tmp;
			}
		}
	}
}

//search if person exsit by Id - Help to search perosn function
//this function get pointer to specific data base and an Id number
//scan the data base if there is a person with this id
//if yes return pointer to the specific person
PERSON* search_id(const DB_MGR* p, unsigned long long int search_id)
{
	PERSON* p_person = NULL;
	for (int i = (p->num_of_person) - (p->actual_num_of_person); i < p->num_of_person; i++)
	{
		if (search_id == p->person[i].id)
		{
			p_person = &p->person[i];
			break;
		}
	}
	return p_person;
}

//check if id input is correct and send it to the right variable
//on each part in the code when the user need to enter id
//this function check if the scan id is valid
//if yes send the id that was scan from the user
long long int check_id()
{
	long long int temp_id;
	do
	{
		fseek(stdin, 0, SEEK_END);
		scanf("%lli", &temp_id);

		if (temp_id < 1)
		{
			printf("You entered wrond id, try again");
			printf("\nEnter Id: ");
		}

	} while (temp_id < 1);

	return temp_id;
}

//search for person by his id
//scan an id from the user and check if there is a peron on the data base with this id
//if found send this perosn to print_person function to print his details
void search_person(const DB_MGR* p)
{
	unsigned long long int id;
	printf("\nEnter the ID of the person you are looking for: ");
	id = check_id();

	PERSON* p_person = search_id(p, id);
	if (p_person == NULL)
	{
		printf("\nThere is no person with this ID");
	}
	else
	{
		printf("\nThe details of person was found on the system.");
		print_person(&*(p_person));
	}
}

//search for person parents, if found print parents details
//this function gets pointer to specific data base
//ask from the user id for the person who he want to search his parents
void search_parents(const DB_MGR* residents)
{
	unsigned long long int id;
	PERSON* p_person_mother = NULL, *p_person_father = NULL;
	printf("\nEnter the ID of the person you are looking for his parents: ");
	id = check_id();

	PERSON* p_person = search_id(residents, id);
	if (!p_person)                                  //check if the person exsist in the data base
	{
		printf("\nThere is no person with this ID");
	}

	else
	{
		if (p_person->FatherId > 0)  //if the person has a father, looking for his father in the data base
		{
			p_person_father = search_id(residents, p_person->FatherId);
			if (p_person_father)  //if the father exsist on the data base print his details
			{
				printf("\nHis father details:");
				print_person(p_person_father);
			}
		}
		if (p_person->motherId > 0)  //if the person has a mother, looking for his mother in the data base
		{
			p_person_mother = search_id(residents, p_person->motherId);
			if (p_person_mother)     //if the mother exsist on the data base print her details
			{
				printf("\nHis mother details:");
				print_person(p_person_mother);
			}
		}
		if (!p_person_mother && !p_person_father) //if none of the parents exsist in data base, print out "no parents"
		{
			printf("\nThis person dosent have a parents");
		}
	}
}

//delete person from database
//this function get pointer to sepcific data base
//ask the user the id of the person he want to delete from the data base
void delete_person(DB_MGR* residents)
{
	unsigned long long int id;
	int i = 0;
	PERSON* p_person, *p_temp;

	printf("\nEnter the ID of the person you are want to delete:");
	id = check_id();

	p_person = search_id(residents, id);
	if (p_person == NULL)  //check if the person exsist in the data base
	{
		printf("\nThis ID dosent exist on the system");
		return;
	}
	if (p_person->partnerId > 0) //if the person has partner and the partner found on the data base
	{                            //set person id to 0 at the partner details 
		p_temp = search_id(residents, p_person->partnerId);
		if (p_temp)
		{
			p_temp->partnerId = 0;
		}
	}
	if (p_person->FatherId > 0)  //if the person has father and the father found on the data base
	{                            //delete from the father the person to delete id - use delete_person_for_parents function
		p_temp = search_id(residents, p_person->FatherId);
		if (p_temp)
		{
			delete_person_for_parents(p_temp, p_person->id);
		}
	}
	if (p_person->motherId > 0)  //if the person has mother and the mother found on the data base
	{                            //delete from the mother the person to delete id - use delete_person_for_parents function
		p_temp = search_id(residents, p_person->motherId);
		if (p_temp)
		{
			delete_person_for_parents(p_temp, p_person->id);
		}
	}
	if (p_person->num_of_childern > 0) //if the perosn to delete have childrens delete from the children the person id
	{                                  //if the children exsist on the data base
		for (i = 0; i < p_person->num_of_childern; i++)
		{
			p_temp = search_id(residents, p_person->children_ptr[i]);
			if (p_temp != NULL)
			{
				if (p_temp->FatherId == p_person->id)
				{
					p_temp->FatherId = 0;
				}
				//check if the person to delete is the mother or the father of the children
				if (p_temp->motherId == p_person->id)
				{
					p_temp->motherId = 0;
				}
			}
		}
	}

	realloc_delete_1_space(residents, p_person->id); //call to realloc_delete_1_space to to reduce 1 sapce in person array
	residents->actual_num_of_person--; //after delete 1 person from the person array, update the actual num of person

	printf("\n\n***The person with id %llu was deleted from the system***", id);
}

//help for delete_person function. delete the id of the person to delete from his parents
//This function get pointer to specific person and his kid id (kid id is the person to delete)
//the function found the person to delete on his mother/father children_ptr
//copy the other children to temp array and allocate the children_ptr again with less 1 space
//then copy the children who we didnt delete back to the origigal children_ptr
void delete_person_for_parents(PERSON* person, unsigned long long int kid_id)
{
	unsigned long long int* c_temp;
	int i = 0, j = 0;

	for (i = 0; i < person->num_of_childern; i++)
	{
		if (person->children_ptr[i] == kid_id)  //make the id of the kid to delete to 0
		{
			person->children_ptr[i] = 0;
			break;
		}
	}
	c_temp = (unsigned long long int*)calloc(person->num_of_childern - 1, sizeof(unsigned long long int)); //allocate temp array
	if (!c_temp)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	for (i = 0; i < person->num_of_childern; i++)
	{
		if (person->children_ptr[i] > 0)   //copy to temp array all the kids of the parents who their id is not 0
		{                                  // (only the kid to delete have id 0)
			c_temp[j] = person->children_ptr[i];
			j++;
		}
	}
	person->num_of_childern--;
	free(person->children_ptr);
	person->children_ptr = (unsigned long long int*)calloc(person->num_of_childern, sizeof(unsigned long long int)); //allocate children_ptr with new size
	if (!person->children_ptr)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	for (i = 0; i < person->num_of_childern; i++)
	{
		person->children_ptr[i] = c_temp[i];  //copy from temp array back to the parent children_ptr
	}
	free(c_temp);
}

// delete 1 space from person array - help to delete person function
//this function get poniter to spefcific data base and the person to delete id
void realloc_delete_1_space(DB_MGR* residents, unsigned long long int id)
{
	int i = 0, j = 0;

	PERSON* temp = (PERSON*)calloc(residents->num_of_person - 1, sizeof(PERSON)); //allocate temp person array with 1 size less the original
	if (!temp)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	for (i = 0; i < residents->num_of_person; i++)
	{
		if (residents->person[i].id != id)  //copy from the original person array to temp all the person
		{                                   //except to the person to delete (found him by id the function recive)
			temp[j] = residents->person[i];
			j++;
		}
	}
	residents->num_of_person--;
	residents->person = (PERSON*)calloc(residents->num_of_person, sizeof(PERSON)); //allocate again the original person array to the new wanted size
	if (!residents->person)
	{
		printf("\nAllocation failed!");
		exit(1);
	}
	for (i = 0; i < residents->num_of_person; i++)
	{
		residents->person[i] = temp[i]; //copy back from temp array to the original prson array
	}
	free(temp);
}

// next generation counter
// this function get pointer to specific data base
// if the person who the user want to check for his generation have children
// the function enter in to "do while" loop until num_of_children1 and num_of_children2 are 0
// thats how the function know that there is no more generation
void get_gen(const DB_MGR* residents)
{
	int counter = 0, num_of_children1 = 0, num_of_children2 = 0, i = 0, j = 0, k = 0, stoper = 0;
	unsigned long long int id;
	PERSON *first_gen = NULL, *new_gen = NULL, *tmp, *ancestor;

	printf("Enter the id of the person you want to check his gen: ");
	id = check_id();

	ancestor = search_id(residents, id);  //check if the person who the user want to check his gen found in the system
	if (ancestor)
	{
		if (ancestor->num_of_childern > 0) //if the person found in the system and he have children
		{
			counter = 1;
			first_gen = (PERSON*)calloc(1, sizeof(PERSON));
			if (!first_gen)
			{
				printf("Allocation failed\n");
				exit(1);
			}
			first_gen[0] = *ancestor;                         //set ancsetor (person to check for his generaion) to first_gen person array
			num_of_children1 = first_gen[0].num_of_childern;
			j = 1;
			do
			{
				stoper = j;
				j = 0;
				new_gen = (PERSON*)calloc(num_of_children1, sizeof(PERSON)); //allocate new_gen person array 
				if (!new_gen)
				{
					printf("Allocation failed\n");
					exit(1);
				}
				for (i = 0; i < stoper; i++)
				{
					for (k = 0; k < first_gen[i].num_of_childern; k++)
					{
						tmp = search_id(residents, first_gen[i].children_ptr[k]);
						if (tmp)
						{
							new_gen[j] = *tmp;   //copy all the children of the persons in first_gen array
							j++;                 //to new_gen array (next generation)
						}
					}
				}
				for (i = 0; i < j; i++)
				{
					if (new_gen[i].num_of_childern > 0)
					{
						num_of_children2 += new_gen[i].num_of_childern; //count the number of children of all the person in new_gen array
					}                                                   //if they have children its mean another generation
				}
				if (num_of_children1 > 0) //if number of children of first_gen array is greater than 0 
				{
					counter++;   //count 1 more generation
				}
				num_of_children1 = 0;

				stoper = j;
				j = 0;
				first_gen = (PERSON*)calloc(num_of_children2, sizeof(PERSON)); //allocate first_gen person array by the size of
				if (!first_gen)                                                //the number of children of all the person in new_gen person array                         
				{
					printf("Allocation failed\n");
					exit(1);
				}
				for (i = 0; i < stoper; i++)
				{
					for (k = 0; k < new_gen[i].num_of_childern; k++)
					{
						tmp = search_id(residents, new_gen[i].children_ptr[k]);
						if (tmp)
						{
							first_gen[j] = *tmp;     //copy all the children of the persons in new_gen array
							j++;                     //to first_gen array (next generation)
						}
					}
				}
				for (i = 0; i < j; i++)
				{
					if (first_gen[i].num_of_childern > 0)
					{
						num_of_children1 += first_gen[i].num_of_childern; //count the number of children of all the person in new_gen array
					}                                                     //if they have children its mean another generation
				}
				if (num_of_children2 > 0)//if number of children of new_gen array is greater than 0
				{
					counter++; //count 1 more generation
				}
				num_of_children2 = 0;

			} while (num_of_children1 > 0 || num_of_children2 > 0); //to this operaion in loop till num_of_children1 and
		}   // num_of_children2 are both 0. thats mean no more generation
		else
		{
			counter = 1;  //if the person found on the system but he dosent have children
		}
	}
	else
	{
		printf("This person does not exsist on the system");
	}
	if (counter > 0)
	{
		printf("This person have %d generation", counter);
	}
	if (first_gen)
	{
		free(first_gen);
	}
	if (new_gen)
	{
		free(new_gen);
	}
}

// print database setails
// this function get pointer to specific data base
// send 1 person on time to print_person function to print his details
// do it to all the person in the data base
void print_db(const DB_MGR* residents)
{
	if (residents->actual_num_of_person > 0)
	{
		int j = 1;
		for (int i = (residents->num_of_person) - (residents->actual_num_of_person); i < residents->num_of_person; i++)
		{
			printf("\n\n***Person number %d***", j);
			print_person(&residents->person[i]);
			j++;
		}
	}
	else
	{
		printf("\n***There are no people in database yet***");
	}
}

//search for person by name
//this function get pointer to specific data base
//ask from the user to input the first and last name of the person to looking for
//compare the first and last name to all the person in the data base
//if found a person with the same first and last name print his deatils
void search_by_name(const DB_MGR* residents)
{
	int i = 0, name, family;
	char str_name[80], str_family[80];
	printf("\nPleas enter first name: ");
	fseek(stdin, 0, SEEK_END);
	gets(str_name);
	printf("\nPleas enter last name: ");
	fseek(stdin, 0, SEEK_END);
	gets(str_family);

	for (i = (residents->num_of_person - residents->actual_num_of_person); i < residents->num_of_person; i++)
	{
		name = strcmp(str_name, residents->person[i].name);
		family = strcmp(str_family, residents->person[i].family);
		if (name == 0 && family == 0)
		{
			print_person(&residents->person[i]);
		}
	}
}

//clear all memory and quit program
//This function get pointer to specific data base
//print message to the user on shouting down the program
//free all the allocate memory 
//check for each person if he have children, if yes free his children_ptr
//free his first and last name
//after scan all the persons free the person array itself
void quitAndClearMemory(DB_MGR* residents)
{
	printf("\nShouting down DataBase System");
	for (int i = (residents->num_of_person - residents->actual_num_of_person); i < residents->num_of_person; i++)
	{
		if (residents->person[i].num_of_childern > 0)
		{
			free(residents->person[i].children_ptr);
		}
		if (residents->person[i].name)
		{
			free(residents->person[i].name);
		}
		if (residents->person[i].family)
		{
			free(residents->person[i].family);
		}
	}

	free(residents->person);
}

