
function YesNo( Prompt, URL ) {
   yes = confirm( Prompt )
   if ( yes == true ) window.location=URL;
}

function OpenNewWin( URL, Width, Height, Mode ) {
   if ( Mode == null ) {
      window.open( URL, 'NewWindow', 'resizable=0,scrollbars=0,width='+Width+',height='+Height);
   } else {
      window.open( URL, 'NewWindow', 'resizable=1,scrollbars=1,width='+Width+',height='+Height);
   }
}

// ���͸������s�H�Χֳt�������
function MakeJumpButton( TotalPages, NowPage, URLStr, PageParm, FormName, Width ) {

   LinkStr = URLStr + "&" + PageParm + "=";

   Pno  = new Array( 1, NowPage - 1, NowPage + 1, TotalPages );
   PStr = new Array( "�Ĥ@��", "�e��", "�᭶", "�̫᭶" );

   if ( NowPage == TotalPages ) {
      Pno[2] = 0;
      Pno[3] = 0;
    }
   if ( NowPage == 1 ) { Pno[0] = 0 ; }

   if ( Width == null ) { Width = "100%" ;}

   document.write( "<table border=0 width=" + Width + ">" );
   document.write( "<form name=" + FormName + " action=" + URLStr + " method=POST>" )
   document.write( "<tr bgcolor=f0f0f8>");
   document.write( "<td align=left valign=middle>");

   for ( i = 0 ; i < 4 ; i ++ ) {

      if ( Pno[i] != 0 ) {
         document.write( "�@<input style='font-size:10pt' type=button value='" + PStr[i] + "' ");
         document.write( "onClick=\"window.location='" );
         document.write( LinkStr );
         document.write( Pno[i] );
         document.write( "'\">" );
       } else {
         document.write( "<font color=AAAAAA>�@[" + PStr[i] + "]�@</font>" );
       }

   }
   document.write( "</td><td align=right>" )

   document.write( "�@�e��&nbsp;" )
   document.write( "<select name=" )
   document.write( PageParm + " " )
   document.write( "onchange=document." + FormName + ".submit()>")

   for ( i = 1 ; i <= TotalPages ; i++ ) {

       if ( i != NowPage ) {
          document.write( "<option value=" + i + ">�� " + i + " ��</option>" )
       } else {
          document.write( "<option selected value=" + i + ">�� " + i + " ��</option>" )
       }
   }

   document.write( "</select>")
   document.write( "�A�@&nbsp;" + TotalPages + "&nbsp��</td></tr></form></table>" )

}


function PreView( Text ) {

    var Len = Text.length ;
    var CNT = Len;
    var ch = "";
    var NewText = "";

    msg=open("","PreView","status=1,scrollbars=1,resizable=0,width=640,height=400" );
    msg.document.write( "<html><head><title>�w���K�X�᪺�ˤl</title></head>");
    msg.document.write( "<body>");
    msg.document.write( "<center>");
    msg.document.write( "<table border=0 width=600><tr bgcolor=#F0F0F8><td>");
    msg.document.write( "<br><p style='letter-spacing:2px; line-height:16pt'><font size=3 color=000090>" );
    for ( i=0 ; i < Len ; i++ ) {
        ch = Text.substring(i,i+1) ;
        if ( ch == "\n" ) {
           NewText += "&nbsp;<br>" ;
        } else {
           NewText += ch ;
        }
        if ( ch < " " ) CNT --;
    }
    msg.document.write( NewText + "</font></p>");
    msg.document.write( "<p align=center>�`�r��(�t�^�Ʀr)�`�@&nbsp;" + CNT + "&nbsp;�Ӧr</p>" );
    msg.document.write( "</td></tr></table>" );
    msg.document.write( "<form><input type=button value='�����w������' style='font-size:12pt' onclick='window.close()'></form>");
    msg.document.write( "</center>");
    msg.document.write( "</body></html>");
    msg.document.close();
}

function DisplaySpecial() {

    SP=new Array("�B","�C","�K","�i","�j","�m ","�n","�q","�r","�u","�v","�y","�z","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","�D","�F","�G","�J","�K","�P","�Q","�R","�T","�U","�V","�W");
    document.write( "�S��Ÿ���J�G<br><font style='font-size:10pt' face='�s�ө���'>" );
    l = 1 ;
    for ( i=0 ; i < SP.length ; i++ ) {
       document.write( "<a style='text-decoration:none;link:000000' href=\"JavaScript:AddCh('" + SP[i] + "')\">" + SP[i] + "</a>&nbsp;" );
       l ++;
       if ( l > 35 ) {
          document.write("<br>");
          l = 1 ;
       }
    }
    document.write( "</font>");
}

function AddCh(str) {
  document.form.Content.value+=str;
  document.form.Content.focus();
}
