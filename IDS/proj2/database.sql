-- TABLES DESTRUCTION--

DROP TABLE art CASCADE CONSTRAINTS;
DROP TABLE employee CASCADE CONSTRAINTS;
DROP TABLE artist CASCADE CONSTRAINTS;
DROP TABLE exposition CASCADE CONSTRAINTS;
DROP TABLE showroom CASCADE CONSTRAINTS;
DROP TABLE prospect CASCADE CONSTRAINTS ;


-- END OF TABLES DESTRUCTION --



CREATE TABLE artist(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(255),
	"last_name" VARCHAR(255)
);
CREATE TABLE art (
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"name" VARCHAR(255),
	"description" VARCHAR(255)
);

CREATE TABLE exposition(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"start_date" DATE,
	"end_date" DATE,
	"type" VARCHAR(255),
	"art_id" INT,
	CONSTRAINT "exposition_art_fk_id" FOREIGN KEY ("art_id") REFERENCES art("id") 
);

CREATE TABLE showroom(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"price" INT,
	"type" VARCHAR(255),
	"height" INT,
	"width" INT,
	"length" INT,
	"equipment" VARCHAR(255),
	"art_id" INT NOT NULL,
	CONSTRAINT "showroom_art_fk_id" FOREIGN KEY ("art_id") REFERENCES art("id") 
        ON DELETE SET NULL
);

CREATE TABLE employee(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(128),
	"last_name" VARCHAR(128),
	"mail" VARCHAR(128),
	"phone" VARCHAR(15),
	"showroom_id" INT,
	CONSTRAINT "employee_showroom_fk_id" FOREIGN KEY ("showroom_id") REFERENCES showroom("id")
);

CREATE TABLE prospect(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(255),
	"last_name" VARCHAR(255),
	"mail" VARCHAR(255),
	"showroom_id" INT,
	"art_id" INT,
	"exposition_id" INT,
	CONSTRAINT "prospect_showroom_fk_id" FOREIGN KEY ("showroom_id") REFERENCES showroom("id")
		ON DELETE SET NULL,
	CONSTRAINT "prospect_art_fk_id" FOREIGN KEY ("art_id") REFERENCES art("id")
		ON DELETE SET NULL,
	CONSTRAINT "prospect_exposition_fk_id" FOREIGN KEY ("exposition_id") REFERENCES exposition("id")
		ON DELETE SET NULL
);

-- TESTING --
INSERT INTO art("name", "description") VALUES('naked lady', 'A pic of naked Rysanek');
INSERT INTO art("name", "description") VALUES('beer', 'Svijany');

INSERT INTO exposition("start_date", "end_date", "type", "art_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 1);
INSERT INTO exposition("start_date", "end_date", "type", "art_id")
	VALUES (TO_DATE('2022-12-22', 'yyyy/mm/dd'), TO_DATE('2022-12-30', 'yyyy/mm/dd'), 'gallery', 2);


INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "art_id" )
	VALUES (150, 'Normal', 120, 500, 200, NULL, 1);

INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "art_id" )
	VALUES (1300, 'Special', 1250, 150, 2200, NULL, 2);

INSERT INTO prospect("first_name", "last_name", "mail", "showroom_id", "art_id", "exposition_id")
	VALUES ('Jan', 'Novak', 'novak@mail.com', 1, 1, 1);

 -- INSERT INTO employee("first_name", "last_name", "mail")
-- END OF TESTING -- 