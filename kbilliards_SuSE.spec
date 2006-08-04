
BuildRequires: arts-devel kdelibs3-devel qt3-devel bzip2 gcc-c++

Name: kbilliards
Version: 0.8.7b
Release: 1
URL: http://www.hostnotfound.it/kbilliards.php
License: GPL
Summary: a funny billiards simulator game
Group: Amusements/Games/Action/Arcade
Packager: Samuele Catuzzi <samuele.c@hostnotfound.it>
BuildRoot:  %{_tmppath}/%{name}-root 
Source: %{name}-%{version}.tar.bz2 


%description
a funny billiards simulator game, under KDE , Linux



Authors:
--------
    Samuele Catuzzi <samuele.c@hostnotfound.it>

%prep
%setup -q
. /etc/opt/kde3/common_options
update_admin

%build
. /etc/opt/kde3/common_options
./configure $configkd
make

%install
. /etc/opt/kde3/common_options
make DESTDIR=$RPM_BUILD_ROOT $INSTALL_TARGET
rm -rf %{buildroot}/opt/kde3/share/doc/HTML/en/kbilliards

%clean
rm -rf %{buildroot}

%files
%defattr(-, root, root)
%doc AUTHORS COPYING ChangeLog NEWS README TODO
/opt/kde3/bin/*
/opt/kde3/share/applnk/*/*.desktop
/opt/kde3/share/apps/kbilliards/*
/opt/kde3/share/icons/*
/opt/kde3/share/locale/*

%changelog
* Sat Jun 03 2006 - samuele.c@hostnotfound.it
- update to ver 0.8.7
* Mon Dec 12 2005 - samuele.c@hostnotfound.it
- update to ver 0.8.6
* Sun Apr 03 2005 - samuele.c@hostnotfound.it
- update to ver 0.8.5
* Mon Mar 21 2005 - samuele.c@hostnotfound.it
- update to ver 0.8.1
* Thu Mar 17 2005 - samuele.c@hostnotfound.it
- update to ver 0.8
* Thu Feb 17 2005 - samuele.c@hostnotfound.it
- update to ver 0.7
- added qt3-devel on BuildRequires
- changed email address and url of home page
* Wed Jan 19 2005 - samuele_catuzzi@yahoo.it
- update to ver 0.6.1
* Mon Jan 17 2005 - samuele_catuzzi@yahoo.it
- update to ver 0.6  
