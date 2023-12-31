# Linux-QT-Control

## 项目概述
### 嵌入式开发环境：
Linux 5.4.13平台下塔建项目，MP157开发板基于Arm® Cortex®-A7架构，QT中Debug部署到远程Linux开发板，Linux支持qmake编译。

#### 
<img src="res/IMG_20230912_224328.jpg" width="250px">  <img src="res/IMG_20230912_224221.jpg" width="250px"> 
<img src="res/IMG_20230912_224338.jpg" width="250px">  <img src="res/IMG_20230912_224404.jpg" width="250px">
<img src="res/IMG_20230912_224414.jpg" width="250px">  <img src="res/IMG_20230912_224716.jpg" width="250px"> 

### 设备驱动移植：
TF-A和uboot移植，Linux内核移植，根文件系统移植，mipi LCD显示屏以及触摸屏GT911驱动移植，WIFI(AP6212)移植，tftp在线下载内核和设备树，SD卡烧录启动文件，SD启动模式，编写设备驱动，测试设备驱动。

设备树dts使能控制的设备，驱动配置make menuconfig，编译得到UImage和设备树dtb文件，mmc启动，开机自启动mipi显示屏，配置ifconfig eth0 地址，连接WIFI。

## 裸机开发和后台服务器部分
参照：[https://github.com/DXT-parsifal/Embedded-Integrated-Project](https://github.com/DXT-parsifal/Embedded-Integrated-Project)

## 驱动相关
### 内核层
主要涉及内核模块的编写，模块编译编写Makefile文件（支持多模块编译），使用设备树初始化设备，驱动of类函数匹配的方式；字符设备自动创建，涉及IO模型应用层和内核层通信_IO（）；PWM控制，塔建在platform总线上，设备树匹配方式；I2C总线驱动读取si7006设备数据；SPI总线驱动M74HC595数码管显示数据。

### 应用层
QT跨平台性，支持Linux、window、macOS，虚拟机QT和开发板ssh调试。QT基于C++语言，使用面向对象的思想对内核层写好的驱动得到的设备文件进行操作open、iotcl、write，设备信息有时需要配置封装好的结构体。实现的功能呢界面有：主页Theme、远程控制（stm32F407 MCU）、数据监控（数据库MySQL）、天气预告（JSON解析）、语音识别（API）、多媒体播放（在线音乐）、视频监控（L4V2框架）、软键盘设计（中文输入），以及本地控制（STM32MP157 Linux），实现了一些智能的场景控制：智能监测，设置阈值自动控制，支持自定义阈值，视频监控红外接近唤醒。扩展夜间睡眠模式和起床场景，支持语音控制。

## 最后
设备树dts有语法规则（有点类似JSON），开发时通常是继承SOC厂商的设备树，可以仿照内核目录中的Documentation下Devicetree中的设备树源文件编写自己的设备树源文件（节点引用，属性覆盖），注意pinctrl引脚的复用。

交叉编译工具链的配置，Linux下的C语言是gcc编译器进行代码编译的，针对的是X86架构，而我们的开发板是ARM架构，为方便驱动的开发，在PC的Ubuntu上编译能够在ARM开发板上运行的测试代码需要安装交叉编译器。

star star star!!!(第一次写完整的项目分享，感谢star支持) ----括号侠
