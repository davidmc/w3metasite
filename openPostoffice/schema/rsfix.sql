DROP SEQUENCE "group_role_serial";
CREATE SEQUENCE "group_role_serial" start 1
  increment 1 maxvalue 2147483647
  minvalue 1  cache 1;
DROP TABLE "group_roles";
CREATE TABLE "group_roles" (
  id bigint DEFAULT nextval('group_role_serial'::text),
  role_id bigint NOT NULL,
  group_id bigint NOT NULL,
  enabled boolean,
  created     date,
  created_by  bigint,
  modified    date,
  modified_by bigint
);

insert into group_roles (  role_id, group_id ) values ( 1,1 );
insert into group_roles (  role_id, group_id ) values ( 2,2 );
