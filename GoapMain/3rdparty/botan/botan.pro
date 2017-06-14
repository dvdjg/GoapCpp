TEMPLATE = subdirs


win32-msvc* {
SUBDIRS += build_msvc
} else {
SUBDIRS += build_gcc
}
