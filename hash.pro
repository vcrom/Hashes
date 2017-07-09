TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    hash_algorithms \
    app \
    tests

hash.depends = hash_algorithms
