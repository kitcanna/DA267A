#include "notes.h"
#include <math.h>
#include <string.h>
#include <stdio.h>

void freq2note(float frequency, char *stringbuff) 
{
    if (frequency <= 800 && frequency >= 430) {

        //Denna är på fel håll tror jag. Betedde sig konstigt i slutet så ska nog vara
        // A, A#, B, C# osv.
        char *notes[] = {"A", "G#", "G" "F#", "F", "E", "D#", "D", "C#", "C", "B", "A#"};

        //nr of semitones from A4:
        int note_nr = lround(12*log2(frequency/440)); 
        
            if (note_nr == 0) {
                strcpy(stringbuff, "A");
            }
            else {
                int note = abs(note_nr + 1);
                strcpy(stringbuff, notes[note]);
            }
    }
    else if(frequency > 800 || frequency < 430) {
        strcpy(stringbuff, "idk");
    }
}
