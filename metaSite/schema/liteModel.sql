-- MySQL dump 10.9
--
-- Host: localhost    Database: junkman
-- ------------------------------------------------------
-- Server version	4.1.20

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `edit_types`
--

DROP TABLE IF EXISTS `edit_types`;
CREATE TABLE `edit_types` (
  `id` bigint(20) NOT NULL auto_increment,
  `name` text,
  `handler` text,
  `sequence` int(11) default NULL,
  PRIMARY KEY  (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

--
-- Dumping data for table `edit_types`
--


/*!40000 ALTER TABLE `edit_types` DISABLE KEYS */;
LOCK TABLES `edit_types` WRITE;
INSERT INTO `edit_types` VALUES (1,'Standard Rich Edit','<SCRIPT type=\"text/javascript\"> \ndoRichEdit = true;\n</script>',1),(2,'A/V Media','<label style=\"font-size:8pt\">File: <input style=\"font-size:8pt\" type=\'text\' name=\'mediaFile\' id=\'mediaFile\' onchange=\"setPlay()\"><a href=\"javaScript:placeFile(\'mediaFile\',\'media\')\">Pick</a></label>\n<label style=\"font-size:8pt\">Width: <input style=\"font-size:8pt\"type=\'text\' size=\"4\" name=\'mediaWidth\' id=\'mediaWidth\' onchange=\"setPlay()\"></label>\n<label style=\"font-size:8pt\">Height: <input style=\"font-size:8pt\" type=\'text\' size=\"4\" name=\'mediaHeight\' id=\'mediaHeight\' onchange=\"setPlay()\"></label>\n<script type=\"text/javascript\">\nfunction parseFunction() { viewMedia(); \nparsePlayer(); setPlay(); }</script>\n<br style=\"clearall\"> \n<br style=\"clearall\">\n',2),(3,'Plain Text Entry','',3);
UNLOCK TABLES;
/*!40000 ALTER TABLE `edit_types` ENABLE KEYS */;

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


/*!40000 ALTER TABLE `global_content` DISABLE KEYS */;
LOCK TABLES `global_content` WRITE;
INSERT INTO `global_content` VALUES (1,2,'top','<h2>All the stuff you might ever want</h2>'),(2,2,'end','Copyright © 2009 - The Junk Man\'s Son<br>');
UNLOCK TABLES;
/*!40000 ALTER TABLE `global_content` ENABLE KEYS */;

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
INSERT INTO `groups` VALUES (1,'Administrator',1),(2,'User',0);
UNLOCK TABLES;
/*!40000 ALTER TABLE `groups` ENABLE KEYS */;

--
-- Table structure for table `links`
--

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
-- Dumping data for table `links`
--


/*!40000 ALTER TABLE `links` DISABLE KEYS */;
LOCK TABLES `links` WRITE;
INSERT INTO `links` VALUES (1,2,'Home','pages.meta',''),(2,2,'About Us','pages.meta/About',''),(3,2,'services','services',''),(4,2,'Contact Us','contactus.meta/Contact','');
UNLOCK TABLES;
/*!40000 ALTER TABLE `links` ENABLE KEYS */;

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


/*!40000 ALTER TABLE `menu_items` DISABLE KEYS */;
LOCK TABLES `menu_items` WRITE;
INSERT INTO `menu_items` VALUES (1,2,1,1,'menuitem','$location','_self','$label',1,1),(2,2,1,2,'menuitem','$location','_self','$label',2,1),(3,2,1,4,'menuitem','$location','_self','$label',3,1);
UNLOCK TABLES;
/*!40000 ALTER TABLE `menu_items` ENABLE KEYS */;

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


/*!40000 ALTER TABLE `menus` DISABLE KEYS */;
LOCK TABLES `menus` WRITE;
INSERT INTO `menus` VALUES (1,2,'lefty','','','');
UNLOCK TABLES;
/*!40000 ALTER TABLE `menus` ENABLE KEYS */;

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


/*!40000 ALTER TABLE `newsletter` DISABLE KEYS */;
LOCK TABLES `newsletter` WRITE;
UNLOCK TABLES;
/*!40000 ALTER TABLE `newsletter` ENABLE KEYS */;

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


/*!40000 ALTER TABLE `page_menus` DISABLE KEYS */;
LOCK TABLES `page_menus` WRITE;
INSERT INTO `page_menus` VALUES (1,2,1,1,0,0),(2,2,1,2,0,0),(3,2,1,3,0,0),(4,2,1,5,0,0);
UNLOCK TABLES;
/*!40000 ALTER TABLE `page_menus` ENABLE KEYS */;

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
-- Dumping data for table `pages`
--


/*!40000 ALTER TABLE `pages` DISABLE KEYS */;
LOCK TABLES `pages` WRITE;
INSERT INTO `pages` VALUES (1,2,7,'Home','pages.meta',''),(2,2,7,'About Us','pages.meta/About',''),(3,2,179,'services','services',''),(5,2,178,'Contact Us','contactus.meta/Contact','');
UNLOCK TABLES;
/*!40000 ALTER TABLE `pages` ENABLE KEYS */;

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
-- Dumping data for table `paragraphs`
--


/*!40000 ALTER TABLE `paragraphs` DISABLE KEYS */;
LOCK TABLES `paragraphs` WRITE;
INSERT INTO `paragraphs` VALUES (1,2,1,1,'middle','$content','This is a test ','<h1><span style=\"font-weight: bold; color: rgb(51, 51, 0);\">The Junkman\'s Son</span></h1><span style=\"color: rgb(51, 51, 0);\"></span>',1,'2009-10-28 20:48:00','2009-10-28 20:48:00','2009-11-28 19:48:00',1,'content',1),(2,2,2,1,'middle','$content','About the Junkman\'s Son ','<h2>About the Junkman\'s Son</h2>',1,'2009-10-31 20:18:00','2009-10-31 20:18:00','2009-12-01 19:18:00',1,'content',1),(3,2,1,2,'middle','$content','All the stuff you never knoew ','All the stuff you never knoew about that you really need.<br>',1,'2009-10-31 20:19:00','2009-10-31 20:19:00','2009-12-01 19:19:00',1,'content',1),(4,2,5,1,'middle','$content','Contact Us ','<h1>Contact Us</h1>',1,'2009-11-01 10:58:00','2009-11-01 10:58:00','2009-12-01 10:58:00',1,'content',1),(7,2,5,1,'mailContent','$content','Form','<table style=\"font-size: 8pt;\" border=\"0\" cellpadding=\"5\" cellspacing=\"0\" width=\"413\">\n <tbody>\n   <tr>\n    <td align=\"left\" width=\"98\">\n        <b>Name:</b><span style=\"color: rgb(51, 51, 153);\">*</span></td>\n    <td width=\"291\"><input id=\"mi1_Name\" name=\"mi1_Name\" size=\"35\" type=\"text\"></td>\n   </tr>            \n    <tr>\n     <td align=\"left\" width=\"98\">\n         <b>Email:</b><span style=\"color: rgb(51, 51, 153);\">*</span></td>\n     <td width=\"291\"><input id=\"mi2_EMail\" name=\"mi2_EMail\" size=\"35\" type=\"text\"></td>\n    </tr>\n    <tr>\n     <td align=\"left\" width=\"98\">\n         <b>Verify Email:</b><span style=\"color: rgb(51, 51, 153);\">*</span></td>\n     <td width=\"291\"><input id=\"ver_EMail\" name=\"ver_EMail\" size=\"35\" type=\"text\"></td>\n    </tr>\n    <tr>\n      <td align=\"left\" width=\"98\">\n         <b>Address:</b></td>\n      <td width=\"291\"><input name=\"mi3_Address\" size=\"35\" type=\"text\"></td>\n    </tr>\n    <tr>\n      <td align=\"left\" width=\"98\">\n           <b>City/ State/ Zip:</b></td>\n       <td width=\"291\"><input name=\"mi4_City\" size=\"15\" type=\"text\">\n                       <input name=\"mi5_State\" size=\"3\" type=\"text\"> \n                       <input name=\"mi6_Zip\" size=\"11\" type=\"text\">\n       </td>\n    </tr>\n    <tr>\n      <td align=\"left\" width=\"98\">\n          <b>Phone:</b></td>\n      <td width=\"291\"><input name=\"mi7_Phone\" size=\"35\" type=\"text\"></td>\n    </tr>\n    <tr>\n      <td colspan=\"2\" align=\"left\">\n         <b>Where did you hear about us?</b> \n         <select name=\"mi8_Heard about us from\" size=\"1\" tabindex=\"41\"><option value=\"\" selected=\"selected\">-select from below-</option><option>Yahoo</option><option>Google</option><option>MSN</option><option>AltaVista</option><option>All the Web</option><option>Lycos</option><option>Other Search Engine</option><option>Other Web Site</option><option>Contractor</option><option>Trade Show</option><option>Direct Mail</option><option>A Friend</option></select>\n      </td>\n    </tr>\n    <tr>\n      <td align=\"left\" width=\"98\">\n         <b>Questions and/or<br>\n            Comments:</b></td>\n      <td width=\"291\"><textarea rows=\"5\" name=\"mi9_Comments\" cols=\"30\"></textarea></td></tr>\n\n<tr>\n  <td colspan=\"2\" align=\"center\" width=\"100%\">\n     <input value=\"Submit\" type=\"submit\"><input value=\"Reset\" type=\"reset\"></td>\n</tr>\n\n</tbody></table></center><div style=\"margin-left: 40px;\">\nRequired fields are marked with <span style=\"color: rgb(51, 51, 153);\">*</span>.<br></div>',1,'2009-11-01 11:48:00','2009-11-01 11:48:00','2009-12-01 11:48:00',1,'mail',1);
UNLOCK TABLES;
/*!40000 ALTER TABLE `paragraphs` ENABLE KEYS */;

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
INSERT INTO `roles` VALUES (1,'Manager','manages users, roles and services'),(2,'Editor','Can do web page edits'),(3,'Registrar','Please leave this role alone. It is to determine who sends and receives registration mail.');
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
INSERT INTO `services` VALUES (1,1,1,1,'Service Authorization','Services','services_liteUI.meta?init=1','_self',55),(2,2,1,1,'Service Authorization','Roles','rolesUI.meta?init=1','_self',65),(12,2,1,1,'Service Authorization','Users','ls_usersUI.meta?init=1','',NULL),(13,3,1,1,'Service Authorization','Groups','groupsUI.meta?init=1','_self',66);
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
INSERT INTO `sites` VALUES (2,'Home','junkman','/','/var/www/html/testing/junkmansson');
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
INSERT INTO `templates` VALUES (7,2,'Main Page','index.htmp'),(72,23,'Meta','meta.html'),(176,23,'Extra','extra.html'),(178,2,'Contact Us','contactus.htmp'),(179,2,'Extranet','extranet.htmp');
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
/*!40000 ALTER TABLE `users` ENABLE KEYS */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

