/*!40000 ALTER TABLE `Work` ENABLE KEYS */;

--
-- Table structure for table `global_content`
--

DROP TABLE IF EXISTS `global_content`;
CREATE TABLE `global_content` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `name` text NOT NULL,
  `content` text NOT NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `global_content`
--


--
-- Table structure for table `group_roles`
--

DROP TABLE IF EXISTS `group_roles`;
CREATE TABLE `group_roles` (
  `id` bigint(20) NOT NULL auto_increment,
  `role_id` bigint(20) NOT NULL default '0',
  `group_id` bigint(20) NOT NULL default '0',
  `enabled` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `group_roles`
--


/*!40000 ALTER TABLE `group_roles` DISABLE KEYS */;
LOCK TABLES `group_roles` WRITE;
INSERT INTO `group_roles` VALUES (40,4,3,1),(39,3,3,1),(36,2,2,1),(55,5,1,1),(2,2,2,NULL),(3,2,3,NULL),(4,2,4,NULL),(5,2,5,NULL),(54,3,1,1),(9,2,6,1),(26,3,7,1),(25,2,7,1),(53,2,1,1),(52,1,1,1),(23,2,9,1),(24,2,10,1),(27,5,7,1),(31,2,11,1),(30,1,11,1);
UNLOCK TABLES;
/*!40000 ALTER TABLE `group_roles` ENABLE KEYS */;

--
-- Table structure for table `group_services`
--

DROP TABLE IF EXISTS `group_services`;
CREATE TABLE `group_services` (
  `id` bigint(20) NOT NULL auto_increment,
  `service_id` bigint(20) NOT NULL default '0',
  `group_id` bigint(20) NOT NULL default '0',
  `sequence` smallint(6) NOT NULL default '0',
  `xml_param1` text,
  `xml_param2` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `group_services`
--


/*!40000 ALTER TABLE `group_services` DISABLE KEYS */;
LOCK TABLES `group_services` WRITE;
INSERT INTO `group_services` VALUES (1,1,1,1,NULL,NULL),(2,2,1,2,NULL,NULL),(3,3,1,3,NULL,NULL),(4,4,1,4,NULL,NULL),(5,5,1,5,NULL,NULL),(6,6,1,6,NULL,NULL),(7,7,1,7,NULL,NULL),(8,9,1,100,'',''),(9,8,1,101,'',''),(10,10,1,102,'',''),(11,8,2,1,'',''),(12,10,2,2,'',''),(43,14,7,0,'',''),(14,8,3,2,'',''),(15,10,3,3,'',''),(16,8,4,0,'',''),(17,2,4,0,'',''),(18,10,4,3,'',''),(19,8,5,1,'',''),(20,10,5,2,'',''),(33,14,5,2,'',''),(22,11,1,201,'',''),(23,12,1,202,'',''),(47,21,7,0,'',''),(25,14,1,204,'',''),(26,15,1,205,'',''),(46,19,7,0,'',''),(28,14,4,2,'',''),(29,15,4,3,'',''),(44,15,7,0,'',''),(31,14,2,1,'',''),(32,15,2,2,'',''),(34,15,5,3,'',''),(45,17,7,0,'',''),(36,14,3,2,'',''),(37,15,3,3,'',''),(87,29,7,0,'',''),(48,18,7,0,'',''),(40,14,6,1,'',''),(41,15,6,2,'',''),(49,20,7,0,'',''),(50,17,1,0,'',''),(51,19,1,0,'',''),(52,21,1,0,'',''),(53,18,1,0,'',''),(54,20,1,0,'',''),(55,2,6,0,'',''),(57,23,7,0,'',''),(58,23,1,0,'',''),(59,22,1,0,'',''),(60,25,1,0,'',''),(61,24,1,0,'',''),(63,14,8,0,'',''),(64,15,8,0,'',''),(109,54,1,0,'',''),(68,14,9,0,'',''),(69,15,9,0,'',''),(70,17,8,0,'',''),(71,19,8,0,'',''),(72,21,8,0,'',''),(73,22,8,0,'',''),(74,23,8,0,'',''),(75,25,8,0,'',''),(76,24,8,0,'',''),(77,18,8,0,'',''),(78,20,8,0,'',''),(79,14,10,0,'',''),(80,15,10,0,'',''),(81,11,4,0,'',''),(82,29,1,0,'',''),(83,28,1,0,'',''),(84,30,1,0,'',''),(85,31,1,0,'',''),(86,32,1,0,'',''),(88,28,7,0,'',''),(89,30,7,0,'',''),(90,31,7,0,'',''),(91,32,7,0,'',''),(92,33,1,0,'',''),(93,34,1,0,'',''),(94,35,1,0,'',''),(95,36,1,0,'',''),(96,34,7,0,'',''),(97,37,1,0,'',''),(98,12,11,0,'',''),(99,14,11,0,'',''),(100,15,11,0,'',''),(101,12,11,0,'',''),(102,11,11,0,'',''),(103,38,7,0,'',''),(104,38,1,0,'',''),(105,39,7,0,'',''),(106,39,1,0,'',''),(107,41,1,0,'',''),(108,53,1,0,'',''),(110,55,1,0,'',''),(111,56,1,0,'',''),(112,57,1,0,'','');
UNLOCK TABLES;
/*!40000 ALTER TABLE `group_services` ENABLE KEYS */;

--
-- Table structure for table `group_sites`
--

DROP TABLE IF EXISTS `group_sites`;
CREATE TABLE `group_sites` (
  `id` bigint(20) NOT NULL auto_increment,
  `group_id` bigint(20) NOT NULL default '0',
  `site_id` bigint(20) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `group_sites`
--


/*!40000 ALTER TABLE `group_sites` DISABLE KEYS */;
LOCK TABLES `group_sites` WRITE;
INSERT INTO `group_sites` VALUES (1,1,1),(2,1,2),(3,1,3),(4,1,5),(5,5,6),(6,1,8),(8,1,13),(9,1,9),(10,8,10),(11,1,26),(12,1,11),(13,1,10),(14,1,18),(15,1,14),(17,1,16),(18,1,17);
UNLOCK TABLES;
/*!40000 ALTER TABLE `group_sites` ENABLE KEYS */;

--
-- Table structure for table `groups`
--

DROP TABLE IF EXISTS `groups`;
CREATE TABLE `groups` (
  `id` bigint(20) NOT NULL auto_increment,
  `name` text,
  `site_admin` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `groups`
--


/*!40000 ALTER TABLE `groups` DISABLE KEYS */;
LOCK TABLES `groups` WRITE;
INSERT INTO `groups` VALUES (1,'Site Editor',1),(2,'Non-Administrative User',0);
UNLOCK TABLES;
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;


DROP TABLE IF EXISTS `links`;
CREATE TABLE `links` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `name` text,
  `url` text,
  `target` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;


--
-- Table structure for table `menu_items`
--

DROP TABLE IF EXISTS `menu_items`;
CREATE TABLE `menu_items` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `menu_id` bigint(20) NOT NULL default '0',
  `link_id` bigint(20) NOT NULL default '0',
  `template_tag` char(128) NOT NULL default '',
  `locate_tag` char(128) NOT NULL default '',
  `target_tag` char(128) NOT NULL default '',
  `label_tag` char(128) NOT NULL default '',
  `place_order` int(11) NOT NULL default '0',
  `place_level` int(11) NOT NULL default '0',
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `menu_items`
--


--
-- Table structure for table `menus`
--

DROP TABLE IF EXISTS `menus`;
CREATE TABLE `menus` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `name` text,
  `template_open_tag` text,
  `template_close_tag` text,
  `reference_template` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `menus`
--


--
-- Table structure for table `newsletter`
--

DROP TABLE IF EXISTS `newsletter`;
CREATE TABLE `newsletter` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `page_id` bigint(20) NOT NULL default '0',
  `address_query` text NOT NULL,
  `name` text,
  `sender_email` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `newsletter`
--


--
-- Table structure for table `page_menus`
--

DROP TABLE IF EXISTS `page_menus`;
CREATE TABLE `page_menus` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `menu_id` bigint(20) NOT NULL default '0',
  `page_id` bigint(20) NOT NULL default '0',
  `place_order` int(11) default NULL,
  `collapsible` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `page_menus`
--



--
-- Table structure for table `pages`
--

DROP TABLE IF EXISTS `pages`;
CREATE TABLE `pages` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `template_id` bigint(20) NOT NULL default '0',
  `name` text NOT NULL,
  `url` text,
  `meta_description` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;



--
-- Table structure for table `paragraphs`
--

DROP TABLE IF EXISTS `paragraphs`;
CREATE TABLE `paragraphs` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `page_id` bigint(20) NOT NULL default '0',
  `place_order` int(11) NOT NULL default '0',
  `template_tag` varchar(128) NOT NULL default '',
  `replace_tag` varchar(128) NOT NULL default '',
  `name` text,
  `content` text,
  `author` bigint(20) default NULL,
  `time_authored` datetime default NULL,
  `time_start` datetime default NULL,
  `time_end` datetime default NULL,
  `approved` int(11) default NULL,
  `section` text,
  `edit_type` bigint(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;



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
INSERT INTO `role_services` VALUES (1,1,1,1,''),(2,1,2,1,''),(3,1,3,1,''),(4,1,4,1,''),(5,1,5,1,''),(6,1,6,1,''),(7,1,7,1,''),(8,1,8,1,''),(9,1,9,1,''),(10,1,10,1,''),(11,1,11,1,''),(12,1,12,1,''),(13,2,6,0,'<Task_Order>edit</Task_Order>\r\n<Name>edit</Name>\r\n<Status>edit</Status>\r\n<Percent_Complete>edit</Percent_Complete>\r\n<Detail>show</Detail>\r\n'),(16,1,15,1,''),(14,1,13,1,''),(15,1,14,1,'<Task_Order>edit</Task_Order>\n<Name>edit</Name>\n<Status>edit</Status>\n<Percent_Complete>edit</Percent_Complete>\n<Detail>show</Detail>\n<Time>enter</Time>\n\n');
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
INSERT INTO `roles` VALUES (1,'Developer','bootstrapped default developer role'),(2,'Executive','for administering the system'),(3,'Registrar','Please leave this role alone, no servces, to determine who sends and recieves registration mail.');
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
INSERT INTO `services` VALUES (1,1,1,1,'Service Authorization','Services','services_liteUI.meta?init=1','_self',55),(2,2,1,1,'Service Authorization','Roles','rolesUI.meta?init=1','_self',65),(3,10,1,1,'Project Management','Companies','CompanyUI.meta?init=1','',110),(4,11,3,1,'Project Management','Contacts','ContactUI.meta?init=1','',120),(5,12,1,1,'Project Management','Projects','ProjectUI.meta?init=1','',130),(6,15,2,1,'Invoicing','Hosting','HostingUI.meta?init=1','',442),(7,201,1,1,'Reports','Design','ReportUI.meta','',200),(8,200,1,1,'Reports','Run','ReportListing.meta','',201),(9,202,4,1,'Reports','Tables','Scheme_TableUI.meta','',202),(10,203,4,1,'Reports','Fields','Scheme_FieldUI.meta','',203),(11,14,2,1,'Invoicing','Invoices','InvoiceUI.meta?init=1','',99),(12,2,1,1,'Service Authorization','Users','ls_usersUI.meta?init=1','',NULL),(13,3,1,1,'Service Authorization','Groups','groupsUI.meta?init=1','_self',66),(14,13,2,1,'Project Management','Tasks','TaskUI.meta?init=1','',500),(15,16,2,1,'Invoicing','Charge','ChargeUI.meta?init=1','',501);
UNLOCK TABLES;
/*!40000 ALTER TABLE `services` ENABLE KEYS */;

--
-- Table structure for table `sites`
--

DROP TABLE IF EXISTS `sites`;
CREATE TABLE `sites` (
  `id` bigint(20) NOT NULL auto_increment,
  `name` text NOT NULL,
  `domain` text NOT NULL,
  `url` text,
  `path` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `sites`
--


/*!40000 ALTER TABLE `sites` DISABLE KEYS */;
LOCK TABLES `sites` WRITE;
INSERT INTO `sites` VALUES (2,'W3 Systems Dev','devlinux','/','/var/www/html/testing'),(23,'Projects','devlinux','/projects/','/var/www/html/testing/projects');
UNLOCK TABLES;
/*!40000 ALTER TABLE `sites` ENABLE KEYS */;

--
-- Table structure for table `templates`
--

DROP TABLE IF EXISTS `templates`;
CREATE TABLE `templates` (
  `id` bigint(20) NOT NULL auto_increment,
  `site_id` bigint(20) NOT NULL default '0',
  `name` text,
  `path` text,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `templates`
--


/*!40000 ALTER TABLE `templates` DISABLE KEYS */;
LOCK TABLES `templates` WRITE;
INSERT INTO `templates` VALUES (7,2,'Main Page','main.htmp'),(72,23,'Meta','meta.html'),(176,23,'Extra','extra.html'),(178,2,'Contact Us','contactus.htmp');
UNLOCK TABLES;
/*!40000 ALTER TABLE `templates` ENABLE KEYS */;

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
INSERT INTO `user_roles` VALUES (19,1,3,1),(18,1,1,1),(17,2,1,1);
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
  `xml_preferences` text,
  `group_id` bigint(20) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `users`
--


/*!40000 ALTER TABLE `users` DISABLE KEYS */;
LOCK TABLES `users` WRITE;
INSERT INTO `users` VALUES (1,'McCombs','David','davidmc','coldIron57','davidmc@w3sys.com','706 581-4607','706 581-4607',NULL,'',NULL,1);
UNLOCK TABLES;


