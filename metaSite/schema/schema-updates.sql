--alter table pages add column meta_description text;
--CREATE TABLE global_content (
--  id bigint not null primary key auto_increment,
--  site_id bigint NOT NULL,    -- site  
--  name text NOT NULL,    -- name
--  content text NOT NULL  -- content   
--);
--
--alter table sites add column path text;
--
--DKMc- below new for 1/2005 - exciting new stuff for blogs and stuff

alter table paragraphs add column ( author bigint null, 
                                    time_authored datetime null, 
                                    time_start datetime null,
                                    time_end datetime null,
                                    approved int null );


-- DKMc new for easy menu linkage
alter table menus add column  reference_template text;
alter table menus add column  template_open_tag text;
alter table menus add column  template_close_tag text;

