-- TABLES DESTRUCTION--


DROP TABLE prospect CASCADE CONSTRAINTS;
DROP TABLE exposition CASCADE CONSTRAINTS;
DROP TABLE employee CASCADE CONSTRAINTS;
DROP TABLE showroom CASCADE CONSTRAINTS;
DROP TABLE art CASCADE CONSTRAINTS;
DROP TABLE charge CASCADE CONSTRAINTS;



-- END OF TABLES DESTRUCTION --
CREATE TABLE prospect(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(255),
	"last_name" VARCHAR(255),
	"firm_name" VARCHAR(255),
	"mail" VARCHAR(255) NOT NULL
    CHECK (REGEXP_LIKE("mail", '^[a-zA-Z0-9\.]+@[a-zA-Z0-9\.]+\.[a-z]+'))
);

CREATE TABLE exposition(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"start_date" DATE NOT NULL,
	"end_date" DATE NOT NULL,
	"type" VARCHAR(255) NOT NULL,
    "prospect_id" INT NOT NULL,
	CONSTRAINT "prospect_exposition_fk_id" FOREIGN KEY ("prospect_id") REFERENCES prospect("id")
		ON DELETE SET NULL
);

CREATE TABLE employee(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(128) NOT NULL,
	"last_name" VARCHAR(128) NOT NULL,
	"mail" VARCHAR(128) NOT NULL
	CHECK (REGEXP_LIKE("mail", '^[a-zA-Z0-9\.]+@[a-zA-Z0-9\.]+\.[a-z]+')),
	"phone" VARCHAR(15) NOT NULL
);

CREATE TABLE showroom(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"price" INT NOT NULL,
	"type" VARCHAR(255) NOT NULL,
	"height" INT NOT NULL,
	"width" INT NOT NULL,
	"length" INT NOT NULL,
	"equipment" VARCHAR(255),
    "employee_id" INT,
    "prospect_id" INT,
	CONSTRAINT "employee_showroom_fk_id" FOREIGN KEY ("employee_id") REFERENCES employee("id")
		ON DELETE SET NULL,
	CONSTRAINT "prospect_showroom_fk_id" FOREIGN KEY ("prospect_id") REFERENCES prospect("id")
		ON DELETE SET NULL
);

CREATE TABLE art (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"name" VARCHAR(255) NOT NULL,
	"description" VARCHAR(255) NOT NULL,
	"exposition_id" INT NOT NULL,
	"showroom_id" INT NOT NULL,
	"prospect_id" INT NOT NULL,
    CONSTRAINT "exposition_art_fk_id" FOREIGN KEY ("exposition_id") REFERENCES exposition("id")
		ON DELETE CASCADE,
	CONSTRAINT "showroom_art_fk_id" FOREIGN KEY ("showroom_id") REFERENCES showroom("id")
        ON DELETE SET NULL,
	CONSTRAINT "prospect_art_fk_id" FOREIGN KEY ("prospect_id") REFERENCES prospect("id")
		ON DELETE SET NULL
);


CREATE TABLE charge
(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"description" VARCHAR(255) NOT NULL,
	"pay_due" DATE NOT NULL,
	"price" FLOAT NOT NULL,
	"paid" NUMBER(1) NOT NULL,
    "prospect_id" INT,
	CONSTRAINT "prospect_charge_fk_id" FOREIGN KEY ("prospect_id") REFERENCES prospect("id")
		ON DELETE SET NULL
);


-- TESTING --

INSERT INTO prospect("first_name", "last_name", "firm_name", "mail")
	VALUES ('Jan', 'Novak', NULL,  'novak@mail.com');

INSERT INTO prospect("first_name", "last_name", "firm_name", "mail")
	VALUES('Jakub', 'Siska', NULL, 'mail@mail.mail');

INSERT INTO prospect("first_name", "last_name", "firm_name", "mail")
	VALUES (NULL, NULL, 'Rysanek s.r.o', 'LeeSheldon@gmail.com');

