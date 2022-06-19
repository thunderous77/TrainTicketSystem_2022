<!-- 注意要改php.ini 中的extension socket -->
<?php
	// $arr[0]=Array('username'=>'zhongzero','password'=>'123456');
	// $arr[1]=Array('username'=>'zlh1','password'=>'233333');
	// echo json_encode($arr);
	
	$sss=$_POST["password"];
	if($sss!=""){
		$socket=socket_create(AF_INET, SOCK_STREAM, SOL_TCP);//创建一个socket
		socket_connect($socket,'127.0.0.1',9030);//开始一个socket连接
		socket_write($socket,$sss);//写数据到socket缓存
		$res=socket_read($socket,300000);//300000为读取数据字节上界
		
		$tmp_arr=explode(" ",$res);//按" "将字符串res分离，放入数组tmp_arr中
		$arr=array('startstation'=>$tmp_arr[0],'startdate'=>$tmp_arr[1],'starttime'=>$tmp_arr[2],'endstation'=>$tmp_arr[4],'enddate'=>$tmp_arr[5],'endtime'=>$tmp_arr[6]);
		echo json_encode($arr);//将数组编码成json
	}
?>