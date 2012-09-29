<?php
//******************************************************************************
//* games.php - provides a display of active games for web browser clients
//******************************************************************************

$page_title = "active games";
require_once("header.php");

require_once("config.php");
require_once("db.php");

$dbr = db_getactivegames();
if ($dbr['err'])
{
   echo "ERROR: " . $dbr['err'];
}
else
{
   echo "<p align='center'><h1 align='center'>Active Games</h1></p>";

   echo "<p align='center'><a href='http://www.falconlobby.com' target='_blank'><h4 align='center'>New Version (v1.1, 08/04/2006) available</h4></a></p>";

   echo "<p align='center'><h4 align='center'>";

   if ($numgames = db_getnumgames())
   {
      echo $numgames . " registered games<br>";
   }

   echo "(page will reload every " . (GAMES_AUTO_REFRESH_MS/1000) . " secs)</h4></p>";

   echo "<p align='center'><table bgcolor='#363F48' border=1 cellpadding=4 cellspacing=0 width='95%'><tr bgcolor='#232634' valign='center' align='center'><td><b>Name</b></td><td><b>Description</b></td><td><b>Links</b></td><td><b>Theater</b></td><td><b>Uptime<br>(hh:mm:ss)</b></td></tr>";

   foreach ($dbr['activegames'] as $game)
   {
      $desc = str_replace("\n","<br>",$game['game_desc']);

      if (strlen($desc) == 0)
      {
         $desc = "<i>none</i>";
      }

      $game_homepage = $game['game_homepage'];
      $game_voicecomm = $game['game_voicecomm'];

      if (strlen($game_homepage) > 0)
      {
         $game_homepage = "<a href='" . $game['game_homepage'] . "' class='navlink' target='_blank'>Home Page</a>";
      }

      if (strlen($game_voicecomm) > 0)
      {
         $game_voicecomm = "<a href='" . $game['game_voicecomm'] . "' class='navlink'>Voice Comms</a>";
      }

      if ((strlen($game_homepage) == 0) &&
          (strlen($game_voicecomm) == 0))
      {
         $game_homepage = "<i>none</i>";
      }

      $tot = $game['game_uptime'];
      $hrs = floor($tot / 3600);
      $mins = floor(($tot % 3600) / 60);
      $secs = ($tot % 3600) % 60;

      $game_uptime = "";
      if ($hrs < 10) $game_uptime .= "0";
      $game_uptime .= $hrs . ":";
      if ($mins < 10) $game_uptime .= "0";
      $game_uptime .= $mins . ":";
      if ($secs < 10) $game_uptime .= "0";
      $game_uptime .= $secs;

      if (strlen($game['game_iploc']) > 0)
      {
         $game_iploc = "<br><h4>(Location: <b>" . $game['game_iploc'] . "</b>)</h4>";
      }
      else
      {
         $game_iploc = "";
      }

      $game_iploc = "";

      echo "<tr valign='top'><td><h4><b><font color='#FFFFFF'>" . $game['game_name'] . "</font></b></h4>" . $game_iploc . "</td><td align='left'>" . $desc . "</td><td>" . $game_homepage . "<br><br>" . $game_voicecomm . "</td><td>" . $game['game_theater'] . "</td><td>" . $game_uptime . "</td></tr>";
   }

   echo "</table></p>";
}

require_once("footer.php");

db_close($db_conn);
$db_conn = NULL;

?>

<script type="text/javascript">
<!-- Begin
function reFresh()
{
  location.reload(true);
}
window.setInterval("reFresh()",<?php echo GAMES_AUTO_REFRESH_MS ?>);
// End -->
</script>

