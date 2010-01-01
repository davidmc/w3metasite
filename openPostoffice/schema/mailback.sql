--
-- Selected TOC Entries:
--
\connect - postgres
--
-- TOC Entry ID 2 (OID 245386)
--
-- Name: group_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "group_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 24 (OID 245405)
--
-- Name: groups Type: TABLE Owner: postgres
--

CREATE TABLE "groups" (
	"id" bigint DEFAULT nextval('group_serial'::text),
	"name" character varying(24) NOT NULL,
	"site_admin" boolean NOT NULL,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 4 (OID 245422)
--
-- Name: location_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "location_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 25 (OID 245441)
--
-- Name: locations Type: TABLE Owner: postgres
--

CREATE TABLE "locations" (
	"id" bigint DEFAULT nextval('location_serial'::text),
	"group_id" bigint NOT NULL,
	"name" character varying(24) NOT NULL,
	"description" character varying(512),
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 6 (OID 245459)
--
-- Name: user_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "user_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 26 (OID 245478)
--
-- Name: users Type: TABLE Owner: postgres
--

CREATE TABLE "users" (
	"id" bigint DEFAULT nextval('user_serial'::text),
	"group_id" bigint NOT NULL,
	"first" character varying(24) NOT NULL,
	"last" character varying(24) NOT NULL,
	"login" character varying(12) NOT NULL,
	"password" character varying(12) NOT NULL,
	"phone_number" character varying(16) NOT NULL,
	"email" character varying(64) NOT NULL,
	"location_id" bigint,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 8 (OID 245501)
--
-- Name: service_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "service_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 27 (OID 245520)
--
-- Name: services Type: TABLE Owner: postgres
--

CREATE TABLE "services" (
	"id" bigint DEFAULT nextval('service_serial'::text),
	"sequence" integer NOT NULL,
	"tree_depth" smallint NOT NULL,
	"enabled" boolean NOT NULL,
	"name" character varying(32) NOT NULL,
	"menu_name" character varying(32) NOT NULL,
	"uri" character varying(64) NOT NULL,
	"local_uri" boolean NOT NULL,
	"secure" boolean NOT NULL,
	"xml_params" character varying(512) NOT NULL,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 10 (OID 245544)
--
-- Name: service_item_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "service_item_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 28 (OID 245563)
--
-- Name: service_items Type: TABLE Owner: postgres
--

CREATE TABLE "service_items" (
	"id" bigint DEFAULT nextval('service_item_serial'::text),
	"service_id" bigint NOT NULL,
	"sequence" integer NOT NULL,
	"name" character varying(32) NOT NULL,
	"enabled" boolean NOT NULL,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 12 (OID 245582)
--
-- Name: service_parameter_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "service_parameter_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 29 (OID 245601)
--
-- Name: service_parameters Type: TABLE Owner: postgres
--

CREATE TABLE "service_parameters" (
	"id" bigint DEFAULT nextval('service_parameter_serial'::text),
	"service_id" bigint NOT NULL,
	"group_id" bigint NOT NULL,
	"sequence" smallint NOT NULL,
	"xml_param1" character varying(512),
	"xml_param2" character varying(512),
	"service_items" character varying(128),
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 14 (OID 245622)
--
-- Name: role_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "role_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 30 (OID 245641)
--
-- Name: roles Type: TABLE Owner: postgres
--

CREATE TABLE "roles" (
	"id" bigint DEFAULT nextval('role_serial'::text),
	"name" character varying(24) NOT NULL,
	"comment" character varying(256),
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 16 (OID 245658)
--
-- Name: user_role_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "user_role_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 31 (OID 245677)
--
-- Name: user_roles Type: TABLE Owner: postgres
--

CREATE TABLE "user_roles" (
	"id" bigint DEFAULT nextval('user_role_serial'::text),
	"user_id" bigint NOT NULL,
	"role_id" bigint NOT NULL,
	"enabled" boolean,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 18 (OID 245695)
--
-- Name: role_service_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "role_service_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 32 (OID 245714)
--
-- Name: role_services Type: TABLE Owner: postgres
--

CREATE TABLE "role_services" (
	"id" bigint DEFAULT nextval('role_service_serial'::text),
	"role_id" bigint NOT NULL,
	"service_id" bigint NOT NULL,
	"enabled" boolean,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 20 (OID 245944)
--
-- Name: mail_group_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "mail_group_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 33 (OID 245963)
--
-- Name: mail_groups Type: TABLE Owner: postgres
--

CREATE TABLE "mail_groups" (
	"id" bigint DEFAULT nextval('mail_group_serial'::text),
	"group_id" bigint NOT NULL,
	"name" character varying(24) NOT NULL,
	"description" character varying(512) NOT NULL,
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 22 (OID 245981)
--
-- Name: global_directory_serial Type: SEQUENCE Owner: postgres
--

CREATE SEQUENCE "global_directory_serial" start 1 increment 1 maxvalue 2147483647 minvalue 1  cache 1 ;

--
-- TOC Entry ID 34 (OID 246081)
--
-- Name: global_directory Type: TABLE Owner: postgres
--

CREATE TABLE "global_directory" (
	"id" bigint DEFAULT nextval('global_directory_serial'::text),
	"group_id" bigint NOT NULL,
	"mail_group_id" bigint NOT NULL,
	"mailto_id" bigint,
	"ex_first" character varying(24),
	"ex_last" character varying(24),
	"ex_phone_number" character varying(16),
	"ex_email" character varying(32),
	"comments" character varying(128),
	"created" date,
	"created_by" bigint,
	"modified" date,
	"modified_by" bigint
);

--
-- TOC Entry ID 35 (OID 246129)
--
-- Name: vw_group_services Type: VIEW Owner: postgres
--

CREATE VIEW "vw_group_services" as SELECT sp.id AS service_parameter_id, g.id AS group_id, g.name AS group_name, s.id AS service_id, s.name AS service_name, s."sequence", s.tree_depth, s.enabled, s.menu_name, s.uri, s.local_uri, s.secure, s.xml_params, sp.xml_param1, sp.xml_param2, sp.service_items FROM ((services s JOIN service_parameters sp ON ((s.id = sp.service_id))) JOIN groups g ON ((g.id = sp.group_id)));

--
-- TOC Entry ID 36 (OID 246161)
--
-- Name: vw_role_services Type: VIEW Owner: postgres
--

CREATE VIEW "vw_role_services" as SELECT rs.role_id, r.name AS role_name, r."comment" AS role_comment, g.service_parameter_id, g.group_id, g.group_name, g.service_id, g.service_name, g."sequence", g.tree_depth, g.enabled AS service_enabled, g.menu_name, g.uri, g.local_uri, g.secure, g.xml_params, g.xml_param1, g.xml_param2, g.service_items FROM ((vw_group_services g JOIN role_services rs ON ((rs.service_id = g.service_id))) JOIN roles r ON ((r.id = rs.role_id)));

--
-- TOC Entry ID 37 (OID 246187)
--
-- Name: vw_global_directory Type: VIEW Owner: postgres
--

CREATE VIEW "vw_global_directory" as SELECT gd.id, gd.group_id, gd.mail_group_id, gd.mailto_id, CASE WHEN (u.first NOTNULL) THEN u.first WHEN (gd.ex_first NOTNULL) THEN gd.ex_first ELSE NULL::"varchar" END AS first_name, CASE WHEN (u.last NOTNULL) THEN u.last WHEN (gd.ex_last NOTNULL) THEN gd.ex_last ELSE NULL::"varchar" END AS last_name, CASE WHEN (u.phone_number NOTNULL) THEN u.phone_number WHEN (gd.ex_phone_number NOTNULL) THEN gd.ex_phone_number ELSE NULL::"varchar" END AS first_phone_number, CASE WHEN (u.email NOTNULL) THEN u.email WHEN (gd.ex_email NOTNULL) THEN gd.ex_email ELSE NULL::"varchar" END AS first_email, gd.ex_first, gd.ex_last, gd.ex_phone_number, gd.ex_email, u.location_id FROM (global_directory gd LEFT JOIN users u ON ((u.id = gd.mailto_id)));

--
-- Data for TOC Entry ID 38 (OID 245405)
--
-- Name: groups Type: TABLE DATA Owner: postgres
--


INSERT INTO "groups" VALUES ('1','Administration','t',NULL,NULL,'2002-08-11','1');
INSERT INTO "groups" VALUES ('2','Highland Rivers','f','2002-08-11','1','2002-08-11','1');
--
-- Data for TOC Entry ID 39 (OID 245441)
--
-- Name: locations Type: TABLE DATA Owner: postgres
--


INSERT INTO "locations" VALUES ('1','1','Dalton','Dalton Test Location','2002-08-11','1','2002-08-11','1');
INSERT INTO "locations" VALUES ('2','2','Dalton IS','IS Department','2002-08-11','1','2002-08-11','1');
--
-- Data for TOC Entry ID 40 (OID 245478)
--
-- Name: users Type: TABLE DATA Owner: postgres
--


INSERT INTO "users" VALUES ('1','1','David','McCombs','davidmc','friendship7','706-673-4606','davidmc@newcottage.com','1',NULL,NULL,'2002-08-12','1');
--
-- Data for TOC Entry ID 41 (OID 245520)
--
-- Name: services Type: TABLE DATA Owner: postgres
--


INSERT INTO "services" VALUES ('1',1,1,'t','manage groups','manage groups','groupList.cgi','t','f','',NULL,NULL,NULL,NULL);
INSERT INTO "services" VALUES ('3',3,1,'t','manage users','manage users','userList.cgi','t','f','',NULL,NULL,NULL,NULL);
INSERT INTO "services" VALUES ('2',5,1,'t','manage roles','manage roles','roleList.cgi','t','f','',NULL,NULL,'2002-08-11','1');
INSERT INTO "services" VALUES ('6',4,1,'t','manage services','manage services','serviceList.cgi','t','f','','2002-08-11','1','2002-08-11','1');
INSERT INTO "services" VALUES ('7',6,1,'t','manage locations','manage locations','locationList.cgi','t','f','','2002-08-11','1','2002-08-11','1');
INSERT INTO "services" VALUES ('8',20,1,'t','manage mail groups','manage mail groups','mailGroupList.cgi','t','f','','2002-08-11','1','2002-08-11','1');
INSERT INTO "services" VALUES ('9',21,1,'t','manage global directory','manage global directory','globalDirList.cgi','t','f','','2002-08-11','1','2002-08-11','1');
--
-- Data for TOC Entry ID 42 (OID 245563)
--
-- Name: service_items Type: TABLE DATA Owner: postgres
--


--
-- Data for TOC Entry ID 43 (OID 245601)
--
-- Name: service_parameters Type: TABLE DATA Owner: postgres
--


INSERT INTO "service_parameters" VALUES ('1','1','1',1,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "service_parameters" VALUES ('2','2','1',2,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "service_parameters" VALUES ('3','3','1',3,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
INSERT INTO "service_parameters" VALUES ('4','6','1',0,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('5','3','2',1,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('6','1','2',0,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('7','7','1',0,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('8','7','2',7,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('9','8','1',20,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('10','8','2',20,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('11','9','1',21,'','','','2002-08-11','1','2002-08-11','1');
INSERT INTO "service_parameters" VALUES ('12','9','2',21,'','','','2002-08-11','1','2002-08-11','1');
--
-- Data for TOC Entry ID 44 (OID 245641)
--
-- Name: roles Type: TABLE DATA Owner: postgres
--


INSERT INTO "roles" VALUES ('2','Client Administrator','Client Self Administration for Users, Locations, Directories etc.','2002-08-11','1','2002-08-11','1');
INSERT INTO "roles" VALUES ('1','Site Administrator','site admin.\015\012Intended for owner of site',NULL,NULL,'2002-08-11','1');
--
-- Data for TOC Entry ID 45 (OID 245677)
--
-- Name: user_roles Type: TABLE DATA Owner: postgres
--


INSERT INTO "user_roles" VALUES ('1','1','1','t',NULL,NULL,NULL,NULL);
--
-- Data for TOC Entry ID 46 (OID 245714)
--
-- Name: role_services Type: TABLE DATA Owner: postgres
--


INSERT INTO "role_services" VALUES ('1','1','1','t',NULL,NULL,NULL,NULL);
INSERT INTO "role_services" VALUES ('2','1','2','t',NULL,NULL,NULL,NULL);
INSERT INTO "role_services" VALUES ('3','1','3','t',NULL,NULL,NULL,NULL);
INSERT INTO "role_services" VALUES ('4','1','6','t','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('5','2','-1','f','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('6','2','3','f','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('7','2','7','t','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('8','1','7','t','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('10','1','8','t','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('9','2','8','t','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('12','2','9','t','2002-08-11','1','2002-08-11','1');
INSERT INTO "role_services" VALUES ('11','1','9','t','2002-08-11','1','2002-08-11','1');
--
-- Data for TOC Entry ID 47 (OID 245963)
--
-- Name: mail_groups Type: TABLE DATA Owner: postgres
--


INSERT INTO "mail_groups" VALUES ('1','2','Dalton IS','Dalton IS','2002-08-11','1','2002-08-11','1');
INSERT INTO "mail_groups" VALUES ('2','2','Dalton HR Administration','Human Resources','2002-08-11','1','2002-08-11','1');
INSERT INTO "mail_groups" VALUES ('3','1','Admin Main','','2002-08-12','1','2002-08-12','1');
--
-- Data for TOC Entry ID 48 (OID 246081)
--
-- Name: global_directory Type: TABLE DATA Owner: postgres
--


INSERT INTO "global_directory" VALUES ('3','1','3','1','','','212-212-1212','du@du','Good Kid','2002-08-12','1','2002-08-12','1');
--
-- TOC Entry ID 3 (OID 245386)
--
-- Name: group_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"group_serial"', 11, 't');

--
-- TOC Entry ID 5 (OID 245422)
--
-- Name: location_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"location_serial"', 2, 't');

--
-- TOC Entry ID 7 (OID 245459)
--
-- Name: user_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"user_serial"', 4, 't');

--
-- TOC Entry ID 9 (OID 245501)
--
-- Name: service_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"service_serial"', 9, 't');

--
-- TOC Entry ID 11 (OID 245544)
--
-- Name: service_item_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"service_item_serial"', 1, 'f');

--
-- TOC Entry ID 13 (OID 245582)
--
-- Name: service_parameter_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"service_parameter_serial"', 12, 't');

--
-- TOC Entry ID 15 (OID 245622)
--
-- Name: role_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"role_serial"', 2, 't');

--
-- TOC Entry ID 17 (OID 245658)
--
-- Name: user_role_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"user_role_serial"', 1, 't');

--
-- TOC Entry ID 19 (OID 245695)
--
-- Name: role_service_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"role_service_serial"', 12, 't');

--
-- TOC Entry ID 21 (OID 245944)
--
-- Name: mail_group_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"mail_group_serial"', 3, 't');

--
-- TOC Entry ID 23 (OID 245981)
--
-- Name: global_directory_serial Type: SEQUENCE SET Owner: 
--

SELECT setval ('"global_directory_serial"', 3, 't');

