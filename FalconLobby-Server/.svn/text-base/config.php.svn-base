<?php
//******************************************************************************
//* config.php - provides global config parameters
//******************************************************************************

//******************************************************************************
// database settings
//******************************************************************************

// database server type; currently, mysql is the only supported value, but this
// is here to support adding other types later; adding another type would
// require creating a db_type.php file which implements the necessary DB
// functions (type would be whatever string is specified below)
define('DB_SERVER_TYPE',        'mysql');

// connection info (host should be localhost unless your web server and database
// server are on different machines)
define('DB_HOST',               'localhost');

//
// removed some actual values below for SVN check-in; 'REMOVED' indicates this
//

// name of the database to use
define('DB_NAME',               'REMOVED');
//define('DB_NAME',               'REMOVED');

// user account used for all DB interaction
define('DB_USER',               'REMOVED');
define('DB_PASSWORD',           'REMOVED');
//define('DB_USER',               'REMOVED');
//define('DB_PASSWORD',           'REMOVED');

// game timeout; if a game is listed as active, but a state update has not been
// received for it for > the # seconds specified below, its state will be
// reverted to inactive
define('GAME_ACTIVE_TIMEOUT_S', '180');

// controls how often the active games pages automatically refreshes
define('GAMES_AUTO_REFRESH_MS', '30000');

// API requests are only accepted from the below HTTP user agent
define('EXPECTED_USER_AGENT',   'REMOVED');

//******************************************************************************
// schema parameters -- changing these requires re-installing or manually
// updating the schema to match
//******************************************************************************

define('MAXLEN_SHA1',          '40');
define('MAXLEN_GAMENAME',      '39');
define('MAXLEN_GAMEIP',        '15');
define('MAXLEN_GAMEIPLOC',     '30');
define('MAXLEN_GAMETHEATER',   '20');
define('MAXLEN_GAMEHOMEPAGE',  '128');
define('MAXLEN_GAMEVOICECOMM', '128');
define('MAXLEN_GAMEDESC',      '255');

?>
