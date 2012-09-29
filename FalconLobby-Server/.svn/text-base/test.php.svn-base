<?php

require_once('util.php');

if (validateUrlSyntax($_REQUEST['url'],'s+S+'))
{
   echo "valid<br>";
}
else
{
   echo "NOT valid<br>";
}

echo "<hr>\n";

$ip_info = get_ip_info($_REQUEST['ip']);

if ($ip_info)
{
   foreach ($ip_info as $name => $val)
   {
      echo $name . ": " . $val . "<br>\n";
   }
}
else
{
   echo "no ip info returned<br>\n";
}

?>

