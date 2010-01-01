--  light weight security (without groups)

create table users
(
id bigint primary key auto_increment NOT NULL,
last text,
first text,
login text,
password text,
email text,
phone text,
mobile text,
fax text,
pager text
);

create table roles
(
id bigint primary key auto_increment NOT NULL,
name text,
comment text
);

create table services
(
id bigint primary key auto_increment NOT NULL,
sequence int,
tree_depth int,
enabled int,
cat_name text,
menu_name text,
uri text,
target text
);

create table role_services
(
id bigint primary key auto_increment NOT NULL,
role_id bigint,
service_id bigint,
enabled int,
xml_params text
);

create table user_roles
(
id bigint primary key auto_increment NOT NULL,
user_id bigint,
role_id bigint,
enabled int
);

-- bootstrapping
insert into users (  last,  first,  login,  password, email, phone,  mobile, pager )
values
( 'McCombs', 'David', 'davidmc', 'freedom7', 'davidmc@w3sys.com', '706 581-4607', '706 581-4607', '' );

insert into services 
( sequence,tree_depth,enabled,cat_name,menu_name,uri,target )
values
( 1,1,1,'Service Authorization','Services','services_liteUI.meta?init=1','_self');
insert into services 
( sequence,tree_depth,enabled,cat_name,menu_name,uri,target )
values
( 2,1,1,'Service Authorization','Roles','rolesUI.meta?init=1','_self');

insert into roles ( name , comment )
values
( "developer", "bootstrapped default developer role" );

insert into role_services
(role_id, service_id, enabled, xml_params )
values
(1,1,1,'');
insert into role_services
(role_id, service_id, enabled, xml_params )
values
(1,2,1,'');

insert into user_roles
(user_id, role_id, enabled )
values
( 1, 1, 1);
insert into user_roles
(user_id, role_id, enabled )
values
( 1, 2, 1);


