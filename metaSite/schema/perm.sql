--
--  This is the schema
--  for the open permissions
--  generic system.
--
--       (n,1)      (1,n)                  (n,1)         (1,n)
--       +----groups-----group_services---------services---+
--       |      |                                          |
--       |      |(1,n)                                     |
--       |      |                                          |
--       |    users---+(1,n)                               |
--  group_roles       |                               role_services
--       |        user_roles                               |
--       |(n,1)       |                                    |
--       +----roles---+(1,n)                               |
--              |                                          |
--              +-----------------(1,n)--------------------+
--
--  Schema design by D.K.McCombs (7/2003)


-- DROP TABLE groups;
CREATE TABLE groups (
  id bigint not null primary key auto_increment,
  name varchar(24) NOT NULL,     -- group name
  domain varchar(255) NOT NULL,  -- domain for email
  url varchar(255) NOT NULL,     -- web address
  site_admin  int NOT NULL       -- admin indicator
);

-- DROP TABLE users;
CREATE TABLE users (
  id bigint not null primary key auto_increment,
  group_id bigint NOT NULL,               -- link to groups
  first varchar(24) NOT NULL,             -- first name
  last varchar(24) NOT NULL,              -- last name
  login varchar(36) NOT NULL,             -- login
  password varchar(12) NOT NULL,          -- users password (encrypted)
  phone_number varchar(16) NOT NULL,      -- user phone number
  email varchar(64) NOT NULL,              -- user email
  preferences text
);

-- DROP TABLE services;
CREATE TABLE services (
  id bigint not null primary key auto_increment,
  sequence integer NOT NULL,         -- order of apearance in a list
  tree_depth smallint NOT NULL,      -- order and tree depth determine tree position without recursion
  enabled int NOT NULL,              -- permits turning off of service for web site maintenance
  name varchar(32) NOT NULL,         -- name (for header)
  menu_name varchar(32) NOT NULL,    -- name (for menu item)
  uri varchar(64) NOT NULL,          -- location of resource
  local_uri int NOT NULL,            -- local service flag
  secure int NOT NULL,               -- determines if url should use https protocol
  xml_params text NOT NULL           -- generic parameters for service
);


/*
  Service parameters links groups to services,
  roles to services and lists service items
*/
-- DROP TABLE group_services;
CREATE TABLE group_services (
  id bigint not null primary key auto_increment,
  service_id     bigint NOT NULL,    -- 1,n relationship to services
  group_id       bigint NOT NULL,    -- 1,n relationship to groups
  sequence       smallint NOT NULL,  -- order of appearance in setup
  xml_param1     text,               -- two xml params for anything else
  xml_param2     text
);

-- DROP TABLE roles;
CREATE TABLE roles (
  id bigint not null primary key auto_increment,
  name varchar(24) NOT NULL,  -- role name
  comment varchar(255)
);

-- DROP TABLE user_roles;
CREATE TABLE user_roles (
  id bigint not null primary key auto_increment,
  user_id bigint NOT NULL,
  role_id bigint NOT NULL,
  enabled int
);

-- DROP TABLE role_services;
CREATE TABLE role_services (
  id bigint not null primary key auto_increment,
  role_id bigint NOT NULL,
  service_id bigint NOT NULL,
  enabled int
);

-- DROP TABLE group_roles;
CREATE TABLE group_roles (
  id bigint not null primary key auto_increment,
  role_id bigint NOT NULL,
  group_id bigint NOT NULL,
  enabled int
);
