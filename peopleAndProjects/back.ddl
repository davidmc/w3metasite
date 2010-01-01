-- MySQL dump 10.9
--
-- Host: localhost    Database: w3sys
-- ------------------------------------------------------
-- Server version	4.1.11

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Appointment`
--

DROP TABLE IF EXISTS `Appointment`;
CREATE TABLE `Appointment` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Company` bigint(20) default NULL,
  `Contact` bigint(20) default NULL,
  `Location` text,
  `Title` text,
  `Description` text,
  `Starts` datetime default NULL,
  `Stops` datetime default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Appointment`
--


/*!40000 ALTER TABLE `Appointment` DISABLE KEYS */;
LOCK TABLES `Appointment` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `Appointment` ENABLE KEYS */;

--
-- Table structure for table `Attachments`
--

DROP TABLE IF EXISTS `Attachments`;
CREATE TABLE `Attachments` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Message` bigint(20) default NULL,
  `Task` bigint(20) default NULL,
  `Type` bigint(20) default NULL,
  `URL` text,
  `Data_Id` bigint(20) default NULL,
  `Title` text,
  `Description` text,
  `Search_String` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Attachments`
--


/*!40000 ALTER TABLE `Attachments` DISABLE KEYS */;
LOCK TABLES `Attachments` WRITE;
INSERT INTO `Attachments` VALUES (1,-1,66,NULL,'Conversemos2.doc',NULL,'Conversemos?','I have no idea',NULL),(2,-1,121,NULL,'InTheBleak-1.pdf',NULL,'In The Bleak Mid Winter','Bleak Christmas Song',NULL);
UNLOCK TABLES;
/*!40000 ALTER TABLE `Attachments` ENABLE KEYS */;

--
-- Table structure for table `Company`
--

DROP TABLE IF EXISTS `Company`;
CREATE TABLE `Company` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Name` text,
  `Key_Contact` bigint(20) default NULL,
  `Address1` text,
  `Address2` text,
  `City` text,
  `State` text,
  `Zip` text,
  `Country` text,
  `Notes` text,
  `Last_Contacted` datetime default NULL,
  `Status` bigint(20) default NULL,
  `Open_PO` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Composite_Query`
--

DROP TABLE IF EXISTS `Composite_Query`;
CREATE TABLE `Composite_Query` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Site_Id` bigint(20) default NULL,
  `Name` text,
  `Select_Part` text,
  `From_Part` text,
  `Where_Part` text,
  `Group_Part` text,
  `Having_Part` text,
  `Order_Part` text,
  `Is_Workflow` int(11) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Composite_Query`
--




--
-- Table structure for table `Contact`
--

DROP TABLE IF EXISTS `Contact`;
CREATE TABLE `Contact` (
  `Id` bigint(20) NOT NULL auto_increment,
  `First` text,
  `Last` text,
  `Prefix` text,
  `Phone` text,
  `Mobile` text,
  `Email` text,
  `Fax` text,
  `Home_Phone` text,
  `Company` bigint(20) default NULL,
  `Password` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Invoice`
--

DROP TABLE IF EXISTS `Invoice`;
CREATE TABLE `Invoice` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Company` bigint(20) default NULL,
  `Project` bigint(20) default NULL,
  `Contact` bigint(20) default NULL,
  `Notes` text,
  `Invoice_Date` date default NULL,
  `Invoice_Time` time default NULL,
  `Status` bigint(20) default NULL,
  `Paid_Date` date default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Invoice`
--

--
-- Table structure for table `Messages`
--

DROP TABLE IF EXISTS `Messages`;
CREATE TABLE `Messages` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Subject` text,
  `From_Id` bigint(20) default NULL,
  `To_Id` bigint(20) default NULL,
  `Importance` int(11) default NULL,
  `Content` text,
  `From_String` text,
  `To_String` text,
  `Company` bigint(20) default NULL,
  `Project` bigint(20) default NULL,
  `Task` bigint(20) default NULL,
  `Author` bigint(20) default NULL,
  `Created` datetime default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Notes`
