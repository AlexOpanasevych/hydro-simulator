# from PythonQt.mgk import *
from PythonQt import *


# from PythonQt.mgk import MgkNumberView


# class Btn(object):
#     def __init__(self, name):
#         self.p_btn = MgkButton(name)
#
#     @property
#     def led(self):
#         return self.p_btn.led()
#
#     @led.setter
#     def led(self, value):
#         self.p_btn.setLed(value)


# class NumberView(object):
#     def __init__(self, name):
#         super().__init__(name)
#
#     @property
#     def value(self):
#         return self.value()
#
#     @value.setter
#     def value(self, value):
#         self.n_view.setValue(value)
#
# class Button(object):
#
#     def __init__(self, name):
#         super().__init__(name)
#
#     @property
#     def led(self):
#         return self.get('led')
#
#     @led.setter
#     def led(self, value):
#         self.set('led', value)


# class MgkContext(object):
#     def __init__(self):
#         self.gl = Button('ep')
#         self.apg = Button('apg')
#         self.pa_btn = Button('ek')
#         # self.shp = MGK345Button('shp')
#         # self.lb = MGK345Button('lb')
#         # self.pb = MGK345Button('pb')
#         #
#         # self.distance_nv = NumberView('distance')
#
#
# ctx = MgkContext()


# def on_gl_pressed():
#     ctx.gl.set('led', 1)
#     print("gl led", ctx.gl.get('led'))
#
#
# def on_apg_pressed():
#     ctx.apg.set('led', 1)

def on_gl_changed(value):
    gl.led = int(not gl.led)
    print("gl led", gl.led)


def on_apg_changed(value):
    apg.led = int(not apg.led)
    print("apg led", apg.led)


def on_bronzeOne_changed(value):
    print(value)
    bronzeOne.led = 1
    bronzeTwo.led = 0
    bronzeThree.led = 0
    bronzeFour.led = 0


def on_bronzeTwo_changed(value):
    bronzeOne.led = 0
    bronzeTwo.led = 1
    bronzeThree.led = 0
    bronzeFour.led = 0


def on_bronzeThree_changed(value):
    bronzeTwo.led = 0
    bronzeThree.led = 1
    bronzeOne.led = 0
    bronzeFour.led = 0


def on_bronzeFour_changed(value):
    bronzeFour.led = 1
    bronzeTwo.led = 0
    bronzeThree.led = 0
    bronzeOne.led = 0