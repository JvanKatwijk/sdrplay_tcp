

---------------------------------------------------------------------
A simple sdrplay_tcp (Work in Progress)
---------------------------------------------------------------------

The rtl_tcp server and the spy server are quite popular, this
(simple) server mimics and rtl_tcp server (with some limitations
obviously)

The commands not implemented are noops

![1.1](/sdrplay_tcp.png?raw=true)


Other than rtl_tcp, this server has a small gui, showing the signal

Since the sdrplay devices have samplerates from 2M up, and the sticks
from app 960000 to something like 2.4, a solution was to be found
for the rates lower than 2 M.
A simple interpolator is used now, in a later stage some more advanced
techniques will be used.

A second issue is the gain mapping. While agc is not a problem, the
regular gain setting is still experimental.

The sdrplay provides a GRdB (where the R stands for Reduction) 20 .. 59
and reduction using the LNA state setting.

The maximum gain with the SDRplay is as far as I can see app 100 dB,
the mapping from a gain setting in the DAB stick to a gain reduction
in the SDRplay device is still under development

