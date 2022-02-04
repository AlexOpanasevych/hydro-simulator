TEMPLATE = subdirs
SUBDIRS += \
        libs \
        plugins \
        app

app.subdir = app
plugins.subdir = plugins
libs.subdir = libs

app.depends = libs
plugins.depends = libs

#CONFIG  += ordered
