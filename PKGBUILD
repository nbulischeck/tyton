# Maintainer: Nick Bulischeck <nbulisc@g.clemson.edu>
# Contributor: Weston Belk <westonb@g.clemson.edu>

_pkgbase=tyton
pkgname=tyton
pkgver=1
pkgrel=1
pkgdesc="Linux Kernel-Mode Rootkit Hunter for 4.4.0-31+."
arch=('i686' 'x86_64')
url="https://github.com/nbulischeck/tyton"
license=('GPL3')
depends=('dkms', 'linux-headers', 'gcc', 'make', 'libnotify', 'pkgconfig', 'gtk3')
conflicts=("${_pkgbase}")
install=${pkgname}.install
source=("${url}/files/tarball.tar.gz"
        'dkms.conf')
md5sums=(use 'updpkgsums')

build() {
  cd ${_pkgbase}-${pkgver}
}

package() {
  # Install
  msg2 "Starting make install..."
  make DESTDIR="${pkgdir}" install

  # Copy dkms.conf
  install -Dm644 dkms.conf "${pkgdir}"/usr/src/${_pkgbase}-${pkgver}/dkms.conf

  # Set name and version
  sed -e "s/@_PKGBASE@/${_pkgbase}/" \
      -e "s/@PKGVER@/${pkgver}/" \
      -i "${pkgdir}"/usr/src/${_pkgbase}-${pkgver}/dkms.conf

  # Copy sources (including Makefile)
  cp -r ${_pkgbase}/* "${pkgdir}"/usr/src/${_pkgbase}-${pkgver}/
}
