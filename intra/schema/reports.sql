/*
  reports.sql
  mysql version of reports schema

*/

drop table Report;
create table Report
(
  Id bigint primary key auto_increment NOT NULL,
  Site_Id bigint,
  Template_Id bigint,
  Query_Id bigint,
  Composite_Query bigint,
  Name text,
  Media_Type bigint,
  Category text,
  Show_In_List int,
  Formatting_Modulus int,
  Items_Per_Page int,
  External_Program text
);

drop table Composite_Query;
create table Composite_Query
(
    Id bigint primary key auto_increment NOT NULL,
    Site_Id bigint,
    Name text,
    Select_Part text,
    From_Part text,
    Where_Part text,
    Group_Part text,
    Having_Part text,
    Order_Part text,
    Is_Workflow int
);

drop table Report_Type;
create table Report_Type
(
    Id bigint primary key auto_increment NOT NULL,
    Name text,
    File_Extension text
);

create table Report_Templates
(
    Id bigint primary key auto_increment NOT NULL,
    Site_Id bigint,
    Name text,
    Path text
);

create table Report_Parameters
(
    Id bigint primary key auto_increment NOT NULL,
    Report_Id bigint,
    Display_Label text,
    Data_Element text,
    Tag_Name text,
    Choices_Query text,
    Parameter_Type text,
    List_Order int,
    Pre_Content text,
    Post_Content text
);


create table Report_Conditioning
(
    Id bigint primary key auto_increment NOT NULL,
    Report_Id bigint,
    Col int,
    Sort_Pos int,
    Col_Handler text,
    Direction text,
    Format text,
    Tuple_Section text,
    spawnedReport bigint,
    spawnedTuplePretext text,
    spawnedTuplePosttext text,
    spawningEvent int
);


create table Work
(
    Id bigint primary key auto_increment NOT NULL,
    Name text,
    Description text,
    Query bigint,
    User bigint,
    User_Group bigint,
    User_Role bigint,
    URL text,
    Search text
);

-- new catalog tables
create table Scheme_Table
(
    Id bigint primary key auto_increment NOT NULL,
    Name text,
    Abbr text
);

create table Scheme_Field
(
    Id bigint primary key auto_increment NOT NULL,
    Scheme_Table bigint,
    Name text,
    Col_Type text,
    Links_To bigint
);


