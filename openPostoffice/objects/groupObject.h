 #ifndef  mail_group_Object_h
 #define  mail_group_Object_h

 class mail_groupObject: public objectBase
 {
 public:
   mail_groupObject(openLogin &lin, cgiScript & script):objectBase(lin,script)
   {

     // add fields
     addField( "id", pgLongOid );
     addField( "group_id", pgLongOid );
     addField( "name", pgVarCharArrayOid );
     addField( "description", pgVarCharArrayOid );

     addField( "created", pgTimestampOid );
     addField( "created_by", pgLongOid );
     addField( "modified", pgTimestampOid );
     addField( "modified_by", pgLongOid );

     // add controls
     addControl( "id", staticEntry( "id", "10" ) );

     if( lin.SiteAdmin() )
     {
       addControl( "group_id", cmboEntry( "group_id",
                                          "select id, name from groups order by name",
                                          "Select a group: " ) );
     }
     else
     {
       addControl( "group_id", hiddenEntry( "group_id" ) );
     }
     addControl( "name", textEntry( "name", "32" ) );
     addControl( "description", areaEntry( "description" ) );

     addControl( "created", staticEntry( "created", "24" ) );
     addControl( "modified", staticEntry( "modified", "24" ) );

     // set name
     setName("mail_groups");
   }
   // must have no user roles on user delete
   bool validate(void){ return true; }

   // will use cleanup to add, delete user account, or change account password
   bool cleanup(void) { return true; }
 };
#endif
