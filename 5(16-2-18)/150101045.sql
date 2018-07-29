
/*
Name : Patoliya Meetkumar
Roll no: 150101045
Assignment 5
*/

use 09feb2018;

-- a

-- just pick distinct course from scheduled table where room number is 2001
SELECT distinct(course_id) from Schedule where room_number='2001';




-- b

-- just pick distinct course from scheduled table where slot letter is C
SELECT distinct(course_id) from Schedule where letter='C';



-- c

-- just pick distinct division from scheduled table where room number is L2 or L3
SELECT distinct(division) from Schedule where room_number='L2' or room_number='L3';



-- d

-- picking distinct pair of courses and room numner and then group them by course id and hence pick courses with count of room number > 1 
-- ie courses which are scheduled in multiple room
SELECT course_id from (SELECT distinct course_id,room_number from Schedule) as t1 GROUP BY course_id HAVING COUNT(room_number)>1;



-- e

-- picking department id from scheduled table where room in L1/L2/L3/L4 and pickeing department names from these ids
SELECT name from Department where department_id IN (SELECT department_id from Schedule where room_number IN ('L1','L2','L3','L4') ); 



-- f

-- picking department id from scheduled table where room in L1/L2 and pickeing department names NOT from these ids
SELECT name from Department where department_id NOT IN (SELECT department_id from Schedule where room_number IN ('L1','L2') ); 



-- g

-- take difference of all slots and slots used by one department. If the result is empty it implies that department is using all the slots 
SELECT name from Department as t1 where NOT EXISTS ( SELECT letter from Slot where  letter not in (SELECT letter from Schedule as t2 where t1.department_id=t2.department_id));



-- h

-- pick distinct letter and courses and group by slot letter to get the count of courses in each slot and then order by count_Courses in ascending order.
SELECT letter,COUNT(course_id) as count_courses from (SELECT distinct letter,course_id from Schedule) as t GROUP BY letter ORDER BY count_courses;



-- i

-- pick distinct room and courses and group by room_number to get the count of courses in each room and then order by count_Courses in descending order.
SELECT room_number,COUNT(course_id) as count_courses from (SELECT distinct room_number,course_id from Schedule) as t GROUP BY room_number ORDER BY count_courses DESC ;



-- j

-- first i found the min count_courses for the slot and then picked the slot letter for which the count_course = min_count_courses
SELECT letter from (Select distinct letter,course_id from Schedule) as t group by letter HAVING count(course_id)=(select min(cnt) from (SELECT distinct letter,count(course_id) as cnt from  (Select distinct letter,course_id from Schedule) as t group by letter) as t2);



-- k

-- pick slots letter with course name endimg with M (Representing minors)
SELECT distinct(letter) from Schedule where course_id LIKE '%M';




-- l

-- select department name and letter where departement id and letter is not present in all the department id and letter which are present in scheduled tables
SELECT distinct d1.name, Slot.letter from Slot, Department as d1 where (d1.department_id,Slot.letter) not in (select department_id, letter from Schedule) order by d1.name ASC; 
