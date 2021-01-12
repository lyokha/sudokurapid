Name:           sudokurapid
Version:        1.4.3
Release:        1%{?dist}
License:        GPL+
Group:          Development/Libraries
Summary:        Fast sudoku solution library with an heuristic algorithm
URL:            https://github.com/lyokha/sudokurapid
Source:         https://github.com/lyokha/sudokurapid/archive/%{name}-%{version}.tar.gz
BuildRoot:      %(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires:  qt-devel >= 4.4, xdg-utils

%description
SudokuRapid is a library to solve sudoku puzzles.
Sudokurapid implements heuristic analysis which makes it possible
to find solution or prove that there is no one very quickly.

%prep
%setup -q


%build
./configure --prefix=$RPM_BUILD_ROOT/usr --libdir=$RPM_BUILD_ROOT/usr/lib64 \
                     --disable-sudokuRapidQt-Xdg
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install


%clean
rm -rf $RPM_BUILD_ROOT


%package        devel
Summary:        Fast sudoku solution library with an heuristic algorithm
Group:          Development/Libraries
Provides:       sudokurapid-static = %{version}-%{release}

%package        qt
Summary:        Simple yet powerful qt application using sudokurapid
Group:          Amusements/Games
#BuildRequires: sudokurapid-static
Requires:       qt >= 4.4


%description    devel
sudokurapid is a library to solve sudoku puzzles.
sudokurapid implements heuristic analysis which makes it possible
to find solution or prove that there is no one very quickly.
This package contains a static library and two header files.

%description    qt
Package provides a simple yet powerful application sudokuRapidQt
which uses sudokurapid library.


%files          devel
%defattr(-,root,root,-)
%{_includedir}/sudokuRapidCommon.h
%{_includedir}/sudokuRapid.h
%{_libdir}/libsudokuRapid.a
%{_bindir}/sudokuRapidConsole
%{_datadir}/%{name}/main.cc

%files          qt
%defattr(-,root,root,-)
%{_bindir}/sudokuRapidQt
%{_datadir}/%{name}/board1.txt
%{_datadir}/%{name}/board2.txt
%{_datadir}/%{name}/sudokuRapidQt.desktop
%{_datadir}/%{name}/sudokuRapidQt-48x48.png

%post           qt
xdg-desktop-menu install --novendor %{_datadir}/%{name}/sudokuRapidQt.desktop
xdg-icon-resource install --novendor --size 48 %{_datadir}/%{name}/sudokuRapidQt-48x48.png sudokuRapidQt

%postun         qt
if [ "$1" = 0 ] ; then
    xdg-icon-resource uninstall --size 48 sudokuRapidQt
    xdg-desktop-menu uninstall sudokuRapidQt.desktop
fi


%changelog
* Tue Jan 12 2021 Alexey Radkov <alexey.radkov@gmail.com> 1.4.3-1
- version 1.4.3

* Tue Jul 29 2014 Alexey Radkov <alexey.radkov@gmail.com> 1.4.2-1
- version 1.4.2

* Fri Jul 4 2014 Alexey Radkov <alexey.radkov@gmail.com> 1.4.1-2
- fixed xdg desktop file

* Thu Jan 24 2013 Alexey Radkov <alexey.radkov@gmail.com> 1.4.1-1
- version 1.4.1

* Mon Mar 23 2009 Alexey Radkov <alexey.radkov@gmail.com> 1.4-1
- version 1.4

* Thu Dec 25 2008 Alexey Radkov <alexey.radkov@gmail.com> 1.3-1
- initial RPM release

