CREATE TABLE ShoppingCart
(
  Id bigint primary key auto_increment NOT NULL,
  XML text NOT NULL,
  Last_Access datetime NOT NULL,
  Checked_Out bit NOT NULL
);

