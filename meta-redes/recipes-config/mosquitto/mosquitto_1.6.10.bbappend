FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI += "file://mosquitto.conf \
            file://passwd \
           "

do_install_append() {
  install -d ${D}${sysconfdir}/mosquitto
  install -m 0644 ${WORKDIR}/mosquitto.conf ${D}${sysconfdir}/mosquitto/mosquitto.conf
  install -m 0644 ${WORKDIR}/passwd ${D}${sysconfdir}/mosquitto/passwd
}