SUMMARY = "Network Challenge custom recipe"
DESCRIPTION = "Recipe to install the programs made for the RPi3"
LICENSE = "GPLv2"

LIC_FILES_CHKSUM = "file://${WORKDIR}/COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"

SRC_URI = "file://camera_mqtt.py \
          file://COPYING \
          file://objectDetectVideo.py \
          "

do_install() {
  # Check for installation diretory
  install -d ${D}/${bindir}

  # Install the files
  install -m 0755 ${WORKDIR}/camera_mqtt.py ${D}/${bindir}/camera_mqtt.py
  install -m 0755 ${WORKDIR}/objectDetectVideo.py ${D}/${bindir}/objectDetectVideo.py
}