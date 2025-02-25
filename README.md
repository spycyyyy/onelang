## T R Y !
1. Download file lang.cpp
2. Simply type this to the command prompt when you in the same path as source file (lang.cpp)
```
 g++ lang.cpp -o lang.exe
```
3. then run by 
```
lang.exe
```
## W H A T
* [X] Condensing everything on a single array (includes both condition block and memory block)
* [X] Interact by typing commands into the block. (APL language)
* [X] GOTO (move stack pointer), changing other memory block
* [ ] DATA type list {} , string " " and loop .~. [may require re-code]
* [ ] express everything in the form a dictionary
* [ ] figure out how to kick start the program
* [ ] +

## Custom Mathematical Expression Language
| Feature       | Description                                                                 |
|---------------|-----------------------------------------------------------------------------|
| **Registers** | The program uses a set of registers (`r`) to store values. Registers are indexed from 0 to 15. |
| **Memory**    | The program uses a stack memory (`mem`) to store values. Memory is indexed from 0 to 255. |
| Arithmetic    | `+`, `-`, `*`, `/`, `^`                                                     |
| Comparison    | `>`, `<`                                                                    |
| Logical       | `&`, `|`                                                                    |
| Generation    | `!`(factorial), `_` (interpolation)                                         |
| Memory        | `[]`, `#` (directly accessing everything on the list) |
| Pointer       | `~` (moving around the stack without changing anything) |

![image](https://github.com/user-attachments/assets/5b56553f-be40-423e-ac50-0260b1c2d161)
![image](https://github.com/user-attachments/assets/39b5d115-36f7-4b87-a4af-7cf9a70ed4c2)
![image](https://github.com/user-attachments/assets/3327d775-cdee-440d-a662-d7278ba28e95)


