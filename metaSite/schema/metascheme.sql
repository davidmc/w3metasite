/*
 This is the schema
 for the metasite system.
 
 +------------------site := main class, links to everything
 |          (n,1)     |
 |                  image := class for image picker
 |
 +--global_content := content on many pages of the site. 
 |              (1,n)
 +-template------------------page------paragraph
 |                             |  (1,n)
 +-link------------+ (1,n)     |
 |       (1,n)     |           | 
 +--menu--------menu_item      |
 |    |                        |
 |    |(1,n)                   | 
 |    |                 (n,1)  |
 +--page_menu------------------+
 
 Schema design by D.K.McCombs (5/2003)
*/

DROP TABLE sites;
CREATE TABLE sites (
  id bigint not null primary key auto_increment,
  name text NOT NULL,    -- site name
  domain text NOT NULL,  -- domain name of site
  url text,              -- fully qualified url
  path text              -- file path to site directory on server
);

DROP TABLE global_content;
CREATE TABLE global_content (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,    -- site  
  name text NOT NULL,    -- name
  content text NOT NULL  -- content   
);

DROP TABLE group_sites;
CREATE TABLE group_sites (
  id bigint not null primary key auto_increment,
  group_id bigint NOT NULL,  -- group 
  site_id bigint NOT NULL    -- site   
);

DROP TABLE images;
CREATE TABLE images (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,         -- link to site
  name text NOT NULL,              -- name of image
  url  text NOT NULL               -- location
);

DROP TABLE templates;
CREATE TABLE templates (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,  -- link to site
  name text,       -- name (for menu item)
  path text        -- path to file
);

DROP TABLE pages;
CREATE TABLE pages (
  id bigint not null primary key auto_increment,
  site_id     bigint NOT NULL,  -- the id to the containing web site  
  template_id bigint NOT NULL,  -- the id to the page template
  name        text NOT NULL,    -- name
  url         text,             -- location
  meta_description text        -- New! description for robots.
);

DROP TABLE paragraphs;
CREATE TABLE paragraphs (
  id bigint not null primary key auto_increment,
  site_id     bigint NOT NULL,
  page_id     bigint NOT NULL,
  place_order int NOT NULL,
  template_tag char(128) NOT NULL, 
  replace_tag  char(128) NOT NULL,  
  name text, 
  content text,
  author bigint null, -- New 1/2005 for blog wiki
  time_authored datetime null, -- New 1/2005 for blog wiki
  time_start datetime null, -- New 1/2005 for blog wiki
  time_end datetime null, -- New 1/2005 for blog wiki
  approved int null -- for New 1/2005 blog wiki
);

DROP TABLE links;
CREATE TABLE links(
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,
  name    text,
  url     text,
  target  text
);

DROP TABLE menus;
CREATE TABLE menus (
  id bigint not null primary key auto_increment,
  site_id  bigint NOT NULL,
  name     text,
  reference_template text,
  template_open_tag text,
  template_close_tag text
);

DROP TABLE page_menus;
CREATE TABLE page_menus (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,
  menu_id bigint NOT NULL,
  page_id bigint NOT NULL,
  place_order int,
  collapsible int
);

DROP TABLE menu_items;
CREATE TABLE menu_items (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,
  menu_id bigint NOT NULL,
  link_id bigint NOT NULL,
  template_tag char(128) NOT NULL,
  locate_tag char(128) NOT NULL,
  target_tag char(128) NOT NULL,
  label_tag char(128) NOT NULL,
  place_order int NOT NULL,
  place_level int NOT NULL  
);

DROP TABLE newsletter;
CREATE TABLE newsletter (
  id bigint not null primary key auto_increment,
  site_id bigint NOT NULL,
  page_id     bigint NOT NULL,
  address_query text NOT NULL
);

