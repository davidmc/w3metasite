

/*
views

select
id,group_id,mail_group_id,mailto_id,ex_first,ex_last,ex_phone_number,ex_email,comments,created,created_by,modified,modified_by
from global_directory where id = 1

select
gd.id, gd.group_id, gd.mail_group_id, gd.mailto_id,
coalesce(u.first,gd.ex_first) as first_name,
coalesce(u.last,gd.ex_last) as last_name,
coalesce(u.phone_number,gd.ex_phone_number) as first_phone_number,
coalesce(u.email,gd.ex_email) as first_email,
gd.ex_first, gd.ex_last, gd.ex_phone_number, gd.ex_email,
u.location_id
from
global_directory gd left outer join users u on u.id = gd.mailto_id;

select
rsp.role_id, r.name as role_name, r.comment as role_comment,
g.service_parameter_id,
g.group_id, g.group_name,
g.service_id, g.service_name,
g.sequence, g.tree_depth, g.enabled as service_enabled, g.menu_name,
g.uri, g.local_uri, g.secure, g.xml_params, g.xml_param1, g.xml_param2
from
vw_group_services g
inner join role_service_parms rsp on rsp.service_parameter_id = g.service_parameter_id
inner join roles r on r.id = rsp.role_id;
*/



