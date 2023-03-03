include $(TOPDIR)/rules.mk

PKG_NAME:=esp
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/esp
	CATEGORY:=Base system
	TITLE:=esp
	DEPENDS:=+libubox +libubus +libusb-1.0 +libserialport
endef

define Package/esp/description
	Serial communication programs
endef

define Package/esp/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp_launcher $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp_ubus $(1)/usr/bin
	$(INSTALL_BIN) ./files/esp.init $(1)/etc/init.d/esp
	$(INSTALL_CONF) ./files/esp.config $(1)/etc/config/esp
endef

$(eval $(call BuildPackage,esp))
