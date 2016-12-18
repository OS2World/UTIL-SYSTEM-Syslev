The SYSLEV utility
==================

Overview
--------

SYSLEV displays the system level information associated with a single
SYSLEVEL file. 

Files named SYSLEVEL.* are scattered all over an OS/2 system; each one
records the 'system level' or version and revision (service level) of a
single system component. 

One can display the contents of these files using the standard OS/2
SYSLEVEL command; however, this displays information from all files, and
(on a large system) takes a long time to run since it scans the whole
file system. 

In many cases, all one needs to know is the information concerning one
component.  If the name of the appropriate SYSLEVEL file is known (it's
usually stored with the component) then SYSLEV will display just that
information. 

Using the program
-----------------

Synopsis: syslev file
 where:
    file    is the name of the SYSLEVEL file whose contents are to be
            displayed


Versions
--------
1.0	- Initial version.
1.1	- Added support for letters in level2 section (third version part)
	- Corrected omission of some zero digits.
	- Added support for component type field.

Bob Eager
rde@tavi.co.uk
17th June 2000
