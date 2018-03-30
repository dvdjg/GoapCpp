TEMPLATE = subdirs


win32-* {
SUBDIRS += build_msvc
} else {
SUBDIRS += build_gcc
}
