create table Session
(
    Id bigint primary key auto_increment NOT NULL,
    User_Id bigint,
    UUID text,
    Instance_Stamp datetime,
    XML_Variables text
);

