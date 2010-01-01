create table Site_History
(
  Id bigint primary key auto_increment NOT NULL,
  Remote_Host text,
  Remote_Log text,
  Remote_User text,
  Request_Time datetime,
  Method text,
  Resource_Requested text,
  Protocol text,
  Original_Status text,
  Response_Size int,
  Referer text,
  User_Agent text,
  log_file text
); 
