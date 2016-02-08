//
//  PresetsRD.h
//  PedreraMaster
//
//  Created by nacho on 04/02/16.
//
//

#define NUM_PRESETS 13

struct preset{
    float feed;
    float kill;
};

class PresetsRD{
protected:
    preset presets[NUM_PRESETS] = {
        { // default
            0.028,
            0.057
        },
        {
            0.037,
            0.06
        },
        { // Solitons
            0.03,
            0.062
        },
        { // Pulsating solitons
            0.025,
            0.06
        },
        { // Worms.
            0.078,
            0.061
        },
        { // Mazes
            0.029,
            0.057
        },
        { // Holes
            0.039,
            0.058
        },
        { // Chaos
            0.026,
            0.051
        },
        { // Chaos and holes (by clem)
            0.034,
            0.056
        },
        { // Moving spots.
            0.014,
            0.054
        },
        { // Spots and loops.
            0.018,
            0.051
        },
        { // Waves
            0.014,
            0.045
        },
        { // The U-Skate World
            0.062,
            0.06093
        }
    };
    
public:
    preset get(int index){
        if(index < NUM_PRESETS)
            return presets[index];
        else
            return presets[0];
    }
};


