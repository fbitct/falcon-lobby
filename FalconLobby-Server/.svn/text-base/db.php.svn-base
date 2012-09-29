<?php
//******************************************************************************
//* db.php - high level DB functions which make calls into the DB provided by
//*          the selected db_type.php file
//******************************************************************************

require_once("config.php");

// pulls in the appropriate DB-specific module based on the DB type chosen
// in config.php
require_once("db_" . DB_SERVER_TYPE . ".php");

// unless disabled, this will establish a connection with the database
if (!isset($disable_auto_connect))
{
   // connect to the DB server and select the DB
   global $db_conn;
   $db_conn = db_connect(DB_HOST,DB_USER,DB_PASSWORD)
      or die(db_err('Unable to connect to database server'));
   db_select_db(DB_NAME)
      or die(db_err('Unable to select database'));
}

// generates an error message which contains the specified text along with
// the mysql error information
function db_err($msg)
{
   return '<b>ERROR: ' . $msg . ': (' . db_errno() . '), ' . db_error() . 
      "</b>";
}

// used for escaping and trimming form data
function db_escape_data($data)
{
   if (ini_get('magic_quotes_gpc'))      
   {
      $data = stripslashes($data);
   }

   return db_real_escape_string($data,$db_conn);
}

// registers a new game and returns the token
function db_newgame($game_name)
{
   $result = array('err' => NULL);

   // is specified game name already registered?   
   $sql = "SELECT game_id FROM games WHERE UPPER(game_name) = UPPER('" . db_escape_data($game_name) . "')";
   $sql_result = db_query($sql);
   $num_rows = db_num_rows($sql_result);
   db_free_result($sql_result);
   if ($num_rows > 0)
   {
      $result['err'] = "specified game name is already registered";
   }
   else
   {
      // insert new game record
      $sql = "INSERT INTO games (game_token,game_name,game_ip,game_iploc,game_theater,game_state,game_laststatechange,game_laststateupdate,game_homepage,game_voicecomm,game_desc) " .
             "VALUES(NULL,'" . db_escape_data($game_name) . "','','','',0,NOW(),NOW(),'','','')";
      if (!db_query($sql))
      {
         $result['err'] = "error registering game: " . db_error();
      }
      else
      {
         // retrieve auto-increment game ID generated when we inserted the new record
         $game_id = db_insert_id();
         
         // generate game token
         $game_token = strtoupper(sha1($game_id . $game_name . gmdate("D, d M Y H:i:s") . $_SERVER['REMOTE_ADDR']));

         // set game token
         $sql = "UPDATE games SET game_token = '" . db_escape_data($game_token) . "' WHERE game_id = " . $game_id;
         if (!db_query($sql))
         {
            $result['err'] = "error generating game token: " . db_error();
         }
         else
         {
            $result['game_token'] = $game_token;
         }
      }
   }
   
   return $result;
}

// set the state of a registered game
function db_setgamestate($game_token,$game_state,$game_ip,$game_theater)
{
   $result = array('err' => NULL);

   // is the token for a valid registered game?
   $sql = "SELECT game_id,game_state,game_ip,game_theater FROM games WHERE game_token = '" . db_escape_data(strtoupper($game_token)) . "'";
   $sql_result = db_query($sql);
   $num_rows = db_num_rows($sql_result);
   if ($num_rows > 0)
   {   
      // get fields
      $row = db_fetch_array($sql_result);
      $game_id = $row['game_id'];
      $prev_game_state = $row['game_state'];
      $prev_game_ip = $row['game_ip'];      
      $prev_game_theater = $row['game_theater'];

      // if state or IP has changed, consider this a state change, write out the new state and the state change timestamp
      if ((strcmp($prev_game_state,$game_state) != 0) ||
          (strcmp($prev_game_ip,$game_ip) != 0) ||
          (strcmp($prev_game_theater,$game_theater) != 0))
      {
         if ($game_state == 1)
         {
            $sql = "UPDATE games SET game_state = " . $game_state . ", game_ip = '" . db_escape_data($game_ip) . "', game_iploc = '" . db_escape_data(get_ip_loc_string($game_ip)) . "', game_theater = '" . db_escape_data($game_theater) . "', game_laststatechange = NOW(), game_laststateupdate = NOW() WHERE game_id = " . $game_id;            
         }
         else
         {            
            $sql = "UPDATE games SET game_state = " . $game_state . ", game_ip = '" . db_escape_data($game_ip) . "', game_iploc = '', game_theater = '" . db_escape_data($game_theater) . "', game_laststatechange = NOW(), game_laststateupdate = NOW() WHERE game_id = " . $game_id;
         }   
      }
      // state and IP are unchanged, just write out a timestamp indicating an update was received for this game
      else
      {
         $sql = "UPDATE games SET game_laststateupdate = NOW() WHERE game_id = " . $game_id;
      }

      if (!db_query($sql))
      {
         $result['err'] = "error setting the game state: " . db_error();
      }
   }
   else
   {
      $result['err'] = "invalid game token specified";
   }
   db_free_result($sql_result);

   return $result;
}

