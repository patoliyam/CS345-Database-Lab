/*
19-01-2018
Name : Patoliya Meetkumar
Roll no: 150101045

Assignment 2
Place the .c file in the directory with the files course-credits.csv , exam-time-table.csv and the folder course-wise-student-list

Features
1. Everything is dyanamically allocated.
2. No. of courses and no. of students are dyanamically stored in structures
3. Duplicacy in course-credits list and exam-time-table is checked and accounted for.
4. Even if a course pair have conflicting mid sem and end sem both, even then only one entry is being printed in the file to prevent redundancy.
*/

#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//structure of student to store name,roll,email,total_credits and courses  
struct student{
    int total_credits,course_cnt;
    char name[100],email[100],roll[100];
    char *course[20];
    struct  student * next;
}; 
struct student *head_student = NULL;

struct date_time{
	char date[20],time[10];
	struct date_time * next;
};

//structure of credit to store coursename,credits,date and time of exam  
struct course{
	int credits;
	char cid[10];//,date1[20],date2[20],stime1[10],stime2[10];
	struct date_time *date_time_start;
    struct  course * next;
};

struct course *head_course = NULL;

// struxture and search function for time conflict entrues to prevent same course pair to print twice
struct time_conflict{
	char roll[100],cid1[10],cid2[10];
	struct time_conflict * next;
};

struct time_conflict * head_time_conflict = NULL;

int search_time_conflict(char *roll,char *cid1 , char * cid2)
{
	struct time_conflict * it = head_time_conflict;
	while(it!=NULL)		
	{
		if(strcmp(it->roll,roll)==0 && strcmp(it->cid1,cid1)==0 &&strcmp(it->cid2,cid2)==0 )
		{
			return 0;
		}
		it = it->next;
	}
	return 1;
}



//funtion to search for a student with a given roll number
struct student * search_stud(char *roll)
{
	struct student * it = head_student;
	while(it!=NULL)		
	{
		if(strcmp(it->roll,roll)==0)
		{
			return it;
		}
		it = it->next;
	}
	return NULL;
}

//funtion to search for a student with a given course name
struct course * search_course(char *cid)
{
	struct course * it = head_course;
	while(it!=NULL)		
	{
		if(strcmp(it->cid,cid)==0)
		{
			return it;
		}
		it = it->next;
	}
	return NULL;
}

//funtion to print data of all student 
void print_student_data()
{
	struct student * it = head_student;
	while(it!=NULL)		
	{
		printf("%s %s %d %d ",it->roll,it->name,it->total_credits,it->course_cnt);
		for(int i = 0; i< it->course_cnt; ++i)
		{
			printf("%s ", it->course[i]);
		}
		printf("\n");
		it = it->next;
	}
}

//funtion to print data of all courses 
void print_course_data()
{
	struct course * it = head_course;
	while(it!=NULL)		
	{
		printf("%s %d ",it->cid,it->credits);
		struct date_time * tmp = it->date_time_start;
		while(tmp!=NULL)
		{
			printf("%s %s ",tmp->date,tmp->time );
			tmp = tmp->next;
		}
		printf("\n");
		it = it->next;
	}
}

//funtion to read and store data from all course wise student list files
void read_files_from_dept(char *path)
{
    DIR *dir;
    struct dirent *entry;
    if (!(dir = opendir(path)))
        return;
    while ((entry = readdir(dir)) != NULL) {
        char tmppath[100],tmp[100],curcourse[10];
        strcpy(tmp,entry->d_name);
        sscanf(tmp,"%[^.].%*s",curcourse);
        // printf("%s\n",curcourse);
        strcpy(tmppath,path);
        strcat(tmppath,"/");
        strcat(tmppath,entry->d_name);
        // printf("%s\n",tmppath);
        FILE *fp;
		fp = fopen(tmppath,"r");
		char roll[100],name[100],email[100],line[200];
		while(fgets(line,200,fp)!=NULL)
		{
			sscanf(line, "%*d,%[^,],%[^,],%s", roll, name,email);
			// printf("%s,%s,%s\n", roll, name,email);
			struct student * checkroll = search_stud(roll);
			//student structure exist , add the course to stud struct
			if (checkroll!=NULL)
			{
				checkroll->total_credits += search_course(curcourse)->credits; 
				checkroll->course[checkroll->course_cnt] = (char *)malloc(sizeof(char)*10);
				strcpy(checkroll->course[checkroll->course_cnt],curcourse);
				checkroll->course_cnt+=1;
			}
			//if a new student i.e whose structure is not present , allocate memory to structure and insert course accordingly
			else
			{
				//allocate memory and set initial values
				struct student * newnode = (struct student *)malloc(sizeof(struct student));
				strcpy(newnode->name,name);	
				strcpy(newnode->roll,roll);	
				strcpy(newnode->email,email);
				newnode->course_cnt = 0;	
				newnode->total_credits = 0;	
				newnode->course[newnode->course_cnt] = (char *)malloc(sizeof(char)*10);
				strcpy(newnode->course[newnode->course_cnt],curcourse);
				newnode->course_cnt+=1;
				newnode->total_credits += search_course(curcourse)->credits; 
				
				//inserting into linked list of students
				struct student * temp = head_student;
				newnode->next = head_student;
				head_student = newnode;
			}
		}
		fclose(fp);
    }
}

