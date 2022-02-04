TEMPLATE  = subdirs

SUBDIRS += \
    SortFilterProxyModel \
    kbus \
    klibcorelite \
    AudioStreaming \
    Audio \
    ClientParser \
    core \
    database \
    PluginsBase \
    pythonqt \
    qnanopainter \
#    raytracer/RayTracer \
    Trajectory \
    TCPWorkers \
    vdebug

SortFilterProxyModel.subdir = SortFilterProxymodel
klibcorelite.subdir = klibcorelite
AudioStreaming.subdir = AudioStreaming/AudioStreamingLib/AudioStreamingLibCore
Audio.subdir = Audio
ClientParser.subdir = CLientParser
core.subdir = core
database.subdir = database
PluginsBase.subdir = PluginsBase
pythonqt.subdir = pythonqt
qnanopainter.subdir = qnanopainter
#raytracer.subdir = raytracer
Trajectory.subdir = Trajectory
TCPWorkers.subdir = TCPWorkers
kbus.subdir = kbus
vdebug.subdir = vdebug

Audio.depends = core klibcorelite AudioStreaming
PluginsBase.depends = core klibcorelite
Trajectory.depends = klibcorelite PluginsBase
TCPWorkers.depends = PluginsBase
kbus.depends = vdebug PluginsBase



#CONFIG += ordered
