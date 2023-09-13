# Linux-QT-Control

## 项目概述
### 嵌入式开发环境：
Linux平台下塔建项目，MP157开发板基于Arm® Cortex®-A7架构。TF-A和uboot移植，Linux内核移植，根文件系统移植，tftp在线下载内核和设备树，SD卡启动模式，烧录启动文件，编写设备驱动。

### 设备驱动移植：
设备树dts使能控制的设备，驱动配置make menuconfig，编译得到UImage和设备树dtb文件，mmc启动，开机自启动mipi显示屏，配置ifconfig eth0 地址，连接WIFI。QT跨平台性，支持Linux、window、macOS，虚拟机QT和开发板ssh调试，
