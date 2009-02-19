Name:		sudokurapid
Version:	1.3
Release:	1%{?dist}
License:	GPL+
Group:		Development/Libraries
Summary:	Fast sudoku solution library with an heuristic algorithm
URL:		http://sudokurapid.sourceforge.net
Source:		http://downloads.sourceforge.net/sudokurapid/%{name}-%{version}.tar.gz
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)
BuildRequires:	qt-devel >= 4.4

%description
SudokuRapid is a library to solve sudoku puzzles.
Sudokurapid implements heuristic analysis which makes it possible
to find solution or prove that there is no one very quickly.

%prep
%setup -q


%build ./configure --prefix=$RPM_BUILD_ROOT/usr --libdir=$RPM_BUILD_ROOT/usr/lib64
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install


%clean
rm -rf $RPM_BUILD_ROOT


%package	devel
Summary: 	Fast sudoku solution library with an heuristic algorithm
Group:		Development/Libraries
Provides:	sudokurapid-static = %{version}-%{release}

%package	qt
Summary: 	Simple yet powerful qt application using sudokurapid
Group:		Amusements/Games
#BuildRequires:	sudokurapid-static
Requires:	qt >= 4.4

%package	fake
Summary: 	Fake package - this just makes check-files script happy


%description	devel
sudokurapid is a library to solve sudoku puzzles.
sudokurapid implements heuristic analysis which makes it possible
to find solution or prove that there is no one very quickly.
This package contains a static library and two header files.

%description	qt
Package provides a simple yet powerful application sudokuRapidQt
which uses sudokurapid library.

%description	fake
Fake package - this just makes check-files script happy :)


%files		devel
%defattr(-,root,root,-)
%{_includedir}/sudokuRapidCommon.h
%{_includedir}/sudokuRapid.h
%{_libdir}/libsudokuRapid.a

%files		qt
%defattr(-,root,root,-)
%{_bindir}/sudokuRapidQt

%files		fake
%defattr(-,root,root,-)
%{_bindir}/sudokuRapidConsole
%{_datadir}/%{name}/board1.txt
%{_datadir}/%{name}/board2.txt


%changelog
* Thu Dec 25 2008 Alexey Radkov <alexey.radkov@gmail.com> 1.3-1
- initial RPM release

