<?
require_once("header.php");
?>

<table width="100%">
<tr>
<td align="left"><h1>FalconLobby</h1></td>
<td align="right">
   created by <a href="mailto:coder1024@gmail.com">Dave &quot;coder1024&quot; Calkins</a>
</td>
</tr>
</table>

<hr size=2 width="100%">

<p>
FalconLobby is an authorized AddOn for <a href="http://www.lead-pursuit.com" target="_blank">Falcon 4.0 Allied Force</a>
which was created to support the multiplayer experience.  FalconLobby retrieves the list of currently active
game servers and imports that list into the Falcon &quot;phone book&quot; (the list of game servers shown on the
MultiPlayer/Internet screen).  This avoids the need to &quot;fat finger&quot; in the IP addresses of the servers and
provides a quick way to see what online games are currently available.  The list of active game servers is populated
from machines running game servers and FalconLobby.
</p>

<p>
<b>To start using FalconLobby now, click on the DOWNLOAD link to the left and follow the instructions.</b>
</p>

<h2>News</h2>

   <hr size=2 width="100%">
   <table border=0 cellspacing=10 cellpadding=0>

   <tr valign="top"><td><i>2006.08.04</i></td><td><b>FalconLobby v1.1 released!</b><br>
   <ul>
<li>fixed handle/memory leak which could cause active servers to eventually drop
  from the list, despite still being active.  this could also cause the client
  to stop being able to download the active server list.  the fix was to replace
  the internal HTML viewer used for the "View Active Servers" tray icon menu
  with a call to view the URL using the user's default browser.  this issue
  wouldn't occur if you just ran FalconLobby and never picked that item from
  the tray icon menu; anyway, it should be fixed now.
<li>the server publishing dialog wasn't always updating the checkbox correctly
  based on the settings.ini file value.  the app logic used the settings.ini
  file value, its just the UI that didn't correctly display the check.
   </ul>
   </td></tr>

   <tr valign="top"><td><i>2006.03.11</i></td><td><b>FalconLobby v1.0 released!</b></td></tr>
   </table>

   <table border=0 cellspacing=10 cellpadding=0>
   <tr><td><p class="footnote">&quot;FalconLobby you are cleared for takeoff, runway one-zero right, depart heading one-two-five&quot;</p></td></tr>
   </table>


<?
require_once("footer.php");
?>
