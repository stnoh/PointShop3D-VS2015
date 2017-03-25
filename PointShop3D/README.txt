Pointshop3D Version 2.0
=======================
04. May 2004

1.0 What is it
--------------

The program Pointshop3D is a 3D-painter which operates on surfels - surface elements - instead
of triangulated models. It provides basic operations such as painting with a brush or
texture, texture filtering and a simple but powerful plugin mechanism for tools and plugins.
The renderer is also changeable at runtime. 

1.1 What is new
---------------
In version 2.0, Pointshop3D was extented such that not only the appearance, but also the geometry
can be changed, for example using the deformation or the CSG tool. Furthermore, elliptical splats
are now supported and a new memory management was implemented.
In Pointshop3D 2.0, the whole pipeline from acquisition to interactive modeling is supported with
additional tools.

2.0 Installation
----------------

For installation under Windows (win32), read the INSTALL.win32 text file.

For installation under Linux (Unix) read the INSTALL.linux text file.

3.0 Requirements
----------------

For Linux or Windows with Microsoft Visual Studio 6.0, Qt 2.3 or higher is required.
For Windows with Microsoft Visual Studio 7.1 you need to install Qt 3.x.

While Qt 2.3 is freely available as a non-commercial version (ftp://ftp.trolltech.com/qt/non-commercial/QtWin230-NonCommercial.exe),
Qt 3.x is available only as educational version for free.

Some additional tools and plugins use the GNU Scientific Library (GSL).

For compiling under Linux you need the gcc 3.x compiler.

A.0 3rd party software
----------------------

Qt    - Cross platform development - http://www.trolltech.com
tmake - platform independent Makefiles - http://www.trolltech.com
GSL   - GNU Scientific Library (GSL) - http://www.gnu.org/software/gsl/

