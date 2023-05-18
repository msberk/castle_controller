# Castle Creations ESC Controller

## Usage
After uploading, the throttle percentage can be controlle from a serial monitor. The settings are as follows:

|Setting | value |
|-----|----|
| Baud rate | 230400|
| Data bits | 8 |
| Stop bits | 1 |
| Parity | None |
| Line endings | LF |
| DTR | Enabled |
| RTS | Enabled |

If you send a line with a number between 0 and 100, it will set the throttle to that value.
If you send any non-number string (including an empty line) it will kill the throttle.
If you send a number outside of 0-100 it will hold the previous setting on the assumption of a mistype.