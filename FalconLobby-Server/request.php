<?php
//******************************************************************************
//* request.php - main module used by the client SW to issue requests and get
//*               responses.  when issuing a request to this page, the method
//*               argument must be set to one of the valid method names and
//*               the necessary arguments must be provided.  the first line
//*               of the response will be either:
//*                  SUCCESS: message
//*               or
//*                  ERROR: error description
//*               and for the case of success, additional lines may provide
//*               result data from the request.
//******************************************************************************

// prevents client-side caching
header("Expires: Tuesday, 1 January 1980 00:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

require_once("config.php");
require_once("db.php");
require_once("util.php");

$result = "ERROR:unknown internal error";

// this page will only accept requests from the intended client application
if ($_SERVER['HTTP_USER_AGENT'] == EXPECTED_USER_AGENT)
{

// branch on method argument
switch ($_REQUEST['method'])
{
//
// register a new game
//
case 'newgame':

   if (!empty($_REQUEST['game_name']))
   {
      $dbr = db_newgame($_REQUEST['game_name']);
      if ($dbr['err'])
      {
         $result = "ERROR:" . $dbr['err'];
      }
      else
      {
         $result = "SUCCESS:game registered\n" . $dbr['game_token'];
      }
   }
   else
   {
      $result = "ERROR:invalid args, game name not specified";
   }

   break;

//
// update state of a registered game
//
case 'setgamestate':

   if (!empty($_REQUEST['game_token']))
   {
      if (!empty($_REQUEST['game_state']))
      {
         if (($_REQUEST['game_state'] == 'down') ||
             ($_REQUEST['game_state'] == 'up'))
         {
            if ($_REQUEST['game_state'] == 'up')
            {
               $dbr = db_setgamestate($_REQUEST['game_token'],
                                      1,
                                      $_SERVER['REMOTE_ADDR'],
                                      $_REQUEST['game_theater']);
            }
            else
            {
               $dbr = db_setgamestate($_REQUEST['game_token'],
                                      0,
                                      '',
                                      '');
            }

            if ($dbr['err'])
            {
               $result = "ERROR:" . $dbr['err'];
            }
            else
            {
               $result = "SUCCESS:set game state";
            }
         }
         else
         {
            $result = "ERROR:invalid args, invalid game state specified";
         }
      }
      else
      {
         $result = "ERROR:invalid args, game state not specified";
      }
   }
   else
   {
      $result = "ERROR:invalid args, game token not specified";
   }

   break;

//
// update public info for a registered game
//
case 'setgameinfo':

   if (!empty($_REQUEST['game_token']))
   {
      $isvalidurl = false;
      $game_homepage = $_REQUEST['game_homepage'];

      if (validateUrlSyntax($game_homepage,'s+H+') ||
          validateUrlSyntax($game_homepage,'s+S+'))
      {
         $isvalidurl = true;
      }
      else
      {
         $game_homepage = "http://" . $_REQUEST['game_homepage'];

         if (validateUrlSyntax($game_homepage,'s+H+') ||
             validateUrlSyntax($game_homepage,'s+S+'))
         {
            $isvalidurl = true;
         }
      }

      if ($isvalidurl)
      {
         $dbr = db_setgameinfo($_REQUEST['game_token'],
                               $game_homepage,
                               $_REQUEST['game_voicecomm'],
                               $_REQUEST['game_desc']);
         if ($dbr['err'])
         {
            $result = "ERROR:" . $dbr['err'];
         }
         else
         {
            $result = "SUCCESS:set game public info";
         }
      }
      else
      {
         $result = "ERROR:invalid home page URL specified";
      }
   }
   else
   {
      $result = "ERROR:invalid args, game token not specified";
   }

   break;

//
// get list of active games
//
case 'getactivegames':

   $dbr = db_getactivegames();

   if ($dbr['err'])
   {
      $result = "ERROR:" . $dbr['err'];
   }
   else
   {
      $result = "SUCCESS:retrieved active games\n";

      foreach ($dbr['activegames'] as $game)
      {
         $game_desc = $game['game_desc'];
         $tmp = str_replace("ENDOFFIELD","",$game_desc);
         $game_desc = $game_desc . "\nENDOFFIELD";

         $result .= "game\n" .
                    "game_name\n" . $game['game_name'] . "\n" .
                    "game_ip\n" . $game['game_ip'] . "\n" .
                    "game_theater\n" . $game['game_theater'] . "\n" .
                    "game_uptime\n" . $game['game_uptime'] . "\n" .
                    "game_homepage\n" . $game['game_homepage'] . "\n" .
                    "game_voicecomm\n" . $game['game_voicecomm'] . "\n" .
                    "game_desc\n" . $game_desc . "\n";
      }
   }

   break;

//
// no method name or invalid method name provided
//
default:
   $result = "ERROR:invalid method, " . $_REQUEST['method'];
   break;
}

}

echo $result;

db_close($db_conn);
$db_conn = NULL;

?>
