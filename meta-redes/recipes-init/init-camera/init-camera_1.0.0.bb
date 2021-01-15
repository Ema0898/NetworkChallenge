SUMMARY = "Network Challenge custom recipe"
DESCRIPTION = "Recipe to init after boot, the programs made for the RPi3"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"

SRC_URI = "file://COPYING    \
          file://CameraMQTT.service \
          "
RDEPENDS_${PN} += "elfutils bash"

# these 3 lines will have the script run on boot
inherit systemd
SYSTEMD_AUTO_ENABLE_${PN} = "enable"
SYSTEMD_SERVICE_${PN} = "CameraMQTT.service"

# install it in the correct location for update-rc.d
do_install() {
  install -d ${D}/${systemd_unitdir}/system

  install -m 0644 ${WORKDIR}/CameraMQTT.service ${D}/${systemd_unitdir}/system
}

FILES_${PN} = "${systemd_unitdir}/system/CameraMQTT.service"