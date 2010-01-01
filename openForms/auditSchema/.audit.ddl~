create table AuditTable
(
    Id bigint primary key auto_increment NOT NULL,
    aDatabase text,
    aTable text,
    aRow bigint,
    Actor bigint,
    Timestamp datetime,
    auditType int
);
create table FieldChange
(
    Id bigint primary key auto_increment NOT NULL,
    auditTable bigint,
    Name text,
    Value text
);
create table Deletion
(
    Id bigint primary key auto_increment NOT NULL,
    auditTable bigint,
    Name text,
    Value text
);

