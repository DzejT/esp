#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
#

include $(TOPDIR)/rules.mk

PKG_NAME:=libserialport
PKG_RELEASE:=1
PKG_VERSION:=0.1.1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/sigrokproject/libserialport.git
PKG_SOURCE_VERSION:=master

PKG_MAINTAINER:=todd <todd@grizzlystar.com>
PKG_LICENSE:=LGPLv3+
PKG_LICENSE_FILES:=LGPLv3+

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

PKG_INSTALL:=1
PKG_FIXUP:=autoreconf

include $(INCLUDE_DIR)/package.mk

TARGET_CFLAGS += $(TARGET_CPPFLAGS)

define Package/libserialport
  SECTION:=libs
  CATEGORY:=libs
  TITLE:=Sigrok libserialport
  URL:=https://sigrok.org/wiki/Libserialport
  MENU:=1
endef

define Package/libserialport/description
 Library for accessing serial ports
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/libserialport.h $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libserialport.pc $(1)/usr/lib/pkgconfig/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.la $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.a $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.so $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.so.* $(1)/usr/lib/
endef

define Package/libserialport/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.la $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.a $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.so $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libserialport.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libserialport))