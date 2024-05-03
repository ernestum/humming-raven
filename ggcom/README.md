The ggcom protocol is a data transmission protocol that builds upon ggwave.

For best compatibility between the python version of ggwave and the arduino version it:
- does not use DSS
- ueses the DT_FASTEST protocol (which is protocolId=8 on the python end)

It uses fixed length messages preceeded by a message counter.
This allows to detect missed messages.
Each message should be a self-contained package of information.
When we miss a message, we re-request it later.
Since each message is self-contained, its content can be processed immediately without waiting for re-transmission of earlier missed messages.

Idea: Harmonic Quantization?
