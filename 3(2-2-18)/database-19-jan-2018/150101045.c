
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void read_files_from_dept(char *path)
{
    DIR *dir;
    FILE *fp2;
    fp2 = fopen("roll_number_cwsl.sql","a+");
    struct dirent *entry;
    if (!(dir = opendir(path)))
        return;
    while ((entry = readdir(dir)) != NULL) {
        char tmppath[100],tmp[100],curcourse[10];
        strcpy(tmp,entry->d_name);
        sscanf(tmp,"%[^.].%*s",curcourse);
        strcpy(tmppath,path);
        strcat(tmppath,"/");
        strcat(tmppath,entry->d_name);
        FILE *fp1;
        fp1 = fopen(tmppath,"r");
        int id;
        long long int roll;
        char name[100],email[100],line[200];
        while(fgets(line,200,fp1)!=NULL)
        {
            sscanf(line, "%d,%lld,%[^,],%s",&id,&roll,name,email);    
            fprintf(fp2, "INSERT INTO cwsl VALUES ('%s',%d,%lld,'%s','%s');\n",curcourse,id,roll,name,email);
            fprintf(fp2, "INSERT INTO cwsl_temp VALUES ('%s',%d,%lld,'%s','%s');\n",curcourse,id,roll,name,email);
            fprintf(fp2, "INSERT INTO cwsl_clone VALUES ('%s',%d,%lld,'%s','%s');\n",curcourse,id,roll,name,email);
        }
        fclose(fp1);
    }
    fclose(fp2);    
}

void read_course_credits()
{
    FILE *fp1,*fp2;
    fp1 = fopen("course-credits.csv","r");
    fp2 = fopen("roll_number_cc.sql","w");
    char course[10],line[100];
    int credits;
    while(fgets(line,100,fp1)!=NULL)
    {
        sscanf(line, "%[^,],%d", course,&credits);        
        fprintf(fp2, "INSERT INTO cc VALUES ('%s',%d);\n",course,credits);
        fprintf(fp2, "INSERT INTO cc_temp VALUES ('%s',%d);\n",course,credits);
        fprintf(fp2, "INSERT INTO cc_clone VALUES ('%s',%d);\n",course,credits);
    }
    fclose(fp1);
    fclose(fp2);
}

void read_exam_time_table()
{
    FILE *fp1,*fp2;
    fp1 = fopen("exam-time-table.csv","r");
    fp2 = fopen("roll_number_ett.sql","w");
    char course[10],date[20],stime[10],etime[10],line[200];
    while(fgets(line,200,fp1)!=NULL)
    {
        sscanf(line, "%[^,],%[^,],%[^,],%s", course,date,stime,etime);        
        fprintf(fp2, "INSERT INTO ett VALUES ('%s','%s','%s','%s');\n",course,date,stime,etime);
        fprintf(fp2, "INSERT INTO ett_temp VALUES ('%s','%s','%s','%s');\n",course,date,stime,etime);
        fprintf(fp2, "INSERT INTO ett_clone VALUES ('%s','%s','%s','%s');\n",course,date,stime,etime);
    }
    fclose(fp1);
    fclose(fp2);
}

void read_course_wise_students_list()
{
    DIR *dir;
    struct dirent *entry;
    char curdir[100];
    strcpy(curdir,"./course-wise-students-list");
    if (!(dir = opendir(curdir)))
        return ;
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
}

int main(int argc, char const *argv[])
{
	
    //will generate a file roll_number_ett.sql
    read_exam_time_table();

    //will generate a file roll_number_ccc.sql
    read_course_credits();

    //will generate a file roll_number_cwsl.sql
    read_course_wise_students_list();
    
	return 0;
}