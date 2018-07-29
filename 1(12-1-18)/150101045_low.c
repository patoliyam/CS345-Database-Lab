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
};

struct student *var[1000000];
int current_students = 0;

int search(int rollnum)
{
	for(int i = 0; i<current_students; ++i)
	{
		if(var[i]->rollno == rollnum)
		{
			return i;
		}
	}
	return -1;
}

int main()
{
	FILE *fp;
	fp = fopen("db.csv","r");
	char c;
	char date[10],attend[10],extra[10],line[100];
	int rollnum,i=0;
	while(fgets(line,100,fp)!=NULL)
	{
		sscanf(line, "%d, %[^,],%s", &rollnum, date,attend);
		int checkroll = search(rollnum);
		
		//if structure for student is already present updates the data 
		if (checkroll!=-1)
		{
			// printf("in checkroll %d\n",rollnum);
			if(strcmp(attend,"Present")==0)
				var[checkroll]->no_class_present+=1;
			var[checkroll]->total_class+=1;
		}
		//if a new student i.e whose structure is not present , allocate memory to structure and insert data
		else
		{
			var[i] = (struct student *)malloc(sizeof(struct student));
			var[i]->rollno = rollnum;
			var[i]->total_class+=1;
			if(strcmp(attend,"Present")==0)
				var[i]->no_class_present=1;
			else
				var[i]->no_class_present=0;
			i++;
			current_students = i;
		}
	}	
	fclose(fp);
	for (int i = 0; i < current_students; ++i)
	{
		var[i]->percent = ((float)var[i]->no_class_present)*100 / var[i]->total_class;
		printf("%d %d %d %f\n",var[i]->rollno,var[i]->no_class_present,var[i]->total_class,var[i]->percent);
	}
	FILE *fpL = fopen("L75.csv","w");
	FILE *fpG = fopen("G75.csv","w");
	for (int i = 0; i < current_students; ++i)
	{
		if(var[i]->percent < 75.00)
			fprintf(fpL,"%d,%d,%f\n",var[i]->rollno,var[i]->no_class_present,var[i]->percent);
		else
			fprintf(fpG,"%d,%d,%f\n",var[i]->rollno,var[i]->no_class_present,var[i]->percent);
	}
	fclose(fpL);
	fclose(fpG);
	return 0;
}