--

DROP TABLE IF EXISTS `Notes`;
CREATE TABLE `Notes` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Company` bigint(20) default NULL,
  `Project` bigint(20) default NULL,
  `Task` bigint(20) default NULL,
  `Author` bigint(20) default NULL,
  `Created` datetime default NULL,
  `Status` bigint(20) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Project`
--

DROP TABLE IF EXISTS `Project`;
CREATE TABLE `Project` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Name` text,
  `Company` bigint(20) default NULL,
  `Estimated_Start` datetime default NULL,
  `Actual_Start` datetime default NULL,
  `Estimated_Time` int(11) default NULL,
  `Actual_Time` int(11) default NULL,
  `Phase` int(11) default NULL,
  `Parent_Project` bigint(20) default NULL,
  `Notes` text,
  `Status` bigint(20) default NULL,
  `Rate` double default NULL,
  `PO` text,
  `Estimate` double default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Report`
--

DROP TABLE IF EXISTS `Report`;
CREATE TABLE `Report` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Site_Id` bigint(20) default NULL,
  `Template_Id` bigint(20) default NULL,
  `Query_Id` bigint(20) default NULL,
  `Composite_Query` bigint(20) default NULL,
  `Name` text,
  `Media_Type` bigint(20) default NULL,
  `Sum_Columns` text,
  `Average_Columns` text,
  `Category` text,
  `Show_In_List` int(11) default NULL,
  `Formatting_Modulus` int(11) default NULL,
  `Items_Per_Page` int(11) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Report_Conditioning`
--

DROP TABLE IF EXISTS `Report_Conditioning`;
CREATE TABLE `Report_Conditioning` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Report_Id` bigint(20) default NULL,
  `Col` int(11) default NULL,
  `Sort_Pos` int(11) default NULL,
  `Col_Handler` text,
  `Direction` text,
  `Format` text,
  `Tuple_Section` text,
  `spawnedReport` bigint(20) default NULL,
  `spawnedTuplePretext` text,
  `spawnedTuplePosttext` text,
  `spawningEvent` int(11) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Report_Parameters`
--

DROP TABLE IF EXISTS `Report_Parameters`;
CREATE TABLE `Report_Parameters` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Report_Id` bigint(20) default NULL,
  `Display_Label` text,
  `Data_Element` text,
  `Tag_Name` text,
  `Choices_Query` text,
  `Parameter_Type` text,
  `List_Order` int(11) default NULL,
  `Pre_Content` text,
  `Post_Content` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;



--
-- Table structure for table `Report_Templates`
--

DROP TABLE IF EXISTS `Report_Templates`;
CREATE TABLE `Report_Templates` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Site_Id` bigint(20) default NULL,
  `Name` text,
  `Path` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Report_Type`
--

DROP TABLE IF EXISTS `Report_Type`;
CREATE TABLE `Report_Type` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Name` text,
  `File_Extension` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Scheme_Field`
--

DROP TABLE IF EXISTS `Scheme_Field`;
CREATE TABLE `Scheme_Field` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Scheme_Table` bigint(20) default NULL,
  `Name` text,
  `Col_Type` text,
  `Links_To` bigint(20) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Scheme_Table`
--

DROP TABLE IF EXISTS `Scheme_Table`;
CREATE TABLE `Scheme_Table` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Name` text,
  `Abbr` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Status`
--

DROP TABLE IF EXISTS `Status`;
CREATE TABLE `Status` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Name` text,
  `Class` text,
  `Sequence` int(11) default NULL,
  `Prog_Enum` int(11) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Status`
--


/*!40000 ALTER TABLE `Status` DISABLE KEYS */;
LOCK TABLES `Status` WRITE;
INSERT INTO `Status` VALUES (1,'Awaiting Approval','Project',0,0),(2,'High Level Design','Project',1,0),(3,'Planned','Task',1,10),(4,'In Progress','Task',2,20),(5,'Complete','Task',2,30),(6,'Developing','Project',3,0),(7,'Invoiced','Task',3,40),(8,'Pre Invoice','Time',0,10),(9,'Invoiced','Time',1,20),(10,'Paid','Time',2,30),(11,'Prospect','Company',0,0),(12,'Active','Company',1,0),(13,'Paid','Task',5,50),(14,'Estimate','Task',0,0),(15,'Former','Company',0,0),(16,'Complete','Project',2000,0),(17,'No Charge','Task',6,60),(18,'No Charge','Time',3,40),(19,'Need to Estimate','Task',-1,0),(20,'Archived','Project',3000,3000);
UNLOCK TABLES;
/*!40000 ALTER TABLE `Status` ENABLE KEYS */;

--
-- Table structure for table `Task`
--

DROP TABLE IF EXISTS `Task`;
CREATE TABLE `Task` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Project` bigint(20) default NULL,
  `Name` text,
  `Description` text,
  `Start` datetime default NULL,
  `Task_Order` int(11) default NULL,
  `Hierarchy` int(11) default NULL,
  `Status` bigint(20) default NULL,
  `Percent_Complete` int(11) default NULL,
  `Parent_Task` bigint(20) default NULL,
  `Estimate` double default NULL,
  `Estimated_Time` double default NULL,
  `Actual_Time` double default NULL,
  `Requestor` bigint(20) default NULL,
  `Requested` date default NULL,
  `Estimated_End` datetime default NULL,
  `Actual_End` date default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `Task_Assignment`
