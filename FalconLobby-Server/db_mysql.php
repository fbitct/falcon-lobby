<?php
//******************************************************************************
//* db_mysql.php - provides implementation for the generic DB functions as
//*                applicable to the MySQL database
//******************************************************************************

function db_error()
{
   return mysql_error();
}

function db_errno()
{
   return mysql_errno();
}

function db_connect($host,$user,$pw)
{
   return mysql_connect($host,$user,$pw);
}

function db_select_db($dbname)
{
   return mysql_select_db($dbname);
}

function db_real_escape_string($str)
{
   return mysql_real_escape_string(trim($str));
}

function db_query($sql)
{
   return mysql_query($sql);
}

function db_num_rows($sql_result)
{
   return mysql_num_rows($sql_result);
}

function db_fetch_array($sql_result)
{
   return mysql_fetch_array($sql_result);
}

function db_free_result($sql_result)
{
   return mysql_free_result($sql_result);
}

function db_insert_id()
{
   return mysql_insert_id();
}

function db_close($conn)
{
   mysql_close($conn);
}

?>

