/*Name : Patoliya Meetkumar Krushnadas
Roll : 150101045
CS345 Assignment 6 */
use 150101045_23feb2018;
delimiter //
drop procedure if exists tt_violation;
create procedure tt_violation()
	begin 
		declare roll_num int;
		declare cnt int default 0;
		declare done int default false;
		declare student cursor for (SELECT DISTINCT roll_number FROM cwsl ORDER BY roll_number ASC);
		declare continue handler for not found set done = true;
		open student;
		drop table if exists exam_conflicts;
		CREATE TABLE exam_conflicts(
			roll_number varchar(12) NOT NULL,
			name varchar(100) NOT NULL,
			course_id1 varchar(7) NOT NULL,
			course_id2 varchar(7) NOT NULL
		);
		roll_loop : LOOP
			FETCH from student into roll_num;
			IF done THEN
				LEAVE roll_loop;
			END IF;
			SET cnt = cnt + 1;
			-- SELECT roll_num;
			-- SELECT cnt;

			BLOCK : BEGIN
				declare done_one_student int default false;
				declare roll1,roll2 varchar(12);
				declare name1,name2 varchar(100);
				declare course_id1,course_id2 varchar(7);
				declare date1,date2 date;
				declare start1,start2 varchar(20);
				declare cwslpair cursor for (
					SELECT  course1.roll_number r1, course1.name as n1, course1.course_id as c1, exam1.exam_date as d1, exam1.start_time as s1, 
							course2.roll_number r2, course2.name as n2, course2.course_id as c2, exam2.exam_date as d2, exam2.start_time as s2 
					FROM cwsl course1, ett exam1, cwsl course2, ett exam2
					WHERE (course1.roll_number = roll_num and course1.course_id<>course2.course_id and course1.course_id<course2.course_id and course1.roll_number=course2.roll_number and exam1.course_id = course1.course_id and exam2.course_id = course2.course_id )
					);
				declare continue handler for not found set done_one_student = true;
				open cwslpair;

				course_for_student_loop : LOOP
					FETCH cwslpair into roll1, name1,course_id1,date1,start1, roll2,name2,course_id2,date2,start2;
					IF done_one_student THEN
						LEAVE course_for_student_loop;
					END IF;
					-- SELECT roll1, name1,course_id1,date1,start1, roll2,name2,course_id2,date2,start2;
					IF (date1=date2 and start1=start2) THEN
						INSERT INTO exam_conflicts (roll_number, name, course_id1, course_id2) VALUES (roll1, name1, course_id1,course_id2);
					END IF;
				END LOOP course_for_student_loop;
				close cwslpair;
			END BLOCK;
		END LOOP roll_loop;
		close student;
		SELECT DISTINCT roll_number as Roll_Number, name as Name, course_id1 as Course1, course_id2 as Course2 FROM exam_conflicts ORDER BY roll_number;
	end//
delimiter ;

call tt_violation();