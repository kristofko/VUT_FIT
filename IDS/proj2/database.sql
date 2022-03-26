-- TABLES DESTRUCTION--

DROP TABLE art CASCADE CONSTRAINTS;
DROP TABLE employee CASCADE CONSTRAINTS;
DROP TABLE artist CASCADE CONSTRAINTS;
DROP TABLE exposition CASCADE CONSTRAINTS;
DROP TABLE showroom CASCADE CONSTRAINTS;
DROP TABLE prospect CASCADE CONSTRAINTS ;


-- END OF TABLES DESTRUCTION --

CREATE TABLE employee(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first_name" VARCHAR(128),
	"last_name" VARCHAR(128)
	"mail" VARCHAR(128),
	"phone" VARCHAR(15)
);

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
	"type" VARCHAR(255)
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

CREATE TABLE prospect(
	"id" INT GENERATED AS IDENTITY NOT NULL PRIMARY KEY,
	"first-name" VARCHAR(255),
	"last-name" VARCHAR(255),
	"mail" VARCHAR(255),
	"paid" VARCHAR(10),
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
INSERT INTO employee("name", "contact") VALUES('JOHN WICK', 'mailasd');
INSERT INTO employee("name", "contact") VALUES('RYSANEK', 'xsryasn05@vutbr.cz');
INSERT INTO employee("name", "contact") VALUES('RYsJAKYBs', 'xsryasn05@vuasdtbr.cz');
INSERT INTO art("name") VALUES('naked lady');
INSERT INTO showroom("price", "type", "height", "width", "length", "equipment", "art_id" )
	VALUES(100, 'normaasl', 120,124,250, 'notg', 1 );

-- END OF TESTING -- 