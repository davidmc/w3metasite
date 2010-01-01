DROP SEQUENCE "user_serial";
CREATE SEQUENCE "user_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "users";
CREATE TABLE "users" (
  id bigint DEFAULT nextval('user_serial'::text),
  group_id bigint NOT NULL,               -- link to groups
  first varchar(24) NOT NULL,             -- first name
  last varchar(24) NOT NULL,              -- last name
  login varchar(36) NOT NULL,             -- login
  password varchar(12) NOT NULL,          -- users password (encrypted)
  phone_number varchar(16) NOT NULL,      -- user phone number
  email varchar(64) NOT NULL,             -- user email
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

