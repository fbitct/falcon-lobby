<?php
//******************************************************************************
//* install.php - provides for complete re-installation, wiping the DB and
//*               creating the schema
//******************************************************************************

require_once("config.php");

//
// removed some actual values below for SVN check-in; 'REMOVED' indicates this
//

//***************************************************************************
// set the below with the proper DB root account info; this is used
// to perform the installation; after install, the user account is
// used instead for interacting with the database
//***************************************************************************
define('DB_ROOT_USER',     'REMOVED');
define('DB_ROOT_PASSWORD', 'REMOVED');
//***************************************************************************
//***************************************************************************

$page_title = "install";
require_once("header.php");

$disable_auto_connect = 1;
require_once("db.php");

if (isset($_REQUEST['submit_install']))
{
   unset($result);

   // connect to the DB server
   $db_conn = db_connect(DB_HOST,DB_ROOT_USER,DB_ROOT_PASSWORD)
      or $result  = "Unable to connect to database server: " . db_error();

   // if database exists, delete it
/*
 * DISABLED, host uses cpanel to create DB
 *
   db_select_db(DB_NAME)
      and (db_query("DROP DATABASE " . DB_NAME)
            or $result = "database already exists and am unable to " .
                         "remove it: " . db_error());

   // create the database
   db_query("CREATE DATABASE " . DB_NAME)
      or $result = "unable to create the database: " . db_error();
*/

   // select the newly created database
   db_select_db(DB_NAME)
      or $result = "unable to select the new database: " . db_error();

   // DB schema and DB user account
   $sql_cmds = array(

      array(
         "desc" => "create games table",
         "sql"  =>
            "CREATE TABLE games ( " .
               "game_id    INT UNSIGNED NOT NULL AUTO_INCREMENT, " .
               "game_token CHAR(" . MAXLEN_SHA1 . "), " .
               "game_name  VARCHAR(" . MAXLEN_GAMENAME . ") NOT NULL, " .
               "game_ip    VARCHAR(" . MAXLEN_GAMEIP . ") NOT NULL, " .
               "game_iploc VARCHAR(" . MAXLEN_GAMEIPLOC . ") NOT NULL, " .
               "game_theater VARCHAR(" . MAXLEN_GAMETHEATER . ") NOT NULL, " .
               "game_state TINYINT UNSIGNED NOT NULL, " .
               "game_laststatechange DATETIME, " .
               "game_laststateupdate DATETIME, " .
               "game_homepage VARCHAR(" . MAXLEN_GAMEHOMEPAGE . ") NOT NULL, " .
               "game_voicecomm VARCHAR(" . MAXLEN_GAMEVOICECOMM . ") NOT NULL, " .
               "game_desc VARCHAR(" . MAXLEN_GAMEDESC . ") NOT NULL, " .
               "PRIMARY KEY (game_id), " .
               "UNIQUE INDEX (game_token), " .
               "INDEX (game_state,game_name), " .
               "UNIQUE INDEX (game_name) )"
      )

/*
 * DISABLED, host uses cpanel to grant user permissions
 *
      array(
         "desc" => "grant user permissions to DB",
         "sql"  =>
            "GRANT SELECT,INSERT,UPDATE,DELETE on " . DB_NAME . ".* TO " .
               "'" . DB_USER . "'@'%' IDENTIFIED BY '" . DB_PASSWORD . "'"
      )
*/

   );

   $log = "";

   // create schema
   foreach($sql_cmds as $sql_cmd)
   {
      if (!$result)
      {
         $log .= $sql_cmd["desc"] . "<br>";

         db_query($sql_cmd["sql"])
            or $result = "ERROR: unable to create schema (" . db_error() . "), failed cmd: " . $sql_cmd["desc"];
      }
   }

   if (! $result)
   {
      $log .= "<font color='#009900'>COMPLETED</font><br>";
   }

   // success!
   if (! $result)
   {
      $result = $log . "installation was successful (" . gmdate("D, d M Y H:i:s") . " GMT)";
   }
   else
   {
      $result = $log . "<font color='" . COLOR_ERRORMSG . "'>" . $result . "</font>";
   }
}
?>

<form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
   <p align="center">
   <input type="submit" name="submit_install" value="Install" />
   </p>
</form>

<p align="center">
<?php
   if (isset($_REQUEST['submit_install']))
   {
      echo "<b>$result</b>";
   }

require_once("footer.php");

db_close($db_conn);
$db_conn = NULL;

?>
