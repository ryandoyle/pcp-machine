%define _builddir %(echo $PWD)


Name: pcp-machine
Version: 0.1
Release: 1
Summary: Lua scripting engine for Performance Co-Pilot
License: GPL
Group: Applications/System
URL: https://github.com/ryandoyle/pcp-machine

%description
pcp-machine  is a programmable query tool for Performance Co-Pilot.
It allows an user to query for live metrics in Lua scripts and
program based on these returned metrics.

%build
%cmake .
make

%install
make install DESTDIR=%{buildroot}

%files
%doc /usr/share/pcp-machine/LICENCE.txt
%doc /usr/share/pcp-machine/README.md
/usr/bin/pcp-machine
/usr/share/man/man1/pcp-machine.1.gz
/usr/share/pcp-machine/samples/