/*

  Test data for permissions database

*/

insert into groups ( name, site_admin ) values ( 'administration', true );

insert into users ( group_id, first, last, login,
                    password, phone_number, email ) values
                  ( 1, 'Jonathan', 'McCombs', 'jonmc',
                    'gijoe321', '706-673-4606', 'jonmc@goofy.com' );

insert into services( sequence, tree_depth, enabled, name,
                      menu_name, uri, local_uri, secure, xml_params ) values
                      (1,1,true,'manage groups', 'manage groups', 'groupList.cgi', true, false, '' );
insert into services( sequence, tree_depth, enabled, name,
                      menu_name, uri, local_uri, secure, xml_params ) values
                      (2,1,true,'manage services', 'manage services', 'serviceList.cgi', true, false, '' );
insert into services( sequence, tree_depth, enabled, name,
                      menu_name, uri, local_uri, secure, xml_params ) values
                      (3,1,true,'manage users', 'manage users', 'userList.cgi', true, false, '' );

insert into service_parameters ( service_id, group_id, sequence ) values
                               ( 1, 1, 1 );
insert into service_parameters ( service_id, group_id, sequence ) values
                               ( 2, 1, 2 );
insert into service_parameters ( service_id, group_id, sequence ) values
                               ( 3, 1, 3 );

insert into roles ( name, comment ) values ( 'administrator', 'site admin'  );


insert into role_services (role_id, service_id, enabled ) values ( 1, 1, true );
insert into role_services (role_id, service_id, enabled ) values ( 1, 2, true );
insert into role_services (role_id, service_id, enabled ) values ( 1, 3, true );


insert into user_roles ( user_id, role_id, enabled ) values ( 1, 1, true );



