/*
  reports.sql
*/

drop table Report;
create table Report
(
    Id bigint primary key auto_increment NOT NULL,
    Site_Id bigint,
    Template_Id bigint,
    Query_Id bigint,
    Name text,
    Media_Type bigint
);

drop table Query;
create table Query
(
    Id bigint primary key auto_increment NOT NULL,
    Site_Id bigint,
    Name text,
    Query text
);
drop table Report_Type;
create table Report_Type
(
    Id bigint primary key auto_increment NOT NULL,
    Name text
);
create table Report_Templates
(
    Id bigint primary key auto_increment NOT NULL,
    Site_Id bigint,
    Name text,
    Path text
);
