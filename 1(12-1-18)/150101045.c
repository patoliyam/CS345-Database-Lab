#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*struct of student to store :
1.roll number
2.total classes
3.no. of classes he is present
4.attendance percentage 
*/
struct student{
	int rollno,no_class_present,total_class;
	float percent;
	struct  student * next;
};

// head pointer of linked list
struct student *head = NULL;

//searching a struct with a given roll number
struct student * search(int rollnum)
{
	struct student * it = head;
	while(it!=NULL)		
	{
		if(it->rollno == rollnum)
		{
			return it;
		}
		it = it->next;
	}
	return NULL;
}

int main()
{
	FILE *fp;
	fp = fopen("database_12jan2017.csv","r");
	char date[10],attend[10],line[100];
	int rollnum;
	while(fgets(line,100,fp)!=NULL)
	{
		sscanf(line, "%d, %[^,],%s", &rollnum, date,attend);
		printf("%d,%s,%s\n", rollnum, date,attend);
		
		struct student * checkroll = search(rollnum);
		//if structure for student is already present updates the data 
		if (checkroll!=NULL)
		{
			if(strcmp(attend,"Present")==0)
				checkroll->no_class_present+=1;
			checkroll->total_class+=1;
		}
		//if a new student i.e whose structure is not present , allocate memory to structure and insert data
		else
		{
			struct student * newnode = (struct student *)malloc(sizeof(struct student));
			newnode->rollno = rollnum;
			newnode->total_class+=1;
			if(strcmp(attend,"Present")==0)
				newnode->no_class_present=1;
			else
				newnode->no_class_present=0;

			//inserting into linked list
			struct student * temp = head;
			newnode->next = head;
			head = newnode;
		}
	}	
	fclose(fp);
	struct student * it = head;
	FILE *fpL75 = fopen("L75.csv","w");
	FILE *fpG75 = fopen("G75.csv","w");
	it = head;
	while(it!=NULL)
	{
		it->percent = ((float)it->no_class_present)*100 / it->total_class;
		//if the percentage is less than 75 write into the file L75.csv
		if(it->percent < 75.00)
			fprintf(fpL75,"%d,%d,%f\n",it->rollno,it->no_class_present,it->percent);
		//if the percentage is less than 75 write into the file G75.csv
		else
			fprintf(fpG75,"%d,%d,%f\n",it->rollno,it->no_class_present,it->percent);
		it = it->next;
	}
	fclose(fpL75);
	fclose(fpG75);
	return 0;
}