INSERT INTO exposition("start_date", "end_date", "type", "prospect_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 1);

INSERT INTO exposition("start_date", "end_date", "type", "prospect_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 2);

INSERT INTO exposition("start_date", "end_date", "type", "prospect_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 3);

INSERT INTO exposition("start_date", "end_date", "type", "prospect_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 2);

INSERT INTO exposition("start_date", "end_date", "type", "prospect_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 3);

INSERT INTO exposition("start_date", "end_date", "type", "prospect_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 3);

INSERT INTO employee("first_name", "last_name", "mail", "phone")
	VALUES ('Pavel', 'Nezval', 'nezval@mail.com', 638321867);

INSERT INTO employee("first_name", "last_name", "mail", "phone")
	VALUES ('Standa', 'Zavaril', 'zavaril@mail.com', 751684486);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "employee_id")
	VALUES (150, 'Normal', 120, 500, 200, NULL, 1);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "employee_id")
	VALUES (1300, 'Special', 1250, 150, 2200, NULL, 1);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "employee_id")
	VALUES (525, 'Abnormal', 250, 250, 200, 'Chair', 2);

INSERT INTO art("name", "description", "exposition_id", "showroom_id", "prospect_id")
    VALUES('Art_42', 'Guide of universe', 1, 2, 1);

INSERT INTO art("name", "description", "exposition_id", "showroom_id", "prospect_id")
    VALUES('Beer', 'Svijany', 2, 3, 2);

INSERT INTO art("name", "description", "exposition_id", "showroom_id", "prospect_id")
    VALUES('R2D2', 'Can of coke', 3, 2, 3);

INSERT INTO art("name", "description", "exposition_id", "showroom_id", "prospect_id")
    VALUES('mona', 'Lisa', 1, 2, 3);

INSERT INTO art("name", "description", "exposition_id", "showroom_id", "prospect_id")
    VALUES('some', 'Picture', 1, 1, 3);

INSERT INTO art("name", "description", "exposition_id", "showroom_id", "prospect_id")
    VALUES('colossus', 'David', 1, 3, 3);


INSERT INTO charge("description", "pay_due", "price", "paid", "prospect_id")
	VALUES('Pay for showroom 5', TO_DATE('2023-12-25', 'yyyy/mm/dd'), 12.5, 0, 3);

INSERT INTO charge("description", "pay_due", "price", "paid", "prospect_id")
	VALUES('Pay for showroom special', TO_DATE('2023-12-25', 'yyyy/mm/dd'), 1250, 1, 2);

INSERT INTO charge("description", "pay_due", "price", "paid", "prospect_id")
	VALUES('Pay for showroom', TO_DATE('2023-11-25', 'yyyy/mm/dd'), 525, 0, 2);




 -- INSERT INTO employee("first_name", "last_name", "mail")
-- END OF TESTING --



-- Third part - Some SELECT questions and descriptions to them --


-- Show which employee is taking care of specific place
SELECT showroom."id", "first_name", "last_name" FROM showroom LEFT JOIN employee on showroom."employee_id" = employee."id";

-- Who has charges paid and who doesn't
SELECT charge."id", "first_name", "last_name", "firm_name", "price", "paid" FROM prospect RIGHT JOIN charge ON prospect."id" = charge."prospect_id";

-- Who has which art on showroom with id 2
SELECT "first_name", "last_name", "firm_name", art."id" FROM showroom FULL OUTER JOIN art ON showroom."id" = art."showroom_id" FULL OUTER JOIN prospect ON art."prospect_id" = prospect."id" WHERE "showroom_id" = 2;

-- How many arts expositions have
SELECT "exposition_id", count("id") as arts FROM art GROUP BY "exposition_id";

-- How many places is employee taking care of
SELECT "employee_id", count("id") as places FROM showroom GROUP BY "employee_id";

-- Show every art which is on place, that employee with id 1 is taking care of
SELECT "id", "name", "description" from art WHERE EXISTS (SELECT * FROM showroom LEFT JOIN employee ON showroom."employee_id" = employee."id" WHERE showroom."id" = art."showroom_id" and "employee_id" = 1);

-- Places where type is normal or abnormal
SELECT * FROM showroom WHERE "type" IN ('Normal', 'Abnormal');

-----------------------------------------------------------------