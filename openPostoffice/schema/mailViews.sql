drop view vw_global_directory;
create view vw_global_directory as
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
