-- TABLES DESTRUCTION--

DROP TABLE art CASCADE CONSTRAINTS;
DROP TABLE employee CASCADE CONSTRAINTS;
DROP TABLE exposition CASCADE CONSTRAINTS;
DROP TABLE showroom CASCADE CONSTRAINTS;
DROP TABLE receipt CASCADE CONSTRAINTS;
DROP TABLE prospect CASCADE CONSTRAINTS;


-- END OF TABLES DESTRUCTION --

CREATE TABLE art (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"name" VARCHAR(255) NOT NULL,
	"description" VARCHAR(255) NOT NULL
);

CREATE TABLE exposition(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"start_date" DATE NOT NULL,
	"end_date" DATE NOT NULL,
	"type" VARCHAR(255) NOT NULL,
	"art_id" INT NOT NULL,
	CONSTRAINT "exposition_art_fk_id" FOREIGN KEY ("art_id") REFERENCES art("id") 
		ON DELETE CASCADE
);

CREATE TABLE showroom(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"price" INT NOT NULL,
	"type" VARCHAR(255) NOT NULL,
	"height" INT NOT NULL,
	"width" INT NOT NULL,
	"length" INT NOT NULL,
	"equipment" VARCHAR(255),
	"art_id" INT,
	CONSTRAINT "showroom_art_fk_id" FOREIGN KEY ("art_id") REFERENCES art("id") 
        ON DELETE SET NULL
);

CREATE TABLE employee(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(128) NOT NULL,
	"last_name" VARCHAR(128) NOT NULL,
	"mail" VARCHAR(128) NOT NULL
	CHECK (REGEXP_LIKE("mail", '^[a-zA-Z0-9\.]+@[a-zA-Z0-9\.]+\.[a-z]+')),
	"phone" VARCHAR(20) NOT NULL,
	"showroom_id" INT,
	CONSTRAINT "employee_showroom_fk_id" FOREIGN KEY ("showroom_id") REFERENCES showroom("id")
		ON DELETE SET NULL
);

CREATE TABLE receipt(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"description" VARCHAR(255) NOT NULL,
	"pay_due" DATE NOT NULL,
	"price" FLOAT NOT NULL,
	"paid" NUMBER(1) NOT NULL
);

CREATE TABLE prospect(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(255),
	"last_name" VARCHAR(255),
	"firm_name" VARCHAR(255),
	"mail" VARCHAR(255) NOT NULL
    CHECK (REGEXP_LIKE("mail", '^[a-zA-Z0-9\.]+@[a-zA-Z0-9\.]+\.[a-z]+')),
	"showroom_id" INT,
	"art_id" INT,
	"exposition_id" INT,
	"receipt_id" INT,
	CONSTRAINT "prospect_showroom_fk_id" FOREIGN KEY ("showroom_id") REFERENCES showroom("id")
		ON DELETE SET NULL,
	CONSTRAINT "prospect_art_fk_id" FOREIGN KEY ("art_id") REFERENCES art("id")
		ON DELETE SET NULL,
	CONSTRAINT "prospect_exposition_fk_id" FOREIGN KEY ("exposition_id") REFERENCES exposition("id")
		ON DELETE SET NULL,
	CONSTRAINT "prospect_receipt_fk_id" FOREIGN KEY ("receipt_id") REFERENCES receipt("id")
		ON DELETE SET NULL
);



-- TESTING --
INSERT INTO art("name", "description") VALUES('Art_42', 'Guide of universe');
INSERT INTO art("name", "description") VALUES('Beer', 'Svijany');
INSERT INTO art("name", "description") VALUES('R2D2', 'Can of coke');


INSERT INTO exposition("start_date", "end_date", "type", "art_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 1);

INSERT INTO exposition("start_date", "end_date", "type", "art_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 2);

INSERT INTO exposition("start_date", "end_date", "type", "art_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 3);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "art_id" )
	VALUES (150, 'Normal', 120, 500, 200, NULL, 1);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "art_id" )
	VALUES (1300, 'Special', 1250, 150, 2200, NULL, 2);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "art_id" )
	VALUES (525, 'Abnormal', 250, 250, 200, 'Chair', 3);

INSERT INTO employee("first_name", "last_name", "mail", "phone", "showroom_id")
	VALUES('Ivan', 'Ivanovic', 'ivanGallery@gallry.com', '+421 948 222 131', 1);

INSERT INTO employee("first_name", "last_name", "mail", "phone", "showroom_id")
	VALUES('Bohumil', 'Bohuslamovic', 'iBohuslav@gallry.com', '+421 441 322 454', 2);



INSERT INTO receipt("description", "pay_due", "price", "paid") 
	VALUES('Pay for showroom 5', TO_DATE('2023-12-25', 'yyyy/mm/dd'), 12.5, 0);

INSERT INTO receipt("description", "pay_due", "price", "paid") 
	VALUES('Pay for showroom special', TO_DATE('2023-12-25', 'yyyy/mm/dd'), 1250, 1 );
    
INSERT INTO receipt("description", "pay_due", "price", "paid") 
	VALUES('Pay for showroom', TO_DATE('2023-11-25', 'yyyy/mm/dd'), 525, 0 );

INSERT INTO prospect("first_name", "last_name", "firm_name", "mail", "showroom_id", "art_id", "exposition_id", "receipt_id")
	VALUES ('Jan', 'Novak', NULL,  'novak@mail.com', 1, 1, 1, 1);

INSERT INTO prospect("first_name", "last_name", "firm_name", "mail", "showroom_id", "art_id", "exposition_id", "receipt_id")
	VALUES('Jakub', 'Siska', NULL, 'mail@mail.mail', 2, 1, 1, 2);
INSERT INTO prospect("first_name", "last_name", "firm_name", "mail", "showroom_id", "art_id", "exposition_id", "receipt_id")
	VALUES (NULL, NULL, 'Rysanek s.r.o', 'LeeSheldon@gmail.com', 3,2,2,3);


 -- INSERT INTO employee("first_name", "last_name", "mail")
-- END OF TESTING -- 