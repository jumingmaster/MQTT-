

1、debug文件是监控stm32所有的usart1数据，源码请看debug.c, 库在lib文件夹里
2、main.py是负责连接stm32的ESP8266模块，通过mqtt协议（TCP/IP形式）获取需要的数据，然后写入时序数据库。

！！！只在LINUX环境下测试，其他平台不行。！！！

依赖系统
Debian-based Linux, or macOS

MQTT框架
Mosquitto - MQTT broker

环境搭建:

InfluxDB时序数据库安装
	1.sudo apt install apt-transport-https curl -y
	
	2.curl -sL https://repos.influxdata.com/influxdb.key | sudo apt-key add -
	
	3.
	Debian：
		echo "deb https://repos.influxdata.com/debian stretch stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
		(stretch自行替换成当前系统版本)
	Ubuntu：
		echo "deb https://repos.influxdata.com/ubuntu/ xenial stable" | sudo tee /etc/apt/sources.list.d/influxdb.list
		(xenial自行替换）

	4.sudo apt update && sudo apt install influxdb

Grafana安装和配置
	1.sudo apt install adduser libfontconfig -y

	2.
	Ubuntu and Debian:
		· wget https://dl.grafana.com/oss/release/grafana_7.1.5_amd64.deb
		· sudo dpkg -i grafana_7.1.5_amd64.deb
	
	Standalone Linux Binaries：
		· wget https://dl.grafana.com/oss/release/grafana-7.1.5.linux-amd64.tar.gz
		· tar -zxvf grafana-7.1.5.linux-amd64.tar.gz
	
	Red Hat, CentOS, RHEL, and Fedora：
		· wget https://dl.grafana.com/oss/release/grafana-7.1.5-1.x86_64.rpm
		· sudo yum install grafana-7.1.5-1.x86_64.rpm

	OpenSUSE and SUSE：
		· wget https://dl.grafana.com/oss/release/grafana-7.1.5-1.x86_64.rpm
		· sudo rpm -i --nodeps grafana-7.1.5-1.x86_64.rpm

	3. sudo systemctl daemon-reload
	
	4. sudo systemctl enable grafana-server

	5. sudo systemctl start grafana-server

Mosquitto 安装:

	1. git clone https://github.com/eclipse/mosquitto.git

	2. make -j4

	3. sudo make install

重启后，mosquitto应该已经在后台运行了，可以在命令行输入mosquitto运行，如果返回address already used错误，那就表明已经在运行了。
同样确认一下influxd。
然后grafana可以在http://localhost:3000上确认。

之后
1. ./debug.c
2. python3 main.py

到此服务器环境已经搭建好了。