--

DROP TABLE IF EXISTS `Task_Assignment`;
CREATE TABLE `Task_Assignment` (
  `Id` bigint(20) NOT NULL auto_increment,
  `User` bigint(20) default NULL,
  `Task` bigint(20) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Task_Assignment`
--


--
-- Table structure for table `Time`
--

DROP TABLE IF EXISTS `Time`;
CREATE TABLE `Time` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Task` bigint(20) default NULL,
  `Start` datetime default NULL,
  `Stop` datetime default NULL,
  `Description` text,
  `Hours` double default NULL,
  `Status` bigint(20) default NULL,
  `Invoice` bigint(20) default NULL,
  `Performed_By` bigint(20) default NULL,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;



--
-- Table structure for table `Work`
--

DROP TABLE IF EXISTS `Work`;
CREATE TABLE `Work` (
  `Id` bigint(20) NOT NULL auto_increment,
  `Name` text,
  `Description` text,
  `Query` bigint(20) default NULL,
  `User` bigint(20) default NULL,
  `User_Group` bigint(20) default NULL,
  `User_Role` bigint(20) default NULL,
  `URL` text,
  `Search` text,
  PRIMARY KEY  (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `Work`
--


/*!40000 ALTER TABLE `Work` DISABLE KEYS */;
LOCK TABLES `Work` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `Work` ENABLE KEYS */;

--
-- Table structure for table `role_services`
--

DROP TABLE IF EXISTS `role_services`;
CREATE TABLE `role_services` (
  `id` bigint(20) NOT NULL auto_increment,
  `role_id` bigint(20) default NULL,
  `service_id` bigint(20) default NULL,
  `enabled` int(11) default NULL,
  `xml_params` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `role_services`
--


/*!40000 ALTER TABLE `role_services` DISABLE KEYS */;
LOCK TABLES `role_services` WRITE;
INSERT INTO `role_services` VALUES (1,1,1,1,''),(2,1,2,1,''),(3,1,3,1,''),(4,1,4,1,''),(5,1,5,1,''),(6,1,6,1,'<Task_Order>edit</Task_Order>\n<Name>edit</Name>\n<Status>edit</Status>\n<Percent_Complete>edit</Percent_Complete>\n<Detail>show</Detail>\n<Time>enter</Time>\n\n'),(7,1,7,1,''),(8,1,8,1,''),(9,1,9,1,''),(10,1,10,1,''),(11,1,11,1,''),(12,1,12,1,''),(13,2,6,0,'<Task_Order>edit</Task_Order>\r\n<Name>edit</Name>\r\n<Status>edit</Status>\r\n<Percent_Complete>edit</Percent_Complete>\r\n<Detail>show</Detail>\r\n');
UNLOCK TABLES;
/*!40000 ALTER TABLE `role_services` ENABLE KEYS */;

--
-- Table structure for table `roles`
--

DROP TABLE IF EXISTS `roles`;
CREATE TABLE `roles` (
  `id` bigint(20) NOT NULL auto_increment,
  `name` text,
  `comment` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `roles`
--


/*!40000 ALTER TABLE `roles` DISABLE KEYS */;
LOCK TABLES `roles` WRITE;
INSERT INTO `roles` VALUES (1,'Developer','bootstrapped default developer role'),(2,'Executive','for administering the system');
UNLOCK TABLES;
/*!40000 ALTER TABLE `roles` ENABLE KEYS */;

--
-- Table structure for table `services`
--

DROP TABLE IF EXISTS `services`;
CREATE TABLE `services` (
  `id` bigint(20) NOT NULL auto_increment,
  `sequence` int(11) default NULL,
  `tree_depth` int(11) default NULL,
  `enabled` int(11) default NULL,
  `cat_name` text,
  `menu_name` text,
  `uri` text,
  `target` text,
  `prog_enum` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `services`
--


/*!40000 ALTER TABLE `services` DISABLE KEYS */;
LOCK TABLES `services` WRITE;
INSERT INTO `services` VALUES (1,1,1,1,'Service Authorization','Services','services_liteUI.meta?init=1','_self',NULL),(2,2,1,1,'Service Authorization','Roles','rolesUI.meta?init=1','_self',NULL),(3,10,1,1,'Project Management','Companies','CompanyUI.meta?init=1','',NULL),(4,11,3,1,'Project Management','Contacts','ContactUI.meta?init=1','',NULL),(5,12,1,1,'Project Management','Projects','ProjectUI.meta?init=1','',NULL),(6,13,3,1,'Project Management','Tasks','TaskUI.meta?init=1','',500),(7,201,1,1,'Reports','Design','ReportUI.meta','',NULL),(8,200,1,1,'Reports','Run','ReportListing.meta','',NULL),(9,202,4,1,'Reports','Tables','Scheme_TableUI.meta','',NULL),(10,203,4,1,'Reports','Fields','Scheme_FieldUI.meta','',NULL),(11,14,1,1,'Project Management','Invoices','InvoiceUI.meta?init=1','',NULL),(12,2,1,1,'Service Authorization','Users','ls_usersUI.meta?init=1','',NULL);
UNLOCK TABLES;
/*!40000 ALTER TABLE `services` ENABLE KEYS */;

--
-- Table structure for table `user_roles`
--

DROP TABLE IF EXISTS `user_roles`;
CREATE TABLE `user_roles` (
  `id` bigint(20) NOT NULL auto_increment,
  `user_id` bigint(20) default NULL,
  `role_id` bigint(20) default NULL,
  `enabled` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `user_roles`
--


/*!40000 ALTER TABLE `user_roles` DISABLE KEYS */;
LOCK TABLES `user_roles` WRITE;
INSERT INTO `user_roles` VALUES (4,1,1,1),(3,2,1,1);
UNLOCK TABLES;
/*!40000 ALTER TABLE `user_roles` ENABLE KEYS */;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
CREATE TABLE `users` (
  `id` bigint(20) NOT NULL auto_increment,
  `last` text,
  `first` text,
  `login` text,
  `password` text,
  `email` text,
  `phone` text,
  `mobile` text,
  `fax` text,
  `pager` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--


/*!40000 ALTER TABLE `users` DISABLE KEYS */;
LOCK TABLES `users` WRITE;
INSERT INTO `users` VALUES (1,'McCombs','David','davidmc','freedom7','davidmc@w3sys.com','706 581-4607','706 581-4607',NULL,''),(2,'Developer','George','george','develops','gearge@w3sys.com','','','',NULL);
UNLOCK TABLES;
/*!40000 ALTER TABLE `users` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

