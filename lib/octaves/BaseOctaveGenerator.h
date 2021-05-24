#ifndef EXT_BASE_OCTAVE_H
#define EXT_BASE_OCTAVE_H

#include <vector>
#include "lib/bukkit/NoiseGenerator.h"

class BaseOctaveGenerator {
public:

    /**
     * Sets the scale used for all coordinates passed to this generator.
     * <p>
     * This is the equivalent to setting each coordinate to the specified
     * value.
     *
     * @param float scale New value to scale each coordinate by
     */
    virtual void setScale(float scale) {
        this->setXScale(scale);
        this->setYScale(scale);
        this->setZScale(scale);
    }

    /**
     * Gets the scale used for each X-coordinates passed
     *
     * @return float X scale
     */
    float getXScale() {
        return this->x_scale;
    }

    /**
     * Sets the scale used for each X-coordinates passed
     *
     * @param float scale New X scale
     */
    void setXScale(float scale) {
        this->x_scale = scale;
    }

    /**
     * Gets the scale used for each Y-coordinates passed
     *
     * @return float Y scale
     */
    float getYScale() {
        return this->y_scale;
    }

    /**
     * Sets the scale used for each Y-coordinates passed
     *
     * @param float scale New Y scale
     */
    void setYScale(float scale) {
        this->y_scale = scale;
    }

    /**
     * Gets the scale used for each Z-coordinates passed
     *
     * @return float Z scale
     */
    float getZScale() {
        return this->z_scale;
    }

    /**
     * Sets the scale used for each Z-coordinates passed
     *
     * @param float scale New Z scale
     */
    void setZScale(float scale) {
        this->z_scale = scale;
    }

protected:
    std::vector<NoiseGenerator *> octaves;
    float x_scale = 1.0;
    float y_scale = 1.0;
    float z_scale = 1.0;
};

#endif
