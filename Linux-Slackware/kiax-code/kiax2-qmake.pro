TEMPLATE    = subdirs
SUBDIRS     = kiax2_core \
				kiax2_gui

# install
sources.files = *.pro
sources.path = .
INSTALLS += sources

kiax2_core.subdir = kiax2core
kiax2_gui.subdir = gui

#CONFIG += ordered
!ordered {
     kiax2_gui.depends = kiax2_core
}
