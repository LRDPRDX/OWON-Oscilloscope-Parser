# OWON Oscilloscope Parser
This is a C++ library for parsing binary files generated by the software for OWON oscilloscopes *only in auto-getting mode*. Binary files which aren't obtained in that way (e.g. by using the `SAVE` option on the oscilloscope directly or *single-getting mode* in the software) *cannot* be parsed by using this library.

# Disclaimer
This library is *not* an official OWON software. Parsing algorithms are based on the method of tries and errors (for SDS series) and some information about the structure of binary files provided by the OWON support.

# Available models
For now the following oscilloscope models are available for parsing

  * TDS8204
  * SDS6062
  * SDS7072
  * SDS8302

If your oscilloscope is not presented in this list you can send me a letter with a request
to write a parser for your model.

# Installation
For the installation process see the documentation (Sec. **Installation**)

# Usage
For the usage see the documentation (Sec. **Usage**)

# Documentation
The documentation is available here: `doc/users_guide.pdf`

# Feedback
Report bugs or suggest to paradox1859@gmail.com

# TODO's
- [ ] Add polarity to channel

# Test plots
As a test I've plotted a waveform using this library (left image) and the official software (right image). Looks pretty well :)
<p float="left">
  <img src="https://github.com/LRDPRDX/OWON-Oscilloscope-Parser/blob/master/example/TDS/pictures/csi_tl_parser.png" height="250">
  <img src="https://github.com/LRDPRDX/OWON-Oscilloscope-Parser/blob/master/example/TDS/pictures/csi_tl_soft.png" height="250">
</p>
