TEMPLATE = subdirs
CONFIG += ordered

SUBDIRS += \
    hash_algorithms \
    app \
    tests

app.depends = hash_algorithms
tests.depends = hash_algorithms
