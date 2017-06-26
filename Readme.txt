


\______ \_______|__|__  __ ___________  |    |    _________     __| _/___________  /_   |   \   _  \  
 |    |  \_  __ \  \  \/ // __ \_  __ \ |    |   /  _ \__  \   / __ |/ __ \_  __ \  |   |   /  /_\  \ 
 |    `   \  | \/  |\   /\  ___/|  | \/ |    |__(  <_> ) __ \_/ /_/ \  ___/|  | \/  |   |   \  \_/   \
/_______  /__|  |__| \_/  \___  >__|    |_______ \____(____  /\____ |\___  >__|     |___| /\ \_____  /
        \/                    \/                \/         \/      \/    \/               \/       \/ 




Introduction
-------------------------------------------------------------------
It Loads any windows driver but in this case it will load
The drivers in Test-Drivers folder. The Drivers inside will
protect the named process from code injection, hooking and 
termination. The driver is the one that prevents all of this
from happening.


Installing
-------------------------------------------------------------------
The Program is portable can be carried in a usb driver and run on
any version of windows,check the build folder.


Running The Program
-------------------------------------------------------------------
Run DriverLoader as admin having disable Driver signing enforcement
see below.Select the start option to "Demand" and error to "Normal".
Then register the driver as a windows service and start the driver.
Try to inject to notepad and you will notice you cant, neither can
you hook. To stop the triver click on stop and maybe unregister.


Compiling The Program
--------------------------------------------------------------------
To compile the program you need Visual Studio 2015 running WDK(Windows
Driver Kit) 10 which can develop drivers for Windows 7,8,8.1 and 10.
Load The ProtectDriver solution and build the program, you can change
settings to suite your needs. The driver compiles to a ".sys" file.

Next you need to install QT 5.9 for visual studio 2015. The open the
DriverLoader pro file which will load the program source files. Build
The program as a release. To make it portable in all versions of windows
you need to go to QT installation folder and look for windeployqt.exe.
You will need to navigate via command prompt, try looking for it in
QTDIR/bin/windeployqt path. Once there, type

windeployqt --qmldir --release path-to-execuatble

The code will be made portable.For more information try
http://doc.qt.io/qt-5/windows-deployment.html




Issues
---------------------------------------------------------------------
(+) The Driver loader Process and Services code print data per line
    making alignment ugly. To be fixed later on. 
(+) Resizing issues. Also to be fixed.  


Disabling Driver signature Enforcement
---------------------------------------------------------------------
To disable in windows 7 follow the below steps
1.) Restart windows.
2.) Press F8 before Windows Logo appears.
3.) Select "Disable Driver Signature Enforcement".
    Continue with boot.

For Windows 8,8.1 and 10:
1.) Click the Start menu and select Settings.
2.) Click Update and Security.
3.) Click on Recovery.
4.) Click Restart now under Advanced Startup.
5.) Click Troubleshoot.
6.) Click Advanced options.
7.) Click Startup Settings.
8.) Click on Restart.
9.) On the Startup Settings screen press 7 or F7 to disable driver signature enforcement.    