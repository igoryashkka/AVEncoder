## Processing of the FBAS signal and the PAL runtime decoder

Passive analog filters separate the FBAS signal into the demodulated luminance signal and the still modulated chroma signal for further signal processing. In the FBAS signal band, the chroma signal occupies a bandwidth of around 1.2 MHz, symmetrical to the frequency of the color subcarrier. A maximum of 3.5 MHz of the 5 MHz remains for the luminance or Y signal.

Next, Y-signal is denoted as luminance. 

So, Y-signal can be extracted using an LC-filter with a cut-off frequency of 4.43 MHz.

|               LC-filter                         |                      Signals                        |
|:-----------------------------------------:|:-----------------------------------------------------:|
| <img src="./images/LC_AV.jpg" alt="Input image" width="300"/> | <img src="./images/ocs_filter.jpg" alt="Predicted mask" width="300"/> |

|               GrayScale                         |                      Original                        |
|:-----------------------------------------:|:-----------------------------------------------------:|
| <img src="./images/GrayScale.jpg" alt="Input image" width="300"/> | <img src="./images/Original.jpg" alt="Predicted mask" width="300"/> |
