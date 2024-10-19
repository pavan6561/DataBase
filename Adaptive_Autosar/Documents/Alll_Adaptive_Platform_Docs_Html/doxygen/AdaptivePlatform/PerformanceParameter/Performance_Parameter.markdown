# Performance Parameter {#Performance_parameter}

## 1) Steps to check communication latency

   -# Communication latency Measurement between two machines could be possible only when , both the machines are synchronize to same time.
   
   -# We need to install NTP client on both the machines, to synchronize the time between machines.

   -# Below link is used to install and configure NTP server. We also mentioned steps to install NTP server on host machine. \n
    <a href="http://www.ubuntugeek.com/install-and-configure-network-time-protocol-ntp-serverclients-on-ubuntu-16-04-server.html" target="_blank"> http://www.ubuntugeek.com/install-and-configure-network-time-protocol-ntp-serverclients-on-ubuntu-16-04-server.html </a>

   
   -# We also need to have a host machine which has NTP server installed on it. Below is the command to install NTP server on host.
~~~ 
sudo apt-get install ntp
~~~

   -# On the host machine in /etc/ntp.conf file add the below line.
~~~ 
restrict 192.168.0.0 mask 255.255.255.0 nomodify notrap
~~~
   -# Image
      ![ntp.conf file on host machine](Images/ntpi.png)
	  
	  
   -# Use the below command to start the ntp server.
~~~
/etc/init.d/ntp start
~~~
   
   -#  Add the meta-networking layer in bblayer.conf as it contains .bb file regarding NTP.
~~~ 
bitbake-layers add-layer ../yocto-layers/meta-networking
~~~

   -# Add the following NTP image using IMAGE_INSTALL in recipe yocto-layers/meta-ara/recipes-core/core-performance-use-case/core-image-apd-minimal-perform-avin.bb
~~~
IMAGE_INSTALL += "\
        .. \
		.. \
		ntp \
        "
~~~

   -# Bitbake respective images.
~~~
    $ bitbake core-image-apd-minimal-perform-ecu1
	$ bitbake core-image-apd-minimal-perform-ecu2
~~~
    
   -# After bitbake, flash the image into SDcard.
   
      -# We can assign static IP address to target in /etc/network/interface file.
~~~
auto eth0
iface eth0 inet static
      address 192.168.20.53
      netmask 255.255.255.0
      network 192.168.20.0
      gateway 192.168.20.1
~~~

   -# After logged in into the board , the /etc/ntp.conf file on the board should have IP address of host machine in the below format.
   
    For Example:
~~~
    $ server <ip adress of host machine>
	$ fudge <ip adress of host machine> stratum 14
~~~
   -# Now restart the respective boards.
   
   -# Use the below command to check whether both the machines have same time.
~~~
date -u
~~~
   -# If both the machines are synchronized to the same host machine, then adjust() method can be used to calculate the communication latency.
   
   -# <b> In the Performance Parameter Sample Application, Adjust() method of Radar and Fusion is used. </b>
   
      * In Fusion Application, print timestamp before Adjust() method is being invoked.
	  ![Fusion Application](Images/1.png)
	  * Pass this timestamp to Adjust() method in Radar.
	  ![Radar Application](Images/8.png)
	  * In Radar Application, print timestamp inside Adjust() method.
	  ![Radar Application](Images/6.png)
	  * Check the difference between current time taken in Radar Application and time which was passed as parameter from Fusion.
	  
      * The difference between the two timestamps specifies the communication latency.
      ![Radar Application](Images/9.png)
	  
	  
   -# <b> Now we will do the above method in reverse order from Radar to Fusion </b>
   
	 * In Radar Application, print timestamp in Adjust() method.
	 ![Radar Application](Images/6.png)
	 * Return this timestamp as out parameter of Adjust() method to Fusion.
	 ![Fusion Application](Images/2.png)
	 * In Fusion Application, print the timestamp.
	 ![Fusion Application](Images/3.png)
	 * Check the difference between current time taken in Fusion Application and the timestamp received from Radar.
	 
     * The difference between the two timestamps specifies the communication latency.
     ![Fusion Application](Images/4.png)
	
	
   -# The above steps illustrates the communication latency between two machines.
	  
	  
	  
## 2) Steps to check booting time
	
>  **2.1 : NXP S32v234SBC**
   
   -# Add the below command into build_s32v234sbc/conf/local.conf file.
~~~
DISTRO_FEATURES +=" ${DISTRO_FEATURES_LIBC} systemd "
~~~
   
   -# Add the following systemd image using IMAGE_INSTALL in recipe yocto-layers/meta-ara/recipes-core/core-per-use-case/core-image-apd-minimal-per.bb
~~~
IMAGE_INSTALL += "\
        .. \
		systemd \
		systemd-analyze \
        "
~~~

   -# Bitbake respective image.
~~~
    $ bitbake core-image-apd-minimal-per
~~~

   -# After bitbake, flash the image into SDcard.
   
   -# Following are the steps to copy image from host system to memory card:
~~~
cd /tmp/deploy/images/s32v234sbc
sudo dd if=image-name.rootfs.sdcard of=/dev/mmcblk0 bs=1M && sync
~~~
   
   -# When you login into board, Enter the below command to check the booting time.
~~~
systemd-analyze
~~~

   -# Enter the below command to store the booting time into .svg image file, which gets stored at /home/root/ directory.
~~~
systemd-analyze plot > filename.svg
~~~

   -# Copy the .svg image file into /opt folder.
~~~
cp filename.svg opt/
~~~

   -# Now insert the SDCard from board into host machine, /opt directory will conatin filename.svg image file.

   -# This .svg file will contain details regarding booting time.
   
    ![Boot Chart output for NXP](Images/nxpsystemd.svg)
	
	-#  While a system is booted up by starting the  Kernel , systemd initializes the system, starting all the system services.
	
	     * multi-user.target : It starts every service that are expected to run in  multi user mode.
		  
		 * sysinit.target : systemd starts system initialization services. This includes swap devices enabling and file systems mounting.
		  
		 * basic.target : Starts the basic services like firewalld and microcode services.
		  
		 * local-fs.target : No user related services are started by this target unit. It handles core level services only. This target is one performing actions on basis of  /etc/fstab/ and /etc/inittab files.
		  
		 * When systemd starts up the system, it will activate all units that are dependencies of default.target (as well as recursively all dependencies of these dependencies). Usually, default.target is simply an alias of graphical.target or 
		 multi-user.target, depending on whether the system is configured for a graphical UI or only for a text console.
		  
		 * Each target has a set of dependencies described in its configuration file. systemd starts the required dependencies. These dependencies are the services required to run the Linux host at a specific level of functionality. 
		 When all of the dependencies listed in the target configuration files are loaded and running, the system is running at that target level.
	
	     * A unit configuration file whose name ends in ".slice" encodes information about a slice unit. A slice unit is a concept for hierarchically managing resources of a group of processes. 
	     This management is performed by creating a node in the Linux Control Group (cgroup) tree. 
	     Units that manage processes (primarily scope and service units) may be assigned to a specific slice. For each slice, certain resource limits may be set that apply to all processes of all units contained in that slice. 

         * By default, service and scope units are placed in system.slice, virtual machines and containers registered with systemd-machined are found in machine.slice, and user sessions handled by systemd-logind in user.slice.
         systemd.unit for the common options of all unit configuration files.
