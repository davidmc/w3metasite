/*

  This is the schema
  for the open permissions
  generic system.

       (n,1)      (1,n)                  (n,1)         (1,n)
       +----groups-----service_parameters-----services---+
       |      |                                          |
       |      |(1,n)                                     |
       |      |                                          |
       |    users---+(1,n)                               |
  group_roles       |                               role_services
       |        user_roles                               |
       |(n,1)       |                                    |
       +----roles---+(1,n)                               |
              |                                          |
              +-----------------(1,n)--------------------+


*/

DROP SEQUENCE "group_serial";
CREATE SEQUENCE "group_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "groups";
CREATE TABLE "groups" (
  id bigint DEFAULT nextval('group_serial'::text),
  name varchar(24) NOT NULL,     -- group name
  domain varchar(256) NOT NULL,  -- domain for email
  url varchar(256) NOT NULL,     -- web address
  site_admin  boolean NOT NULL,  -- admin indicator
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);


DROP SEQUENCE "location_serial";
CREATE SEQUENCE "location_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "locations";
CREATE TABLE "locations" (
  id bigint DEFAULT nextval('location_serial'::text),
  group_id bigint NOT NULL,      -- need group to limit locations selection
  name varchar(24) NOT NULL,     -- location name
  description varchar(512),      -- location description
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

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
  location_id bigint,                     -- points to user location
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

DROP SEQUENCE "service_serial";
CREATE SEQUENCE "service_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "services";
CREATE TABLE "services" (
  id bigint DEFAULT nextval('service_serial'::text),
  sequence integer NOT NULL,         -- order of apearance in a list
  tree_depth smallint NOT NULL,      -- order and tree depth determine tree position without recursion
  enabled boolean NOT NULL,          -- permits turning off of service for web site maintenance
  name varchar(32) NOT NULL,         -- name (for header)
  menu_name varchar(32) NOT NULL,    -- name (for menu item)
  uri varchar(64) NOT NULL,          -- location of resource
  local_uri boolean NOT NULL,        -- local service flag
  secure boolean NOT NULL,           -- determines if url should use https protocol
  xml_params varchar(512) NOT NULL,  -- generic parameters for service
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

DROP SEQUENCE "service_item_serial";
CREATE SEQUENCE "service_item_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "service_items";
CREATE TABLE "service_items" (
  id bigint DEFAULT nextval('service_item_serial'::text),
  service_id bigint NOT NULL,  -- parent service
  sequence integer NOT NULL,   -- order of apearance in a list
  name varchar(32) NOT NULL,   -- name (for administration)
  enabled boolean NOT NULL,    -- turn off for web site maintenance
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);


/*
  Service parameters links groups to services,
  roles to services and lists service items
*/
DROP SEQUENCE "service_parameter_serial";
CREATE SEQUENCE "service_parameter_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "service_parameters";
CREATE TABLE "service_parameters" (
  id bigint DEFAULT nextval('service_parameter_serial'::text),
  service_id     bigint NOT NULL,    -- 1,n relationship to services
  group_id       bigint NOT NULL,    -- 1,n relationship to groups
  sequence       smallint NOT NULL,  -- order of appearance in setup
  xml_param1     varchar(512),
  xml_param2     varchar(512),
  service_items  varchar(128),
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

DROP SEQUENCE "role_serial";
CREATE SEQUENCE "role_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "roles";
CREATE TABLE "roles" (
  id bigint DEFAULT nextval('role_serial'::text),
  name varchar(24) NOT NULL,  -- role name
  comment varchar(256),
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

DROP SEQUENCE "user_role_serial";
CREATE SEQUENCE "user_role_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "user_roles";
CREATE TABLE "user_roles" (
  id bigint DEFAULT nextval('user_role_serial'::text),
  user_id bigint NOT NULL,
  role_id bigint NOT NULL,
  enabled boolean,
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

DROP SEQUENCE "role_service_serial";
CREATE SEQUENCE "role_service_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "role_services";
CREATE TABLE "role_services" (
  id bigint DEFAULT nextval('role_service_serial'::text),
  role_id bigint NOT NULL,
  service_id bigint NOT NULL,
  enabled boolean,
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);
