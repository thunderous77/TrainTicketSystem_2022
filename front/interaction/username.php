<!-- 注意要改php.ini 中的extension socket -->
<?php
	$sss=$_POST["username"];
	if($sss!=""){
		$socket=socket_create(AF_INET, SOCK_STREAM, SOL_TCP);//创建一个socket
		socket_connect($socket,'127.0.0.1',9030);//开始一个socket连接
		socket_write($socket,$sss);//写数据到socket缓存
		$res=socket_read($socket,300000);//300000为读取数据字节上界
		echo $res;
	}
?>