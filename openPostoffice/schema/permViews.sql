/*
 views

*/
-- how to get group services
drop view vw_group_services;
create view vw_group_services as
select
sp.id as service_parameter_id,
g.id as group_id, g.name as group_name,
s.id as service_id, s.name as service_name,
s.sequence, s.tree_depth, s.enabled, s.menu_name,
s.uri, s.local_uri, s.secure, s.xml_params, sp.xml_param1, sp.xml_param2, sp.service_items
from
services s
inner join service_parameters sp on s.id = sp.service_id
inner join groups g on g.id = sp.group_id;

-- how to get role services
drop view vw_role_services;
create view vw_role_services as 
select rs.role_id, r.name as role_name, r.comment as role_comment,
g.service_parameter_id,
g.group_id, g.group_name,
g.service_id, g.service_name,
g.sequence, g.tree_depth, g.enabled as service_enabled, g.menu_name,
g.uri, g.local_uri, g.secure, g.xml_params, g.xml_param1, g.xml_param2, g.service_items
from
vw_group_services g
inner join group_roles gr on gr.group_id = g.group_id
inner join role_services rs on rs.service_id = g.service_id and rs.role_id = gr.role_id
inner join roles r on r.id = gr.role_id;