//function to store credits for each course after reading from course_credit file
void read_course_credits()
{
	FILE *fp;
    fp = fopen("course-credits.csv","r");
	char course[10],line[100];
	int credits;
    while(fgets(line,100,fp)!=NULL)
	{
		sscanf(line, "%[^,],%d", course,&credits);
		// printf("%s,%d\n", course,credits);
		struct course * checkroll = search_course(course);
		//course structure exist , add the course to course struct . duplicacy also  handled 
		if (checkroll!=NULL)
		{
			checkroll->credits = credits;
		}
		//if a new course i.e whose structure is not present , allocate memory to structure and insert course accordingly
		else
		{
			struct course * newnode = (struct course *)malloc(sizeof(struct course));
			strcpy(newnode->cid,course);
			newnode->credits = credits;
			newnode->date_time_start = NULL;	

			//inserting into linked list of all courses
			struct course * temp = head_course;
			newnode->next = head_course;
			head_course = newnode;
		}
	}
	fclose(fp);
}

//function to store date and time of exam for each course after reading from exam-time-table.csv file
void read_exam_time_table()
{
	FILE *fp;
    fp = fopen("exam-time-table.csv","r");
	char course[10],date[20],stime[10],etime[10],line[200];
    while(fgets(line,200,fp)!=NULL)
	{
		sscanf(line, "%[^,],%[^,],%[^,],%s", course,date,stime,etime);
		// printf("%s,%s,%s,%s\n", course,date,stime,etime);
		struct course * checkroll = search_course(course);
		//course structure exist , add the course to course struct
		if (checkroll!=NULL)
		{
			int to_insert = 1;
			struct date_time * tmp = checkroll->date_time_start;
			//checking duplicacy in exam time table list
			while(tmp)
			{
				if(strcmp(tmp->date,date)==0 && strcmp(tmp->time,stime)==0)
					to_insert = 0;
				tmp = tmp->next;
			}
			//if not duplicate then insert
			if(to_insert)
			{
				struct date_time * newnodedate_time = (struct date_time *)malloc(sizeof(struct date_time));
				strcpy(newnodedate_time->date,date);
				strcpy(newnodedate_time->time,stime);

				newnodedate_time->next = checkroll->date_time_start;
				checkroll->date_time_start = newnodedate_time;
			}
		}
		//if a new course i.e whose structure is not present , allocate memory to structure and insert course accordingly
		//note that: if credits was not given for that course , 0 is assumed 
		else
		{
			struct course * newnode = (struct course *)malloc(sizeof(struct course));
			strcpy(newnode->cid,course);
			newnode->credits = 0;	

			struct date_time * newnodedate_time = (struct date_time *)malloc(sizeof(struct date_time));
			strcpy(newnodedate_time->date,date);
			strcpy(newnodedate_time->time,stime);
			newnode->date_time_start = newnodedate_time;
			
			newnode->next = head_course;
			head_course = newnode;	
		}
	}
	fclose(fp);
}

//function to check time conflict for students and store the corresponding students into a file
void check_times_conflict()
{
	struct student * it = head_student;
	FILE *fp;
	fp = fopen("time_conflict.csv","w");
	while(it!=NULL)		
	{
		for(int p = 0; p< it->course_cnt; ++p)
		{
			for(int q = p+1; q<it->course_cnt; ++q)
			{
				struct course * i = search_course(it->course[p]);
				struct course * j = search_course(it->course[q]);
				struct date_time * istart = i->date_time_start;
				struct date_time * jstart = j->date_time_start;
				
				//checking the conflicts for time of exams
				while(istart!=NULL)
				{
					jstart = j->date_time_start;
					while(jstart!=NULL)
					{
						if(strcmp(istart->date,jstart->date)==0)
						{
							if(strcmp(istart->time,jstart->time)==0)
							{
								//checking if that particular roll number with the course pairs is already there or not.
								// otherwise same course pairs will be listed twice due to clash of both midsem and endsem
								if (search_time_conflict(it->roll,it->course[p],it->course[q]))
								{
									fprintf(fp, "%s,%s,%s,%s\n",it->roll,it->name,it->course[p],it->course[q]);

									struct time_conflict * newnode = (struct time_conflict *)malloc(sizeof(struct time_conflict));
									strcpy(newnode->roll,it->roll);
									strcpy(newnode->cid1,it->course[p]);
									strcpy(newnode->cid2,it->course[q]);

									newnode->next = head_time_conflict;
									head_time_conflict = newnode;
								}
							}
						}		
						jstart = jstart->next;
					}
					istart = istart->next;
				}
			}
		}
		it = it->next;
	}
}

//function to check credit conflict (max 40) for students and store the corresponding students into a file
void check_credit_conflict()
{
	struct student * it = head_student;
	FILE *fp;
	fp = fopen("credit_conflict.csv","w");
	while(it!=NULL)
	{
		//if the no. of credits > 40 then print the details in file
		if (it->total_credits >40)
		{
			fprintf(fp, "%s,%s,%d\n",it->roll,it->name,it->total_credits);
		}
		it = it->next;
	}
}

int main(int argc, char const *argv[])
{
	read_course_credits();
	read_exam_time_table();
	
    DIR *dir;
    struct dirent *entry;
    char curdir[100];
    strcpy(curdir,"./course-wise-students-list");
    if (!(dir = opendir(curdir)))
        return 0;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            char dept[10];
            strcpy(dept,entry->d_name);
            char path[100];
            snprintf(path, sizeof(path), "%s/%s", curdir, dept);
            read_files_from_dept(path);
        }
    }
    closedir(dir);

    //uncomment the following print functions to see the data
    // print_course_data();
    print_student_data();

    check_credit_conflict();

    check_times_conflict();

	return 0;
}