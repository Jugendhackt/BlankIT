<?php
#exec("echo 'request:".$_POST['text']." FQ: ".utf8_encode($_POST['fq'])."' >> ./log",$ou,$re);
#echo utf8_encode("echo \"".$_POST['text']."\" | /srv/http/WordCount | java -jar FindBlank.jar ".$_POST['fq']);
exec("echo \"".utf8_encode($_GET['text'])."\" | /srv/http/WordCount | java -jar /srv/http/FindBlank.jar ".utf8_encode($_GET['fq']),$output,$return);
foreach($output as $s){
	echo ($s." ");
}
echo ("[@24]");
?>
