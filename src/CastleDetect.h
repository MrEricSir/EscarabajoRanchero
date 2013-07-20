#include "Level.h"

#ifndef CASTLEDETECT_H_
#define CASTLEDETECT_H_

// Default max detection area.
#define CASTLE_DETECT_DEFAULT_MAX_AREA 1024

namespace Escarabajo
{

/**
 * Castle detection class.
 */
class CastleDetect
{
public:
    virtual ~CastleDetect();
    CastleDetect( Level* l );

    /**
     * Resets the detection before each iteration.
     */
    void resetDetection();

    /**
     * Sets the maximum area size.
     */
    void setMaxArea( int a );

    /**
     * Run detection on the following coordinate.
     * If true is returned, there is exactly one
     * castle in the designated area.
     * Call getMask() to retrieve the area and
     * getUsedSnakes() to retrieve the used snakes.
     */
    bool runDetection( int x, int y );

    /**
     * Gets the mask from the previous detection.
     * The value of this function is only valid when
     * runDetection() returned true on the previous
     * invocation.
     */
    Array2D<bool>* getMask();

    // Call this after detection to merge any used snakes
    // that were cached back in.
    void mergeUsedSnakes();

private :

    // Resets the area prior to detection.
    void resetInternal();

    // Recursive detection routine.
    bool detectRecursive( int testX, int testY );

    // Check edges for snakes.
    bool checkSnakes( int x, int y );

    // Build edge mask.
    void checkSnakeRecursive( int x, int y, Array2D<int>& edgeMask );

    // Pointer to level.
    Level* level;

    // Detection area mask.
    // Corresponds with level dimensions.
    // True is areas that were searched.
    Array2D<bool>* mask;

    // Mask for used snake areas.
    Array2D<bool>* usedSnakes;

    // Maximum area to search for a castle.
    int maxArea;

    // Number of castles found.
    int numCastles;

    // Whether or not an area is bounded.
    bool bounded;

    // Number of tiles found.
    int numTiles;

};

}

#endif /* CASTLEDETECT_H_ */
