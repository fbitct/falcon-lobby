<?php
//******************************************************************************
//* header.php - general page header
//******************************************************************************

require_once("config.php");

// prevents client-side caching
header("Expires: Tuesday, 1 January 1980 00:00:00 GMT");
header("Last-Modified: " . gmdate("D, d M Y H:i:s") . " GMT");
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
   "http://www.w3.org/TR/2000/REC-xhtml1-20000126/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>

   <meta http-equiv="content-type" content="text/html"; charset="iso-8859-1" />
   <title><?php echo $page_title; ?></title>

   <style type="text/css" media="screen">

      a:link {
         color: #FFFFFF;
         text-decoration: underline;
      }

      a:visited {
         color: #FFFFFF;
         text-decoration: underline;
      }

      a:hover {
         color: #555555;
         text-decoration: underline;
      }

      * {
         font-size: 12.0pt;
         font-family: Arial;
      }

      p.footnote {
         font-size: 10.0pt;
         font-family: Arial;
      }

      h1 {
         font-size: 24.0pt;
         font-family: Arial;
      }

      h2 {
         font-size: 20.0pt;
         font-family: Arial;
      }

      h3 {
         font-size: 14.0pt;
         font-family: Arial;
      }

      h4 {
         font-size: 10.0pt;
         font-family: Arial;
      }

   </style>

</head>

<!-- removed Google analytics snippet for SVN check-in -->

<body bgcolor="#2C3337">
<font color="#E8E8EA" face="Arial" size="2">
