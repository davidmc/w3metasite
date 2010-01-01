insert into paragraphs (site_id,page_id,place_order,
                        template_tag,replace_tag,name,content)
values(1,2,1,'First Paragraph','$content','Main 1',
       'This is a test paragraph for the main page');
insert into paragraphs (site_id,page_id,place_order,
                        template_tag,replace_tag,name,content)
values(1,2,2,'Next Paragraph','$content','Main 2',
       'This is a test paragraph for the main page'); 
insert into paragraphs (site_id,page_id,place_order,
                        template_tag,replace_tag,name,content)
values(1,3,1,'First Paragraph','$content','Search text',
       '
    <FORM method="get" action="http://www.google.com/custom" id=form1 name=form1>
    <TABLE cellspacing="0" border="0">
    <tr valign=top><td>
    <td class="formbody">
    <INPUT name=q size=31 maxlength=255 ><br>     
    <input type=radio name=sitesearch value="" checked>Search WWW 
    <input type=radio name=sitesearch value="www.w3sys.com">Search Site 
    </td>    
    <td>
    <INPUT type=image src="http://www.google.com/logos/Logo_40wht.gif"                      
            name="sa" 
            alt="Search"
            VALUE="Google Search">
    <INPUT type=hidden 
            name=cof 
           VALUE="L:http://www.w3sys.com/logo_2.gif;AH:center;S:http://www.w3sys.com;AWFID:2946b5581fc4f1ce;">    
    <input type=hidden name=domains value="www.w3sys.com">
    </td></tr>
    </TABLE>
    </FORM> 
       ');              