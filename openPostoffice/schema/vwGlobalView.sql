create view vw_global_dir as
select gd.id, mg.group_id , mg.name as mail_group_name,
coalesce( u.first, gd.ex_first, '-') as first,
coalesce( u.last, gd.ex_last, '-') as last,
coalesce( u.email, gd.ex_email, '-') as email,
coalesce( u.phone_number, gd.ex_phone_number, '-') as phone_number,
gd.ex_first, gd.ex_last, gd.ex_email, gd.ex_phone_number
from global_directory gd
inner join mail_groups mg
on gd.mail_group_id = mg.id
left outer join users u on gd.mailto_id = u.id;

