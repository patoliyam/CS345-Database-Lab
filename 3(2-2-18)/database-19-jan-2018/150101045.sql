
/*
Name : Shubham Singhal
Roll no: 150101072

*/
CREATE DATABASE if NOT EXISTS 25jan2018;

use 25jan2018;

CREATE TABLE if NOT EXISTS cc( cid varchar(50) PRIMARY KEY COMMENT 'character string, Since one course cannot be entered twice in course credits list',
								credits int NOT NULL COMMENT 'credits cant be NULL for a course',
								CHECK(credits>0)
								);

CREATE TABLE if NOT EXISTS ett( cid varchar(50) NOT NULL COMMENT 'character string, cant be null in time table list' , 
								examdate date NOT NULL COMMENT 'date datatype, cant be null in time table list',
								stime time NOT NULL COMMENT 'time datatype, cant be null in time table list', 
								etime time NOT NULL COMMENT 'time datatype, cant be null in time table list',
								PRIMARY KEY(cid,examdate) COMMENT 'course and examdate act as PRIMARY key since on one day two exams of same course cant ocur',
								UNIQUE(cid,examdate,stime,etime) COMMENT 'to check if multiple same entries arent added'
								);

CREATE TABLE if NOT EXISTS cwsl( cid varchar(50) NOT NULL COMMENT 'character string, cant be null',  
								 id int NOT NULL COMMENT 'integer, cant be null ', 
								 roll int NOT NULL COMMENT 'integer string, cant be null ', 
								 name  char(50) NOT NULL COMMENT 'character string, cant be null',
								 email char(50) NOT NULL COMMENT 'character string, cant be null',
								 PRIMARY KEY(cid,roll)) COMMENT 'PRIMARY key since  a student can have a single course';

CREATE TEMPORARY TABLE if NOT EXISTS cc_temp LIKE cc;

CREATE TEMPORARY TABLE if NOT EXISTS ett_temp LIKE ett;

CREATE TEMPORARY TABLE if NOT EXISTS cwsl_temp LIKE cwsl;

CREATE TABLE if NOT EXISTS cc_clone LIKE cc;

CREATE TABLE if NOT EXISTS ett_clone LIKE ett;

CREATE TABLE if NOT EXISTS cwsl_clone LIKE cwsl;