// set the public info for the specified game
function db_setgameinfo($game_token,$game_homepage,$game_voicecomm,$game_desc)
{
   $result = array('err' => NULL);

   // is the token for a valid registered game?
   $sql = "SELECT game_id FROM games WHERE game_token = '" . db_escape_data(strtoupper($game_token)) . "'";
   $sql_result = db_query($sql);
   $num_rows = db_num_rows($sql_result);
   if ($num_rows > 0)
   {   
      // get fields
      $row = db_fetch_array($sql_result);
      $game_id = $row['game_id'];

      // set public info
      $sql = "UPDATE games SET game_homepage = '" . db_escape_data(strip_tags($game_homepage)) . "', game_voicecomm = '" . db_escape_data(strip_tags($game_voicecomm)) . "', game_desc = '" . db_escape_data(strip_tags($game_desc)) . "' WHERE game_id = " . $game_id;      
      if (!db_query($sql))
      {
         $result['err'] = "error setting the game public info: " . db_error();
      }
   }
   else
   {
      $result['err'] = "invalid game token specified";
   }
   db_free_result($sql_result);
   
   return $result;
}

// get active games
function db_getactivegames()
{
   $result = array('err' => NULL);
   $result = array('activegames' => array());

   // change state to inactive for any games listed as active but for which a recent update has not been received
   $sql = "UPDATE games SET game_state = 0, game_laststatechange = NOW() WHERE game_state = 1 AND (UNIX_TIMESTAMP(NOW()) - UNIX_TIMESTAMP(game_laststateupdate)) > " . GAME_ACTIVE_TIMEOUT_S;
   db_query($sql);

   // retrieve list of games listed as active
   $sql = "SELECT *,UNIX_TIMESTAMP(NOW())-UNIX_TIMESTAMP(game_laststatechange) AS game_uptime FROM games WHERE game_state = 1 ORDER BY game_name";
   $sql_result = db_query($sql);
   if ($sql_result)
   {
      // extract results
      $ag = array();
      $x = 1;
      while ($row = db_fetch_array($sql_result))
      {
         $ag[$x]['game_name'] = $row['game_name'];
         $ag[$x]['game_ip'] = $row['game_ip'];
         $ag[$x]['game_iploc'] = $row['game_iploc'];
         $ag[$x]['game_theater'] = $row['game_theater'];
         $ag[$x]['game_homepage'] = $row['game_homepage'];
         $ag[$x]['game_voicecomm'] = $row['game_voicecomm'];
         $ag[$x]['game_desc'] = $row['game_desc'];
         $ag[$x]['game_uptime'] = $row['game_uptime'];
         $x++;
      }
      
      $result['activegames'] = $ag;
   
      db_free_result($sql_result);
   }
   
   return $result;
}

// get # registered games
function db_getnumgames()
{
   $result = NULL;

   $sql = "SELECT COUNT(*) AS numgames FROM games";
   $sql_result = db_query($sql);
   if ($sql_result)
   {
      if ($row = db_fetch_array($sql_result))      
      {
         $result = $row['numgames'];
      }
   
      db_free_result($sql_result);
   }
   
   return $result;
}

?>
