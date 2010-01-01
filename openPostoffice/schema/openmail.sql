/*
  This is the schema for the postoffice extensions to open permissions.

     [groups]----(1,n)-----------+
         |                       |
         | (1,n)                 |
         |                       |
      mail_groups----(1,n)---+   |
                             |   |
                             |   |
     [users]--(0,n)----global_directory
      | | |  (entry)          ex_name
      | | |                   ex_email
      | | |(1,n)              ex_phone
      | | |
      | | +- personal_mail_groups---+
      | |                           |
      | |                           |
      | +-(owner1,n)----personal_directory
      |                  |    ex_name
      +---(entry0,n)-----+    ex_email
                              ex_phone

  Schema design by D.K.McCombs (8/2002)

*/

/*
  phase 1 - have the global stuff ready

  -mail_groups
  -global_directory

*/
DROP SEQUENCE "mail_group_serial";
CREATE SEQUENCE "mail_group_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "mail_groups";
CREATE TABLE "mail_groups" (
  id bigint DEFAULT nextval('mail_group_serial'::text),
  group_id bigint NOT NULL,            -- group id
  name varchar(24) NOT NULL,           -- mail group name
  description  varchar(512) NOT NULL,  -- mail group description
  created     date,
  created_by  bigint,                  -- audits
  modified    date,
  modified_by bigint
);

DROP SEQUENCE "global_directory_serial";
CREATE SEQUENCE "global_directory_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "global_directory";
CREATE TABLE "global_directory" (
  id bigint DEFAULT nextval('global_directory_serial'::text),
  group_id bigint NOT NULL,      -- link to groups
  mail_group_id bigint NOT NULL, -- link to mail_groups
  mailto_id bigint,              -- link to user mail target.
                                 --   if null, use ex fields.
                                 --   if not null, ex becomes secondary info.
  ex_first varchar(24),          -- ex first name
  ex_last varchar(24),           -- ex last name
  ex_phone_number varchar(16),   -- ex phone number
  ex_email varchar(64),          -- ex email
  comments varchar(128),         -- comment
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

/*
  phase 2 - have the personal stuff ready

  -personal_mail_groups
  -personal_directory

*/
