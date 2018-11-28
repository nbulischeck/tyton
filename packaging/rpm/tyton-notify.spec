%define real_name tyton
%define  debug_package %{nil}

Summary: Tyton Notify is a thing
Name: tyton-notify
Version: master
Release: 1%{?dist}

License: GPL
Group: System Environment/Kernel
URL: https://github.com/nbulischeck/tyton

Source: tyton-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires: gcc, make, libnotify-devel, gtk3-devel
Requires: libnotify, gtk3, dkms-tyton

%description


%prep
%setup -q -n %{real_name}-%{version}


%build
make %{?_smp_mflags}


%install
make install -C notify/ DESTDIR=$RPM_BUILD_ROOT


%clean
rm -rf $RPM_BUILD_ROOT


%files
%defattr(-,root,root,-)
/usr/bin/tyton-notify
/etc/xdg/autostart/tyton-notify.desktop
%doc



%changelog
