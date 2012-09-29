<?php

/**
 * queries arin.net for info on the specified IP and returns an array containing
 * all the fields which were returned
 */
function get_ip_info($ip)
{
   $result = NULL;
   
   $file = fopen("http://ws.arin.net/cgi-bin/whois.pl?queryinput=" . $ip,"r");
   if ($file)
   {
      $result = array();
      
      while (!feof($file))
      {
         $line = fgets($file);

         if (preg_match("/^.*: /",$line))
         {
            list($name,$val) = split(":", $line);

            $name = trim($name);
            $val = trim($val);

            if ((strlen($name) > 0) &&
                (strlen($val) > 0))
            {
               $result[$name] = $val;
            }
         }
      }
      
      fclose($file);
   }
   
   return $result;
}

/** 
 * format a location string for the specified URL
 */
function get_ip_loc_string($ip)
{
   $result = "";
   
   $ip_info = get_ip_info($ip);
   if ($ip_info)
   {
      if (strlen($ip_info["StateProv"]) == 0)
      {
         $result .= $ip_info["City"];
      }
      
      if ((strlen($ip_info["StateProv"]) > 0) && (strlen($result) > 0)) $result .= ", ";
      $result .= $ip_info["StateProv"];
      
      if ((strlen($ip_info["Country"]) > 0) && (strlen($result) > 0)) $result .= ", ";
      $result .= $ip_info["Country"];
   }
   
   return $result;
}

?> 

