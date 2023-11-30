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

## Version 1.1.0 to 1.2.0

- New IR Remote controls
- Persistant calibration with EEPROM storage
- Store default Grey Matter Reccomendded values

| Button                   | Action                                      |
| :-----------             | :------------------------------------------ |
| Play                     | enter calibration menu                      |
| Left Arrow               | Navigation next waveform                    |
| Left Arrow (calibration) | change decimal place in calibration left    |
| Right Arrow              | Navigation previous waveform                |
| Right Arrow (calibration)| change decimal place in calibration right   |
| Up Arrow                 | Increment calibration value at Nth place    |
| Down Arrow               | Decrement calibration value at Nth place    |
| Number Pad               | Adjust set the calibration number           |
| FUNC/STOP                | return from calibration menu                |
| EQ                       | Reset the value under the cursor            |
| ST/REPT                  | Reset all values to default                 |