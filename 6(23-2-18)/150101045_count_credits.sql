/*Name : Patoliya Meetkumar Krushnadas
Roll : 150101045
CS345 Assignment 6 */
use 150101045_23feb2018;
delimiter //
drop procedure if exists count_credits;
create procedure count_credits()
	begin 
		declare rolln int;
		declare cnt int default 0;
		declare done int default false;
		declare student cursor for (SELECT DISTINCT roll_number FROM cwsl ORDER BY roll_number ASC);
		declare continue handler for not found set done = true;
		open student;
		drop table if exists credit_sum;
		CREATE TABLE credit_sum(
			roll_number varchar(12) NOT NULL,
			name varchar(100) NOT NULL,
			credits int NOT NULL,
			primary key (roll_number)
		);
		roll_loop : LOOP
			FETCH from student into rolln;
			IF done THEN
				LEAVE roll_loop;
			END IF;
			SET cnt = cnt + 1;
			-- SELECT rolln;
			-- SELECT cnt;
			BLOCK : BEGIN
				declare done_one_student int default false;
				declare name varchar(100);
				declare cid varchar(7);
				declare credit int default 0;
				declare cwsl cursor for (SELECT cwsl.roll_number as roll, cwsl.name as name, cwsl.course_id  as cid, cc.number_of_credits as credit 
					FROM cwsl, cc WHERE cc.course_id = cwsl.course_id and cwsl.roll_number = rolln);
				declare continue handler for not found set done_one_student = true;
				open cwsl;
				credits_for_student_loop : LOOP
					FETCH cwsl into rolln,name,cid,credit;
					-- select rolln, name, cid, credit;
					IF done_one_student THEN
						LEAVE credits_for_student_loop;
					END IF;
					
					IF (SELECT COUNT(*) FROM credit_sum WHERE roll_number = rolln)=0 THEN
						INSERT INTO credit_sum (roll_number, name, credits) VALUES (rolln,name,credit);
					ELSE
						UPDATE credit_sum SET credits = credits + credit WHERE roll_number = rolln;
					END IF;

				END LOOP credits_for_student_loop;
				close cwsl;
			END BLOCK;
		END LOOP roll_loop;
		close student;
		SELECT roll_number as Roll_Number, name as Name, credits as Total_number_of_credits FROM credit_sum WHERE credits >40 ORDER BY roll_number;
	end//
delimiter ;

call count_credits();