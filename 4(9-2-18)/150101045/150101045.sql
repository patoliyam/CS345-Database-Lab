
/*
Name : Patoliya Meetkumar
Roll no: 150101045
Assignment 4
*/

/*
Assumptions while popualting data in CSV FILES:
	1. NO LAB ENTRIES are present in Schedule table BUT Lab courses name are present in Courses table.
	2. Design Courses didn't follow time table according to Institute and hence are not included but two minor courses of design are included.  
*/

drop database 09feb2018;
CREATE DATABASE if NOT EXISTS 09feb2018;
use 09feb2018;

-- drop database test;
-- CREATE DATABASE if NOT EXISTS test;
-- use test;



CREATE TABLE if NOT EXISTS Course(course_id varchar(10) NOT NULL COMMENT 'character string',
division varchar(10) NOT NULL DEFAULT 'NA' COMMENT 'division cant be none and hence default value is NA' ,
PRIMARY KEY(course_id,division),
CHECK (division in ('1','2','3','4','NA'))
);


CREATE TABLE if NOT EXISTS Department(department_id varchar(10) PRIMARY KEY COMMENT 'character string',
name varchar(50) NOT NULL COMMENT 'full name of department'
);




CREATE TABLE if NOT EXISTS Slot(letter varchar(10) NOT NULL COMMENT 'slot name letter', 
day varchar(10) NOT NULL COMMENT 'day of the week',
stime time NOT NULL COMMENT 'time datatype, cant be null ', 
etime time NOT NULL COMMENT 'time datatype, cant be null ',
PRIMARY KEY(letter,day) COMMENT 'letter day pair is primary key. Check are placed as mentioned in question',
CHECK (stime<etime),
CHECK (letter in ('A','B','C','D','E','F','G','H','I','J','K','L','A1','B1','C1','D1','E1')),
CHECK (day in ('Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday'))
);



CREATE TABLE if NOT EXISTS Room(room_number varchar(20) PRIMARY KEY COMMENT 'room_number is primary key',  
location varchar(10) NOT NULL COMMENT 'location must not be null constraint as mentioned in question',
CHECK(location in ('Core-I', 'Core-II','Core-III', 'Core-IV', 'LH', 'Local'))
);


CREATE TABLE if NOT EXISTS Schedule(course_id varchar(10) NOT NULL COMMENT 'course_id is primary key of Course table',
division varchar(10) NOT NULL COMMENT 'division is primary key of Course table' ,
department_id varchar(10) NOT NULL COMMENT 'department_id is primary key of Department table',
room_number varchar(20) NOT NULL COMMENT 'room_number is primary key of Room table',
letter varchar(10) NOT NULL COMMENT 'letter is primary key of Slot table' , 
day varchar(10) NOT NULL COMMENT 'day is primary key of Slot table',
PRIMARY KEY(course_id,division,letter,day,room_number),
FOREIGN KEY fk_course(course_id,division) REFERENCES Course(course_id,division)  on delete CASCADE on update restrict,
FOREIGN KEY fk_dept(department_id) REFERENCES Department(department_id) on delete CASCADE on update restrict,
FOREIGN KEY fk_slot(letter,day) REFERENCES Slot(letter,day) on delete CASCADE on update restrict,
FOREIGN KEY fk_room(room_number) REFERENCES Room(room_number)on delete CASCADE on update restrict
);

Describe Course;
Describe Slot;
Describe Room;
Describe Department;
Describe Schedule;


-- TO directly load the files in the database we can use the below commands

LOAD DATA LOCAL INFILE '150101072_Course.csv'     INTO TABLE Course FIELDS TERMINATED BY ','  ENCLOSED BY '"' LINES TERMINATED BY '\n';
LOAD DATA LOCAL INFILE '150101072_Room.csv'       INTO TABLE Room FIELDS TERMINATED BY ','  ENCLOSED BY '"' LINES TERMINATED BY '\n';
LOAD DATA LOCAL INFILE '150101072_Slot.csv'       INTO TABLE Slot FIELDS TERMINATED BY ','  ENCLOSED BY '"' LINES TERMINATED BY '\n';
LOAD DATA LOCAL INFILE '150101072_Department.csv' INTO TABLE Department FIELDS TERMINATED BY ','  ENCLOSED BY '"' LINES TERMINATED BY '\n';
LOAD DATA LOCAL INFILE '150101072_Schedule.csv'   INTO TABLE Schedule FIELDS TERMINATED BY ','  ENCLOSED BY '"' LINES TERMINATED BY '\n';
