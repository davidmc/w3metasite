-- note that this is intended for the local business schema and not the metasite schema
create table Document
(
  Id bigint primary key auto_increment NOT NULL,
  Category text,
  Name text,
  Description text,
  Filename text,
  Visible int,
  Sequence int
);