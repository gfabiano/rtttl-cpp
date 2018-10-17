# Ring Tone Text Transfer Language Parser

This single header library can parse RTTTL text and deliver the frequency and duration for each note.

# Using the RTTTL library

Instantiate an instance of the RTTTL class, passing the c_str to the constuctor

```cpp
char str[] = "PacMan:d=16,o=6,b=140:b5,b,f#,d#,8b,8d#,c,c7,g,f,8c7,8e,b5,b,f#,d#,8b,8d#,32d#,32e,f,32f,32f#,g,32g,32g#,a,8b:"
RTTTL tune(str);
```

Then use the notes generated (freq, msec) to play music. The method `get_notes()` returns a vector of struct with all needed info, in this form:

```cpp
struct atomic_note {
    char name[3] = {0};  // Name of note, including diesis
    int duration;
    int octave;
    double freq;
    int msec;
};
```
