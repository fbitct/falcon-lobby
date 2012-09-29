<?
require_once("header.php");
?>

<img src="images/pilot.jpg">
<a href="javascript:navTo('home.php')"><h3>HOME</h3></a>
<a href="javascript:navTo('download.php')"><h3>DOWNLOAD</h3></a>
<a href="javascript:navTo('games.php')"><h3>ACTIVE GAMES</h3></a>
<!-- <a href="javascript:navTo('chat/index.php')"><h3>CHAT</h3></a> -->
<a href="javascript:navTo('links.php')"><h3>LINKS</h3></a>

<script language="javascript" type="text/javascript">
<!--
   function navTo(dest)   
   {
      parent.content.document.location.href = dest;
   }
-->
</script>

<?
require_once("footer.php");
?>
