TEMPLATE=subdirs
SUBDIRS +=  \
    src \
    Client #\
    #pluginmanager #\
    #tests #\
    #experimental

src.subdir = src
Client.subdir = Client

Client.depends = src

#CONFIG += ordered

