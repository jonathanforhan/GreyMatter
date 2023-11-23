# Change Log

## Version 1.0.0 to 1.1.0

- Optimize vector allocation on `reserve` to allocate only required memory
- Sensor callibration from LCD with IR remote
- New IR Remote controls

| Button                   | Action                                      |
| :-----------             | :------------------------------------------ |
| Play                     | enter calibration menu                      |
| Left Arrow               | Navigation next waveform                    |
| Left Arrow (calibration) | change decimal place in calibration left    |
| Right Arrow              | Navigation previous waveform                |
| Right Arrow (calibration)| change decimal place in calibration right   |
| Up Arrow                 | Increment calibration value at Nth place    |
| Down Arrow               | Decrement calibration value at Nth place    |
| FUNC/STOP                | return from calibration menu                |

- Doxygen integration with complete coverage